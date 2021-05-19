#include <Keyboard.h>
#include "keymap.h"

#define RESETPIN 2 //Connected to keyboard reset pin, active low

#define NUMKEYS 6 //KRO limit of KB library
#define MESSAGESIZE 110 //Amount of message to grab at a time (>= # of KB keys)
char oldMessage[NUMKEYS];
char tempMessage[MESSAGESIZE];

void setup() {
  Serial.begin(115200); //Debug output
  Serial1.begin(1200, SERIAL_7N1); //Connection to keyboard
  Serial1.setTimeout(20); //May need tuning, lower=better but not too low
  Keyboard.begin(); //HID output (USB keyboard)

  //Enable keyboard
  pinMode(RESETPIN, OUTPUT);
  digitalWrite(RESETPIN, LOW); //Pull low to reset
  delay(500);
  digitalWrite(RESETPIN, HIGH); //High to release
  delay(100);
  while (Serial1.available()) { //Keyboard send junk data after reset
    Serial1.read(); //Discard (it's just 0x7e a bunch of times)
    delay(10);
  }
  Serial1.write(0x37); //Caps lock light on enables keystroke sending
  delay(100);
  Serial1.write(0x38); //Caps lock light off
  delay(100);

  //0 as a key gets ignored so just fill initially
  for (int i=0; i<NUMKEYS; i++){
    oldMessage[i]=0;
  }
  Serial.println("Ready");
}

//Map key code and send over USB
void sendCode(char keycode, bool isDown){
  if (keycode == 0){
    return;
  }
  if (keycode == '@' && isDown) {
    Keyboard.releaseAll();
  }

  //Reference keymap.h and get mapping
  char mapped = keyMap(keycode, isDown);
  if (mapped == 0){
    return;
  }

  //Toggle HID key status
  if (isDown){
    Keyboard.press(mapped);
  } else {
    Keyboard.release(mapped);
  }
}

//Find keys present in old message but absent in new message
void findDiff(char subMessage[]){
  for (int i=0; i<NUMKEYS; i++) {
    bool missing = true;
    for (int j=0; j<NUMKEYS; j++) {
      if (oldMessage[i] == subMessage[j]){
        missing = false;
      }
    }
    if (missing) {
      sendCode(oldMessage[i], false); //Say they've been unpressed
    }
    oldMessage[i] = subMessage[i]; //So the next message can be compared to this one
  }

  //Set all new keys to pressed, duplicates don't matter
  for (int i=0; i<NUMKEYS; i++){
    sendCode(subMessage[i], true);
  }
}

void loop() {
  if (Serial1.available()>0) { //New message from keyboard
    //Start with array filled with ignore code
    for (int i=0; i<MESSAGESIZE; i++){
      tempMessage[i]=0;
    }
    
    Serial1.readBytes(tempMessage, MESSAGESIZE); //Fetch keyboard message, stop once timeout elapsed

    //Cut the entire message into NUMKEYS sized chunks
    char newMessage[NUMKEYS];
    for (int i=0; i<MESSAGESIZE/NUMKEYS; i++){
      for (int j=0; j<NUMKEYS; j++){
        int pos = i*NUMKEYS+j;
        if (pos > MESSAGESIZE){
          newMessage[j]=0;
        } else {
          newMessage[j]=tempMessage[pos];
        }
      }
      findDiff(newMessage); //Send chunk off to be mapped and sent over USB
      if (tempMessage[(i+1)*NUMKEYS] == 0){ //Stop if rest of message is empty
          break;
      }
    }
  }
}
