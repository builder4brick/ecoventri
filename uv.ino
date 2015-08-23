void uv_sense()
{
//  Serial.println("UV");
  int sensorValue;
  long  sum=0;
    
  for(int i=0; i<32; i++)
  {
      sensorValue=analogRead(A0);
      sum=sensorValue+sum;
      delay(1);
  }
  
  sum = sum >> 5;
  
  data.uv = (int)sum*4980.0/1023.0;
  exposed_uv += data.uv * (now_time - uv_time);
  if(exposed_uv > max_uv)
  {
    for(int k=0; k<5; k++)
    {
      for(int o=0; o<400; o++)
      {
        digitalWrite(buzzer, HIGH);
        delayMicroseconds(200);
        digitalWrite(buzzer, LOW);
        delayMicroseconds(200);
      }
      delay(200);
      for(int o=0; o<400; o++)
      {
        digitalWrite(buzzer, HIGH);
        delayMicroseconds(200);
        digitalWrite(buzzer, LOW);
        delayMicroseconds(200);
      }
    }
  }
}
