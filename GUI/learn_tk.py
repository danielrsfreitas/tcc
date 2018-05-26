import tkinter as tk
import socket


def insert(T, text):
	T.insert(tk.END, text)
	print("chama")

def socket_conn():
	HOST = '127.0.0.1'
	PORT = 50007
	address =  (HOST,PORT)
	print("Chama")
	#insert(T,'Socket on...')
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock: # SOCK_STREAM is commonly used for TCP based transmission. AF_INET is used for ipv4 kind of address.

		
	    sock.bind(address)
	    sock.listen(1) # Allows only one client
	    conn, addr = sock.accept()
	    
	    a = open("audio_received1.wav",'wb') # Create a audiofile and open as binary

	    with conn: # make sure connection stands
	        print("Connected to :", addr)
	        #insert(T,'Connected to: '+str(addr))
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
# Creating a TK interface with a text console
root = tk.Tk()
T = tk.Text(root, height=20, width=60)
T.pack(padx=30,pady=30,side=tk.TOP)
frame = tk.Frame(root)
frame.pack()

button_play = tk.Button(frame,
					text="Connect",
					command=socket_conn) 
button_play.pack(side=tk.TOP)

#if (input()):
#	insert(T, "DANIEL")
   

root.mainloop()