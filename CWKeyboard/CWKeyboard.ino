#include <BleKeyboard.h>
#include "CWDefine.h"

//BT keyboard
BleKeyboard bleKeyboard;
//CW timer
static hw_timer_t *tm = NULL;
static portMUX_TYPE tmMux = portMUX_INITIALIZER_UNLOCKED;

// CW state
typedef enum {
  //no cw
  kCWStateNone = 0,
  //short signal
  kCWStateShort,
  //short signal space
  kCWStateShortBlank,
  //long signal
  kCWStateLong,
  //longsignal space
  kCWStateLongBlank,
  //character output term
  kCWStateBlank,
} eCWState;
static volatile eCWState state = kCWStateNone;
//short -> long, long -> short flag
static volatile bool nextFlg = false;

//signal terms
#define kShortCnt (1)
#define kLongCnt (3)
#define kBlankCnt (2)
static volatile int cwCnt = 0;

//output port
#define kLedPin (15)
#define kPWMPin (12)
#define kPWMCh (1)
#define kPWMFreq (700)
static volatile int ledState = HIGH;

//paddle input pin
#define kShortPin (27)
#define kLongPin (26)
//cw short signal time[us]
#define kCWMinTimeCount (50000)

//CW character branch
static volatile pCWBranch cwChara = pcw_Root;

//cw sound on(led pin LOW)
void cwOn();
//cw sound off(led pin HIGH)
void cwOff();
//cw timer on
//@param cnt: kShortCnt, kLongCnt kBlankCnt, 0 is timer continue
void timerOn(int cnt);
//timer off
void timerOff();
//timer interrupt function
void IRAM_ATTR interruptFunc();

//initialize
void setup() {
  //serial port begin
  Serial.begin(115200);
  Serial.println("start");

  //setup each pin
  pinMode(kLedPin, OUTPUT);
  digitalWrite(kLedPin, ledState);
  pinMode(kShortPin, INPUT_PULLUP);
  pinMode(kLongPin, INPUT_PULLUP);

  pinMode(kPWMPin, OUTPUT);
  ledcSetup(kPWMCh, kPWMFreq, 16);

  //timer setup
  //clock:80MHz, (_,80,_) -> 1us
  tm = timerBegin(0, 80, true);
  timerAttachInterrupt(tm, &interruptFunc, true);
  timerAlarmWrite(tm, kCWMinTimeCount, true);


  //BTKeyboard setup
  bleKeyboard.begin();
}

//main loop
void loop() {
  //timer cli()
  portENTER_CRITICAL_ISR(&tmMux);
  //read paddle pin state
  switch (state) {
    //user input
    case kCWStateNone:
    case kCWStateBlank:
      if (digitalRead(kShortPin) == LOW) {
        state = kCWStateShort;
        cwChara = cwChara->nextShort;
        Serial.println(cwChara->chara);
        cwOn();
        timerOn(kShortCnt);
      } else if (digitalRead(kLongPin) == LOW) {
        state = kCWStateLong;
        cwChara = cwChara->nextLong;
        Serial.println(cwChara->chara);
        cwOn();
        timerOn(kLongCnt);
      }
      break;
    //short -> long input read
    case kCWStateShort:
    case kCWStateShortBlank:
      if (digitalRead(kLongPin) == LOW) nextFlg = true;
      break;
    //long -> short input read
    case kCWStateLong:
    case kCWStateLongBlank:
      if (digitalRead(kShortPin) == LOW) nextFlg = true;
      break;
  }
  //timer sei()
  portEXIT_CRITICAL_ISR(&tmMux);
  //loop delay
  delay(10);
}

//cw output on
void cwOn() {
  digitalWrite(kLedPin, LOW);
  ledcAttachPin(kPWMPin, kPWMCh);
  ledcWriteTone(kPWMCh, kPWMFreq);
}

//cw output off
void cwOff() {
  digitalWrite(kLedPin, HIGH);
  ledcDetachPin(kPWMPin);
}

//cw timer on
//@param cnt: kShortCnt, kLongCnt kBlankCnt, 0 is timer continue
void timerOn(int cnt) {
  cwCnt = (cnt == 0) ? cwCnt : cnt;
  timerAlarmEnable(tm);
}
//cw timer off
void timerOff() {
  timerAlarmDisable(tm);
}

//timer interrupt function
void IRAM_ATTR interruptFunc() {
  //timer cli()
  portENTER_CRITICAL_ISR(&tmMux);
  //continue timer
  if (0 <= --cwCnt) {
    timerOn(0);
  } else {
    //change state
    switch (state) {
      //short space time
      case kCWStateShort:
        state = kCWStateShortBlank;
        cwOff();
        timerOn(kShortCnt);
        break;
      //long space time
      case kCWStateLong:
        state = kCWStateLongBlank;
        cwOff();
        timerOn(kShortCnt);
        break;
      //long signal or end blank
      case kCWStateShortBlank:
        if (nextFlg) {
          state = kCWStateLong;
          cwChara = cwChara->nextLong;
          Serial.println(cwChara->chara);
          cwOn();
          timerOn(kLongCnt);
          nextFlg = false;
        } else {
          state = kCWStateBlank;
          timerOn(kBlankCnt);
        }
        break;
      //short signal or end blank
      case kCWStateLongBlank:
        if (nextFlg) {
          state = kCWStateShort;
          cwChara = cwChara->nextShort;
          Serial.println(cwChara->chara);
          cwOn();
          timerOn(kShortCnt);
          nextFlg = false;
        } else {
          state = kCWStateBlank;
          timerOn(kBlankCnt);
        }
        break;
      //character input end
      case kCWStateBlank:
        state = kCWStateNone;
        Serial.print("your char : ");
        Serial.println(cwChara->chara);
        if (bleKeyboard.isConnected()) {
          bleKeyboard.print(cwChara->chara);
        }
        cwChara = pcw_Root;
        timerOff();
        break;
    }
  }
  portEXIT_CRITICAL_ISR(&tmMux);
}
