package main

import (
	"fmt"
	"net"
	"sync"
)

var running int

const TARGET_IP = "" // control server IP here
const SERVER_IP = "" // target IP here

func main() {
	var threads sync.WaitGroup

	// server socket details and setup
	fmt.Println("Connecting to server...")
	server_addr := TARGET_IP + ":" + "8080"

	// target socket details and setup

	// connect server socket
	server_conn, err := net.Dial("tcp", server_addr)

	// infinite loop
}
