package main

import (
	"fmt"
	"os"
)

func addUserToDB(userData []byte) {
	file, err := os.ReadFile("db.txt")
	if err != nil {
		panic(err)
	}

	file = append(file, userData...)
	err = os.WriteFile("db.txt", file, 0666)
	if err != nil {
		panic(err)
	}
	fmt.Println("Usuario Agregado!")

}
