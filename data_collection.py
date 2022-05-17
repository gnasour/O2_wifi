import socket
import sys
import os

server_address = './Sp_data'

fw = open("data.txt", "w")

try:
    os.unlink(server_address)
except OSError:
    if os.path.exists(server_address):
        print("No file")
        raise

sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

sock.bind(server_address)
sock.listen(1)

print('waiting for a connection')
connection, client_address = sock.accept()