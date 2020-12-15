// Copyright 2015 The Gorilla WebSocket Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// +build ignore

package main

import (
	"encoding/json"
	"flag"
	"fmt"
	"log"
	"net/url"
	"os"
	"os/signal"

	"github.com/gorilla/websocket"
)

var addr = flag.String("addr", "localhost:1234", "http service address")

//var addr = flag.String("addr", "81.68.86.146:9699", "http service address")
//var addr = flag.String("addr", "180.76.163.81:9699", "http service address")

type Request struct {
	Event string    `json:"event"`
	Data  *SyncData `json:"data"`
}

type SyncData struct {
	SN       string `json:"sn"`
	Mac      string `json:"mac"`
	Ip       string `json:"ip"`
	DevModel string `json:"devModel"`
	DevType  string `json:"devType"`
}

func main() {
	flag.Parse()
	log.SetFlags(0)

	interrupt := make(chan os.Signal, 1)
	signal.Notify(interrupt, os.Interrupt)

	u := url.URL{Scheme: "ws", Host: *addr, Path: ""}
	fmt.Println("connecting to", u.String())

	c, _, err := websocket.DefaultDialer.Dial(u.String(), nil)
	if err != nil {
		fmt.Println("dial:", err)
		return
	}
	defer c.Close()

	fmt.Println("dail success")
	request := &Request{}
	request.Data = &SyncData{}
	request.Data.SN = "7kssn"
	request.Data.Mac = "ss-uu-2d-8c"
	request.Data.Ip = "192.168.1.105"
	request.Data.DevModel = "PMs21.5"
	request.Data.DevType = "ES-2.5"
	request.Event = "cs_syncinfo"

	senddata, _ := json.Marshal(request)
	fmt.Println("senddata is ", senddata)
	err = c.WriteMessage(websocket.TextMessage, senddata)
	if err != nil {
		fmt.Println("write:", err)
		return
	}

	fmt.Println("send success")
	//接收offer
	message := make([]byte, 1024)
	_, message, err = c.ReadMessage()
	if err != nil {
		fmt.Println("read:", err)
		return
	}
	fmt.Printf("recv: %s", message)

	message = make([]byte, 1024)
	_, message, err = c.ReadMessage()
	if err != nil {
		fmt.Println("read:", err)
		return
	}
	fmt.Printf("recv: %s", message)
	/*
		req = &model.RequestStruct{}
		err = json.Unmarshal(message, req)
		if err != nil {
			log.Println("json unmarshal failed , error is ", err.Error())
			continue
		}*/
	fmt.Println("...................................")
}
