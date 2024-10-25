// YPR is in radians
// Also its goes from -180 to 180
void update_ypr(bool verbose = false){
  mpu.dmpGetQuaternion(&quant, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &quant);
  mpu.dmpGetYawPitchRoll(ypr, &quant, &gravity);
  if(verbose){
    Serial.print("ypr\t");
    Serial.print(ypr[0] * degree);
    Serial.print("\t");
    Serial.print(ypr[1] * degree);
    Serial.print("\t");
    Serial.println(ypr[2] * degree);
  }
}
// Accerleation relative to you,  i.e. left is always left.
void update_accel(bool verbose = false){
  mpu.dmpGetQuaternion(&quant, fifoBuffer);
  mpu.dmpGetAccel(&aa, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &quant);
  mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
  if(verbose){
    Serial.print("areal\t");
    Serial.print(aaReal.x);
    Serial.print("\t");
    Serial.print(aaReal.y);
    Serial.print("\t");
    Serial.println(aaReal.z);
  }
}
// Accerlation relative to the world, i.e. your left can be forward if it turns, but you'd know its left.
// X axis is always x-Axis, not left and right only.
void update_worldAccel(bool verbose = false){
  mpu.dmpGetQuaternion(&quant, fifoBuffer);
  mpu.dmpGetAccel(&aa, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &quant);
  mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
  mpu.dmpConvertToWorldFrame(&aaWorld, &aaReal, &quant);
  if(verbose){
    Serial.print("aworld\t");
    Serial.print(aaWorld.x);
    Serial.print("\t");
    Serial.print(aaWorld.y);
    Serial.print("\t");
    Serial.println(aaWorld.z);
  }
}
// everyhting i care about atleast
void update_all(bool verbose = false){
  mpu.dmpGetQuaternion(&quant, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &quant);
  mpu.dmpGetYawPitchRoll(ypr, &quant, &gravity);
  mpu.dmpGetAccel(&aa, fifoBuffer);
  mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
  if(verbose){
    Serial.print("ypr\t");
    Serial.print(ypr[0] * degree);
    Serial.print("\t");
    Serial.print(ypr[1] * degree);
    Serial.print("\t");
    Serial.println(ypr[2] * degree);
    Serial.print("areal\t");
    Serial.print(aaReal.x);
    Serial.print("\t");
    Serial.print(aaReal.y);
    Serial.print("\t");
    Serial.println(aaReal.z);
  }
}
void init_mpu(){
  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);

  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  // See Calibrate.ino
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788); // 1688 factory default for my test chip
  if (devStatus == 0) {
      // Calibration Time: generate offsets and calibrate our MPU6050
      mpu.CalibrateAccel(6);
      mpu.CalibrateGyro(6);
      mpu.PrintActiveOffsets();
      // turn on the DMP, now that it's ready
      Serial.println(F("Enabling DMP..."));
      mpu.setDMPEnabled(true);

      // enable Arduino interrupt detection
      Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
      Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
      Serial.println(F(")..."));
      attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
      mpuIntStatus = mpu.getIntStatus();

      // set our DMP Ready flag so the main loop() function knows it's okay to use it
      Serial.println(F("DMP ready! Waiting for first interrupt..."));
      dmpReady = true;

      // get expected DMP packet size for later comparison
      packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
      // ERROR!
      // 1 = initial memory load failed
      // 2 = DMP configuration updates failed
      // (if it's going to break, usually the code will be 1)
      Serial.print(F("DMP Initialization failed (code "));
      Serial.print(devStatus);
      Serial.println(F(")"));
  }
}