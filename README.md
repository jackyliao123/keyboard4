## About the 4 button keyboard

The 4 button keyboard is a 3D printed RGB mechanical keyboard, with a custom PCB. It can house 4 Cherry MX compatible RGB key switches.

More about it here (Insert link here).

This repository contains the API for writing a firmware for the 4 button keyboard, which runs on the ATmega32u4. It uses the TLC5940 chip to drive the 4 RGB LEDs. A few examples are provided.

This uses the LUFA USB library in order to establish the USB communication with the computer.

## Compiling

```
git clone https://github.com/jackyliao123/keyboard4.git
cd keyboard4
make
```

After determining which serial port the device is attached to, 

```
make upload port=[the serial port]
```

to upload the code onto the device.

## Documentation

### Header files

#### tlc5940.h
This header file contains function that can be used to interface with the TLC5940 LED driver chip.

`void tlc5940_init(void)`

> Initializes the TLC5940.

`void tlc5940_set_rgb(uint8_t led, uint16_t r, uint16_t g, uint16_t b)`

> Sets the RGB values of the `led` (0 - 3), to `r`, `g`, `b` values (0 - 4095). This does not immediately updat the colours of the LEDs.

`void tlc5940_update(void)`

> Sends the updated colour data to the TLC5940 chip, and therefore updating the colours of the LEDs.


#### usb.h
This header file contains function that can be used to interface with the USB host.

`void usb_init(void)`

> Initializes USB.

`void usb_run(void)`

> Updates the USB, and processes CDC packets received from the computer

`bool usb_cdc_send(uint8_t *data, uint8_t len)`

> Send the first `len` bytes of `data` to the computer. `len` must not be greater than 16.
> Returns true when successful, returns false when otherwise.

`FILE usb_cdc_stdout_create_stream(void)`

> Creates a stdio stream for CDC serial data going to the USB host. This can be used to remap stdout.

`void usb_cdc_recv(uint8_t *data, uint8_t *len)`

> This is a function prototype, and it is called whenever data has been received on the USB serial console.
> The first `len` bytes of `data` represents the data that was received from the USB host.

`void usb_keyboard_update_led(uint8_t led)`

> This is a function prototype, and it is called wheneter an update regarding the state of the LEDs on the keyboard is received. (e.g. Caps Lock led state change)
> `led` is a mask of all the LEDs.

`void usb_keyboard_key_down(uint8_t key)`

> Sends a USB HID report, pressing down `key`.

`void usb_keyboard_key_up(uint8_t key)`

> Sends a USB HID report, releasing `key`.

`void usb_keyboard_modifier_down(uint8_t modifier)`

> Sends a USB HID report, pressing down the `modifier` key.

`void usb_keyboard_modifier_up(uint8_t modifier)`

> Sends a USB HID report, releasing the `modifier` key.

`void usb_mouse_button_down(uint8_t button)`

> Sends a USB HID report, pressing down the `button` on the mouse.

`void usb_mouse_button_up(uint8_t button)`

> Sends a USB HID report, releasing the `button` on the mouse.

`void usb_mouse_move(int16_t x, int16_t y)`

> Sends a USB HID report, moving the mouse by (+x, +y) in relative mode, and to (x, y) in absolute mode.

#### config.h

Contains configuration options for this device.

`DEBOUNCE_TIMEOUT`

> The number of microseconds a key should be disabled for, when a state change occurs, in order to accomplish debouncing.

`MOUSE_MIN_AXIS`

> To be documented

`MOUSE_MAX_AXIS`

> To be documented

`MOUSE_MIN_PHYS`

> To be documented

`MOUSE_MAX_PHYS`

> To be documented

`MOUSE_ABS`

> A boolean. `true` if this device should report itself as an absolute pointing device (touchscreen), `false` otherwise (mouse).

`USB_VID`

> The USB manufacturer ID that this device should report itself as.

`USB_PID`

> The USB product ID that this device should report itself as.

`USB_MANUFACTURER`

> The USB manufacturer string this device should report.

`USB_PRODUCT`

> The USB product string that this device should report.

`USB_SERIAL`

> The USB serial string that this device should report. If not present, the internal serial will be used.

#### util.h

Contains utilities for writing code for the device

`void util_init(void)`

> Initializes ports and timers.

`uint32_t util_micros(void)`

> Returns the number of microseconds elapsed since `util_init()`.

`void util_key_poll(void)`

> Polls for changes in the keys, and call `util_key_update` when such event happens.

`void util_key_update(uint8_t key, uint8_t state)`

> This is a function prototype, and it is called whenever `key` changes state to `state`.
> `state` is 0 when key is released, and 1 when key is pressed.
