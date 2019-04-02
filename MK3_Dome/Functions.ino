
#define REAR_LOGIC_COLOR1 CRGB(23, 67, 96)
#define REAR_LOGIC_COLOR2 CRGB(23, 67, 96)


void rearLogic() {
    leds[LOGIC_C_PIXEL1]=CRGB(23, 67, 96);
    leds[LOGIC_C_PIXEL2]=CRGB(23, 67, 96);
}

void doubleLogic() {    
  leds[LOGIC_A_PIXEL1]=CRGB(47, 49, 50);
  leds[LOGIC_A_PIXEL2]=CRGB(47, 49, 50);
  leds[LOGIC_B_PIXEL1]=CRGB(0, 0, 0);
  leds[LOGIC_B_PIXEL2]=CRGB(0, 0, 0);
}
          


        
void holo(){
  leds[LOGIC_A_PIXEL1]=CRGB(0, 0, 0);
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
}

void doubleLogicRandom() {  
  if(millis() - randomMillis > 300) {
    int random_i = random(2,5);
    leds[random_i] = CRGB(random(0,255), random(0,255), random(0,255)); 
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
    }else if(hpBlue <= 250){
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
}

void rearLogicRandom() {
  if(millis() - randomMillisSingle > 300) {
    int random_i = random(LOGIC_C_PIXEL1,LOGIC_C_PIXEL2);
    leds[random_i]=CRGB(random(0,255), random(0,255), random(0,255)); 
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
}

void bpmPSI()
{
  #define G_HUE 20
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
 // for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[PSI_PIXEL] = ColorFromPalette(palette, G_HUE+(PSI_PIXEL*2), beat-G_HUE+(PSI_PIXEL*10));
  //}
}



void off_state(){
  
  radar.setAnimation(BB8_RADAR_OFF, 1000, 0);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
}

void state1(){
  bpmPSI();

  // doubleLogic();
  // holo();
  // rearLogic();
}

void state2(){

  radar.setAnimation(BB8_RADAR_ON, 1000, 0);
  doubleLogicFade();
  holoPulse();
  rearLogicFade();

//   if(hpCycleState != 0){
//     hpCycleState = 0;
//   }
}

void state3(){
  radar.setAnimation(BB8_RADAR_IDLE, 1000, 0);
  doubleLogicRandom();
  holoCycle();
  rearLogicRandom();  
}

void sendAndReceive(){
  if(millis() - lastSendRecMillis >= REC_DELAY){
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
      // else if(radio.SENDERID == uint8_t(REMOTE_ADDRESS))
      // {
       

      // leds[PSI_PIXEL]=CRGB(0, 255, 0); 
      // DEBUG("remote packet")
      // }
      else 
       DEBUG(radio.SENDERID);
    }
    lastSendRecMillis = millis(); 
  }

  battLevel();
}

void battLevel(){
  if(millis() - lastBattUpdate >= SEND_DELAY){
    if(millis() - lastBodyReceive >= BODY_TIMEOUT){
      sendFromDome.bodyBatt = 99.99;
    }else{
      sendFromDome.bodyBatt = recFromBody.bodyBatt;
    }
    g_domeBatt = 100;//analogRead(BATT_PIN);
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


void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = BRIGHTNESS;
  
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += STEPS;
  }
}

void Rainbow()
{ 
  FastLED.setBrightness(  BRIGHTNESS );
  currentPalette = RainbowColors_p;
  
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; 

  FillLEDsFromPaletteColors( startIndex);
    
  FastLED.show();
  FastLED.delay(SPEEDO);  
}

void SetupGradientPalette()
{
  CRGB light = CHSV( HUE + 25, SATURATION - 20, BRIGHTNESS);
  CRGB dark  = CHSV( HUE, SATURATION - 15, BRIGHTNESS);
  CRGB medium = CHSV ( HUE - 25, SATURATION, BRIGHTNESS);
  
  currentPalette = CRGBPalette16( 
    light,  light,  light,  light,
    medium, medium, medium,  medium,
    dark,  dark,  dark,  dark,
    medium, medium, medium,  medium );
}

void Gradient()
{
  SetupGradientPalette();

  static uint8_t startIndex = 0;
  startIndex = startIndex + 1;  // motion speed
  FillLEDsFromPaletteColors( startIndex);
  FastLED.show();
  FastLED.delay(SPEEDO);
}


void getSerial(Stream &Serialx, struct debug_data *data) {
      if (Serialx.available() > 0) {
                // read the incoming byte:
                byte incomingByte = Serialx.read();
                
                
                // say what you got:
                Serialx.print("I received: ");
                Serialx.println(incomingByte);
               
               int c = (int)incomingByte - 48;
                if(c >=0 && c < sizeof(domePattern)/(sizeof domePattern[0]) )
                {
                  data->update = 1;
                  data->mode = c;
                  
                 Serialx.print("nymber: ");
                Serialx.println(data->mode);
                }


        }
}




