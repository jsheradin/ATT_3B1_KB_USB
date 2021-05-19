//Map keyboard codes to HID keycodes
//See here for list https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/

int layer = 0; //Change to your desired layer for a key event

//This is a hack and can get out of sync with the PC but it works fine
//See here for how to patch KB lib if you want to do it properly
//https://forum.arduino.cc/t/leonardo-keyboard-leds-emulation/169582/7
bool capsState = false;
void toggleCaps(){
  if (capsState){ //Caps currently enabled
      Serial1.write(0x38); //Turn off light
    } else {
      Serial1.write(0x37);//Turn on light
    }
    capsState = !capsState;
}

//Arduino lib doesn't have separate chars for numpad numbers so
//this just maps them to regular numbers currently.
bool numLockState = false;
void toggleNumLock(){
  if (numLockState){ //Caps currently enabled
      Serial1.write(0x22); //Turn off light
    } else {
      Serial1.write(0x21);//Turn on light
    }
    numLockState = !numLockState;
}

char keyMap(char key, bool isDown) {
  switch (key){
    case 0: //Ignore
      return 0;
    case '@': //All keys released
      if(isDown){
        layer = 0;
      }
      return 0;

    //Below is where the keymap of the keyboard can be changed
    case 'q': //Keycode from keyboard
      return 'q'; //Keycode to send via USB
    case 'w':
      return 'w';
    case 'e':
      return 'e';
    case 'r':
      return 'r';
    case 't':
      return 't';
    case 'y':
      return 'y';
    case 'u':
      return 'u';
    case 'i':
      return 'i';
    case 'o':
      return 'o';
    case 'p':
      return 'p';
    case 'a':
      return 'a';
    case 's':
      return 's';
    case 'd':
      return 'd';
    case 'f':
      return 'f';
    case 'g':
      return 'g';
    case 'h':
      return 'h';
    case 'j':
      return 'j';
    case 'k':
      return 'k';
    case 'l':
      return 'l';
    case 'z':
      return 'z';
    case 'x':
      return 'x';
    case 'c':
      return 'c';
    case 'v':
      return 'v';
    case 'b':
      return 'b';
    case 'n':
      return 'n';
    case 'm':
      return 'm';
    case '1':
      return '1';
    case '2':
      return '2';
    case '3':
      return '3';
    case '4':
      return '4';
    case '5':
      return '5';
    case '6':
      return '6';
    case '7':
      return '7';
    case '8':
      return '8';
    case '9':
      return '9';
    case '0':
      return '0';
    case ' ':
      return ' ';
    case '-':
      return '-';
    case '=':
      return '=';
    case '[':
      return '[';
    case ']':
      return ']';
    case ';':
      return ';';
    case '\'':
      return '\'';
    case '`':
      return '`';
    case ',':
      return ',';
    case '.':
      return '.';
    case '/':
      return '/';
    case '\\':
      return '\\';
    case 0x15:
      return KEY_F1;
    case 0x16:
      return KEY_F2;
    case 0x17:
      return KEY_F3;
    case 0x18:
      return KEY_F4;
    case 0x19:
      return KEY_F5;
    case 0x1a:
      return KEY_F6;
    case 0x1c:
      return KEY_F7;
    case 0x1d:
      return KEY_F8;
    case 0x08:
      return KEY_BACKSPACE;
    case 0x1b:
      return KEY_ESC;
    case '%':
      return KEY_DELETE;
    case 'H':
      return KEY_LEFT_SHIFT;
    case 'E':
      if(isDown){
        toggleCaps();
      }
      return KEY_CAPS_LOCK;
    case 'I':
      return KEY_RIGHT_SHIFT;
    case 'L':
      return KEY_LEFT_CTRL;
    case 'M':
      return KEY_RIGHT_CTRL;
    case '~': //Num Lock key
      if(isDown){
        toggleNumLock();
      }
      return 0;
    case 0x09:
      return KEY_TAB;
    case 0x0D: //Return
      return KEY_RETURN;
    case 0x0A: //Tall enter
      return KEY_LEFT_GUI;
    case 0x01:
      if(numLockState){
        return '2';
      } else {
        return KEY_UP_ARROW;
      }
    case 0x0B:
    if(numLockState){
        return '0';
      } else {
        return KEY_DOWN_ARROW;
      }
    case 0x0E:
      if(numLockState){
        return '-';
      } else {
        return KEY_LEFT_ARROW;
      }
    case 0x0F:
      if(numLockState){
        return '.';
      } else {
        return KEY_RIGHT_ARROW;
     }
    case 0x29:
      return KEY_LEFT_ALT;
    case 0x05:
      if(numLockState){
        return '1';
      } else {
        return 0;
     }
     case 0x0C:
      if(numLockState){
        return '3';
      } else {
        return 0;
     }
     case 0x47:
      if(numLockState){
        return '4';
      } else {
        return 0;
     }
     case 0x4A:
      if(numLockState){
        return '5';
      } else {
        return 0;
     }
     case 0x4B:
      if(numLockState){
        return '6';
      } else {
        return 0;
     }
     case 0x43:
      if(numLockState){
        return '7';
      } else {
        return 0;
     }
     case 0x44:
      if(numLockState){
        return '8';
      } else {
        return 0;
     }
     case 0x46:
      if(numLockState){
        return '9';
      } else {
        return 0;
     }
    
    default:
      Serial.print("Unknown: ");
      Serial.println(key, HEX);
      return 0;
    
  }
}
