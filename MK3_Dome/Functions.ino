         
void rearLogic() {
  for(int i = LOGIC_C_PIXEL1; i <= LOGIC_C_PIXEL2; i++){
    leds[i]=CRGB(23, 67, 96)); //small logic is light blue
    //FastLED.show();
  }
}

void doubleLogic() {    
  leds[LOGIC_A_PIXEL1]=CRGB(47, 49, 50);
  leds[LOGIC_A_PIXEL2]=CRGB(47, 49, 50);
  leds[LOGIC_B_PIXEL1]=CRGB(0, 0, 0);
  leds[LOGIC_B_PIXEL2]=CRGB(0, 0, 0);
}

void radarEye(){
  analogWrite(RADAR_PIN1, RADAR_BRIGHTNESS);
}
          
void Radar_Show(){ 
  for(int i=0; i< NUM_RADAR_LEDS; i++){
    analogWrite(readar_pins[i], radar_leds[i]);
  }
        
void holo(){
  leds[LOGIC_A_PIXEL1]=CRGB(0, 0, 0);
  //FastLED.show();
} 
           
void PSILED(){
  static int psiVal=255;
  static int flashtime;

  if(recFromBody.psi != 0){
    if(millis() - lastFlash > flashtime){
      lastFlash = millis();
      flashtime = random(20,70);
      if(psiVal == 255){
        psiVal = 60;
      }else{
        psiVal = 255; 
      }
    }     
  }else{
    psiVal = 0;
  }
    leds[1]= CRGB(psiVal,psiVal,psiVal); 
    //FastLED.show();  
}

void doubleLogicRandom() {  
  if(millis() - randomMillis > 300) {
    int random_i = random(2,5);
    leds[random_i] = CRGB(random(0,255),random(0,255),random(0,255))); 
    randomMillis = millis();
  }
}

void doubleLogicFade() {
  static byte Set=1;
  static byte a;
  static byte b;
  if(Set == 1){
    a++;
    if (b > 0){
      b--;
    }
  }else if (Set == 2){
    a--;
    b++;
  }
  constrain(a, 0, 50);
  constrain(b, 0, 50);
  
  if(a== 50){
    Set=2;
  } else if (a == 0){
    Set=1;
  }

  leds[LOGIC_A_PIXEL1]=CRGB(a, a, a); 
  leds[LOGIC_A_PIXEL2]=CRGB(a, a, a); 
  leds[LOGIC_B_PIXEL1]=CRGB(b, b, b); 
  leds[LOGIC_B_PIXEL2]=CRGB(b, b, b);       
}

void UpdateDomeState(){
  static int LEDState = 1;

  if(but4State == 0){
    but4StateMillis = millis();
    but4State = 1;
  }
  if(but4State == 1 && (millis() - but4StateMillis > 400)){
    if(LEDState == 3){
      LEDState = 1;
    }else{
      LEDState++;
    }
  but4State = 2;
  }
}

void holoPulse(){
  static int bpulse=HOLO_PULSE;
  static int holoPulseState = 2;

  if(holoPulseState == 1 && bpulse <= 155){
    bpulse++;
  }else if(bpulse >= 155){
    holoPulseState = 2;
  }

  if(holoPulseState == 2 && bpulse >= 80){
    bpulse--;
  }else if(bpulse <= 80){
    holoPulseState = 1;
  }
    
  leds[HOLO_PIXEL]=CRGB(0, 0, bpulse); 
  //FastLED.show();    
}

void holoCycle(){
  static int hpCycleState, hpRed, hpGreen, hpBlue;

switch(hpCycleState){
  default: break;

  case 0: 
    hpRed = 0;
    hpGreen = 0;
    hpBlue = 0;
    hpCycleState++;  
  break;

  case 1: 
    if( hpRed <= 250){
      hpRed += 3;
    } else {
      hpCycleState++;
    }  
  break;

  case 2: 
    if(hpRed >= 10){
      hpRed -= 3;
    } else {
      hpCycleState++;
    }    
  break;

  case 3:  
    if(hpGreen <= 250){
      hpGreen += 3;
    } else {
      hpCycleState++;
    } 
  break;

  case 4:  
    if(hpGreen >=10){
      hpGreen -= 3;
    }else {
      hpCycleState++;
    }
  break;

  case 5: 
    if(hpBlue <= 250){
      hpBlue += 3;
    }else {
      hpCycleState++; 
    }
  break;
  
  case 6: 
    if(hpBlue >=10){
      hpBlue -= 3;
    }else {
      hpCycleState++;
    }
  break;
  
  case 7: 
    if(hpRed <= 250){
      hpRed+= 3;
    }else if(hpGreen <= 250){
      hpGreen+= 3;
    }else if(hpBlHol <= 250){
      hpBlue+= 3;
    }else{
      hpCycleState=8;
    }
  break;

  case 8:
    if(hpRed >= 10){
     hpRed-= 3;
    }else if(hpGreen >= 10){
      hpGreen-= 3;
    }else if(hpBlue >= 10){
      hpBlue-= 3;
    }else{
      hpCycleState = 1;
    }
  break;
}

  leds[HOLO_PIXEL]=CRGB(hpRed, hpGreen, hpBlue);
  //FastLED.show();  
}

void rearLogicRandom() {
  if(millis() - randomMillisSingle > 300) {
    int random_i = random(LOGIC_C_PIXEL1,LOGIC_C_PIXEL2);
    leds[random_i]=CRGB(random(0,255),random(0,255),random(0,255))); 
    //FastLED.show();
    randomMillisSingle = millis();
  }
}

void rearLogicFade() {
  static int rearFadeState, rearFadeRed, rearFadeBlue, rearFadeGreen;

  if(rearFadeState == 0){
    if(rearFadeRed < 24){
      rearFadeRed++;
      rearFadeGreen = map(rearFadeRed, 0, 23, 0, 67);
      rearFadeBlue = map(rearFadeRed, 0, 23, 0, 96);
    }else{
      rearFadeState = 1;
    }
  }else if(rearFadeState == 1){
    if(rearFadeRed > 0){
      rearFadeRed--;
      rearFadeGreen = map(rearFadeRed, 0, 23, 0, 67);
      rearFadeBlue = map(rearFadeRed, 0, 23, 0, 96);
    }else{
      rearFadeState = 0;
    }
  }

  leds[LOGIC_C_PIXEL1]=CRGB(rearFadeRed, rearFadeGreen, rearFadeBlue); 
  leds[LOGIC_C_PIXEL2]=CRGB(rearFadeRed, rearFadeGreen, rearFadeBlue); 
  //FastLED.show();
}

void off(){
  for(int i=0; i< NUM_LEDS; i++)
    leds[i]=CRGB(0,0,0);

  for(int i=0; i< NUM_RADAR_LEDS; i++)
    radar_leds[i]=255;
}

void state1(){
  radarEye();
  doubleLogic();
  holo();
  rearLogic();
}

void state2(){
  radarEye();
  doubleLogicFade();
  holoPulse();
  rearLogicFade();

  if(hpCycleState != 0){
    hpCycleState = 0;
  }
}

void state3(){
  radarEye();
  doubleLogicRandom();
  holoCycle();
  rearLogicRandom();  
}

void sendAndReceive(){
  if(millis() - lastSendRecMillis >= recDelay){
    if (radio.receiveDone()) {
      if(radio.SENDERID == uint8_t(BODY_ADDRESS)){
        if (radio.DATALEN != sizeof(recFromBody)){
          Serial.print("Invalid payload received, not matching Payload struct! Size received: "); Serial.print(sizeof(radio.DATALEN));
          Serial.print(" Size should be: "); Serial.println(sizeof(recFromBody)); 
        }else{
          recFromBody = *(recBodyData*)radio.DATA;
          PSILED(); 
          lastBodyReceive = millis();
        }
      }     
    }
    lastSendRecMillis = millis(); 
  }

  battLevel();
}

void battLevel(){
  if(millis() - lastBattUpdate >= sendDelay){
    if(millis() - lastBodyReceive >= 3000){
      sendFromDome.bodyBatt = 99.99;
    }else{
      sendFromDome.bodyBatt = recFromBody.bodyBatt;
    }
    g_domeBatt = analogRead(BATT_PIN);
    g_domeBatt *= 2;    // we divided by 2, so multiply back
    g_domeBatt *= 3.3;  // Multiply by 3.3V, our reference voltage
    g_domeBatt /= 1024; // convert to voltage
    sendFromDome.domeBatt = g_domeBatt;
    lastBattUpdate = millis();
    memcpy(packet, &sendFromDome, sizeof(sendFromDome)); //Copy data from "sendToBody" array to "send_buf" byte array 
    radio.send(REMOTE_ADDRESS, packet, sizeof(packet)); //target node Id, message as string or byte array, message length
    delay(5);
  }
}