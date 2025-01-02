package main

import (
	"fmt"
	"strings"
)

func main() {
	s := "Welcome"
	fmt.Println("", s)

	result := strings.Split(s, " ")
	fmt.Println("Result:", result[1])
}
