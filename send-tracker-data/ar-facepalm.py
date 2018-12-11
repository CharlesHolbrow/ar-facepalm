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
            for each in tracker_pose:
                txt += "%.4f " % each
            try:
                client.send_message("/pos", tracker_pose) # send
            except socket.error as e:
                print("Socket Error when sending /pos osc:", e)
        except:
            print("failed to get tracker pose")
            txt += "failed to get tracker pose - "

        # Get the Controllers if they exist:
        txt += " --- "
        try:
            controller = controller_pose = v.devices["controller_1"]
            controller_pose = v.devices["controller_1"].get_pose_quaternion()
            # I believe controller.index should be the argument to
            # getControllerState, but I'm not totally sure.
            # controller_state = v.vr.getControllerState(controller.index)[1] # this call ruins everything
            # trigger = controller_state.rAxis[1].x
            # pad_x = controller_state.rAxis[0].x
            # pad_y = controller_state.rAxis[0].y
            # controller_pose.append(trigger)
            if not printed:
                printed = True
                print(dir(controller))
            try:
                client.send_message("/controller", controller_pose) # send
            except socket.error as e:
                print("Socket Error when sending /controller osc:", e)
            for each in controller_pose:
                txt += "%.4f " % each
        except:
            print('Failed to get controller pose')
            txt += "failed to get Controller Pose"

        print("\r" + txt, end="")
        sleep_time = interval-(time.time()-start)
        if sleep_time>0:
            time.sleep(sleep_time)