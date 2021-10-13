
/*
  Pinout:
  SD card attached to SPI bus as follows:
  - MOSI - pin51
  - MISO - pin 50
  - CLK - pin 52
  - CS - pin 53

*/

#include <TinyGPS++.h>
#include <SPI.h>
#include <SD.h>

#define GPS_Serial Serial3
#define GSM_Serial Serial2
#define wifi_Serial Serial1
#define interruptPin 21

String NUMBERS[] = {"+201147468880", "+201277430894", "+201111666252", "+201229942419", "+201028570068"};

bool DEBUG = true;   //show more logs
int responseTime = 10; //communication timeout
String incoming_message = "", sms_body = "", sms_number = ""; //recieved
String TextMessage = "" ; //to send
String message = "";
boolean new_message = 0;
boolean sos_present = false;
double longg, latt;
char long_[20], lat_[20];
char new_long[20], new_lat[20];
String old_long;
String old_lat;
String Time = "";
char car_speedc[7];
const int CS = 53;
String filename = "location.txt";
byte s_led = 10;
byte r_led = 11;
byte car_led = 7;
byte car_motor = 12;
byte loca_led = 9;
byte first_loca_led = 8;
byte recieve_wifi_led = 6;
byte send_wifi_led = 5;
byte sd_led = 4;
byte sms_check_led = 3;
String la, lo;
String sm="";

File LOCAT;
TinyGPSPlus gps;

void setup()
{
  Serial.print("Initializing SD card... ");
  if (!SD.begin(CS))
  {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization success!");
  //Initialization induction leds && motors
  pinMode(s_led, OUTPUT);
  pinMode(r_led, OUTPUT);
  pinMode(car_led, OUTPUT);
  pinMode(car_motor, OUTPUT);
  pinMode(loca_led, OUTPUT);
  pinMode(first_loca_led, OUTPUT);
  pinMode(recieve_wifi_led, OUTPUT);
  pinMode(send_wifi_led, OUTPUT);
  pinMode(sd_led , OUTPUT);
  pinMode(sms_check_led, OUTPUT);
  digitalWrite(s_led, LOW);
  digitalWrite(r_led, LOW);
  digitalWrite(car_led, HIGH);
  digitalWrite(car_motor, HIGH);
  digitalWrite(loca_led, LOW);
  digitalWrite(first_loca_led, LOW);
  digitalWrite(recieve_wifi_led, LOW);
  digitalWrite(send_wifi_led, LOW);
  digitalWrite(sd_led , LOW);
  digitalWrite(sms_check_led, LOW);
  //Start Serial-Communication with Modules
  Serial.begin(9600);
  GPS_Serial.begin(9600);
  GSM_Serial.begin(9600);
  GSM_Serial.setTimeout(2000);
  wifi_Serial.begin(115200);
  //set configurations of Esp8266
  sendToWifi("AT+CWMODE=2", responseTime, DEBUG); // configure as access point
  sendToWifi("AT+CIFSR", responseTime, DEBUG); // get ip address
  sendToWifi("AT+CIPMUX=1", responseTime, DEBUG); // configure for multiple connections
  sendToWifi("AT+CIPSERVER=1,80", responseTime, DEBUG); // turn on server on port 80
  sendToUno("Wifi connection is running!", responseTime, DEBUG);
  delay(300);
  // START HARDWARE INTERRUPT FOR SOS BUTTON
  pinMode(interruptPin, INPUT_PULLUP);
  digitalWrite(interruptPin, HIGH); // TRY WITHOUT
  attachInterrupt(digitalPinToInterrupt(interruptPin), sosMESSAGE, CHANGE);
  get_location_first_time(); // GET INITIAL LOCATION AFTER POWERON
}

void loop()
{

  get_location();
  wifi();
  SD_();
  RecieveMessage();
  if (sos_present)
  {
    get_location_();
    get_time();
    TextMessage += "\nPlease Help Me !";
    for (int i = 0; i < 5; i++)
    {
      sms_number = NUMBERS[i];
      smartDelay(5000);
      SendMessage();
    }
    sos_present = false;
  }
  Time = "";
}

void wifi()
{
  digitalWrite(recieve_wifi_led, HIGH);
  message = readWifiSerialMessage();
  wifi_();
  digitalWrite(recieve_wifi_led, LOW);

}


void wifi_()
{
  digitalWrite(send_wifi_led, HIGH);
   if (String(new_long).toInt() != 0 && String(new_lat).toInt() != 0)
  {
    la = new_lat, lo = new_long;
    sm = String("My Location is : https://maps.google.com/?q=") + la + String(",") + lo ;
  }
  else if (String(new_long).toInt() == 0 && String(new_lat).toInt() == 0)
  {
    la = old_lat, lo = old_long;
    sm = String("My Location is : https://maps.google.com/?q=") + la + String(",") + lo;
  }
  else
  {
    la = lat_, lo = long_;
    sm = String("My Location is : https://maps.google.com/?q=") + la + String(",") + lo;
  }
  get_time_();
  sendData("\n");
  sendData(sm);
  get_speed();
  String s;
  s = car_speedc;
  sendData("\nCar Speed is : " + s + String(" Km/Hr."));
  delay(1000);
  digitalWrite(send_wifi_led, LOW);
}

void sms()
{
  digitalWrite(sms_check_led, HIGH);
  if (new_message)
  {

    if (sms_body == "location" && sms_body != "speed" && sms_body != "turn" && sms_body != "stop" && sms_number == "+201147468880" || sms_number == "+201277430894" || sms_number == "+201111666252")
    {
      get_location_();
      get_time();
      SendMessage();
    }

    else if (sms_body == "speed" && sms_body != "location" && sms_body != "turn" && sms_body != "stop" && sms_number == "+201147468880" || sms_number == "+201277430894" || sms_number == "+201111666252")
    {

      get_speed();
      SendMessage();
    }
    else if (sms_body == "stop" && sms_body != "speed" && sms_body != "turn" && sms_body != "location" && sms_number == "+201147468880" || sms_number == "+201277430894" || sms_number == "+201111666252")
    {

      stop_car();
      SendMessage();
    }

    else if (sms_body == "turn" && sms_body != "speed" && sms_body != "location" && sms_body != "stop" && sms_number == "+201147468880" || sms_number == "+201277430894" || sms_number == "+201111666252")
    {

      run_car();
      SendMessage();
    }
  }
  new_message = 0;
  clear_body();
  Time = "";
  digitalWrite(sms_check_led, LOW);
}

void sosMESSAGE()
{
  sos_present = true;
}
void SD_()
{
  digitalWrite(sd_led, HIGH);
  get_location_();
  get_time();
  writeToSD(TextMessage); //Write location to SD
  digitalWrite(sd_led, LOW);
}
