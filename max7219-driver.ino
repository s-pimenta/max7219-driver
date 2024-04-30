/* No-decode mode
* LED Segments:         A
*                     ----
*                   F|    |B
*                    |  G |
*                     ----
*                   E|    |C
*                    |    |
*                     ----  o DP
*                       D
*   Register bits:
*      bit:  7  6  5  4  3  2  1  0
*           DP  A  B  C  D  E  F  G
*/

#include <SPI.h>

#define SCK_PIN 13   //Clock signal  [SCK (Serial Clock)] -> [CLK] Pin (13) on MAX719
#define CS_PIN 10    //Chip Select [Slave Select pin (SS)] -> [LOAD / CS] Pin (12) (Load-Data Input)
#define COPI_PIN 11  //Controller Out Peripheral In [Master Out Slave In (MOSI)] -> [DIN] Pin (1) (Serial-Data Input)

//*** Register macros ***

//No Operation register
#define NO_OP 0x00

//Digit register
#define DIGIT_0 0x01
#define DIGIT_1 0x02
#define DIGIT_2 0x03
#define DIGIT_3 0x04
#define DIGIT_4 0x05
#define DIGIT_5 0x06
#define DIGIT_6 0x07
#define DIGIT_7 0x08

//Defines wich digits to decode
#define DECODE_MODE 0x09
//Some of the available options
/* Register Data
D7 | D6 | D5 | D4 | D3 | D2 | D1 | D0 |
 1 |  1 |  1 |  1 |  1 |  1 |  1 |  1 | 0b11111111 -> DECODE_MODE_ALL
 0 |  0 |  1 |  1 |  0 |  0 |  1 |  0 | 0b00110010 -> Decode Mode on digit: 2, 5 and 6
*/
#define DECODE_MODE_NONE 0b00000000
#define DECODE_MODE_ALL 0b11111111
 
//Code B Font (BCD format)
#define CHAR_0 0x00    // 0 (digit zero)
#define CHAR_1 0x01    // 1 (digit one)
#define CHAR_2 0x02    // 2 (digit two)
#define CHAR_3 0x03    // 3 (digit three)
#define CHAR_4 0x04    // 4 (digit four)
#define CHAR_5 0x05    // 5 (digit five)
#define CHAR_6 0x06    // 6 (digit six)
#define CHAR_7 0x07    // 7 (digit seven)
#define CHAR_8 0x08    // 8 (digit eight)
#define CHAR_9 0x09    // 9 (digit nine)
#define CHAR_DASH 0x0A // - (char Dash)
#define CHAR_E 0x0B    // E (letter "E")
#define CHAR_H 0x0C    // H (letter "H")
#define CHAR_L 0x0D    // L (letter "L")
#define CHAR_P 0x0E    // P (letter "P")
#define CHAR_BLANK 0x0F// " " (blank space)

//Defines the display led intensity (16 levels available)
#define INTENSITY 0x0A
//Available options
#define INTENSITY_1 0x00 //Minimum intensity
#define INTENSITY_2 0x01
#define INTENSITY_3 0x02
#define INTENSITY_4 0x03
#define INTENSITY_5 0x04
#define INTENSITY_6 0x05
#define INTENSITY_7 0x06
#define INTENSITY_8 0x07
#define INTENSITY_9 0x08
#define INTENSITY_10 0x09
#define INTENSITY_11 0x0A
#define INTENSITY_12 0x0B
#define INTENSITY_13 0x0C
#define INTENSITY_14 0x0D
#define INTENSITY_15 0x0E
#define INTENSITY_16 0x0F //Maximum intensity

// Defines the number of digits displayed on the display (1 to 8 digits)
#define SCAN_LIMIT   0x0B
//Available options
#define SCAN_LIMIT_1 0x00
#define SCAN_LIMIT_2 0x01
#define SCAN_LIMIT_3 0x02
#define SCAN_LIMIT_4 0x03
#define SCAN_LIMIT_5 0x04
#define SCAN_LIMIT_6 0x05
#define SCAN_LIMIT_7 0x06
#define SCAN_LIMIT_8 0x07

//Turn off the display
#define SHUTDOWN_MODE 0x0C
// Available options
#define SHUTDOWN_MODE_ON 0x00  //(Shutdown Mode)
#define SHUTDOWN_MODE_OFF 0x01 //(Normal Operation)

/* Display test register turns on all LED of the display
NOTE: Regardless of the previously set SCAN_LIMIT (how many digits to display),
the DISPLAY_TEST turns on all digits, but does not override the previous SCAN_LIMIT */
#define DISPLAY_TEST 0x0F
// Available options
#define DISPLAY_TEST_ON 0x01 //(Display Test Mode)
#define DISPLAY_TEST_OFF 0x00 //(Normal Operation)

void setup() {
  max7219Setup(); //initializes the max7219 driver

  delay(3000);

  sendData(SCAN_LIMIT, SCAN_LIMIT_6);
  sendData(INTENSITY, INTENSITY_1);
 
  //sendData(DECODE_MODE, 0xFC); // 1-6
  delay(3000);
  sendData(SHUTDOWN_MODE, SHUTDOWN_MODE_OFF);
  delay(3000);
  sendData(DISPLAY_TEST, DISPLAY_TEST_ON);
  delay(3000);
  sendData(DISPLAY_TEST, DISPLAY_TEST_OFF);

  delay(3000);

  sendData(DECODE_MODE, DECODE_MODE_ALL);
  sendData(DIGIT_0, CHAR_1);
  delay(3000);
  sendData(DIGIT_5, CHAR_6);
  delay(3000);
  sendData(DIGIT_1, CHAR_2);
  delay(3000);
  sendData(DIGIT_4, CHAR_5);
  delay(3000);
  sendData(DIGIT_2, CHAR_3);
  delay(3000);
  sendData(DIGIT_3, CHAR_4);
}

void loop() {
  setDisplayIntensity(INTENSITY_2);
  delay(100);
  setDisplayIntensity(INTENSITY_3);
  delay(100);
  setDisplayIntensity(INTENSITY_4);
  delay(100);
  setDisplayIntensity(INTENSITY_5);
  delay(100);
  setDisplayIntensity(INTENSITY_6);
  delay(100);
  setDisplayIntensity(INTENSITY_7);
  delay(100);
  setDisplayIntensity(INTENSITY_8);
  delay(100);
  setDisplayIntensity(INTENSITY_9);
  delay(100);
  setDisplayIntensity(INTENSITY_10);
  delay(100);
  setDisplayIntensity(INTENSITY_11);
  delay(100);
  setDisplayIntensity(INTENSITY_12);
  delay(100);
  setDisplayIntensity(INTENSITY_13);
  delay(100);
  setDisplayIntensity(INTENSITY_14);
  delay(100);
  setDisplayIntensity(INTENSITY_15);
  delay(100);
  setDisplayIntensity(INTENSITY_16);
  delay(100);
  setDisplayIntensity(INTENSITY_15);
  delay(100);
  setDisplayIntensity(INTENSITY_14);
  delay(100);
  setDisplayIntensity(INTENSITY_13);
  delay(100);
  setDisplayIntensity(INTENSITY_12);
  delay(100);
  setDisplayIntensity(INTENSITY_11);
  delay(100);
  setDisplayIntensity(INTENSITY_9);
  delay(100);
  setDisplayIntensity(INTENSITY_8);
  delay(100);
  setDisplayIntensity(INTENSITY_7);
  delay(100);
  setDisplayIntensity(INTENSITY_6);
  delay(100);
  setDisplayIntensity(INTENSITY_5);
  delay(100);
  setDisplayIntensity(INTENSITY_4);
  delay(100);
  setDisplayIntensity(INTENSITY_3);
  delay(100);
  setDisplayIntensity(INTENSITY_2);
  delay(100);
  setDisplayIntensity(INTENSITY_1);
  delay(100);
    
}

// max7219 initialization setup
void max7219Setup() {
  pinMode(CS_PIN, OUTPUT);  // Sets the CS Pin to output

  SPI.begin();
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));

  digitalWrite(CS_PIN, HIGH);  // Sets the CS Pin to HIGH

  sendData(DISPLAY_TEST, DISPLAY_TEST_OFF); //clears the Display Test
  sendData(SHUTDOWN_MODE, SHUTDOWN_MODE_ON); //ensures the display is off

  max7219Clear(); //Clears the screen
}

//clears the digits regiters
void max7219Clear() {
  sendData(DECODE_MODE, 0xFF); //all digits
  sendData(DIGIT_0, CHAR_BLANK);
  sendData(DIGIT_1, CHAR_BLANK);
  sendData(DIGIT_2, CHAR_BLANK);
  sendData(DIGIT_3, CHAR_BLANK);
  sendData(DIGIT_4, CHAR_BLANK);
  sendData(DIGIT_5, CHAR_BLANK);
  sendData(DIGIT_6, CHAR_BLANK);
  sendData(DIGIT_7, CHAR_BLANK);
}

void sendData (byte address, byte data) {
  digitalWrite(CS_PIN, LOW);  // Sets the CS Pin to LOW
  SPI.transfer(address);
  SPI.transfer(data);
  digitalWrite(CS_PIN, HIGH);  // Sets the CS Pin to HIGH
}

void shutdown (bool turnOff) {
  if (turnOff == true) {
    sendData(SHUTDOWN_MODE, SHUTDOWN_MODE_ON);
  }
  else { //false
    sendData(SHUTDOWN_MODE, SHUTDOWN_MODE_OFF);
  }
}

void displayTest (bool turnOn) {
  if (turnOn == true) {
    sendData(DISPLAY_TEST, DISPLAY_TEST_ON);
  }
  else { //false
    sendData(DISPLAY_TEST, DISPLAY_TEST_OFF);
  }
}

// Digits displayed = 1-8
void setScanLimit (byte numDigits) {
  switch (numDigits) {
    case 1:
      sendData (SCAN_LIMIT, SCAN_LIMIT_1);
      break;
    case 2:
      sendData (SCAN_LIMIT, SCAN_LIMIT_2);
      break;
    case 3:
      sendData (SCAN_LIMIT, SCAN_LIMIT_3);
      break;
    case 4:
      sendData (SCAN_LIMIT, SCAN_LIMIT_4);
      break;
    case 5:
      sendData (SCAN_LIMIT, SCAN_LIMIT_5);
      break;
    case 6:
      sendData (SCAN_LIMIT, SCAN_LIMIT_6);
      break;
    case 7:
      sendData (SCAN_LIMIT, SCAN_LIMIT_7);
      break;
    case 8:
      sendData (SCAN_LIMIT, SCAN_LIMIT_8);
      break;
  }
}

// Intensity levels = (min)1 - 16(max)
void setDisplayIntensity (byte intensity) {
  switch (intensity) {
    case 1:
      sendData (INTENSITY, INTENSITY_1);
      break;
    case 2:
      sendData (INTENSITY, INTENSITY_2);
      break;
    case 3:
      sendData (INTENSITY, INTENSITY_3);
      break;
    case 4:
      sendData (INTENSITY, INTENSITY_4);
      break;
    case 5:
      sendData (INTENSITY, INTENSITY_5);
      break;
    case 6:
      sendData (INTENSITY, INTENSITY_6);
      break;
    case 7:
      sendData (INTENSITY, INTENSITY_7);
      break;
    case 8:
      sendData (INTENSITY, INTENSITY_8);
      break;
    case 9:
      sendData (INTENSITY, INTENSITY_9);
      break;
    case 10:
      sendData (INTENSITY, INTENSITY_10);
      break;
    case 11:
      sendData (INTENSITY, INTENSITY_11);
      break;
    case 12:
      sendData (INTENSITY, INTENSITY_12);
      break;
    case 13:
      sendData (INTENSITY, INTENSITY_13);
      break;
    case 14:
      sendData (INTENSITY, INTENSITY_14);
      break;
    case 15:
      sendData (INTENSITY, INTENSITY_15);
      break;
    case 16:
      sendData (INTENSITY, INTENSITY_16);
      break;
  }
}