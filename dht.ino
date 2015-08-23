void dht_read()
{
  float t = 0.0;
  float h=0.0;
  if(dht.readHT(&t, &h))
  {
    data.humid = h;
    data.temp = t;
  }
}
