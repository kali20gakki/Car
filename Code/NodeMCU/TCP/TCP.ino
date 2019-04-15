#include <ESP8266WiFi.h>
#include <String.h>
#define    LED   2


char terminator = '?';  // 终止字符
String comdata = "";         // com 数据缓冲

const char *ssid     = "zhangwei";
const char *password = "12345678";
const char *host     = "192.168.179.82";
const int   Port     =  8888;

char buff[3];
int num;

WiFiClient client; //声明一个客户端对象，用于与服务器进行连接

void setup()
{
  Serial.begin(9600);
  pinMode(LED,OUTPUT);
  WiFi.begin(ssid, password);
  

  /* 连接WiFi */
  while (WiFi.status() != WL_CONNECTED)
  {
     delay(50);
  }
  Serial.println("1");  // 连接WIFI成功
   /* 连接IP */
    while (!client.connected())//几个非连接的异常处理
    {
        if (!client.connect(host, Port))
        {
            delay(50); 
        }
    }
  Serial.println("2");  // 连接IP成功
}

void loop() 
{
  /* Server发来消息 */
   if (client.available())
   {     
          num = client.readBytes(buff,3);
          Serial.println(buff); // 转发给串口
   }
   /* 32发来消息 */
   if (Serial.available())
   {
       comdata = Serial.readStringUntil('?');
       client.println(comdata); // 转发给Srever
   }
}
