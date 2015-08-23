void dust_sense()
{
  ratio = lowpulseoccupancy/(dust_interval*10.0);  // Integer percentage 0=>100
  data.dust = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
  lowpulseoccupancy = 0;
}
