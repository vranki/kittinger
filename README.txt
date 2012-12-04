kittinger
=========

Stratosphere balloon flight software and simulator

Target hw: Raspberry Pi


To build:

sudo apt-get install libgps-dev libgnokii-dev libqt4-dev
qmake
make



** Setting up persistent USB device names **

Copy 99-usb-serial.rules to /etc/udev/rules.d
Modify it if needed, see instructions here http://hintshop.ludvig.co.nz/show/persistent-names-usb-serial-devices/
