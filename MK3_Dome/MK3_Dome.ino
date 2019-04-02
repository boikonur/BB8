// #define FASTLED_INTERRUPT_RETRY_COUNT 1
#include <FastLED.h> //TODO: disables Interrupts 30µs per pixel 
#include <RFM69.h>   //get it here: https://www.github.com/lowpowerlab/rfm69
#include <SPI.h>

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define RFM69_IRQ     7
#define RFM69_IRQN    4  // Pin 7 is IRQ 4!
#define RFM69_RST     4
#define RFM69_CS      8
#define BATT_PIN      A9
#define NUM_RADAR_LEDS 6
#define RADAR_PIN1    5
#define RADAR_PIN2    6       
#define RADAR_PIN3    9         
#define RADAR_PIN4    10       
#define RADAR_PIN5    11       
#define RADAR_PIN6    13
#define PIXEL_PWR_PIN 4
#define PIXEL_DAT_PIN 12
#define LED_TYPE      WS2811
#define COLOR_ORDER   RGB
#define NUM_LEDS      8
#define BRIGHTNESS    96
#define FRAMES_PER_SECOND  120
#define HOLO_PIXEL    0  
#define PSI_PIXEL     1
#define LOGIC_A_PIXEL1 2
#define LOGIC_A_PIXEL1 3
#define LOGIC_B_PIXEL1 4
#define LOGIC_B_PIXEL1 5
#define LOGIC_C_PIXEL1 6
#define LOGIC_C_PIXEL1 7

#define DATA_DELAY      0
#define REC_DELAY       10
#define UPDATE_INTERVAL 40
#define SEND_DELAY      50 
#define HOLO_PULSE     80
#define RADAR_BRIGHTNESS 210
#define BODY_TIMEOUT 3000

//*********************************************************************************************
// *********** IMPORTANT SETTINGS - YOU MUST CHANGE/CONFIGURE TO FIT YOUR HARDWARE ************
//*********************************************************************************************
#define NETWORKID      100  // The same on all nodes that talk to each other
#define REMOTE_ADDRESS 1    // The unique identifier of this node
#define BODY_ADDRESS   2    // The recipient of packets
#define DOME_ADDRESS   3    // The recipient of packets
#define DRIVE_ADDRESS  4
#define FREQUENCY      RF69_915MHZ  // THIS NEEDS TO MATCH YOUR FEATHER
#define IS_RFM69HCW    true // set to 'true' if you are using an RFM69HCW module

//*********************************************************************************************
byte radar_pins[NUM_RADAR_LEDS]= {RADAR_PIN1, RADAR_PIN2, RADAR_PIN3, RADAR_PIN4, RADAR_PIN5, RADAR_PIN6};
byte radar_leds[NUM_RADAR_LEDS]= {255, 255, 255, 255, 255, 255};
CRGB leds[NUM_LEDS];

RFM69 radio = RFM69(RFM69_CS, RFM69_IRQ, IS_RFM69HCW, RFM69_IRQN);

struct SEND_DATA_STRUCTURE{
      float bodyBatt;
      float domeBatt;  
}sendFromDome;
 
typedef struct RECEIVE_DATA_STRUCTURE{
      int psi=0;
      byte button4 = 1;
      float bodyBatt;      
}recBodyData;

byte packet[sizeof(sendFromDome)];
recBodyData recFromBody;
float g_domeBatt;

unsigned long randomMillis, previousMillis, previousMillis2, lastSendRecMillis;
unsigned long lastHPCycleMillis, randomMillisSingle, but4StateMillis, lastBattUpdate;
unsigned long lastBodyReceive, lastFlash;


int but4State;

typedef void (*ModeFunc[])();
ModeFunc domePattern = { off, state1, state2, state3 };
uint8_t gCurrentMode = 0; 


void setup() {

  Serial.begin(115200);

  delay(3000);
  FastLED.addLeds<LED_TYPE, PIXEL_DAT_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  
  pinMode(PIXEL_PWR_PIN, OUTPUT);
  pinMode(RADAR_PIN1, OUTPUT);
  pinMode(RADAR_PIN2, OUTPUT);
  pinMode(RADAR_PIN3, OUTPUT);
  pinMode(RADAR_PIN4, OUTPUT);
  pinMode(RADAR_PIN5, OUTPUT);
  pinMode(RADAR_PIN6, OUTPUT);
  pinMode(RFM69_RST, OUTPUT);  

  // Hard Reset the RFM module
  digitalWrite(RFM69_RST, HIGH);
  delay(100);
  digitalWrite(RFM69_RST, LOW);
  delay(100);

  // Initialize radio
  radio.initialize(FREQUENCY,DOME_ADDRESS,NETWORKID);
  if (IS_RFM69HCW) {
    radio.setHighPower();    // Only for RFM69HCW & HW!
  }
  radio.setPowerLevel(31); // power output ranges from 0 (5dBm) to 31 (20dBm)

  Radar_Show();    
}
  
void loop () {
  
  sendAndReceive();

  if(millis() - previousMillis > UPDATE_INTERVAL) {
    previousMillis = millis();

    domePattern[gCurrentMode]();
    FastLED.show();  
    Radar_Show();

    if (recFromBody.button4 == 0 && but4State < 2){
      UpdateDomeState();
    } else if(recFromBody.button4 == 1 && but4State != 0){
      but4State = 0;
    }
  }
}  

