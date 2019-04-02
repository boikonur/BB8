#include "bb8-radar.h"


BB8Radar::BB8Radar()
{
    maxOut = 255;
    speed = 1000;
    irisLed = 0;
    animSeqLen = 0;
    lastRefreshTime = 0;
    refreshMillis = 1000/50;
}

void BB8Radar::initPWM(byte pin)
{
    byte *pins_ = (byte *)malloc(1);
    pins_[0] = pin;

    initPWM(1, pins_);
}


void BB8Radar::initPWM(int numLeds, byte *pins)
{
    this->numLeds = numLeds;
    this->pins = pins;

    for (int x=0; x<numLeds ; x++)
    {
        pinMode(pins[x], OUTPUT);
        digitalWrite(pins[x], LOW);
    }

    // allocate and clear leds array
    leds = (byte *)malloc(numLeds);
    memset(leds, 0, numLeds);
}

void BB8Radar::setBrightness(byte maxOut)
{
    this->maxOut = maxOut;
}

void BB8Radar::setRefreshRate(int refreshRate)
{
    this->refreshMillis = 1000/refreshRate;
}


void BB8Radar::setAnimation(int animation, long speed, bool isSeq)
{
    if (this->animation == animation && this->speed == speed)
        return;

    this->animation = animation;
    this->speed = speed;

	if (!isSeq)
		animSeqLen=0;
	setAnimationFunc(animation);
    animStartTime = millis();
}


void BB8Radar::setAnimation(BB8RadarSeq animSeq[])
{
    this->animSeq = animSeq;

    // initialize animSeqDuration and animSeqLen variables
    animSeqDuration = 0;
    for(animSeqLen=0; animSeq[animSeqLen].animation!=ALA_ENDSEQ; animSeqLen++)
    {
        animSeqDuration = animSeqDuration + animSeq[animSeqLen].duration;
    }
    animSeqStartTime = millis();
    setAnimation(animSeq[0].animation, animSeq[0].speed, true);
}

void BB8Radar::setSpeed(long speed)
{
    this->speed = speed;
	animStartTime = millis();
}


int BB8Radar::getAnimation()
{
    return animation;
}


bool BB8Radar::runAnimation()
{
    if(animation == ALA_STOPSEQ)
        return true;

    // skip the refresh if not enough time has passed since last update
    unsigned long cTime = millis();
    if (cTime < lastRefreshTime + refreshMillis)
        return false;

    // calculate real refresh rate
    refreshRate = 1000/(cTime - lastRefreshTime);

    lastRefreshTime = cTime;

    // if it's a sequence we have to calculate the current animation
    if (animSeqLen != 0)
    {
        long c = 0;
        long t = (cTime-animSeqStartTime) % animSeqDuration;
        for(int i=0; i<animSeqLen; i++)
        {
            if (t>=c && t<(c+animSeq[i].duration))
            {
                setAnimation(animSeq[i].animation, animSeq[i].speed, true);
                break;
            }
            c = c + animSeq[i].duration;
        }
    }


    // run the animantion calculation
    if (animFunc != NULL)
        (this->*animFunc)();

    // update leds
    for(int i=0; i<numLeds; i++)
        analogWrite(pins[i], leds[i]);

    
    return true;
}



///////////////////////////////////////////////////////////////////////////////


void BB8Radar::setAnimationFunc(int animation)
{
    switch(animation){     
       case BB8_RADAR_ON:                  animFunc = &BB8Radar::on;                    break;
        case BB8_RADAR_OFF:                 animFunc = &BB8Radar::off;                   break;
         case BB8_RADAR_IDLE:                animFunc = &BB8Radar::IdleEye;               break;
        default:                        animFunc = &BB8Radar::off;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////
// Switching effects
////////////////////////////////////////////////////////////////////////////////////////////

void BB8Radar::on()
{
    for(int i=0; i<numLeds; i++)
    {
        leds[i] = 0;
    }
    leds[this->irisLed]=maxOut;
}

void BB8Radar::onAll()
{
    for(int i=0; i<numLeds; i++)
    {
        leds[i] = maxOut;
    }
}

void BB8Radar::off()
{
    for(int i=0; i<numLeds; i++)
    {
        leds[i] = 0;
    }
}


void BB8Radar::blink()
{
    int t = getStep(animStartTime, speed, 2);

    for(int x=0; x<numLeds; x++)
    {
        leds[x] = ((t+1)%2)*maxOut;
    }
}


void BB8Radar::blinkAlt()
{
    int t = getStep(animStartTime, speed, 2);

    for(int x=0; x<numLeds; x++)
    {
        leds[x] = ((t+x)%2)*maxOut;
    }
}


void BB8Radar::sparkle()
{
    int p = speed/100;
    for(int x=0; x<numLeds; x++)
    {
        leds[x] = (random(p)==0)*maxOut;
    }
}

void BB8Radar::sparkle2()
{
    int p = speed/10;
    for(int x=0; x<numLeds; x++)
    {
        if(random(p)==0)
            leds[x] = maxOut;
        else
            leds[x] = leds[x] * 0.88;
    }
}



void BB8Radar::strobo()
{
    int t = getStep(animStartTime, speed, ALA_STROBODC);

    for(int x=0; x<numLeds; x++)
    {
        leds[x] = (t==0)*maxOut;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////
// Shifting effects
////////////////////////////////////////////////////////////////////////////////////////////

void BB8Radar::pixelShiftRight()
{
    int t = getStep(animStartTime, speed, numLeds);

    for(int x=0; x<numLeds; x++)
    {
        leds[x] = (x==t ? 1:0)*maxOut;
    }
}


void BB8Radar::pixelShiftLeft()
{
    int t = getStep(animStartTime, speed, numLeds);

    for(int x=0; x<numLeds; x++)
    {
        leds[x] = (x==(numLeds-1-t) ? 1:0)*maxOut;
    }
}


void BB8Radar::pixelBounce()
{
    int t = getStep(animStartTime, speed, 2*numLeds-2);

    for(int x=0; x<numLeds; x++)
    {
        leds[x] = (x==(-abs(t-numLeds+1)+numLeds-1) ? 1:0)*maxOut;
    }
}



void BB8Radar::pixelSmoothShiftRight()
{
    float t = getStepFloat(animStartTime, speed, numLeds+1);

    for(int x=0; x<numLeds; x++)
    {
        leds[x] = max(0, (int)((-abs(t-1-x)+1)*maxOut));
    }
}


void BB8Radar::pixelSmoothShiftLeft()
{
    float t = getStepFloat(animStartTime, speed, numLeds+1);

    for(int x=0; x<numLeds; x++)
    {
        leds[x] = max(0, (int)((-abs(numLeds-t-x)+1)*maxOut));
    }
}


void BB8Radar::comet()
{
    float l = numLeds/2;  // length of the tail

    float t = getStepFloat(animStartTime, speed, 2*numLeds-l);

    for(int x=0; x<numLeds; x++)
    {
        leds[x] = (int)constrain( (((x-t)/l+1.2f))*(((x-t)<0)? 1:0)*maxOut, 0, maxOut);
    }
}


void BB8Radar::barShiftRight()
{
    int l = numLeds/2+1;  // length of the bar

    int s = getStep(animStartTime, speed, numLeds+l);

    for(int x=0; x<numLeds; x++)
    {
        if(x<s && x>s-l)
            leds[x] = maxOut;
        else
            leds[x] = 0;
    }
}

void BB8Radar::barShiftLeft()
{
    int l = numLeds/2+1;  // length of the bar

    int s = map(getStep(animStartTime, speed, 1000), 0, 1000, 0, numLeds+l);

    for(int x=0; x<numLeds; x++)
    {
        if(x<s && x>s-l)
            leds[numLeds-x-1] = maxOut;
        else
            leds[numLeds-x-1] = 0;
    }
}

void BB8Radar::pixelSmoothBounce()
{
    // see larsonScanner
    float t = getStepFloat(animStartTime, speed, 2*numLeds-2);

    for(int x=0; x<numLeds; x++)
    {
        leds[x] = constrain((int)((-abs(abs(t-numLeds+1)-x)+1)*maxOut), 0, maxOut);
    }
}


void BB8Radar::larsonScanner()
{
    float l = numLeds/4;
    float t = getStepFloat(animStartTime, speed, 2*numLeds-2);

    for(int x=0; x<numLeds; x++)
    {
        leds[x] = constrain((int)((-abs(abs(t-numLeds+1)-x)+l)*maxOut), 0, maxOut);
    }
}

void BB8Radar::larsonScanner2()
{
    float l = numLeds/4;  // 2>7, 3-11, 4-14
    float t = getStepFloat(animStartTime, speed, 2*numLeds+(l*4-1));

    for(int x=0; x<numLeds; x++)
    {

        leds[x] = constrain((int)((-abs(abs(t-numLeds-2*l)-x-l)+l)*maxOut), 0, maxOut);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
// Fading effects
////////////////////////////////////////////////////////////////////////////////////////////

void BB8Radar::fadeIn()
{
    int s = getStep(animStartTime, speed, maxOut);
    for(int x=0; x<numLeds; x++)
    {
        leds[x] = s;
    }
}


void BB8Radar::fadeOut()
{
    int s = getStep(animStartTime, speed, maxOut);
    for(int x=0; x<numLeds; x++)
    {
        leds[x] = abs(maxOut-s);
    }
}


void BB8Radar::fadeInOut()
{
    int s = getStep(animStartTime, speed, 2*maxOut) - maxOut;

    for(int x=0; x<numLeds; x++)
    {
        leds[x] = abs(maxOut-abs(s));
    }
}

void BB8Radar::flame()
{
    for(int x=0; x<numLeds; x++)
    {
        if (random(4) == 0)
            leds[x] = map(random(30)+70, 0, 100, 0, maxOut);
    }
}

void BB8Radar::IdleEye()
{
     
    for(int x=0; x<numLeds; x++)
    {      
        leds[x] =0 ;
    }  

  leds[this->irisLed] = maxOut;
}

void BB8Radar::setIrisLed( int LedNum){
    this->numLeds=LedNum;
}

int BB8Radar::getIrisLed(){
    return this->irisLed;
}


int BB8Radar::getStep(long t0, long t, int v)
{
  return ((millis()-t0)%t)*v/t;
}

float BB8Radar::getStepFloat(long t0, long t, float v)
{
  return ((millis()-t0)%t)*v/t;
}

float BB8Radar::mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

