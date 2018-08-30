#include <Arduino.h>
#include <Camera.h>
#include <SD.h>
#include <HTTPServer.h>
#include <mbed_rpc.h>
#include <SdUsbConnect.h>
#include <ESP32Interface.h>
 
ESP32Interface wifi;
#define WIFI_SSID "ssid"
#define WIFI_PW "pwd"
 
Camera camera(320, 240);
SdUsbConnect storage("storage");
 
static int snapshot_req(const char ** pp_data) {
  size_t size = camera.createJpeg();
  *pp_data = (const char*)camera.getJpegAdr();
  return size;
}
 
void setup(void) {
 
  Serial.begin(9600);
  Serial.println("Starts.");
 
  // SD & USB
  Serial.print("Finding strage..");
  storage.wait_connect();
  Serial.println("done");
 
  camera.begin();
 
  Serial.print("Connecting Wi-Fi..");
  wifi.connect(WIFI_SSID, WIFI_PW, NSAPI_SECURITY_WPA_WPA2);
  Serial.println("done");  
 
  Serial.print("MAC Address is ");
  Serial.println(wifi.get_mac_address());
  Serial.print("IP Address is ");
  Serial.println(wifi.get_ip_address());
  Serial.print("NetMask is ");
  Serial.println(wifi.get_netmask());
  Serial.print("Gateway Address is ");
  Serial.println(wifi.get_gateway());
  Serial.println("Network Setup OK\r\n");
 
  SnapshotHandler::attach_req(&snapshot_req);
  HTTPServerAddHandler<SnapshotHandler>("/camera"); //Camera
  FSHandler::mount("/storage", "/");
  HTTPServerAddHandler<FSHandler>("/");
  HTTPServerAddHandler<RPCHandler>("/rpc");
  HTTPServerStart(&wifi, 80);
 
}
 
void loop() {
}
