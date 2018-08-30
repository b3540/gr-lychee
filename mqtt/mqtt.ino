#include <Arduino.h>
#include "TCPSocket.h"
#include "ESP32Interface.h"
#include <PubSubClient.h>
#include <Camera.h>
#include <opencv.hpp>


#define APP_WIFI_SSID "ssid"
#define APP_WIFI_PASSWORD "pwd"
ESP32Interface wifi;

const char* mqtt_server = "mqtt server";

PubSubClient client;
TCPSocket socket;
long lastMsg = 0;
char msg[50];
int value = 0;
using namespace cv;


#define IMAGE_HW 320
#define IMAGE_VW 240

Camera camera(IMAGE_HW, IMAGE_VW);

void canny() {
  Mat img_raw(IMAGE_VW, IMAGE_HW, CV_8UC2, camera.getImageAdr());
  Mat src, dst;
  cvtColor(img_raw, src, COLOR_YUV2GRAY_YUYV); //covert from YUV to GRAY
  Canny(src, dst, 50, 150); // Canny
  size_t jpegSize = camera.createJpeg(IMAGE_HW, IMAGE_VW, dst.data, Camera::FORMAT_GRAY);
  client.publish("image/output", camera.getJpegAdr(), jpegSize);
}

void drawing() {
  static int x = 0, y = 0, ax = 10, ay = 10;
  static long time = millis();
  Scalar red(0, 0, 255), green(0, 255, 0), blue(255, 0, 0);
  Scalar yellow = red + green;
  Scalar white = Scalar::all(255);
  Scalar pink = Scalar(154, 51, 255);

  Mat img_raw(IMAGE_VW, IMAGE_HW, CV_8UC2, camera.getImageAdr());
  Mat src;
  cvtColor(img_raw, src, COLOR_YUV2BGR_YUYV); //covert YUV to RGB

  if((millis() - time) > 50){
    x+=ax; y+=ay;
    if(x > (src.cols - 10) || x < 10){
      ax*=-1;
    }
    if(y > (src.rows - 10) || y < 10){
      ay*=-1;
    }
}

  line(src, Point(10, 10), Point(src.cols - 10, 10), blue, 3, LINE_AA); //Line
  line(src, Point(10, src.rows - 10), Point(src.cols - 10, src.rows - 10), blue, 3, LINE_AA); //Line
  rectangle(src, Point(10, 30), Point(src.cols - 10, 60), white, FILLED);
  putText(src, "Gadget Renesas", Point(15, 55), FONT_HERSHEY_COMPLEX, 1, pink, 2);
  circle(src, Point(x, y), 10, yellow, FILLED);

  stringstream ss;
  ss << x << ", " << y;
  putText(src, ss.str(), Point(10, src.rows - 20), FONT_HERSHEY_SCRIPT_SIMPLEX, 1, white, 1);
  size_t jpegSize = camera.createJpeg(IMAGE_HW, IMAGE_VW, src.data, Camera::FORMAT_RGB888);
  client.publish("image/output", camera.getJpegAdr(), jpegSize);
}

void setup() {
  Serial.begin(115200);
  camera.begin();

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {
    printf("\r\nConnecting...\r\n");
    int ret = wifi.connect(APP_WIFI_SSID, APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    if (ret != 0) {
        printf("\r\nConnection error\r\njmghmj");
        while(1);
    }
 
    printf("Success\r\n\r\n");
    printf("MAC: %s\r\n", wifi.get_mac_address());
    printf("IP: %s\r\n", wifi.get_ip_address());
    printf("Netmask: %s\r\n", wifi.get_netmask());
    printf("Gateway: %s\r\n", wifi.get_gateway());
    printf("RSSI: %d\r\n\r\n", wifi.get_rssi());
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  
  if (strcmp(topic, "opencv/") == 0 && length == 1) {
    if (payload[0] == '1') {
      canny();
    } else if (payload[0] == '2') {
      drawing();
    }
     Serial.print("publish finish");
  } else if (strcmp(topic, "image/") == 0 && length == 1) {
    size_t jpegSize = camera.createJpeg();
    client.publish("image/output", camera.getJpegAdr(), jpegSize);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // set non blocking and timeout = 1500ms
    socket.open(&wifi);
    socket.set_blocking(false);
    socket.set_timeout(1500);
    client.setClient(&socket);

    if (client.connect("GR-LYCHEE Client")) {
      Serial.println("connected");
      client.publish("outTopic", "hello world");
      client.subscribe("inTopic/");
      client.subscribe("opencv/");
      client.subscribe("image/");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int sensorVal = 0;
  
  for (int i = 0; i < 100; i++)
    sensorVal += analogRead(A0);
  sensorVal /= 100;
  Serial.print("ir : ");
  Serial.println(sensorVal);

  if (sensorVal >= 200) {
    size_t jpegSize = camera.createJpeg();
    client.publish("image/output", camera.getJpegAdr(), jpegSize);
  }

  delay(100);
}
