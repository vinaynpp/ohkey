/*    Sketch for Prototyping Keyboard V1.2
 *    by Cameron Coward 1/30/21
 *    
 *    Tested on Arduino Uno. Requires custom PCB
 *    and a 74HC595 shift register.
 *    
 *    More info: https://www.hackster.io/cameroncoward/64-key-prototyping-keyboard-matrix-for-arduino-4c9531
 */

const int rowData = 2; // shift register Data pin for rows
const int rowLatch = 3; // shift register Latch pin for rows
const int rowClock = 4; // shift register Clock pin for rows

// these are our column input pins. Pin 0 and Pin 1 are not used,
// because they cause issues (presumably because they're TX and RX)
const int colA = A0; 
const int colB = A1; 
const int colC = A2; 
const int colD = A3;
const int colE = A4;
const int colF = A5;
const int colG = 5;
const int colH = 6;


// shiftRow is the required shift register byte for each row, rowState will contain pressed keys for each row
const byte shiftRow[] = {B01111111, B10111111, B11011111, B11101111, B11110111, B11111011, B11111101, B11111110};
byte rowState[] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};
byte prevRowState[] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};

// ASCII codes for keys with no modifiers pressed. Modifiers are NULL (0),
// because we will check those separately and their values should not be printed.
const char key[] = {
  0, 49, 50, 51, 52, 53, 54, 55,
  56, 57, 48, 45, 61, 0, 9, 113,
  119, 101, 114, 116, 121, 117, 105, 111,
  112, 91, 93, 92, 7, 97, 115, 100,
  102, 103, 104, 106, 107, 108, 59, 39,
  0, 0, 122, 120, 99, 118, 98, 110,
  109, 44, 46, 47, 0, 0, 0, 0,
  32, 0, 0, 0, 0, 0, 0, 0
};

// ASCII codes for keys with shift pressed AND caps is active
const char capsShiftKey[] = {
  0, 33, 64, 35, 36, 37, 94, 38,
  42, 40, 41, 95, 43, 0, 9, 113,
  119, 101, 114, 116, 121, 117, 105, 111,
  112, 123, 125, 124, 7, 97, 115, 100,
  102, 103, 104, 106, 107, 108, 58, 22,
  0, 0, 122, 120, 99, 118, 98, 110,
  109, 44, 46, 47, 0, 0, 0, 0,
  32, 0, 0, 0, 0, 0, 0, 0
};

// ASCII codes for keys with shift pressed.
const char shiftKey[] = {
  0, 33, 64, 35, 36, 37, 94, 38,
  42, 40, 41, 95, 43, 0, 9, 81,
  87, 69, 82, 84, 89, 85, 73, 79,
  80, 123, 125, 124, 7, 65, 83, 68,
  70, 71, 72, 74, 75, 76, 58, 22,
  0, 0, 90, 88, 67, 86, 66, 78,
  77, 44, 46, 47, 0, 0, 0, 0,
  32, 0, 0, 0, 0, 0, 0, 0
};

// ASCII codes for keys with ctrl pressed.
const char ctrlKey[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 9, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  32, 0, 0, 0, 0, 0, 0, 0
};

// ASCII codes for keys with spcl pressed.
const char spclKey[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

// ASCII codes for keys with alt pressed.
const char altKey[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

// ASCII codes for keys with fn pressed.
const char fnKey[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

// ASCII codes for keys with caps is active
const char capsKey[] = {
  0, 49, 50, 51, 52, 53, 54, 55,
  56, 57, 48, 45, 61, 0, 9, 81,
  87, 69, 82, 84, 89, 85, 73, 79,
  80, 91, 93, 92, 7, 65, 83, 68,
  70, 71, 72, 74, 75, 76, 59, 39,
  0, 0, 90, 88, 67, 86, 66, 78,
  77, 44, 46, 47, 0, 0, 0, 0,
  32, 0, 0, 0, 0, 0, 0, 0
};

long previousKeyboardMicros = 0;        // will store last time keyboard was checked
 
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long keyboardInterval = 500;           // interval at which to check keyboard (microseconds)

int rowToCheck = 0; // We check one row per loop of checkKeyboard(), this combined with keyboardInterval 
                    // gives the shiftRegister time to fully update between row checks

bool caps = false;  // is caps lock on?
bool shift = false; // is either left or right shift pressed?
bool capsShift = false; // are shift AND caps active?
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
 mainTimer();
}

void mainTimer() {

  unsigned long currentMicros = micros(); // how many microseconds has the Arduino been running?
  
  if(currentMicros - previousKeyboardMicros > keyboardInterval) { // if elapsed time since last check exceeds the interval
    // save the last time the keyboard was checked
    previousKeyboardMicros = currentMicros;   
 
    checkKeyboard(); // check all of the keys and print out the results to serial
  }
}

void updateShiftRegister(byte row) {
  //this function sets the shift register according to the byte that was passed to it
 
  digitalWrite(rowLatch, LOW); // set latch to low so we can write an entire byte at once
  shiftOut(rowData, rowClock, MSBFIRST, row);  // write that byte
  digitalWrite(rowLatch, HIGH); // set latch back to high so it shift register will remain stable until next change
}

void checkKeyboard() {

  // set the shift register to the current row's byte value, from the shiftRow[] byte array
  updateShiftRegister(shiftRow[rowToCheck]);

  // Check each column
  if (digitalRead(colA) == LOW) {
    bitSet(rowState[rowToCheck], 0);
  } else {
    bitClear(rowState[rowToCheck], 0);
  }
  
  if (digitalRead(colB) == LOW) {
    bitSet(rowState[rowToCheck], 1);
  } else {
    bitClear(rowState[rowToCheck], 1);
  }
  
  if (digitalRead(colC) == LOW) {
    bitSet(rowState[rowToCheck], 2);
  } else {
    bitClear(rowState[rowToCheck], 2);
  }
  
  if (digitalRead(colD) == LOW) {
    bitSet(rowState[rowToCheck], 3);
  } else {
    bitClear(rowState[rowToCheck], 3);
  }
  
  if (digitalRead(colE) == LOW) {
    bitSet(rowState[rowToCheck], 4);
  } else {
    bitClear(rowState[rowToCheck], 4);
  }
  
  if (digitalRead(colF) == LOW) {
    bitSet(rowState[rowToCheck], 5);
  } else {
    bitClear(rowState[rowToCheck], 5);
  }
  
  if (digitalRead(colG) == LOW) {
    bitSet(rowState[rowToCheck], 6);
  } else {
    bitClear(rowState[rowToCheck], 6);
  }
  
  if (digitalRead(colH) == LOW) {
    bitSet(rowState[rowToCheck], 7);
  } else {
    bitClear(rowState[rowToCheck], 7);
  }

  // set all shift register pins to HIGH, this keeps values from "bleeding" over to the next loop
  updateShiftRegister(B11111111);

  rowToCheck = rowToCheck + 1; // iterate to next row

  // after checking the 8th row, check the states (button presses) and then start over on the 1st row
  if (rowToCheck > 7 ) {
    checkPressedKeys();
    rowToCheck = 0;
  }
}

void checkPressedKeys() {
  // check if either shift key is pressed
  if (bitRead(rowState[5], 1) | bitRead(rowState[6], 4)) {
    shift = true;
  } else {
    shift = false;
  }

  // check if either ctrl key is pressed
  if (bitRead(rowState[6], 5) | bitRead(rowState[7], 3)) {
    ctrl = true;
  } else {
    ctrl = false;
  }

  // check if either spcl key is pressed
  if (bitRead(rowState[6], 6) | bitRead(rowState[7], 2)) {
    spcl = true;
  } else {
    spcl = false;
  }

  // check if either alt key is pressed
  if (bitRead(rowState[6], 7) | bitRead(rowState[7], 1)) {
    alt = true;
  } else {
    alt = false;
  }

  // check if FN key is pressed
  if (bitRead(rowState[7], 4)) {
    fn = true;
  } else {
    fn = false;
  }

  // check caps is active and shift is pressed
  if (shift == true && caps == true) {
    capsShift = true;
  } else {
    capsShift = false;
  }
  
  for (int i = 8; i >= 0; i--) {                    // iterate through each row
    for (int j = 7; j >= 0; j--) {                  // iterate through each bit in that row
      
      bool newBit = bitRead(rowState[i], j);             // check the state of that bit
      bool prevBit = bitRead(prevRowState[i], j);         // check the previous state of that bit
      
      if ((newBit == 1) && (prevBit == 0)) {                       // only allows button press if state has changed to true
          int thisChar = (i * 8) + j;               // calculate which position in char array to select

          if (capsShift == true) {
            processKey(capsShiftKey[thisChar]);
          } else if (shift == true) {
            processKey(shiftKey[thisChar]);
          } else if (ctrl == true) {
            processKey(ctrlKey[thisChar]);
          } else if (alt == true) {
            processKey(altKey[thisChar]);
          } else if (spcl == true) {
            processKey(spclKey[thisChar]);
          } else if (fn == true) {
            processKey(fnKey[thisChar]);
          } else if (caps == true) {
            processKey(capsKey[thisChar]);
          } else {
            processKey(key[thisChar]);     
          }
      }
      
      if (newBit == 1) {
          bitSet(prevRowState[i], j);     // set previous bit state to true if a key is pressed
      } else {
          bitClear(prevRowState[i], j);   // set previous bit state to false if key isn't pressed, so it can be pressed again
      }
    }
  }
}

void processKey(char receivedKey) {
  if (receivedKey == 7) {                 // check for special functions in the same way as caps (add new "else if" statements)
    caps = !caps;
  } else {
    Serial.print(receivedKey);            // if char does not correspond to a special function, simply print that char
  }
}
