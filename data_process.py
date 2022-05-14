import socket
import sys
import os
import time, csv
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm

#Plot package
plt.style.use('ggplot')

server_address = './Sp_data'
f = open("data.txt", "w")
datafile = open("data.txt", "r")

data_f = []


try:
    os.unlink(server_address)
except OSError:
    if os.path.exists(server_address):
        print("No file")
        raise

sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

print(sys.stderr, 'starting up on %s' % server_address)
sock.bind(server_address)

sock.listen(1)


print(sys.stderr, 'waiting for a connection')
connection, client_address = sock.accept()
try:
    print(sys.stderr, 'Connection from', client_address)
    while  True:
        try: 
            data = connection.recv(64)
            if data:
                data_f.append(data.decode("utf-8"))
        except KeyboardInterrupt:
            connection.close()
            f.close()
            print("foo")
            break
except KeyboardInterrupt:
    connection.close()
    f.close()
    print("bar")

print(data_f)
# t_vec, ir_vec, red_vec=[],[],[]
# ir_prev,red_prev=0.0,0.0
# for ii in range(3,len(data_f)):
#     try:
        
    
