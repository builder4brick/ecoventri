void sound_sense()
{
//  Serial.println("Sound");
  int sensorValue;
  long  sum=0;
  
  for(int i=0; i<32; i++)
  {
      sensorValue=analogRead(A1);
      sum=sensorValue+sum;
      delay(1);
  }
  
  sum = sum >> 5;
  
  data.noise = (int)sum*4980.0/1023.0;
}
