
#include <WiFi.h>
//============Credenciales de WiFi=============
const char* ssid     = "EVER.";
const char* password = "Everconexion123";
//============Servidor WEB puerto 80===========
WiFiServer server(80);
//================Variables====================
int pir = 22;
int buzzer = 23;
int rele = 25;
String header; //Variable para guardad el HTTP request
String estadoRele = "off";

//==================CODIGO HTML================
String pagina = "<!DOCTYPE html>"
"<html>"
"<head>"
"<meta charset='utf-8' />"
"<title>Servidor Web Casa Segura con ESP32</title>"
"</head>"
"<body>"
"<center>"
"<h1>Servidor Web Casa Segura con ESP32</h1>"
"<p><a href='/on'><button style='height:25px;width:150px'>ENCENDER LUZ</button></a></p>"
"<p><a href='/off'><button style='height:25px;width:150px'>APAGAR LUZ</button></a></p>"
"<p><a href='/active'><button style='height:25px;width:300px'>ACTIVAR SEGURIDAD REMOTA</button></a></p>"
"<p><a href='/desactive'><button style='height:25px;width:300px'>DESACTIVAR SEGURIDAD REMOTA</button></a></p>"
"</center>"
"</body>"
"</html>";

void setup()
{
    Serial.begin(115200);
    pinMode(pir, INPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(rele, OUTPUT);
    digitalWrite(rele, LOW);
    Serial.print("Conectando a ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);//Conexion WIFI
    while(WiFi.status()!=WL_CONNECTED){
      delay(500);
      Serial.print(".");
    }
    // We start by connecting to a WiFi network

    Serial.println("");
    Serial.println("Conectado a red Wifi.");
    Serial.print("Dirección IP");
    Serial.println(WiFi.localIP());
    server.begin();
    }


void loop(){
 WiFiClient client = server.available();   // escucha a los clientes entrantes

  if (client) {                             // if you get a client,
    Serial.println("Nuevo Cliente");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            //Encender y apagar foco
            if (header.indexOf("GET /on") >= 0){
              Serial.println("FOCO on");
              estadoRele = "on";
              digitalWrite(rele, HIGH);
            } else if (header.indexOf("GET /off") >= 0){
              Serial.println("FOCO off");
              estadoRele = "off";
              digitalWrite(rele, LOW);
            }
            else if (header.indexOf("GET /active") >= 0){
              Serial.println("Seguridad Remota activada");
              while (digitalRead(pir)!=HIGH){
                estadoRele = "off";
                digitalWrite(rele, LOW);
                digitalWrite(buzzer, LOW);
              }
              if (digitalRead(pir) == HIGH){
                estadoRele = "on";
                digitalWrite(rele, HIGH);
                digitalWrite(buzzer, HIGH);
                delay(2000);
              }
            }
            else if (header.indexOf("GET /desactive") >= 0){
              Serial.println("Seguridad Remota Desactivada");
              estadoRele = "off";
              digitalWrite(rele, LOW);
              digitalWrite(buzzer, LOW);
              }
            // The HTTP response ends with another blank line:
            client.println(pagina);
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        }
        }
    header = "";
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
    Serial.println("");
    }
  }
