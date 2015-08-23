void send_data()
{
//  Serial.println("Send Data");
  LDateTime.getRtc(&rtc);
  data.time = (unsigned long)rtc;
  String data_string = "";
  data_string += String(data.token);
  data_string += String(',');
  data_string += String(data.token) + String(data.id);
  data_string += String(','); 
  data_string += String(data.time);
  data_string += String(',');
  data_string += String(2, DEC); //command to send data
  data_string += String(',');
  data_string += String(data.ax, DEC);
  data_string += String(',');
  data_string += String(data.ay, DEC);
  data_string += String(',');
  data_string += String(data.az, DEC);
  data_string += String(',');
  data_string += String(abs(data.bump), DEC);
  data_string += String(',');
  data_string += String(data.temp, DEC);
  data_string += String(',');
  data_string += String(data.humid, DEC);
  data_string += String(',');
  data_string += String(data.noise, DEC);
  data_string += String(',');
  data_string += String(data.lati, DEC);
  data_string += String(',');
  data_string += String(data.longi, DEC);
  data_string += String(',');
  data_string += String(data.alt, DEC);
  data_string += String(',');
  data_string += String(data.uv, DEC);
  data_string += String(',');
  data_string += String(data.dust, DEC);
  data_string += String(',');
  data_string += String(abs(data.sped), DEC);
  data_string += String('\n');
  //Serial.println(data_string);
  
  char* datafile_;
  String file=String(data.token)+String(data.id)+".txt";
  file.toCharArray(datafile_, sizeof(file));
  
  LFile dataFile = Drv.open("datalogg.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(data_string);
    dataFile.close();
    // print to the serial port too:
    Serial.println(data_string);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening file");
  }
}
