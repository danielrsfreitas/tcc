import socket

HOST = '127.0.0.1'
PORT = 50007
address =  (HOST,PORT)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock: # SOCK_DGRAM is commonly used for UDP based transmission. AF_INET is used for ipv4 kind of address.

    sock.bind(address)
    sock.listen(1)
    conn, addr = sock.accept() 
    with conn: # make sure connection stands
        print('Connected by', addr)
        while True:
            data = conn.recv(1024)
            if not data: break
            
            conn.sendall(data)
            print('Received:', data)
   