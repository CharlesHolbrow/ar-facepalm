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
        # Get the tracker values
        try:
            tracker_pose = v.devices["tracker_1"].get_pose_quaternion()
        except:
            print("failed to get tracker pose")
        for each in tracker_pose:
            txt += "%.4f " % each
        try:
            client.send_message("/pos", tracker_pose) # send
        except socket.error as e:
            print("Socket Error when sending osc:", e)

        # Get the Controllers if they exist:
        try:
            controller_pose = v.devices["controller_1"].get_pose_quaternion()
        except:
            print("failed to get controller pose")
        for each in controller_pose:
            txt += "%.4f " % each
        if not printed:
            try:
                print(dir(v.devices["controller_1"]))
                printed = True

        print("\r" + txt, end="")
        sleep_time = interval-(time.time()-start)
        if sleep_time>0:
            time.sleep(sleep_time)