package main

import (
	"fmt"
	"net"
)

func attack(target string) {

}

func main() {
	target_ip := "10.0.0.12"
	port := "80"
	target_address := target_ip + ":" + port
	// Connect to the server
	conn, err := net.Dial("tcp", target_address)
	if err != nil {
		fmt.Println(err)
		return
	}

	request := fmt.Sprintf("GET / HTTP/1.1\r\nHost: %s\r\n\r\n", target_ip)

	// Send some data to the server
	_, err = conn.Write([]byte(request))
	if err != nil {
		fmt.Println(err)
		return
	}

	// Close the connection
	conn.Close()
}
