# ref: https://docs.micropython.org/en/latest/rp2/quickref.html

import json

from machine import Pin, PWM, machine

def play(note: str, duration: int)
def chord(notes: list, duration: int)
def tempo(bpm: int)
def pause(ms: int)
def volume(level: int)
# set_waveform("square") ----  "square", "triangle", "sine" and "noise"
def set_waveform(type: str)
def loop(fn: function, times: int)
def load_score(filename: str)

# pwm = PWM(pin, freq=note, duty_u16=note_duty)

file_ext = ".msong"
# TODO: make this list in a json file and include it with the JSON() function.
notes = {
	"c4": "261.63",
	"d4": "293.66",
	"e4": "329.63",
	"f4": "349.23",
	"g4": "392.00",
	"a4": "440.00",
	"b4": "493.88",
	"c5": "523.24"
}

def InitPwm():
	pwm.init()
