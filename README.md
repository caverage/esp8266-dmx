# esp8266-dmx
o ya

## Useful Commands
These are pretty specific to my setup, but with a bit of research this could work for you.

### Compile and Upload Locally
With the root of this repo as the working dir:

`arduino-cli compile --warnings all -t --fqbn esp8266:esp8266:d1_mini:CpuFrequency=80,UploadSpeed=921600,FlashSize=4M3M esp8266-dmx/ -p /dev/ttyUSB0 \
&& ~/.arduino15/packages/esp8266/tools/esptool/0.4.9/esptool -vv -cd nodemcu -cb 921600 -cp "/dev/ttyUSB0" -ca 0x00000 -cf "esp8266-dmx/build/esp8266.esp8266.d1_mini/esp8266-dmx.ino.bin"`

*NOTE:* The above command takes forever, waiting at `Waiting for upload port...`. Not sure why.

### Compile and Upload OTA to Controller
With `IP` defined and with the root of this repo as the working dir:

`arduino-cli compile --warnings all -t --fqbn sp8266:esp8266:d1_mini:CpuFrequency=80,UploadSpeed=921600,FlashSize=4M3M esp8266-dmx/ -p /dev/ttyUSB0 -t \
&& util/espota.py -d -i $IP -f esp8266-dmx/build/esp8266.esp8266.d1_mini/esp8266-dmx.ino.bin`

### Find IP of Controller

#### Remotely
Requires installation of `avahi-utils` on Debian.

`avahi-browse -ptr  "_arduino._tcp" | egrep ^= | cut -d\; -f4,8,9`

#### With Physical Access
The device prints it's IP over serial upon connection.

### Monitor Serial
This requires being plugged in (duh).

`screen /dev/ttyUSB0 115200`

## Notes
* Install `pyserial` in a Python venv and run `arduino-cli` and `arduino` from it.
* Some random async webserver stuff has to be manually installed (it's not installable through the Arduino IDE package manager)
