import socket
import sys
import os
import time, csv
import re
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm

#Plot package
plt.style.use('ggplot')

#File constants
server_address = './Sp_data'
data_csv = "data.csv"

#Gradient Approx. Constants
heart_rate_span = [10,250]
pts = 1800
smoothing_size = 20

#Incoming data from the socket
data_f = []


try:
    os.unlink(server_address)
except OSError:
    if os.path.exists(server_address):
        print("No file")
        raise

fig = plt.figure(figsize=(14,8))
ax1 = fig.add_subplot(111)
line1, = ax1.plot(np.arange(0,pts),np.zeros((pts,)), linewidth=4,label='Smoothed Data')
line2, = ax1.plot(0,0,label='Gradient Peaks', marker='o', linestyle='',color='k',markersize=10)
ax1.set_xlabel('Time [s]', fontsize=16)
ax1.set_ylabel('Amplitude', fontsize=16)
ax1.legend(fontsize=16)
ax1.tick_params(axis='both',which='major',labelsize=16)


sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

print(sys.stderr, 'starting up on %s' % server_address)
sock.bind(server_address)
sock.listen(1)



print(sys.stderr, 'waiting for a connection')
connection, client_address = sock.accept()
t_vec, ir_vec, red_vec, y_vals=[],[],[],[]
ir_prev,red_prev=0.0,0.0
while True:
    try:
        print(sys.stderr, 'Connection from', client_address)
        while  True:
            try: 
                data = connection.recv(512)
                if data:
                    while len(y_vals)<pts:
                        curr_line = data.decode("utf-8")
            except KeyboardInterrupt:
                connection.close()
                file_write.close()
                print("foo")
                break
    except KeyboardInterrupt:
        connection.close()
        file_write.close()
        print("bar")



for line in file_read:
    if line[-1] == '\n':
        data_f.append(line[0:-1])
    else:
        data_f.append(line)



for ii in range(3,len(data_f)):
    try:
        curr_data = re.split(":|,", data_f[ii])
    except:
        continue

    if len(curr_data)==3:

        # if abs((float(curr_data[1])-ir_prev)/float(curr_data[1])) >1.01 or\
        #     abs((float(curr_data[2])-red_prev)/float(curr_data[2]))>1.01:
        #     continue

        t_vec.append(float(curr_data[0])/1000000.0)
        y_vals.append(float(curr_data[2]))
        # ir_vec.append(float(curr_data[1]))
        # red_vec.append(float(curr_data[2]))
        # ir_prev = float(curr_data[1])
        # red_prev = float(curr_data[2])
    
t1 = time.time()
samp_rate = 1/np.mean(np.diff(t_vec))
min_time_bw_samps = (60.0/heart_rate_span[1])
y_vals = np.convolve(y_vals, np.ones((smoothing_size,)),'same')/smoothing_size
red_grad = np.gradient(y_vals,t_vec)
red_grad[0:int(smoothing_size/2)+1] = np.zeros((int(smoothing_size/2)+1,))
red_grad[-int(smoothing_size/2)-1:] = np.zeros((int(smoothing_size/2)+1,))

y_vals = np.append(np.repeat(y_vals[int(smoothing_size/2)],int(smoothing_size/2)),y_vals[int(smoothing_size/2):-int(smoothing_size/2)])
y_vals = np.append(y_vals,np.repeat(y_vals[-int(smoothing_size/2)],int(smoothing_size/2)))

line1.set_xdata(t_vec)
line1.set_ydata(y_vals)

ax1.set_xlim([np.min(t_vec),np.max(t_vec)])
if line1.axes.get_ylim()[0]<0.95*np.min(y_vals) or\
    np.max(y_vals)>line1.axes.get_ylim()[1] or\
    np.min(y_vals)<line1.axes.get_ylim()[0]:
                ax1.set_ylim([np.min(y_vals),np.max(y_vals)])
plt.pause(0.001)

peak_locs = np.where(red_grad<-np.std(red_grad))
# if len(peak_locs[0])==0:
#     continue

prev_pk = peak_locs[0][0]
true_peak_locs,pk_loc_span = [],[]
for ii in peak_locs[0]:
    y_pk = y_vals[ii]
    if (t_vec[ii]-t_vec[prev_pk])<min_time_bw_samps:
        pk_loc_span.append(ii)
    else:
        if pk_loc_span==[]:
            true_peak_locs.append(ii)
        else:
            true_peak_locs.append(int(np.mean(pk_loc_span)))
            pk_loc_span = []

    prev_pk = int(ii)

t_peaks = [t_vec[kk] for kk in true_peak_locs]
if t_peaks==[]:
    pass
else:
    print('BPM: {0:2.1f}'.format(60.0/np.mean(np.diff(t_peaks))))
    ax1.set_title('{0:2.0f} BPM'.format(60.0/np.mean(np.diff(t_peaks))),fontsize=24)
# plot gradient peaks on original plot to view how BPM is calculated
scatter_x,scatter_y = [],[]
for jj in true_peak_locs:
    scatter_x.append(t_vec[jj])
    scatter_y.append(y_vals[jj])
    
line2.set_data(scatter_x,scatter_y)

plt.pause(0.001)

savefig = input("Save Figure? ")
if savefig=='y':
    plt.savefig('gradient_plot.png',dpi=300,facecolor=[252/255,252/255,252/255])



# print('Sample Rate: {0:2.1f}Hz'.format(1.0/np.mean(np.abs(np.diff(t_vec)))))


# with open(data_csv, 'a') as f:
#     writer = csv.writer(f,delimiter=',')
#     for t,x,y in zip(t_vec, ir_vec, red_vec):
#         writer.writerow([t,x,y])

# fig = plt.figure(figsize=(12,8))
# ax1 = fig.add_subplot(111)
# ax1.set_xlabel('Time [s]', fontsize=24)
# ax1.set_ylabel('IR Amplitude', fontsize=24, color='#CE445D', labelpad=10)
# ax1.tick_params(axis='both', which='major', labelsize=16)
# plt1 = ax1.plot(t_vec, ir_vec, label='IR', color='#CE445D', linewidth=4)
# ax1_2 = plt.twinx()
# ax1_2.grid('off')
# ax1_2.set_ylabel('Red Amplitude',fontsize=24,color='#37A490',labelpad=10)
# ax1_2.tick_params(axis='y',which='major',labelsize=16)
# plt2 = ax1_2.plot(t_vec,red_vec,label='Red',color='#37A490',linewidth=4)
# lns = plt1 + plt2
# labels = [l.get_label() for l in lns]
# ax1_2.legend(lns, labels, fontsize=16)
# plt.xlim([t_vec[0], t_vec[-1]])
# plt.tight_layout(pad=1.2)
# plt.savefig('max30102_python_example.png',dpi=300,facecolor=[252/255,252/255,252/255])