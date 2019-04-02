#ifndef bb8radar_h
#define bb8radar_h

#include <stdint.h>
#include "Arduino.h"

// Animations

#define BB8_RADAR_OFF 111
#define BB8_RADAR_ON 222
#define BB8_RADAR_IDLE 333




#define ALA_ENDSEQ 0
#define ALA_STOPSEQ 1
// Strobo effect duty cycle (10=1/10)
#define ALA_STROBODC 10

struct BB8RadarSeq
{
    int animation;
    long speed;
    long duration;
};


class BB8Radar
{

public:

    BB8Radar();

    void initPWM(byte pin);
    void initPWM(int numLeds, byte *pins);
    void setBrightness(byte maxOut);

    /**
    * Sets the maximum refresh rate in Hz (default value is 50 Hz).
    */
    void setRefreshRate(int refreshRate);
    int getRefreshRate();
    void setAnimation(int animation, long speed, bool isSeq=false);
    void setAnimation(BB8RadarSeq animSeq[]);
	void setSpeed(long speed);
    int getAnimation();
    bool runAnimation();

    void setIrisLed(int ledNum);
    int getIrisLed();


private:

    void setAnimationFunc(int animation);
    void on();
       void onAll();
    void off();
    void blink();
    void blinkAlt();
    void sparkle();
    void sparkle2();
    void strobo();

    void pixelShiftRight();
    void pixelShiftLeft();
    void pixelBounce();
    void pixelSmoothShiftRight();
    void pixelSmoothShiftLeft();
    void pixelSmoothBounce();
    void comet();
    void barShiftRight();
    void barShiftLeft();
    void larsonScanner();
    void larsonScanner2();

    void fadeIn();
    void fadeOut();
    void fadeInOut();
    void glow();
    void flame();


    void IdleEye();


    byte *pins;  // pins where the leds are attached to
    byte *leds;  // array to store leds brightness values
    int numLeds; // number of leds
    int irisLed;
    int maxOut;
    int refreshMillis;
    int refreshRate;   // current refresh rate

    int animation;
    long speed;
    BB8RadarSeq *animSeq;
    int animSeqLen;
    long animSeqDuration;

    void (BB8Radar::*animFunc)();
    unsigned long animStartTime;
    unsigned long animSeqStartTime;
    unsigned long lastRefreshTime;

    int getStep(long t0, long t, int v);
    float getStepFloat(long t0, long t, float v);
    float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);


};

#endif