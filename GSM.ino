void stop_car()
{
  digitalWrite(car_led, LOW);
  digitalWrite(car_motor, LOW);
  TextMessage = String("the car stopped ");
}

void run_car()
{
  digitalWrite(car_led, HIGH);
  digitalWrite(car_motor, HIGH);
  TextMessage = String("the car is running ");
}

void SendMessage()
{
  digitalWrite(s_led, HIGH);
  GSM_Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(500);  // Delay of 1000 milli seconds or 1 second
  GSM_Serial.print("AT+CMGS=\""); // Replace x with mobile number
  GSM_Serial.println(sms_number + "\"");
  delay(500);
  GSM_Serial.println(TextMessage);// The SMS text you want to send
  delay(500);
  GSM_Serial.println((char)26);// ASCII code of CTRL+Z
  digitalWrite(s_led, LOW);

}

void RecieveMessage()
{
  digitalWrite(r_led, HIGH);
  GSM_Serial.println("AT+CNMI=2,2,0,0,0");
  incoming_message = "";
  while (!GSM_Serial.available()); // wait until gsm sends something //modify for 5 seconds
  if (GSM_Serial.find("+CMT:"))
  {
    new_message = 1;
    incoming_message = GSM_Serial.readString(); //setTimeOut if problem with length
    sms_body = incoming_message.substring(incoming_message.lastIndexOf('"') + 3);
    sms_number = incoming_message.substring(incoming_message.indexOf('"') + 1, incoming_message.indexOf('"', incoming_message.indexOf('"') + 1));
    sms_body.toLowerCase();

    if (find_text("location", sms_body) != -1)// if not found returns -1 and if found returns the start
    {
      sms_body = sms_body.substring(find_text("location", sms_body), find_text("location", sms_body) + 8);
    }

    else if (find_text("speed", sms_body) != -1) // if not found returns -1 and if found returns the start
    {
      sms_body = sms_body.substring(find_text("speed", sms_body), find_text("speed", sms_body) + 5);
    }

    else if (find_text("turn", sms_body) != -1)// if not found returns -1 and if found returns the start
    {
      sms_body = sms_body.substring(find_text("turn", sms_body), find_text("turn", sms_body) + 4);
    }

    else if (find_text("stop", sms_body) != -1) // if not found returns -1 and if found returns the start
    {
      sms_body = sms_body.substring(find_text("stop", sms_body), find_text("stop", sms_body) + 4);
    }
  }
  delay(1000);
  get_location();
  sms();
  digitalWrite(r_led, LOW);
}
int find_text(String needle, String haystack)
{
  int foundpos = -1;
  for (int i = 0; i <= haystack.length() - needle.length(); i++) {
    if (haystack.substring(i, needle.length() + i) == needle) {
      foundpos = i;
    }
  }
  return foundpos;
}

void clear_body()
{
  sms_body = "";
  sms_number = "";
}
