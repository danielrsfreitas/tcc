import pyaudio
import wave	
import sys
import tkinter as tk

def play_audio():
	CHUNK = 1024

	if len(sys.argv) < 2:
	    print("Plays a wave file.\n\nUsage: %s filename.wav" % sys.argv[0])
	    sys.exit(-1)

	wf = wave.open(sys.argv[1], 'rb')

	# instantiate PyAudio (1)
	p = pyaudio.PyAudio()

	# open stream (2)
	stream = p.open(format= p.get_format_from_width(wf.getsampwidth()),
	                channels=wf.getnchannels(),
	                rate= 22050,#wf.getframerate(),
	                output=True)

	# read data
	data = wf.readframes(CHUNK)

	# play stream (3)
	while len(data) > 0:
	    stream.write(data)
	    data = wf.readframes(CHUNK)

	# stop stream (4)
	stream.stop_stream()
	stream.close()

	# close PyAudio (5)
	p.terminate()


root = tk.Tk()
frame = tk.Frame(root)
frame.pack()

button = tk.Button(frame, 
                   text="QUIT", 
                   fg="red",
                   command=quit)
button.pack(side=tk.LEFT)

slogan = tk.Button(frame,
                   text="Hello",
                   command=play_audio)
slogan.pack(side=tk.LEFT)


root.mainloop()