import time
import socket
from pythonosc import osc_message_builder
from pythonosc import udp_client

if __name__ == "__main__":
    ip = '18.85.27.78'
    #ip = 'charless-mbp.media.mit.edu'
    port = 6969
    client = udp_client.SimpleUDPClient(ip, port)

    i = 0
    while True:
        try:
            client.send_message("/hi", i)
        except socket.error as e:
            print("Socket Error when sending osc:", e)
        i = (i + 1) % 200
        print(i)
        time.sleep(0.25)
