
import pyaudio 
import struct
import numpy as np
import matplotlib.pyplot as plt

#%matplotlib tk

CHUNK = 1024 * 8 # Period of record
CHANNELS = 1 # Mono microfone
FORMAT = pyaudio.paInt16 # Format (Need to understand)
RATE = 44100 # Sampling rate


p = pyaudio.PyAudio()

stream = p.open(
    format = FORMAT,
    rate = RATE,
    channels = CHANNELS,
    input = True,
    output = True,
    frames_per_buffer = CHUNK
)

data = stream.read(CHUNK)

data_int = np.array(struct.unpack(str(2 * CHUNK) + 'B', data)) + 300

fig, ax = plt.subplots()
ax.plot(data_int)
ax.set_ylim(0,1000)

plt.show()