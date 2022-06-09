#!/usr/bin/python3

import socket
import os

COLLECT_AMT = 1800
FIFO = "myfifo"
server_address = './Sp_data'

#Write all incoming data
fw = open("data.txt", "w")
data_flag = open(FIFO, "w")

#Create the Unix Domain Socket
try:
    os.unlink(server_address)
except OSError:
    if os.path.exists(server_address):
        print("No file")
        raise

#Connect to the socket
sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
sock.bind(server_address)
sock.listen(1)
print('waiting for a connection')
connection, client_address = sock.accept()

#Amount of recorded data received from UDS
rcrd_count = 0
print('Connection accepted. Begin data collection.')
while True:
    if rcrd_count < COLLECT_AMT:
        data = connection.recv(128)
        if data:
            fw.write(data.decode("utf-8"))
            rcrd_count+=1
    else:
        fw.flush()
        data_flag.write("1")
        data_flag.flush()
        rcrd_count = 0
            
    