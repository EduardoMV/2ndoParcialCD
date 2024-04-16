package main

import (
	"fmt"
	"net"
	"os"
	"sync"
	"time"
)

func listen_to_socket(connection *net.UDPConn, waitingGroup sync.WaitGroup){
    defer waitingGroup.Done()
    fmt.Println("listening...")
    for {
	    var buffer [512]byte
	    _, _, err := connection.ReadFromUDP(buffer[0:])
      
      if(err != nil){
	      fmt.Println(err)
	      return
	    }
	    fmt.Println("Server: ", string(buffer[0:]))
    }
}

func ping_server(connection *net.UDPConn, waitingGroup sync.WaitGroup){
    defer waitingGroup.Done()

    var counter int = 1;
    for{
	    message := fmt.Sprintf("Ping: %d", counter)
	    _, err := connection.Write([]byte(message))

	    if(err != nil){
	      fmt.Println(err)
	      return
	    }
	    time.Sleep(1 * time.Second);
	    counter += 1
    }
}

func main() {
    var wg sync.WaitGroup
    // Resolve the string address to a UDP address
    udpAddr, err := net.ResolveUDPAddr("udp", "127.0.0.1:8080")

    if err != nil {
	    fmt.Println(err)
	    os.Exit(1)
    }

    // Dial to the address with UDP
    conn, err := net.DialUDP("udp", nil, udpAddr)


    if err != nil {
	    fmt.Println(err)
	    os.Exit(1)
    }
    wg.Add(1)
    go ping_server(conn, wg)
    wg.Add(1)
    go listen_to_socket(conn, wg)
    wg.Wait()
}
