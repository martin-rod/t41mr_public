#! /bin/bash

# https://imagemagick.org/script/command-line-options.php

OUTPUT_OPTIONS="-resize 480x480 -depth 8 -alpha off"

convert cestice_color_1.png $OUTPUT_OPTIONS cesticec1.bmp
file cesticec1.bmp

convert cestice_color_2.png $OUTPUT_OPTIONS cesticec2.bmp
file cesticec2.bmp

convert cestice_color_3.png $OUTPUT_OPTIONS cesticec3.bmp
file cesticec3.bmp

convert cestice_color_4.png $OUTPUT_OPTIONS cesticec4.bmp
file cesticec4.bmp

convert cestice_blue_3k.png $OUTPUT_OPTIONS cesticek3.bmp
file cesticek3.bmp

convert cestice_blue_5k.png $OUTPUT_OPTIONS cesticek5.bmp
file cesticek5.bmp

convert cestice_blue_10k.png $OUTPUT_OPTIONS cesticek10.bmp
file cesticek10.bmp

convert cestice_blue_15k.png $OUTPUT_OPTIONS cesticek15.bmp
file cesticek15.bmp
