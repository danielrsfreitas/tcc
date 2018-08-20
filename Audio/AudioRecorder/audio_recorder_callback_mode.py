"""
Type in command line :

	python3 audio_recorder_callback_mode.py audiofile_name.wav length_in_seconds levels_quant new_rate

levels_quant : 1 - 8bits
			   2 - 16bits 
			   3 - 24bits
			   4 - 32 bits

new_rate    :  Pick a new sapling rate, for instance
			   44100   Hz
			   46000   Hz 
			   88200   Hz
			   96000   Hz
"""

import pyaudio
import wave	
import sys
import time
from scipy.io import wavfile
import numpy as np
import matplotlib.pyplot as plt


CHUNK = 1024 * 10 # Buffer size
CHANNELS = 1 # Mono input
FORMAT = {1 : pyaudio.paInt8, 2 : pyaudio.paInt16, 3 : pyaudio.paInt24, 4 : pyaudio.paInt32} # format options 
RATE = 96000 # Maximum sampling rate microphone driver 
SAMP_WIDTH = [1, 2, 3, 4]

"""
	
	Quantization and Normalization of signal proposed by Lucas Gabrielli

"""

def quantizate(data_sampled):



    # Re-normaliza para [0; q]: q / 2 *(data_sampled + 1)
    # Parte inteira, limitada em [0; q-1]: numpy.clip(numpy.floor(...), None, q - 1)
    # Re-normaliza para [-1; 1]: ... / ((q - 1) / 2) - 1
    data_quant = numpy.clip(numpy.floor(q / 2 * (data_sampled + 1)), None, q - 1) / ((q - 1) / 2) - 1

    plt.figure(4)
    plt.plot(data_quant)

    """

    """

def sample_with_scipy(audio_name, audio_rate, new_rate):

	q = 300
	# Ratio is the pace which the audio will be sampled in the numpy array
	RATIO = round(audio_rate/new_rate)

	# Opens the audio that was acquired with maximum sampling rate
	rate, data = wavfile.read(audio_name)

	# New numpy array with a lower sampling rate
	data_sampled = data[::RATIO]

	# Normalized from -1 to 1 
	# Casting np array to float32 to carry more info (see scypi.io.wavfile.write doc.)
	flt = data_sampled.astype(np.float32)/np.amax(np.abs(data_sampled))

	# Plot raw chart (96kHz)
	plt.figure(1)
	plt.plot(data)

	# Plot sampled chart
	plt.figure(2)
	plt.plot(data_sampled)

	# Plot -1 to 1 chart
	plt.figure(3)
	plt.plot(flt)

	data_quant = np.clip(np.floor(q / 2 * (flt + 1)), None, q - 1) / ((q - 1) / 2) - 1

	plt.figure(4)
	plt.plot(data_quant)
	
	plt.show()

	# Write audio files
	wavfile.write('after_sample.wav',audio_rate, data)
	wavfile.write('after_sample_32bits.wav',new_rate, flt)
	wavfile.write('after_sample_worse.wav', new_rate, data_sampled)
	wavfile.write('after_quantization.wav', new_rate, data_quant)



"""

	Did not work. Basically works as when we play the audio with a different sampling rate


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


"""

Function to acquire audio in callback mode.
	

"""
def callback(in_data, frame_count, time_info, status):
	wf.writeframesraw(in_data) 
	return (in_data, pyaudio.paContinue)


"""

	WAVE audio file parameters. 

	input:
		audio_name = audio name
		channels = number of channels (1 mono, 2 stereo)
		samp_width = 1 for 8bits, 2 for 16bits, 3 for 24bits and 4 for 32bits
		rate = sampling rate	

"""

def set_audiofile_params(audio_name, channels, samp_width, audio_rate):
	wf = wave.open(audio_name, 'w') # Returns a wave_write object  

	# Fill object parameters
	wf.setnchannels(channels)
	wf.setsampwidth(samp_width)
	wf.setframerate(audio_rate)

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
new_rate = int(sys.argv[4])



wf = set_audiofile_params(audio_name, CHANNELS, audio_format, RATE) 
# instantiate PyAudio 
p = pyaudio.PyAudio()

# open stream 
stream = p.open(
				frames_per_buffer = CHUNK,
	       		format = int(FORMAT[audio_format]),
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

sample_with_scipy(audio_name, RATE, new_rate)




