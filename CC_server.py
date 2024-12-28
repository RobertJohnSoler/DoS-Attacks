import socket
import threading
from threading import Event
import time

# A function to print a given message in a fancy way
def print_waitmsg(wait_msg, stop_event: Event):
    wait_msgs = [
        f"{wait_msg}   ",
        f"{wait_msg}.   ",
        f"{wait_msg}..   ",
        f"{wait_msg}...   "
    ]
    while not stop_event.is_set():
        for msg in wait_msgs:
            print(msg, end='\r', flush=True)
            time.sleep(0.3)


# function to handle a client connection if any
def handle_client(conn: socket.socket, addr, stop_event: Event):

   
    print("Got connection from ", addr)
    conn.settimeout(1)

    
    while not stop_event.is_set():
        try:
            command = input("> ")
            print("command is ", command.encode())
            conn.send(command.encode())
        except socket.timeout:  
            # use periodic time outs to check if the user has pressed ctrl+c, 
            # because recv() is a blocking call and it keeps ctrl+c in the queue until it a connection finally comes in
            continue
        except :
            print("")
            print("Client must have disconnected.")
            break



if __name__ == "__main__":
    
    # event that can be used to tell all threads to stop running
    stop_event = threading.Event() 

    # socket creation and initialization
    s = socket.socket()
    print("")
    print("Socket created.")
    s.settimeout(1)
    s.bind(('0.0.0.0', 8080))
    print("Socket binded to 8080")
    print("")

    # listen for any incoming connections from clients
    s.listen(5)

    try:

        # start a thread to do the fancy printing of the message "Socket is listening...,"
        # waitmsg_thread = threading.Thread(target=print_waitmsg, args=("Socket is listening", stop_event))
        # waitmsg_thread.start()

        # as long as the socket is running and the stop event is not set, keep accepting client connections
        while s and not stop_event.is_set():
            try:
                connection, address = s.accept()
                # for every client that connects, start a thread to handle it
                client_thread = threading.Thread(target=handle_client, args=(connection, address, stop_event))
                client_thread.start()
            except socket.timeout:  
                # use periodic time outs to check if the user has pressed ctrl+c, 
                # because accept() is a blocking call and it keeps ctrl+c in the queue until it a connection finally comes in
                continue
    except KeyboardInterrupt:
        # clean everything up once the user presses ctrl+c
        stop_event.set()
        print("")
        print("Closing socket...")
        s.close()
        print("Closing server...")
        print("")
        exit()       