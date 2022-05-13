/*    Sketch for Prototyping Keyboard V1.1
 *    by Cameron Coward 1/17/21
 *    
 *    Tested on Arduino Uno. Requires custom PCB
 *    and a 74HC595 shift register.
 *    
 *    More info: https://www.hackster.io/cameroncoward/projects
 */

int rowData = 2; // shift register Data pin for rows
int rowLatch = 3; // shift register Latch pin for rows
int rowClock = 4; // shift register Clock pin for rows

// these are our column input pins. Pin 0 and Pin 1 are not used,
// because they cause issues (presumably because they're TX and RX)
int colA = A0; 
int colB = A1; 
int colC = A2; 
int colD = A3;
int colE = A4;
int colF = A5;
int colG = 5;
int colH = 6;

byte shiftRows = B11111111; // Byte to store pin states for row shift register

long previousMillis = 0;        // will store last time keyboard was checked
 
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 5;           // interval at which to check keyboard (milliseconds)

int lastKey = 0;  // this stores the last key that was pressed (#1-64)
int keyReset = 0; // this counts loops until a key can be pressed again
int keysPressed = 0; // this counts how many keys were pressed in a loop

bool caps = false;  // is caps lock on?
bool rShift = false;  // is the right shift key pressed?
bool lShift = false;  // is the left shift key pressed?
bool shift = false; // is either left or right shift pressed?
bool ctrl = false; // is the ctrl key pressed?
bool spcl = false;  // is the spcl key pressed?
bool alt = false; // is the alt key pressed?
bool fn = false;  // is the function key pressed?

void setup() {
  Serial.begin(9600);

  // setup all column pin as inputs with internal pullup resistors
  pinMode(colA, INPUT_PULLUP); 
  pinMode(colB, INPUT_PULLUP);
  pinMode(colC, INPUT_PULLUP);
  pinMode(colD, INPUT_PULLUP);
  pinMode(colE, INPUT_PULLUP); 
  pinMode(colF, INPUT_PULLUP);
  pinMode(colG, INPUT_PULLUP);
  pinMode(colH, INPUT_PULLUP);

  // the outputs needed to control the 74HC595 shift register
  pinMode(rowLatch, OUTPUT);
  pinMode(rowClock, OUTPUT);
  pinMode(rowData, OUTPUT);

  updateShiftRegister(B11111111); // make sure shift register starts at all HIGH
}

void loop() {
  unsigned long currentMillis = millis(); // how many milliseconds has the Arduino been running?
 
  if(currentMillis - previousMillis > interval) { // if elapsed time since last check exceeds the interval
    // save the last time the keyboard was checked
    previousMillis = currentMillis;   
 
    checkKeyboard(); // check all of the keys and print out the results to serial
  }
}

void updateShiftRegister(byte rows) {
  //this function sets the shift register according to the byte that was passed to it
  
  digitalWrite(rowLatch, LOW); // set latch to low so we can write an entire byte at once
  shiftOut(rowData, rowClock, MSBFIRST, rows);  // write that byte
  digitalWrite(rowLatch, HIGH); // set latch back to high so it shift register will remain stable until next change
}

void checkKeyboard() {

  keysPressed = 0;  // no keys have been pressed

  /*    we start by "activating" a row (setting it to LOW)
   *    then we check each column in that row. If it is LOW, 
   *    we can assume that the corresponding key in the matrix
   *    is pressed. If it is, we add to the keysPressed counter
   *    so that we know if any keys have been pressed on this
   *    loop.
   *    
   *    To avoid "sticky keys" (repeated presses when key is
   *    briefly held down), we only allow a key press if that
   *    key hasn't recently been pressed (within 300ms). The
   *    exceptions are if the key has first been released or
   *    if another key has been pressed. This doesn't apply
   *    to some keys, like shift, which are meant to be held
   *    down.
   *    
   *    Once we determine that the key was meant to be pressed,
   *    we then have to decide what to print. This is dependent
   *    on the modifiers. It is currently setup for a standard
   *    keyboard layout without many modifier or special
   *    functions. Modifiers take precedence in order, so FN
   *    overules SPCL, which overules shift, and so on.
   *    
   *    Shift and CAPS are special cases, because characters
   *    are only capitalized if one or the other is active,
   *    but not when both are. Further, some keys are effected
   *    by shift but not CAPS (like punctuation and numbers).
   *    The latter kinds of keys have a slightly different
   *    algorithm. 
   *    
   *    Some keys, like USER1, SPCL2, etc. have no assigned
   *    function. You can use them however you like. 
   *    
   *    Additional information and algorithms are at the end
   *    of this function.
   */

  // Set Row 1 on shift register to LOW
  updateShiftRegister(B11111110); 

  // Check each column
  if (digitalRead(colA) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 1) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        Serial.print(" ");
      } else if (ctrl == true) {
        Serial.print(" ");
      } else if ((caps == true) && (shift == true)){
        Serial.print(" "); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print(" "); // if either shift or caps lock are active, but not both
      } else {
        Serial.print(" "); // if no modifiers are active
      }
      lastKey = 1;
      keyReset = 0;
    }    
  }
  if (digitalRead(colB) == LOW) {
  }
  if (digitalRead(colC) == LOW) {
  }
  if (digitalRead(colD) == LOW) {
  }
  if (digitalRead(colE) == LOW) {
    fn = true;
  } else {
    fn = false;
  }
  if (digitalRead(colF) == LOW) {
  }
  if (digitalRead(colG) == LOW) {
  }
  if (digitalRead(colH) == LOW) {
  }

  // Set Row 2 on shift register to LOW
  updateShiftRegister(B11111101);

  // Check each column
  if (digitalRead(colA) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 9) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("m"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("M"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("m"); // if no modifiers are active
      }
      lastKey = 9;
      keyReset = 0;
    }
  }
  if (digitalRead(colB) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 10) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print("<"); // if shift is pressed while caps lock is active
      } else {
        Serial.print(","); // if no modifiers are active
      }
      lastKey = 10;
      keyReset = 0;
    }
  }
  if (digitalRead(colC) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 11) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print(">"); // if shift is pressed while caps lock is active
      } else {
        Serial.print("."); // if no modifiers are active
      }
      lastKey = 11;
      keyReset = 0;
    }
  }
  if (digitalRead(colD) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 12) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print("?"); // if shift is pressed while caps lock is active
      } else {
        Serial.print("/"); // if no modifiers are active
      }
      lastKey = 12;
      keyReset = 0;
    }
  }
  if (digitalRead(colE) == LOW) {
    rShift = true;
  } else {
    rShift = false;
  }
  if (digitalRead(colF) == LOW) {
    ctrl = true;
  } else {
    ctrl = false;
  }
  if (digitalRead(colG) == LOW) {
    spcl = true;
  } else {
    spcl = false;
  }
  if (digitalRead(colH) == LOW) {
    alt = true;
  } else {
    alt = false;
  }

  // Set Row 3 on shift register to LOW
  updateShiftRegister(B11111011);

  // Check each column
  if (digitalRead(colA) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 17) {
      Serial.print("\r\n");
      lastKey = 17;
      keyReset = 0;
    }
  }
  if (digitalRead(colB) == LOW) {
    lShift = true;
  } else {
    lShift = false;
  }
  if (digitalRead(colC) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 19) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("z"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("Z"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("z"); // if no modifiers are active
      }
      lastKey = 19;
      keyReset = 0;
    }
  }
  if (digitalRead(colD) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 20) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("x"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("X"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("x"); // if no modifiers are active
      }
      lastKey = 20;
      keyReset = 0;
    }
  }
  if (digitalRead(colE) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 21) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("c"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("C"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("c"); // if no modifiers are active
      }
      lastKey = 21;
      keyReset = 0;
    }
  }
  if (digitalRead(colF) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 22) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("v"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("V"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("v"); // if no modifiers are active
      }
      lastKey = 22;
      keyReset = 0;
    }
  }
  if (digitalRead(colG) == LOW) {
    keysPressed = keysPressed + 1;
    
    if (lastKey != 23) {     
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("b"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("B"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("b"); // if no modifiers are active
      }
      lastKey = 23;
      keyReset = 0;
    }
  }
  if (digitalRead(colH) == LOW) {
    keysPressed = keysPressed + 1;
    
    if (lastKey != 24) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("n"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("N"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("n"); // if no modifiers are active
      }
      lastKey = 24;
      keyReset = 0;
    }
  }

  // Set Row 4 on shift register to LOW
  updateShiftRegister(B11110111);

  // Check each column
  if (digitalRead(colA) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 25) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("f"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("F"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("f"); // if no modifiers are active
      }
      lastKey = 25;
      keyReset = 0;
    }
  }
  if (digitalRead(colB) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 26) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("g"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("G"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("g"); // if no modifiers are active
      }
      lastKey = 26;
      keyReset = 0;
    }
  }
  if (digitalRead(colC) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 27) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("h"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("H"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("h"); // if no modifiers are active
      }
      lastKey = 27;
      keyReset = 0;
    }
  }
  if (digitalRead(colD) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 28) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("j"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("J"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("j"); // if no modifiers are active
      }
      lastKey = 28;
      keyReset = 0;
    }
  }
  if (digitalRead(colE) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 29) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("k"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("K"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("k"); // if no modifiers are active
      }
      lastKey = 29;
      keyReset = 0;
    }
  }
  if (digitalRead(colF) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 30) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("l"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("L"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("l"); // if no modifiers are active
      }
      lastKey = 30;
      keyReset = 0;
    }
  }
  if (digitalRead(colG) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 31) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print(":"); // if shift is pressed while caps lock is active
      } else {
        Serial.print(";"); // if no modifiers are active
      }
      lastKey = 31;
      keyReset = 0;
    }
  }
  if (digitalRead(colH) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 32) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print('"'); // if shift is pressed while caps lock is active
      } else {
        Serial.print("'"); // if no modifiers are active
      }
      lastKey = 32;
      keyReset = 0;
    }
  }

  // Set Row 5 on shift register to LOW
  updateShiftRegister(B11101111);

  // Check each column
  if (digitalRead(colA) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 33) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("p"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("P"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("p"); // if no modifiers are active
      }
      lastKey = 33;
      keyReset = 0;
    }
  }
  if (digitalRead(colB) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 34) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print("{"); // if shift is pressed while caps lock is active
      } else {
        Serial.print("["); // if no modifiers are active
      }
      lastKey = 34;
      keyReset = 0;
    }
  }
  if (digitalRead(colC) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 35) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print("}"); // if shift is pressed while caps lock is active
      } else {
        Serial.print("]"); // if no modifiers are active
      }
      lastKey = 35;
      keyReset = 0;
    }
  }
  if (digitalRead(colD) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 36) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print("|"); // if shift is pressed while caps lock is active
      } else {
        Serial.print("\\"); // if no modifiers are active
      }
      lastKey = 36;
      keyReset = 0;
    }
  }
  if (digitalRead(colE) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 37) {
      caps = !caps; // toggles caps lock
      lastKey = 37;
      keyReset = 0;
    }
  }
  if (digitalRead(colF) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 38) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("a"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("A"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("a"); // if no modifiers are active
      }
      lastKey = 38;
      keyReset = 0;
    }
  }
  if (digitalRead(colG) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 39) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("s"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("S"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("s"); // if no modifiers are active
      }
      lastKey = 39;
      keyReset = 0;
    }
  }
  if (digitalRead(colH) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 40) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("d"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("D"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("d"); // if no modifiers are active
      }
      lastKey = 40;
      keyReset = 0;
    }
  }

  // Set Row 6 on shift register to LOW
  updateShiftRegister(B11011111);

  // Check each column
  if (digitalRead(colA) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 41) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("w"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("W"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("w"); // if no modifiers are active
      }
      lastKey = 41;
      keyReset = 0;
    }
  }
  if (digitalRead(colB) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 42) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("e"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("E"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("e"); // if no modifiers are active
      }
      lastKey = 42;
      keyReset = 0;
    }
  }
  if (digitalRead(colC) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 43) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("r"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("R"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("r"); // if no modifiers are active
      }
      lastKey = 43;
      keyReset = 0;
    }
  }
  if (digitalRead(colD) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 44) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("t"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("T"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("t"); // if no modifiers are active
      }
      lastKey = 44;
      keyReset = 0;
    }
  }
  if (digitalRead(colE) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 45) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("y"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("Y"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("y"); // if no modifiers are active
      }
      lastKey = 45;
      keyReset = 0;
    }
  }
  if (digitalRead(colF) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 46) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("u"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("U"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("u"); // if no modifiers are active
      }
      lastKey = 46;
      keyReset = 0;
    }
  }
  if (digitalRead(colG) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 47) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("i"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("I"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("i"); // if no modifiers are active
      }
      lastKey = 47;
      keyReset = 0;
    }
  }
  if (digitalRead(colH) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 48) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("o"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("O"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("o"); // if no modifiers are active
      }
      lastKey = 48;
      keyReset = 0;
    }
  }

  // Set Row 7 on shift register to LOW
  updateShiftRegister(B10111111);

  // Check each column
  if (digitalRead(colA) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 49) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print("*"); // if shift is pressed while caps lock is active
      } else {
        Serial.print("8"); // if no modifiers are active
      }
      lastKey = 49;
      keyReset = 0;
    }
  }
  if (digitalRead(colB) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 50) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print("("); // if shift is pressed while caps lock is active
      } else {
        Serial.print("9"); // if no modifiers are active
      }
      lastKey = 50;
      keyReset = 0;
    }
  }
  if (digitalRead(colC) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 51) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print(")"); // if shift is pressed while caps lock is active
      } else {
        Serial.print("0"); // if no modifiers are active
      }
      lastKey = 51;
      keyReset = 0;
    }
  }
  if (digitalRead(colD) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 52) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print("_"); // if shift is pressed while caps lock is active
      } else {
        Serial.print("-"); // if no modifiers are active
      }
      lastKey = 52;
      keyReset = 0;
    }
  }
  if (digitalRead(colE) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 53) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print("+"); // if shift is pressed while caps lock is active
      } else {
        Serial.print("="); // if no modifiers are active
      }
      lastKey = 53;
      keyReset = 0;
    }
  }
  if (digitalRead(colF) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 54) {
      lastKey = 54;
      keyReset = 0;
    }
  }
  if (digitalRead(colG) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 55) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print("    "); // if shift is pressed while caps lock is active
      } else {
        Serial.print("  "); // if no modifiers are active
      }
      lastKey = 55;
      keyReset = 0;
    }
  }
  if (digitalRead(colH) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 56) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if ((caps == true) && (shift == true)){
        Serial.print("q"); // if shift is pressed while caps lock is active
      } else if ((caps == true) || (shift == true)){
        Serial.print("Q"); // if either shift or caps lock are active, but not both
      } else {
        Serial.print("q"); // if no modifiers are active
      }
      lastKey = 56;
      keyReset = 0;
    }
  }

  // Set Row 8 on shift register to LOW
  updateShiftRegister(B01111111);

  // Check each column
  if (digitalRead(colA) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 57) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print("~"); // if shift is pressed while caps lock is active
      } else {
      }
      lastKey = 57;
      keyReset = 0;
    }
  }
  if (digitalRead(colB) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 58) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print("!"); // if shift is pressed while caps lock is active
      } else {
        Serial.print("1"); // if no modifiers are active
      }
      lastKey = 58;
      keyReset = 0;
    }
  }
  if (digitalRead(colC) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 59) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print("@"); // if shift is pressed while caps lock is active
      } else {
        Serial.print("2"); // if no modifiers are active
      }
      lastKey = 59;
      keyReset = 0;
    }
  }
  if (digitalRead(colD) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 60) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print("#"); // if shift is pressed while caps lock is active
      } else {
        Serial.print("3"); // if no modifiers are active
      }
      lastKey = 60;
      keyReset = 0;
    }
  }
  if (digitalRead(colE) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 61) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print("$"); // if shift is pressed while caps lock is active
      } else {
        Serial.print("4"); // if no modifiers are active
      }
      lastKey = 61;
      keyReset = 0;
    }
  }
  if (digitalRead(colF) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 62) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print("%"); // if shift is pressed while caps lock is active
      } else {
        Serial.print("5"); // if no modifiers are active
      }
      lastKey = 62;
      keyReset = 0;
    }
  }
  if (digitalRead(colG) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 63) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print("^"); // if shift is pressed while caps lock is active
      } else {
        Serial.print("6"); // if no modifiers are active
      }
      lastKey = 63;
      keyReset = 0;
    }
  }
  if (digitalRead(colH) == LOW) {
    keysPressed = keysPressed + 1;
    if (lastKey != 64) {
      if (fn == true) {
        // no fn definition
      } else if (spcl == true) {
        // no spcl definition
      } else if (alt == true) {
        // no alt definition
      } else if (ctrl == true) {
        // no ctrl definition
      } else if (shift == true){
        Serial.print("&"); // if shift is pressed while caps lock is active
      } else {
        Serial.print("7"); // if no modifiers are active
      }
      lastKey = 64;
      keyReset = 0;
    }
  }

  // if either shift key is pressed, then the shift function is enabled
  if ((lShift) || (rShift)) {
    shift = true;
  } else {
    shift = false;
  }

  // Set all rows back to HIGH
  updateShiftRegister(B11111111);

  // This allows a pressed key to be pressed again after the specified number of loops
  keyReset = keyReset + 1;
  if (keyReset > 60) {
    lastKey = 0;
    keyReset = 0;
  }

  // This allows a pressed key to be pressed again if it was released first (by detecting if NO keys were pressed for this loop)
  if (keysPressed == 0) {
    lastKey = 0;
  }

}
