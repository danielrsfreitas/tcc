"""PyAudio Example: Play a wave file."""

import pyaudio
import wave	
import sys

CHUNK = 1024 * 20 # Buffer size
CHANNELS = 1 # Mono input
FORMAT = pyaudio.paInt16 # Format

RATE = 44100 # Sampling rate 

if len(sys.argv) < 2:
    print("Records a wave file named %s \n\n" % sys.argv[0])
    sys.exit(-1)

wf = wave.open(sys.argv[1], 'w') # Returns a Wave_write object named 

# Fill object parameters
wf.setnchannels(CHANNELS) 
wf.setsampwidth(2)
wf.setframerate(RATE)

# instantiate PyAudio 
p = pyaudio.PyAudio()

# open stream 
stream = p.open(
				frames_per_buffer = CHUNK,
	       		format = FORMAT,
                channels = CHANNELS,
                rate = RATE,
                input = True)

data = stream.read(CHUNK)

# Write acquired date in wave file
wf.writeframesraw(data)

# stop stream 
stream.stop_stream()
stream.close()

# Close wave file
wf.close()

# close PyAudio 
p.terminate()