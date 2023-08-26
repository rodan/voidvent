
## voidvent

 ////// UNDER DEVELOPMENT ///////////////////////

 room climate control project that replaces the microcontroller inside a blueair pro air purifier with a transceiver-enabled cc430f5137 devboard from Olimex.

```
 source:       https://github.com/rodan/voidvent
 author:       Petre Rodan <2b4eda@subdimension.ro>
 license:      BSD
```

### Features

 * RF-based remote control of the air purifier via Intertechno ITLS-16
 * limit the ionizer activity to a custom number of minutes per hour (3 levels)
 * control air extractor via RF signal
 * control window opening via RF signal
 
### Components

project directory structure

 * ./firmware/base_station/ - source code for the base station
 * ./firmware/window_control/ - source code for the window opening jig
 * ./hardware/blueair_pro_m/ - kicad schematics of the digital subsystem of the air purifier


### Build requirements

you will need TI's excelent [GCC toolchain](https://www.ti.com/tool/MSP430-GCC-OPENSOURCE) or Code Composer Studio for linux and my [reference libraries for msp430 micros](https://github.com/rodan/atlas430) cloned in /opt/.

```
cd ./firmware/base_station
make
make install
```


