#include <mutex>
#define ALWAYS_CALIBRATING CALIBRATION_LOOPS == -1

#define CALIB_OVERRIDE false
#if USING_CALIB_PIN && COMMUNICATION == COMM_SERIAL && PIN_CALIB == 0 && !CALIB_OVERRIDE
  #error "You can't set your calibration pin to 0 over usb. You can calibrate with the BOOT button when using bluetooth only. Set CalibOverride to true to override this."
#endif

bool calibrate = false;
bool calibButton = false;
#if USING_TWO_CALIB_PINS
bool flexionCalib = false;
bool splayCalib = false;
bool calibFlex = false;
bool calibFlexPrev = false;
bool calibSplay = false;
bool calibSplayPrev = false;
#else
bool flexionCalib = true;
bool splayCalib = true;
#endif
int* fingerPos = (int[]){0,0,0,0,0,0,0,0,0,0};

ICommunication* comm;

#if ESP32_DUAL_CORE_SET
//std::mutex fingerPosMutex;
ordered_lock* fingerPosLock = new ordered_lock();
TaskHandle_t Task1;
int threadLoops = 1;
int totalLocks = 0;
int lastMicros = 0;
int fullLoopTime = 0;
int fullLoopTotal = 0;
void getInputs(void* parameter){
    for(;;){
      fullLoopTime = micros() - lastMicros;
      fullLoopTotal += fullLoopTime;
      lastMicros = micros();
      {
        fingerPosLock->lock();
        totalLocks++;
        getFingerPositions(calibrate, calibButton, flexionCalib, splayCalib); //Save finger positions in thread

        fingerPosLock->unlock();
      }
      threadLoops++;
      if (threadLoops%100 == 0){
        vTaskDelay(1); //keep watchdog fed
      }
      delayMicroseconds(1);
    }           
}
#endif

int loops = 0;
void setup() {
  #ifndef ESP32S3
  pinMode(32, INPUT_PULLUP);
  #endif
  pinMode(DEBUG_LED, OUTPUT);
  #ifdef NEOPIXEL 
  neopixelWrite(DEBUG_LED,RGB_BRIGHTNESS,0,0); // Red 
  #else   
  digitalWrite(DEBUG_LED, HIGH); 
  #endif 

  setupInputs();

  #if COMMUNICATION == COMM_SERIAL
    comm = new SerialCommunication();
  #elif COMMUNICATION == COMM_BTSERIAL
    comm = new BTSerialCommunication();
  #elif COMMUNICATION == COMM_BLESERIAL
    comm = new BLESerialCommunication();
  #elif COMMUNICATION == COMM_UWBSERIAL
    comm = new UWBSerialCommunication();
  #endif  
  comm->start();

  #if USING_FORCE_FEEDBACK
    setupServoHaptics();  
  #endif
  
  #if ESP32_DUAL_CORE_SET
    xTaskCreatePinnedToCore(
      getInputs, /* Function to implement the task */
      "Get_Inputs", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      tskIDLE_PRIORITY,  /* Priority of the task */
      &Task1,  /* Task handle. */
      0); /* Core where the task should run */
  #endif
}


int lastMainMicros = micros();
int mainMicros = 0;
int mainMicrosTotal = 0;
int mainloops = 1;

void loop() {
  mainloops++;
  mainMicros = micros() - lastMainMicros;
  mainMicrosTotal += mainMicros;
  lastMainMicros = micros();
  
  if (comm->isOpen()){
    #if !USING_TWO_CALIB_PINS
      #if USING_CALIB_PIN
      calibButton = getButton(PIN_CALIB) != INVERT_CALIB;
      //Serial.println(getButton(PIN_CALIB));
      if (calibButton)
        loops = 0;
      #else
      calibButton = false;
      #endif

      //bool calibrate = false;
      if (loops < CALIBRATION_LOOPS || ALWAYS_CALIBRATING){
        calibrate = true;
        loops++;
      }
      else{
        calibrate = false;
      }
    #else
      // setup previous vals
      calibFlexPrev = calibFlex;
      calibSplayPrev = calibSplay;
      // get new vals
      calibFlex = getButton(PIN_CALIB_FLEX) != INVERT_CALIB_FLEX;
      calibSplay = getButton(PIN_CALIB_SPLAY) != INVERT_CALIB_SPLAY;
      
      // calibration button setup
      if(((calibFlex != calibFlexPrev) && (calibFlexPrev = 0)) || ((calibSplay != calibSplayPrev) && (calibSplayPrev = 0))){
        // if one of the buttons was just pressed set reset mode
        calibButton = true;  // reset
        calibrate = false;   // don't calibrate quite yet
      }
      else if(calibFlex || calibSplay){
        calibButton = false;  // don't reset
        calibrate = true;   // calibrate
      }
      else{
        calibButton = false;  // don't reset
        calibrate = false;   // don't calibrate
      }

      if(calibSplay){
        flexionCalib = false; // not flexion mode
        splayCalib = true;  // splay mode
      }
      else if(calibFlex){
        flexionCalib = true; // flexion mode
        splayCalib = false;  // not splay mode
      }
      else{
        flexionCalib = false; // not flexion mode
        splayCalib = false;  // not splay mode
      }
    #endif

    #if !ESP32_DUAL_CORE_SET
      getFingerPositions(calibrate, calibButton, flexionCalib, splayCalib);
    #endif
    bool joyButton = getButton(PIN_JOY_BTN) != INVERT_JOY;

    #if TRIGGER_GESTURE
    bool triggerButton = triggerGesture(fingerPos);
    #else
    bool triggerButton = getButton(PIN_TRIG_BTN) != INVERT_TRIGGER;
    #endif

    bool aButton = getButton(PIN_A_BTN) != INVERT_A;
    bool bButton = getButton(PIN_B_BTN) != INVERT_B;

    #if GRAB_GESTURE
    bool grabButton = grabGesture(fingerPos);
    #else
    bool grabButton = getButton(PIN_GRAB_BTN) != INVERT_GRAB;
    #endif

    #if PINCH_GESTURE
    bool pinchButton = pinchGesture(fingerPos);
    #else
    bool pinchButton = getButton(PIN_PNCH_BTN) != INVERT_PINCH;
    #endif

    int fingerPosCopy[10];
    int mutexTimeDone;
    
    #if MENU_BUTTON_AB
    bool menuButton = aButton && bButton; // set menu to a + b
    if(menuButton){
      aButton = bButton = false;  // set a and b to false if menu is true.
    }
    #else
    bool menuButton =  getButton(PIN_MENU_BTN) != INVERT_MENU;
    #endif

    {
      #if ESP32_DUAL_CORE_SET
      int mutexTime = micros();
      //const std::lock_guard<std::mutex> lock(fingerPosMutex);
      fingerPosLock->lock();
      mutexTimeDone = micros()-mutexTime;
      #endif
      //memcpy(fingerPosCopy, fingerPos, sizeof(fingerPos));
      for (int i = 0; i < 10; i++){
        fingerPosCopy[i] = fingerPos[i];
      }
      #if ESP32_DUAL_CORE_SET
      fingerPosLock->unlock();
      #endif
      
    }
    comm->output(encode(fingerPosCopy, getJoyX(), getJoyY(), joyButton, triggerButton, aButton, bButton, grabButton, pinchButton, calibButton, menuButton));
    #if USING_FORCE_FEEDBACK
      char received[100];
      if (comm->readData(received)){
        int hapticLimits[5];
        //This check is a temporary hack to fix an issue with haptics on v0.5 of the driver, will make it more snobby code later
        if(String(received).length() >= 5) {
           decodeData(received, hapticLimits);
           writeServoHaptics(hapticLimits); 
        }
      }
    #endif
    delay(LOOP_TIME);
  }
}
