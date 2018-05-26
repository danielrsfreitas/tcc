import socket

HOST = '127.0.0.1'    		# The remote host
PORT = 50007             	# The same port as used by the server


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:

	a = open('../../Audio/AudioRecorder/daniel.wav','rb') # Open audiofile as binary
	audio_len = a.read(1024) # Buffer size of audio_len
	s.connect((HOST, PORT)) # Open the socket stream (TO CONFIRM)

	while (audio_len):

		s.send(audio_len) # Send the audio in pieces through binary file
		audio_len = a.read(1024) # Read another amount of the audio to be sent

s.close() # Close socket connection
	    
print('Audio sent')