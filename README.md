# monster_m4sk_candy_bowl
Using Adafruit's Monster M4SK for a Halloween candy bowl.

This was modified from [THIS](https://learn.adafruit.com/monster-m4sk-is-watching-you/overview) project which uses a thermal camera to make the eyes follow you.  I added the ability to play detect when a hand is reached into the candy bowl along with playing audio clips (growling when a person is seen and a roar when a hand is being reached in).

I used [VSCode](https://code.visualstudio.com/) and [platformio](https://platformio.org/), but this could just as easily be done with the Arduino IDE.

Please follow the instructions [HERE](https://learn.adafruit.com/adafruit-monster-m4sk-eyes) to generally setup your Monster M4SK for building from source.

I have included the [EYE Config File] I used here as well.  todo: insert link to config.eye

The plastic parts were printed on a BambuLabs P1S.  If you have a similar printer, you should be able to use the 3MF to print everything easily.


## Parts List

1. [Monster M4SK](https://www.adafruit.com/product/4343)
2. [Eye Lenses](https://www.adafruit.com/product/3917)
3. [Eye Connector Cable](https://www.adafruit.com/product/4350)
4. [Lithium Battery](https://www.adafruit.com/product/1781)
5. [Thermal Camera Cable](https://www.adafruit.com/product/3950)
6. [AMG8833 Thermal Camera](https://www.adafruit.com/product/3538)
7. [QT Connector Cable](https://www.adafruit.com/product/4210)
8. [APDS9960 Proximity Sensor](https://www.adafruit.com/product/3595)
9. [Mini-Speaker](https://www.adafruit.com/product/3923)
10. [3-Pin Connector](https://www.adafruit.com/product/3893)
11. [Neopixel LED Stick](https://www.adafruit.com/product/1426)
12. 10-in Skull to mount everything into.  I got mine at a Spirit Halloween, but these look like the same one [LINK1](https://www.tractorsupply.com/tsc/product/red-shed-large-light-up-skull-decoration-2352173?store=1922) [LINK2](https://partyconnexion.com/new-skull-lu-10/)
13. Optional: switch for easy on/off [LINK1](https://www.digikey.com/en/products/detail/e-switch/RA1113112R/3778055?utm_adgroup=&utm_source=google&utm_medium=cpc&utm_campaign=PMax%20Shopping_Product_Low%20ROAS%20Categories&utm_term=&utm_content=&utm_id=go_cmp-20243063506_adg-_ad-__dev-c_ext-_prd-3778055_sig-Cj0KCQjw99e4BhDiARIsAISE7P9G5rRpclzWcZg-3TNdtSHT65RccuPgaGUKwaSfYaPJbDgD9tsjGOQaAsgBEALw_wcB&gad_source=1&gclid=Cj0KCQjw99e4BhDiARIsAISE7P9G5rRpclzWcZg-3TNdtSHT65RccuPgaGUKwaSfYaPJbDgD9tsjGOQaAsgBEALw_wcB)
14. M2.5 Nylon Screws - I recommend buying [THIS](https://www.adafruit.com/product/3299)


## Instructions

1. Use a dremel tool to remove base of the skull just below where the jow bone connects.
2. Once open, remove the light-up electronics from the kull (includes pieces connected to the eies and nose locations).
3. Assemble all the internal plastic pieces as shown here using super glue only if needed after press-fitting
4. Attach electronics to the internal pieces
5. slide full internal assembly into skull.  It should hold in-place by the front and back pushing against the plastic skull
6. Slide the back of the skull into the groove on the bown top
7. slide the skull and bowl top down onto the bowl making sure the jaw bone is open and held in-place by the bottom bracket on the bowl
8. Scare some kids... ;-)
