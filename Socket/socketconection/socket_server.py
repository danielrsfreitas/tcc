"""

Type in terminal:

    python3 socket_server.py 


"""

import socket

HOST = '127.0.0.1'
PORT = 50007
address =  (HOST,PORT)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock: # SOCK_STREAM is commonly used for TCP based transmission. AF_INET is used for ipv4 kind of address.

	
    sock.bind(address)
    sock.listen(1) # Allows only one client
    conn, addr = sock.accept()
    
    a = open("audio_received1.wav",'wb') # Create a audiofile and open as binary

    with conn: # make sure connection stands
        print('Connected to: ', addr)
        while True:

            #data = conn.recv(1024)
            data = conn.recv(1024)

            while(data):
            	a.write(data)
            	data = conn.recv(1024)

            #if not data: break
            #conn.sendall(data)
            #print('Received:', data)
            
conn.close()   