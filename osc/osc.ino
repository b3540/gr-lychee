#include <SPI.h>
#include <ESP32Interface.h>
#include <ArdOSC.h>

int  serverPort  = 8080;
int t=100;

ESP32Interface wifi;
#define WIFI_SSID "ssid"
#define WIFI_PW "pwd"

void setup(){ 
  Serial.begin(9600);
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
  
  delay(1000);
}
  
void sendOSCmessage() {
  OSCClient client;
  int destPort=10000;
  char *sendip = "192.168.2.1";

  char str1[]="HOGEHOGE";
  int v1=0;
  float v2=0.0;
  OSCMessage global_mes;

  global_mes.setAddress(sendip, destPort);
  global_mes.beginMessage("/osc");
  global_mes.addArgInt32(v1);
  global_mes.addArgFloat(v2);
  global_mes.addArgString(str1);

  client.setStack(&wifi);
  client.send(&global_mes);
}

void loop() {  
  sendOSCmessage();
  Serial.println("loop...");
  delay(100);
}

