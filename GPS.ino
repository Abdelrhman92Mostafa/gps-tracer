
void get_location_first_time()
{
  digitalWrite(first_loca_led, HIGH);

  do {
    while (GPS_Serial.available() > 0)
      gps.encode(GPS_Serial.read());

    if (gps.location.isValid());
    {
      dtostrf(gps.location.lng(), 7, 6, long_);
      dtostrf(gps.location.lat(), 7, 6, lat_);

    }
  } while (String(long_).toInt() == 0 || String(lat_).toInt() == 0);

  digitalWrite(first_loca_led, LOW);

}

void get_location()
{

  digitalWrite(loca_led, HIGH);
  unsigned long start = millis();
  do {
    while (GPS_Serial.available() > 0)
      gps.encode(GPS_Serial.read());

    if (gps.location.isValid());
    {
      dtostrf(gps.location.lng(), 7, 6, new_long);
      old_long == new_long;
      String(old_long).toInt();
      dtostrf(gps.location.lat(), 7, 6, new_lat);
      old_lat == new_lat;
      String(old_lat).toInt();
    }

  } while (millis() - start < 5000);



  if (String(new_long).toInt() != 0 && String(new_lat).toInt() != 0)
  {
    la = new_lat, lo = new_long;
    TextMessage = String("My Location is : https://maps.oogle.com/?q=") + la + String(",") + lo ;
  }
  else if (String(new_long).toInt() == 0 && String(new_lat).toInt() == 0)
  {
    la = old_lat, lo = old_long;
    TextMessage = String("My Location is : https://maps.oogle.com/?q=") + la + String(",") + lo;
  }
  else
  {
    la = lat_, lo = long_;
    TextMessage = String("My Location is : https://maps.oogle.com/?q=") + la + String(",") + lo;
  }
  digitalWrite(loca_led, LOW);
}

void get_location_()
{

  digitalWrite(loca_led, HIGH);
  unsigned long start = millis();
  do {
    while (GPS_Serial.available() > 0)
      gps.encode(GPS_Serial.read());

    if (gps.location.isValid());
    {
      dtostrf(gps.location.lng(), 7, 6, new_long);
      old_long == new_long;
      String(old_long).toInt();
      dtostrf(gps.location.lat(), 7, 6, new_lat);
      old_lat == new_lat;
      String(old_lat).toInt();
    }

  } while (millis() - start < 5000);



  if (String(new_long).toInt() != 0 && String(new_lat).toInt() != 0)
  {
    la = new_lat, lo = new_long;
    TextMessage = String("My Location is : https://maps.google.com/?q=") + la + String(",") + lo ;
  }
  else if (String(new_long).toInt() == 0 && String(new_lat).toInt() == 0)
  {
    la = old_lat, lo = old_long;
    TextMessage = String("My Location is : https://maps.google.com/?q=") + la + String(",") + lo;
  }
  else
  {
    la = lat_, lo = long_;
    TextMessage = String("My Location is : https://maps.google.com/?q=") + la + String(",") + lo;
  }
  digitalWrite(loca_led, LOW);
}

void get_time()
{
  Time += "\nTime : ";

  while (GPS_Serial.available() > 0)
    gps.encode(GPS_Serial.read());
  {
    if (gps.date.isValid())
    {
      Time += String(gps.date.year());
      Time += "/";
      Time += String(gps.date.month());
      Time += "/";
      Time += String(gps.date.day());
      Time += " ";
    }


    if (gps.time.isValid())
    {
      if ((gps.time.hour() + 2) < 10) TextMessage += "0";
      Time += String((gps.time.hour() + 2));
      Time += ":";
      if (gps.time.minute() < 10) Time += "0";
      Time += String(gps.time.minute());
      Time += ":";
      if (gps.time.second() < 10) Time += "0";
      Time += String(gps.time.second());
      Time += ".";
    }
    else {
      TextMessage += "\n Last Time Unknown";
    }

    TextMessage += Time;
  }
  Time = "";
}

void get_time_()
{
  Time += "\nTime : ";

  while (GPS_Serial.available() > 0)
    gps.encode(GPS_Serial.read());
  {
    if (gps.date.isValid())
    {
      Time += String(gps.date.year());
      Time += "/";
      Time += String(gps.date.month());
      Time += "/";
      Time += String(gps.date.day());
      Time += " ";
    }


    if (gps.time.isValid())
    {
      if ((gps.time.hour() + 2) < 10) TextMessage += "0";
      Time += String((gps.time.hour() + 2));
      Time += ":";
      if (gps.time.minute() < 10) Time += "0";
      Time += String(gps.time.minute());
      Time += ":";
      if (gps.time.second() < 10) Time += "0";
      Time += String(gps.time.second());
      Time += ".";
    }
    else {
      sm += "\n Last Time Unknown";
    }

    sm += Time;
  }
}

void get_speed()
{

  //speed 1, speed 2 , speed 3 then take average 1 second delay between each readings
  float car_speed = 0.0;
  int count = 0;

  while (1)
  {
    while (GPS_Serial.available() > 0)
      gps.encode(GPS_Serial.read());

    if (gps.speed.isValid() && count < 3);
    {
      car_speed += gps.speed.kmph();
      smartDelay(1000);
      count++;
    }
    if (count == 3)break;
  }

  car_speed /= 3.0;

  dtostrf(car_speed, 3, 2, car_speedc);

  TextMessage = String("Car Speed : ") + String(car_speedc) + String(" Km/Hr.");
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (GPS_Serial.available())
      gps.encode(GPS_Serial.read());
  } while (millis() - start < ms);
}
