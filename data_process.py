import socket
import sys
import os
import time, csv
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm

server_address = './Sp_data'
f = open("data.txt", "r")

plt.style.use('ggplot')

# try:
#     os.unlink(server_address)
# except OSError:
#     if os.path.exists(server_address):
#         print("No file")
#         raise

# sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

# print(sys.stderr, 'starting up on %s' % server_address)
# sock.bind(server_address)

# sock.listen(1)

# while True:
#     print(sys.stderr, 'waiting for a connection')
#     connection, client_address = sock.accept()
#     try:
#         print(sys.stderr, 'Connection from', client_address)
#         while  True:
#             try: 
#                 data = connection.recv(512)
#                 if data:
#                     f.write(data.decode("utf-8"))
#             except:
#                 connection.close()
#                 f.close()
#                 break
#     except KeyboardInterrupt:
#         connection.close()
#         f.close()
#         break
#     break

all_data = []
for x in f:
    all_data.append(x)

t_vec,ir_vec,red_vec = [],[],[]
ir_prev,red_prev = 0.0,0.0
for ii in range(3, len(all_data)):
    try:
        curr_data = all_data[ii]