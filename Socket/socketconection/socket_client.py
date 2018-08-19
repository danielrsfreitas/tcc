"""

Type in terminal:

	python3 socket_client.py audio_file.wav

audio_file = wave file to send

"""


import socket
import sys

HOST = '127.0.0.1'    		# The remote host
PORT = 50007             	# The same port as used by the server

audio_name = sys.argv[1]

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:

	a = open(audio_name,'rb') # Open audiofile as binary
	audio_len = a.read(1024) # Buffer size of audio_len
	s.connect((HOST, PORT)) # Open the socket stream (TO CONFIRM)

	while (audio_len):

		s.send(audio_len) # Send the audio in pieces through binary file
		audio_len = a.read(1024) # Read another amount of the audio to be sent

s.close() # Close socket connection
	    
print('Audio sent')