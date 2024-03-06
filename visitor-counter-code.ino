// Use the Parola library to scroll text on the display
//
// Demonstrates the use of the scrolling function to display text received
// from the serial interface
//
// User can enter text on the serial monitor and this will display as a
// scrolling message on the display.
// Speed for the display is controlled by a pot on SPEED_IN analog in.
// Scrolling direction is controlled by a switch on DIRECTION_SET digital in.
// Invert ON/OFF is set by a switch on INVERT_SET digital in.
//
// UISwitch library can be found at https://github.com/MajicDesigns/MD_UISwitch
// MD_MAX72XX library can be found at https://github.com/MajicDesigns/MD_MAX72XX
//

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Turn on debug statements to the serial output
#define DEBUG 0

const char *ssid = "SCRC_LAB_IOT";
const char *password = "Scrciiith@123";

#if DEBUG
#define PRINT(s, x) { Serial.print(F(s)); Serial.print(x); }
#define PRINTS(x) Serial.print(F(x))
#define PRINTX(x) Serial.println(x, HEX)
#else
#define PRINT(s, x)
#define PRINTS(x)
#define PRINTX(x)
#endif

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   4
#define DATA_PIN  15
#define CS_PIN    2
#define URL "http://192.168.1.20:5000/visitor_count"
// HARDWARE SPI
// MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// SOFTWARE SPI
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);




// Scrolling parameters

uint8_t scrollSpeed = 70;    // default frame delay value
textEffect_t scrollEffect = PA_SCROLL_LEFT;
textPosition_t scrollAlign = PA_LEFT;
uint16_t scrollPause = 2500; // in milliseconds

// Global message buffers shared by Serial and Scrolling functions
#define	BUF_SIZE	75
char curMessage[BUF_SIZE] = { "" };
char newMessage[BUF_SIZE] = { "Hello! Enter new message?" };
String payload;


void wifi_init() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC ADDRESS : ");
  Serial.println(WiFi.macAddress());
}


void recieve_data(){
  HTTPClient http;
  http.begin(URL);
  int httpCode = http.GET();
  if ((httpCode > 0) && (httpCode < 300)){
    payload = http.getString();
  }
  else{
    Serial.println("BAD REQUEST");
  }
  Serial.println(payload);
  http.end();
}


void setup()
{
  Serial.begin(115200);
  Serial.print("\n[Parola Scrolling Display]\nType a message for the scrolling display\nEnd message line with a newline");
  wifi_init();
  P.begin();
  recieve_data();
  P.displayText(payload.c_str(), scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
  P.displayReset();
}

void loop()
{
  if (P.displayAnimate())
  {P.displayText(payload.c_str(), scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
  P.displayText(payload.c_str(), scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
  P.displayText(payload.c_str(), scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
  P.displayText(payload.c_str(), scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
  P.displayText(payload.c_str(), scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
  recieve_data();}
}

