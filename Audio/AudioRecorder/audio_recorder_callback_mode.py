"""PyAudio Example: Play a wave file."""

import pyaudio
import wave	
import sys
import time

CHUNK = 1024 * 20 # Buffer size
CHANNELS = 1 # Mono input
FORMAT = pyaudio.paInt16 # Format
RATE = 44100 # Sampling rate 

# Define callback
def callback(in_data, frame_count, time_info, status):
	wf.writeframesraw(in_data) 
	return (in_data, pyaudio.paContinue)


# Receive the name of the wave file as an argument
if len(sys.argv) < 3:
    print("Records a wave file named %s %d\n\n" % sys.argv[0])
    sys.exit(-1)

audio_name = sys.argv[1]
audio_len = sys.argv[2]

wf = wave.open(audio_name, 'w') # Returns a wave_write object named 

# Fill object parameters
wf.setnchannels(CHANNELS) 
wf.setsampwidth(2) # why 2 ? look for it in pyaudios documentation
wf.setframerate(RATE)

# instantiate PyAudio 
p = pyaudio.PyAudio()

# open stream 
stream = p.open(
				frames_per_buffer = CHUNK,
	       		format = FORMAT,
                channels = CHANNELS,
                rate = RATE,
                input = True,
                stream_callback = callback)

#data = stream.read(CHUNK)
# Write acquired date in wave file

stream.start_stream()

#while stream.is_active():
time.sleep(int(audio_len))

# stop stream 
stream.stop_stream()
stream.close()

# Close wave file
wf.close()

# close PyAudio 
p.terminate()