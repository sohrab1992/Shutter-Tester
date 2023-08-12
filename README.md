# Shutter-Tester
## An Arduino-based shutter tester featuring average and deviation calculations

It uses external interrupt and Timer1 on Arduino Uno and LCD keypad shield.

Each time the shutter has triggered, a measurement(by default up to 50 measurements) is made and can be previewed using UP/DOWN keys on the shield.

By pressing the RIGHT key, the average of the measurements is calculated and is useful for calibrating a specific shutter speed.

The SELECT key is used to show the deviation of the speeds from the standard 1-stop increments shutter speeds.

The LEFT key is used to go back to the normal mode.

An additional push button on Arduino's pin1 is used to zero the measurements.

## Phototransitor Connection

![transistor-phototransistor-common-emitter-circuit-01](https://github.com/sohrab1992/Shutter-Tester/assets/16500641/4a0ea7c0-15e6-4bca-9936-2889c6921faf)


## Dependency

It uses Erriez LCD Keypad Shield library:

https://github.com/Erriez/ErriezLCDKeypadShield







