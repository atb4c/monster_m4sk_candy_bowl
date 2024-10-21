// SPDX-FileCopyrightText: 2019 Phillip Burgess for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#if 1 // Change to 0 to disable this code (must enable ONE user*.cpp only!)

#include "globals.h"
#include "HeatSensor.h"
#include "Adafruit_APDS9960.h"
#include <Adafruit_NeoPixel.h>

// This file provides a crude way to "drop in" user code to the eyes,
// allowing concurrent operations without having to maintain a bunch of
// special derivatives of the eye code (which is still undergoing a lot
// of development). Just replace the source code contents of THIS TAB ONLY,
// compile and upload to board. Shouldn't need to modify other eye code.

// User globals can go here, recommend declaring as static, e.g.:
// static int foo = 42;

// Audio variables
static bool                       growlPlaying = false;
static bool                       roarPlaying = false;
#define audioPlaying              (growlPlaying || roarPlaying)
static uint32_t                   audioPlayCount = 0;
static File                       f;
static uint32_t                   sampleRate = 0;
static wavStatus                  status;

void wavOutCallback(void);

// Heat sensing variables
#define HEAT_MEAS_FREQ            16
#define HEAT_MEAS_MS              (1000 / HEAT_MEAS_FREQ)
#define PERSON_DETECT_SECS        0.25
#define PERSON_DETECT_SAMPS       (PERSON_DETECT_SECS * HEAT_MEAS_FREQ)
#define PERSON_DETECT_VAR_THRESH  75

static HeatSensor                 heatSensor;
static long                       heatMeasTime = 0;
static uint32_t                   heatMeasCntr = 0;
static uint32_t                   personSeenCnt = 0;
static bool                       personSeen = false;

// Proximity sensing variables
static Adafruit_APDS9960          apds;
static bool                       handSeen = false;
#define PROX_THRESH               5

// LED Variables
#define LED_PIN                   2
#define LED_COUNT                 8
static Adafruit_NeoPixel          strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Vibration Motor Variables
#define VIBE_PIN                  3
#define VIBE_EN_LVL               HIGH
#define VIBE_DIS_LVL              LOW
#define VIBE_ENABLE()             (digitalWrite(VIBE_PIN, VIBE_EN_LVL))
#define VIBE_DISABLE()            (digitalWrite(VIBE_PIN, VIBE_DIS_LVL)) 


// other variables
static uint32_t                   loopCnt = 0;
static long                       currentMillis = 0;


void _startPlayingAudio(const char *filename)
{
  f = arcada.open(filename);
  // Try to load it as a wave file
  status = arcada.WavLoad(f, &sampleRate);
  Serial.print("Playing file: "); Serial.println(filename);
  delay(100);
  arcada.enableSpeaker(true);  // enable speaker output
  arcada.timerCallback(sampleRate, wavOutCallback); // stop heat measurement and start playing audio
}

void startPlayingGrowl(void)
{
  _startPlayingAudio("audio/growl.wav");
  growlPlaying = true;
}

void startPlayingRoar(void)
{
  _startPlayingAudio("audio/roar.wav");
  roarPlaying = true;
}

void stopPlayingAudio(void)
{
  arcada.timerStop();
  growlPlaying = false;
  roarPlaying = false;
  arcada.enableSpeaker(false);
  f.close();
  audioPlayCount++;
  // startPersonDetection();   // restart heat measurement
  Serial.printf("Played Audio %u times\n", audioPlayCount);
  delay(100);
}

void getAudioSamples(void)
{
  if (arcada.WavReadyForData()) 
  {
    // Serial.println("reading wav data");
    status = arcada.WavReadFile();
    if ((status != WAV_OK) && (status != WAV_LOAD)) {    // EOF or Error encountered
      stopPlayingAudio();
      VIBE_DISABLE();
    }
  }
}



// Person detection via Heat-Sensor called as a loop at defined lower requency htan user loop
void personDetectLoop(void) 
{
  heatMeasCntr++;

  // Estimate the focus position.
  heatSensor.find_focus();

  // check if it looks like there is a person (high variacnce across 2D data)
  if(heatSensor.var > PERSON_DETECT_VAR_THRESH)
  {
    personSeenCnt++;
  }

  // update whether there is a person every PERSON_DETECT_SAMPS samples from heat sensor
  if(heatMeasCntr >= PERSON_DETECT_SAMPS)
  {
    if(personSeenCnt > (PERSON_DETECT_SAMPS / 2))
    {
      if(!personSeen) Serial.println("Person Seen!");   // first time seen (transition)
      personSeen = true;                                // set person state
      moveEyesRandomly = false;                         // stop moving eyes randomly
      
      // look at person
      eyeTargetX = heatSensor.x;
      eyeTargetY = -heatSensor.y;
    }
    else
    {
      personSeen = false;                              // set person state
      moveEyesRandomly = true;                        // back to moving eyes randomly
    }

    // reset counters
    heatMeasCntr = 0;
    personSeenCnt = 0;
  }  
}








// Called once near the end of the setup() function. If your code requires
// a lot of time to initialize, make periodic calls to yield() to keep the
// USB mass storage filesystem alive.
void user_setup(void) 
{
  showSplashScreen = false;
  heatSensor.setup();
  heatMeasTime = millis() + 5000;   // 5 second delay to give everything time to boot up

  if(!apds.begin()){
    Serial.println("failed to initialize APDS-9960!");
  }
  else Serial.println("APDS-9960 initialized!");

  //enable proximity mode
  apds.enableProximity(true);

  // Initialize Neopixel LEDs
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  for (int i=0; i<strip.numPixels(); i++)
  {
    strip.setPixelColor(i, 0xFF, 0x55, 0x00);
  }
  strip.setBrightness(25); // Set BRIGHTNESS to about 1/10 (max = 255)
  strip.show();

  // setup vibration motor
  pinMode(VIBE_PIN, OUTPUT);
  VIBE_DISABLE();
}

// Called periodically during eye animation. This is invoked in the
// interval before starting drawing on the last eye (left eye on MONSTER
// M4SK, sole eye on HalloWing M0) so it won't exacerbate visible tearing
// in eye rendering. This is also SPI "quiet time" on the MONSTER M4SK so
// it's OK to do I2C or other communication across the bridge.
// This function BLOCKS, it does NOT multitask with the eye animation code,
// and performance here will have a direct impact on overall refresh rates,
// so keep it simple. Avoid loops (e.g. if animating something like a servo
// or NeoPixels in response to some trigger) and instead rely on state
// machines or similar. Additionally, calls to this function are NOT time-
// constant -- eye rendering time can vary frame to frame, so animation or
// other over-time operations won't look very good using simple +/-
// increments, it's better to use millis() or micros() and work
// algebraically with elapsed times instead.
void user_loop(void) 
{

  loopCnt++;
  currentMillis = millis();

  // Serial.println(apds.readProximity());

  handSeen = (PROX_THRESH < apds.readProximity());

  if(handSeen)
  {
    if(!roarPlaying)
    {
      if(growlPlaying)
      {
        stopPlayingAudio();
        //VIBE_DISABLE();
      }
      startPlayingRoar();
      //VIBE_ENABLE();
    }
  }

  if(audioPlaying)
  {
    // Repeats until WAV_EOF or WAV_ERR_*
    getAudioSamples();
  }
  else
  {
    // start growling
    if(personSeen)
    {
      startPlayingGrowl();
    }
  }


  // TODO: still need to handle roll-overs
  if(heatMeasTime <= currentMillis)
  {
    heatMeasTime += HEAT_MEAS_MS;
    personDetectLoop();
  }
  
}



// Single-sample-playing callback function for timerCallback() above.
void wavOutCallback(void) 
{
  status = arcada.WavPlayNextSample();
  // Serial.printf("samp\t%u\n", sampleCnt++);
  // Serial.printf("play status: %d \n", status);
  if ((status != WAV_OK) && (status != WAV_LOAD)) {    // EOF or Error encountered
    if (status == WAV_ERR_STALL)
    {
      Serial.println("wav buffer stall");
    }
    else
    {
      Serial.printf("play error = %u\n", status);
      stopPlayingAudio(); 
      //VIBE_DISABLE();
    }
  }
}

#endif // 0
