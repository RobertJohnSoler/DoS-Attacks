package main

import (
	"fmt"
	"net"
)

// function to attack

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

// function ot send message via socket connection
