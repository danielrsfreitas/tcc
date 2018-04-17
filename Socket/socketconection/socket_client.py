import socket



with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    sock.connect(("127.0.0.1",50007))
    
    while True:
    	msg = input('Write something:' )
		msg = msg.encode()
        sock.sendall(msg)
        data = sock.recv(1024)

    print('Received', data)