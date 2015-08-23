void accel_sense()
{ 
  acc.get(&ax, &ay, &az);             // get value from adxl345
  
  data.bump = (az-data.az)/(now_time - accel_time);
  
  data.ax = ax;
  data.ay = ay;
  data.az = az;

}
