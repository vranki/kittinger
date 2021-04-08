Kittinger
=========


Stratosphere balloon flight software and simulator

Note: Looking for new maintainer

Target hw: Raspberry Pi

To build:
```bash
sudo apt-get install libgps-dev libgnokii-dev libqt5webkit5-dev qtbase5-dev
qmake
make
```


** Setting up persistent USB device names **

Copy 99-usb-serial.rules to /etc/udev/rules.d
Modify it if needed, see instructions here http://hintshop.ludvig.co.nz/show/persistent-names-usb-serial-devices/
