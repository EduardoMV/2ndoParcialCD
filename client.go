package main

import (
    "fmt"
    "net"
    "os"
    "sync"
    "net/url"
    "time"
    "github.com/zserge/lorca"
)

func listen_to_socket(canvasStreamPtr *string, connection *net.UDPConn, waitingGroup *sync.WaitGroup) {
    defer waitingGroup.Done()
    fmt.Println("listening...")
    for {
	var buffer [5120]byte
	_, _, err := connection.ReadFromUDP(buffer[0:])
	if err != nil {
	    fmt.Println(err)
	    return
	}
	//fmt.Println("Server: ", string(buffer[0:]))
	*canvasStreamPtr = string(buffer[0:])
	
    }
}

func update_server(canvasStreamPtr *string, connection *net.UDPConn, waitingGroup *sync.WaitGroup) {
    defer waitingGroup.Done()
    for {
	var data = []byte(*canvasStreamPtr)

	_, err := connection.Write(data[:])
	if err != nil {
	    fmt.Println(err)
	    return
	}
	time.Sleep(time.Second)
    }
}

func listen_for_updates(connection *net.UDPConn, waitingGroup *sync.WaitGroup) {
    defer waitingGroup.Done()
    for {
	_, err := connection.Write([]byte("update"))
	if err != nil {
	    fmt.Println(err)
	    return
	}
	time.Sleep(100 * time.Millisecond)
    }
}

func openUI(canvasStreamPtr *string, newCanvas *string, waitingGroup *sync.WaitGroup){
    defer waitingGroup.Done()
    
    
    data, err := os.ReadFile("./index.html") 
    if err != nil {
	panic(err)
    }

    html := string(data[:])
    
    ui, err := lorca.New("data:text/html," + url.PathEscape(html), "", 480, 320)
    if err != nil {
	fmt.Println(err);
	return;
    }
    
    defer ui.Close()
    for{
	fun := fmt.Sprintf(`updateCanvas("%s")`, *newCanvas)
	res := ui.Eval(fun)

	canvas := ui.Eval(`getCanvasData()`).String()
	*canvasStreamPtr = canvas;

	fmt.Println(res);

    }

    <- ui.Done()
}

func main() {
    var wg sync.WaitGroup
    var canvasStream string;
    var newCanvasData string;
    canvasPtr := &canvasStream;

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
    go openUI(canvasPtr, &newCanvasData, &wg)

    wg.Add(1)
    go update_server(canvasPtr, conn, &wg)

    wg.Add(1)
    go listen_to_socket(&newCanvasData, conn, &wg)
    
    wg.Add(1)
    go listen_for_updates(conn, &wg)

    wg.Wait()
}
