### SD card

## Soure of map images

<https://ns6t.net/azimuth/azimuth.html>
<https://ns6t.net/AzShadowMap/experimental.html>

## How to use maps

1. Prepare image snip tool
2. Convert image to suitable format and size - size 480x480 or 800x480, BMP format, 24 bit per pixel, without alpha, `gimp` or script `convert.sh`
3. prepare file `my_map.txt` (see example)

## How to use config

There is no `#define` in file `MyConfigurationFile.h`. Configuration is moved to `my_conf.txt` and `my_cal.txt`.

1. prepare files `my_conf.txt` and `my_cal.txt`  (see example)
2. Load configuration in menu `Config. EEPROM` or `Calib. EEPROM`, choice `SD->RAM->EEPROM`
3. Be carful, it is experimantal

