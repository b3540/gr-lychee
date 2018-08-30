package main

import (
    "flag"
    "fmt"
    "os"
    "time"
    "bytes"
    "image"
    "image/jpeg"
    MQTT "github.com/eclipse/paho.mqtt.golang"
    "github.com/line/line-bot-sdk-go/linebot"
)

var defaultMQTTHandler MQTT.MessageHandler = func(client MQTT.Client, msg MQTT.Message) {
    fmt.Printf("Received MQTT message on topic: %s\n", msg.Topic())

    byteJpeg := msg.Payload()
    img, _, err := image.Decode(bytes.NewReader(byteJpeg))
    if err != nil {
        fmt.Println(err)
        os.Exit(1)
    }

    day := time.Now()
    const layout = "20060102150405"

    // output jpg image file
    outputimg := "output" + day.Format(layout) + ".jpg"
    out, err := os.Create(outputimg)
    if err != nil {
        fmt.Println(err)
        os.Exit(1)
    }

    // output jpeg to file
    err = jpeg.Encode(out, img, nil) 
    if err != nil {
        fmt.Println(err)
        os.Exit(1)
    }

    bot, err := linebot.New("", "")
    if err != nil {
    }

    if _, err := bot.PushMessage("********", linebot.NewImageMessage("https://endpoint/" + outputimg, "https://endpoint/" + outputimg)).Do(); err != nil {
    }
}

func main() {
    opts := MQTT.NewClientOptions().AddBroker("tcp://mqtt server:1883").SetClientID("sensorlogger")
    opts.SetDefaultPublishHandler(defaultMQTTHandler)

    client := MQTT.NewClient(opts)
    if token := client.Connect(); token.Wait() && token.Error() != nil {
       panic(token.Error())
    }

    qos := flag.Int("qos", 1, "The QoS to subscribe to messages at")
    if token := client.Subscribe("image/output", byte(*qos), nil); token.Wait() && token.Error() != nil {
        panic(token.Error())
    }

    for {
        time.Sleep(1 * time.Second)
    }
    client.Disconnect(250)
}

