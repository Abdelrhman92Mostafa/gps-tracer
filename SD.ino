void writeToSD(String LOCATION)
{
  // Open file
  LOCAT = SD.open(filename, FILE_WRITE);

  // If the file opened okay, write to it:
  if (LOCAT)
  {
    //Debug to Serial

    //Serial.print("Writing to ");
    //Serial.print(filename);
    //Serial.println("...");


    //Write to file
    LOCAT.print(LOCATION);
    LOCAT.println();

    // close the file:
    LOCAT.close();

    //Debug to Serial
    //Serial.println("done.");
  }
  else
  {
    // if the file didn't open, print an error:
    //Serial.print("error opening ");
    //Serial.println(filename);
  }
}

void readFromSD()
{
  // Open the file for reading:
  LOCAT = SD.open(filename);
  if (LOCAT)
  {
    //Serial.print("Contents of ");
    //Serial.print(filename);
    //Serial.println(":");

    // Read from file until there's nothing else in it:
    while (LOCAT.available())
    {
      //Serial.write(LOCAT.read());
    }
    // Close the file:
    LOCAT.close();
  }
  else
  {
    // If the file didn't open, print an error:
    //Serial.print("error opening ");
    //Serial.println(filename);
  }
}
