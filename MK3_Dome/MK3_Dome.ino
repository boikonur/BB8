// #define FASTLED_INTERRUPT_RETRY_COUNT 1
#include <FastLED.h> //TODO: disables Interrupts 30µs per pixel
#include <RFM69.h>   //get it here: https://www.github.com/lowpowerlab/rfm69
#include <SPI.h>
#include "bb8-radar.h"

BB8Radar radar;


FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif
#define DEBUG_ENABLED 1

#define RFM69_IRQ 7
#define RFM69_IRQN 4 // Pin 7 is IRQ 4!
#define RFM69_RST 4
#define RFM69_CS 8
#define BATT_PIN A9

#define PIXEL_PWR_PIN 4
#define PIXEL_DAT_PIN 12

#define NUM_RADAR_LEDS 6
#define RADAR_PIN1 13
#define RADAR_PIN2 11
#define RADAR_PIN3 10
#define RADAR_PIN4 9
#define RADAR_PIN5 6
#define RADAR_PIN6 5

#define RADAR_BRIGHTNESS 210

byte radar_pins[] = { RADAR_PIN1, RADAR_PIN2, RADAR_PIN3, RADAR_PIN4, RADAR_PIN5, RADAR_PIN6 };



// Neo Pixel Settings
#define LED_TYPE WS2811
#define COLOR_ORDER RGB
#define NUM_LEDS 8
#define FRAMES_PER_SECOND 120

// Neo Pixel Order
#define HOLO_PIXEL 0
#define PSI_PIXEL 1
#define LOGIC_A_PIXEL1 2
#define LOGIC_A_PIXEL2 3
#define LOGIC_B_PIXEL1 4
#define LOGIC_B_PIXEL2 5
#define LOGIC_C_PIXEL1 6
#define LOGIC_C_PIXEL2 7

#define DATA_DELAY 0
#define REC_DELAY 10
#define UPDATE_INTERVAL 40
#define SEND_DELAY 50
#define HOLO_PULSE 80


#define BODY_TIMEOUT 3000

//*********************************************************************************************
// ***********                 Network Communication Settings                      ************
//*********************************************************************************************
#define NETWORKID 100    // The same on all nodes that talk to each other
#define REMOTE_ADDRESS 1 // The unique identifier of this node
#define BODY_ADDRESS 2   // The recipient of packets
#define DOME_ADDRESS 3   // The recipient of packets
#define DRIVE_ADDRESS 4

#define FREQUENCY RF69_433MHZ
//#define FREQUENCY     RF69_868MHZ
//#define FREQUENCY     RF69_915MHZ

#define IS_RFM69HCW true // set to 'true' if you are using an RFM69HCW module
// #define ACK_TIME      30
// #define ENCRYPTKEY    "aaaaaaaa11133222"



CRGBArray<NUM_LEDS> leds;

/********** NeoPixel Setup *************/

#define UPDATES_PER_SECOND 100
CRGBPalette16 currentPalette( CRGB::Black);
CRGBPalette16 targetPalette( PartyColors_p );
TBlendType    currentBlending;

int SPEEDO = 25;          
int STEPS = 20;         
int HUE = 200;    // starting color          
int SATURATION = 255;          
int BRIGHTNESS = 200; 
int glitter = 0; 

#if DEBUG_ENABLED
#define DEBUG(X)          \
  Serial.print("Debug:"); \
  Serial.println(X);

typedef struct debug_data
{
  byte mode;
  byte update;
} debug_data;

debug_data debugS = {0, 0};

#endif

RFM69 radio = RFM69(RFM69_CS, RFM69_IRQ, IS_RFM69HCW, RFM69_IRQN);

struct SEND_DATA_STRUCTURE
{
  float bodyBatt;
  float domeBatt;
} sendFromDome;

typedef struct RECEIVE_DATA_STRUCTURE
{
  int psi = 0;
  byte button4 = 1;
  float bodyBatt;
} recBodyData;

typedef struct RECEIVE_DATA_STRUCTURE_DEBUG
{
  int Joy1Y = 256; //main drive
  int Joy1X = 256; //tilt / steer
  int Joy2Y = 256; //head tilt
  int Joy2X = 256; //head spin
  int Joy3X = 256; //spin Flywheel
  int Joy4X = 256;
  byte ServoMode; //Select on left joystick
  byte lBut1 = 0; //left 1
  byte lBut2 = 0; //left 2
  byte lBut3;     //left3
  byte Fwd;       //Select on right joystick = rJoySelect
  byte Speed;
  byte rBut2;           //right 2
  byte rBut3 = 0;       //right 3
  byte motorEnable = 1; //toggle on top
  byte CalibID;
  byte wireless = 1;
} recRemoteData;

byte packet[sizeof(sendFromDome)];
recBodyData recFromBody;
recRemoteData recFromRemote;
float g_domeBatt;

unsigned long randomMillis, previousMillis, previousMillis2, lastSendRecMillis;
unsigned long lastHPCycleMillis, randomMillisSingle, but4StateMillis, lastBattUpdate;
unsigned long lastBodyReceive, lastFlash;

void off_state(void);
void state1(void);
void state2(void);
void state3(void);

int but4State;

typedef void (*ModeFunc[])();
ModeFunc domePattern = {off_state, state1, state2, state3};
uint8_t gCurrentMode = 0;

void setup()
{

  Serial.begin(115200);
  while (!Serial)
  {
    delay(1);
  }

  DEBUG("init");
  delay(3000); // power-up safety delay

  FastLED.addLeds<LED_TYPE, PIXEL_DAT_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  pinMode(PIXEL_PWR_PIN, OUTPUT);
  // pinMode(RADAR_PIN1, OUTPUT);
  // pinMode(RADAR_PIN2, OUTPUT);
  // pinMode(RADAR_PIN3, OUTPUT);
  // pinMode(RADAR_PIN4, OUTPUT);
  // pinMode(RADAR_PIN5, OUTPUT);
  // pinMode(RADAR_PIN6, OUTPUT);
  // pinMode(RFM69_RST, OUTPUT);



  // Hard Reset the RFM module
  digitalWrite(RFM69_RST, HIGH);
  delay(100);
  digitalWrite(RFM69_RST, LOW);
  delay(100);

  // Initialize radio
  radio.initialize(FREQUENCY, DOME_ADDRESS, NETWORKID);
  if (IS_RFM69HCW)
  {
    radio.setHighPower(); // Only for RFM69HCW & HW!
  }
  radio.setPowerLevel(31); // power output ranges from 0 (5dBm) to 31 (20dBm)

Gradient(); 

  gCurrentMode = 1;

  radar.initPWM(6, radar_pins);

  // set the animation sequence
  // radar.setAnimation(seq);
}

unsigned int gCurentPosition=0;
void loop()
{

  radar.runAnimation();
  sendAndReceive();

#ifdef DEBUG_ENABLED
  getSerial(Serial, &debugS);
#endif

#ifdef DEBUG_ENABLED
  if (debugS.update)
  {
    gCurrentMode = debugS.mode;
    debugS.update = 0;
    debugS.mode = 0;
  }
#endif




    domePattern[gCurrentMode]();
    FastLED.show();
    FastLED.delay(1000 / FRAMES_PER_SECOND);


  if (millis() - previousMillis > UPDATE_INTERVAL)
  {
    previousMillis = millis();


    if (recFromBody.button4 == 0 && but4State < 2)
    {
      UpdateDomeState();
    }
    else if (recFromBody.button4 == 1 && but4State != 0)
    {
      but4State = 0;
    }
  }
}
