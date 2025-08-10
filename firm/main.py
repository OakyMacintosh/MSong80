# REF: https://docs.micropython.org/en/latest/library/machine.USBDevice.html
# REF: https://github.com/Bucknalla/micropython-i2c-lcd
# REF: https://docs.micropython.org/en/latest/rp2/quickref.html
# REF: https://github.com/HughMaingauche/PICO-VGA-Micropython

from machine import I2C, Pin, USBDevice

import utime
import i2c_lcd

#I2C_ADDR = 0x27
#I2C_NUM_ROWS = 2
#I2C_NUM_COLS = 16

#i2c = I2C(0, sda=Pin(0), scl=Pin(1), freq=400000)
#lcd = I2cLcd(i2c, I2C_ADDR, I2C_NUM_ROWS, I2C_NUM_COLS))

led = Pin(2, Pin.OUT)
i2c = I2C(0)
d = i2c_lcd.Display(i2c)
d.clear
d.move(0,0)

while True:
	led.on()
	d.write("MSong-80")
