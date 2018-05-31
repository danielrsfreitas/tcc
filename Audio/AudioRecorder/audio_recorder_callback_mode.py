"""
Type in command line :

	python3 audio_recorder_callback_mode.py audiofile_name.wav length_in_seconds levels_quant samp_rate

levels_quant : 1 - 8bits
			   2 - 16bits 
			   3 - 24bits
			   4 - 32 bits

samp_rate    : 44.1 kHz
			   46   kHz 
			   88.2 kHz
			   96   kHz
"""

import pyaudio
import wave	
import sys
import time

CHUNK = 1024 * 10 # Buffer size
CHANNELS = 1 # Mono input
FORMAT = {1 : pyaudio.paInt8, 2 : pyaudio.paInt16, 3 : pyaudio.paInt24, 4 : pyaudio.paInt32} # format options 
RATE = [44100, 48000, 88200, 96000] # Maximum sampling rate microphone driver 
SAMP_WIDTH = [1, 2, 3, 4]

"""

Function to acquire audio in callback mode.
	

"""
def callback(in_data, frame_count, time_info, status):
	wf.writeframesraw(in_data) 
	return (in_data, pyaudio.paContinue)

"""

	Did not work. Basically works as when we play the audio with a different sampling rate

"""
def sample_audio_44100(audio_name):
	wf_read = wave.open(audio_name, 'r')
	data = wf_read.readframes(wf_read.getnframes())

	wf_write = wave.open(audio_name+"_sampled_44100Hz", 'w')
	wf_write.setnchannels(CHANNELS) 
	wf_write.setsampwidth(4)
	wf_write.setframerate(44100)
	wf_write.writeframesraw(data)

	wf_read.close()
	wf_write.close()

"""

	WAVE audio file parameters. 

	input:
		audio_name = audio name
		channels = number of channels (1 mono, 2 stereo)
		samp_width = 1 for 8bits, 2 for 16bits, 3 for 24bits and 4 for 32bits
		rate = sampling rate	

"""

def set_audiofile_params(audio_name, channels, samp_width, rate):
	wf = wave.open(audio_name, 'w') # Returns a wave_write object  

	# Fill object parameters
	wf.setnchannels(channels)
	wf.setsampwidth(samp_width)
	wf.setframerate(rate)

	return wf


"""
	Main 
"""

# Receive the name of the wave file as an argument
if len(sys.argv) < 5:
    print("Records a wave file named %s %d %d %d\n\n" % sys.argv[0])
    sys.exit(-1)

audio_name = sys.argv[1]
audio_len = sys.argv[2]
audio_format = int(sys.argv[3])
audio_rate = int(sys.argv[4])



wf = set_audiofile_params(audio_name, CHANNELS, audio_format, audio_rate) 
# instantiate PyAudio 
p = pyaudio.PyAudio()

# open stream 
stream = p.open(
				frames_per_buffer = CHUNK,
	       		format = int(FORMAT[audio_format]),
                channels = CHANNELS,
                rate = audio_rate,
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

#sample_audio_44100(audio_name)




