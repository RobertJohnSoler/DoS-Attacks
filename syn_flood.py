from scapy.all import *
from random import getrandbits

target_ip = ''
target_port = 80

ip = IP(src=RandIP(''), dst = target_ip)