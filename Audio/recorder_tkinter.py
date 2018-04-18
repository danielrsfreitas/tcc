
import pyaudio 
import struct
import tkinter
import numpy as np
import matplotlib.pyplot as plt
import tkinter
#%matplotlib inline

CHUNK = 1024 * 10 # Period of record. Limited by buffer size
CHANNELS = 1 # Mono microfone
FORMAT = pyaudio.paInt16 # Format (Need to understand)
RATE = 44100 # Sampling rate


p = pyaudio.PyAudio() # Object 

stream = p.open(
    format = FORMAT,
    channels = CHANNELS,
    rate = RATE,
    input = True,
    output = True,
    frames_per_buffer = CHUNK
) 
# Creating a socket 

"""data = stream.read(CHUNK) #
data_int = np.array(struct.unpack(str(2  * CHUNK) + 'B', data), dtype='b')[::2] # A little bit o processing


fig, ax = plt.subplots()
ax.plot(data_int, '-')

plt.show()
"""
fig, ax = plt.subplots() # Create subplot
x = np.arange(0, 2 * CHUNK, 2) # returns an evenly spaced array of size 2 * CHUNK and pace of 2

line, = ax.plot(x, np.random.rand(CHUNK)) # plot returns a 2D line
ax.set_ylim(0, 255) # Y-axis chart bounds
ax.set_xlim(0, CHUNK) # X-axis chart bounds

while True:
    data = stream.read(CHUNK) # Reading audio  
    data_int = np.array(struct.unpack(str(2  * CHUNK) + 'B', data), dtype='b')[::2] + 130 # A little bit of display processing
    line.set_ydata(data_int) # 1D array manipulation which is amplitude of audio
    fig.canvas.draw() 
    fig.canvas.flush_events() # Buffer size must be respected. Other than that input overflow occurs
