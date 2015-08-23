void start_ride()
{
  Serial.println("Start");
  data.time = millis();
  String data_string = "";
  data_string += String(data.token);
  data_string += String(',');
  data_string += String(data.time);
  data_string += String(','); 
  data_string += String(0, DEC);
  data_string += String('\n');
  Serial.println(data_string);
  ride_started = TRUE;
}
