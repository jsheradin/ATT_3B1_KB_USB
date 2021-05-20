# ATT_3B1_KB_USB
![keybored](https://raw.githubusercontent.com/jsheradin/ATT_3B1_KB_USB/main/images/KB_front.jpg)

## About
### Project
This project is a reversible USB conversion for the AT&T Unix PC keyboard (3B1). The firmware is a basic starting point with many keys unmapped but it can be altered fairly easily to make a useful keymap. It should be straight-forward to implement in QMK.

In it's current state, the keyboard is fully functional as a normal USB keyboard with no major quirks.

### Keyboard
This particular example uses plate mounted ITW Magnetic Valve switches. The keycaps are dye sublimated (top and sides) and appear to be ABS.

### Converter
The converter is largely limited by the Arduino libraries. The Pro Micro (32u4) used only support 6KRO and requires an external logic chip to invert the TX/RX lines.

## Protocol
The keyboard communicates over inverted, 7-bit, 1 stop, no parity, 1200 baud serial. For every keystroke event, the keyboard sends a message containing all of the currently pressed keys. In the event that all keys are released, the '@' character is sent. It seems to finish sending the last message before sending the next, most up to date message. There is a minimum time between messages that seems to be ~20ms but that can likely be better determined for a more responsive converter.

Presumably a mouse will also send information over the same serial connection but I don't have one available to test.

To enable the keyboard:
* Reset held low then released to high
* KB sends 0x7F many times then stops
* Writing 0x37 (caps lock light on) will enable keystrokes to be sent
* Writing 0x38 will turn off caps lock light
* Keyboard now sends information on every key event

#### Discovered codes:
|Code sent to KB|Function|
|-|-|
|0x37|Caps lock light on|
|0x38|Caps lock light off|
|0x21|Num Lock light on|
|0x22|Num Lock light off|

## Hardware
This project uses:
* Arduino Pro Micro
* SN74LS00 (but any NAND/NOR/etc can be used)
* Assorted lengths of wire

#### Keyboard Pinout (as labeled on the Keyboard socket PCB footprint):
|KB pin|Function|
|-|-|
|1, 2, 3|GND|
|4, 8|+5V|
|5|KB Reset|
|6|KB RX|
|7|KB TX|

With an adaptation to a different chip/library you may not need the inverter. In the project as described and coded, you MUST pass both TX/RX through an inverter. For mine I used an SN74LS00 quad NAND gate with one pin of each of the gate just tied to Vcc.

#### Arduino Pinout:
|Arduino Pin|Function|
|-|-|
|RX|KB TX (through inverter)|
|TX|KB RX (through inverter)|
|3|KB Reset|

## Firmware
### Flow
1. Initialize keyboard
2. Wait for available message
3. Read message in it's entirety (until timeout for last byte expires)
4. Truncate message due to 6KRO limitation
5. Map keycode to a code compatible with Arduino Keyboard library
6. Send over USB
7. Goto 2

### Editing keymap
The file keymap.h contains a massive switch case that can be editied to reflect your desired operation. It should be fairly straightforward to use layers, etc.

Any unknown codes will be sent over serial (view with Arduino Serial Monitor) and can be added to the switch case.
