Adafruit_BMP085 bmp;
  
void bmp_setup() {
  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }
}

double calc_depth(double p){
  //P = rho *g *d
  // P entered as Pa
  double rho = 997.0474;//997.0474 kg/m^3 fressh; 1023.6 kg/m^3 salt
  double g = 9.80665; // m/s^2
  double d = p/(rho*g);
  return d;
}
  
void bmp_loop() {
    double pressure = bmp.readPressure();
    Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    
    Serial.print("Pressure = ");
    Serial.print(pressure);
    Serial.println(" Pa");

    Serial.print("Depth = ");
    Serial.print(calc_depth(pressure));
    Serial.println(" m");
    
  //   // Calculate altitude assuming 'standard' barometric
  //   // pressure of 1013.25 millibar = 101325 Pascal
  //   Serial.print("Altitude = ");
  //   Serial.print(bmp.readAltitude());
  //   Serial.println(" meters");

  //   Serial.print("Pressure at sealevel (calculated) = ");
  //   Serial.print(bmp.readSealevelPressure());
  //   Serial.println(" Pa");

  // // you can get a more precise measurement of altitude
  // // if you know the current sea level pressure which will
  // // vary with weather and such. If it is 1015 millibars
  // // that is equal to 101500 Pascals.
  //   Serial.print("Real altitude = ");
  //   Serial.print(bmp.readAltitude(101500));
  //   Serial.println(" meters");
    
    Serial.println();
    delay(500);
}
