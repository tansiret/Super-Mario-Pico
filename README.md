# Super-Mario-Pico
A basic Super Mario demo written in C for Raspberry Pi Pico/Pico 2. (Still WIP)

# Hardware
PCB coming soon

Basic breadboard setup:
![Super Mario Pico on breadboard](circuit.jpg)

ST7735 screen connection:

| TFT PinNum | Pindesc |  HW SPI |
| --- | --- | --- | 
| 1 | LED | VCC |   
| 2 | SCLK | GPIO18 |
| 3 | SDA | GPIO19 |
| 4 | A0/DC |  GPIO3  |
| 5 | RESET |   GPIO4 |
| 6 | SS/CS |  GPIO2 |
| 7 | GND | GND |
| 8 | VCC |  VCC  |

Buttons:
| Pindesc | GPIO |
| --- | --- |
| BTN_JUMP | 28 |
| BTN_RIGHT | 27 |
| BTN_LEFT | 26 |
| BTN_DOWN | 25 |


# Licensing
This software is developed and distributed with [GPL 3.0](LICENSE) by Tan S. Akinci. [displaylib_16bit_PICO](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO) library is developed and distributed with GPL 3.0 by Gavin Lyon. Super Mario is a trademark of Nintendo. All Super Mario World tiles and sprites, and all Super Mario tunes used in this software are Nintendo's property and are not part of the licensing of the software. This software is a fan project. Developers of this software are not responsible for potential copyright and trademark infringements.
