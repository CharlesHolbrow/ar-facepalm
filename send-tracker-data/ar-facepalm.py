import triad_openvr
import time
import sys
import time
import socket
from pythonosc import osc_message_builder
from pythonosc import udp_client

ip = '18.85.27.78'
#ip = 'charless-mbp.media.mit.edu'
port = 6969
client = udp_client.SimpleUDPClient(ip, port)

v = triad_openvr.triad_openvr()
v.print_discovered_objects()

if len(sys.argv) == 1:
    interval = 1/190
elif len(sys.argv) == 2:
    interval = 1/float(sys.argv[0])
else:
    print("Invalid number of arguments")
    interval = False
    
printed = False
if interval:
    while(True):
        start = time.time()
        txt = ""
        try:
            pose = v.devices["tracker_1"].get_pose_quaternion()
        except:
            print("failed to get vals")
        if not printed:
            printed = True
            print(dir(v.devices["tracker_1"]))
        for each in pose:
            txt += "%.4f" % each
            txt += " "
        print("\r" + txt, end="")

        try:
            client.send_message("/pos", pose)
        except socket.error as e:
            print("Socket Error when sending osc:", e)

        sleep_time = interval-(time.time()-start)
        if sleep_time>0:
            time.sleep(sleep_time)