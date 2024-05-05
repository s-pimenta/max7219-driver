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
#define DECODE_MODE_DIGITS_NONE 0b00000000
#define DECODE_MODE_ALL_DIGITS 0b11111111
 
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

//------------------------------------------------

void setup() {

  max7219_Setup(SCAN_LIMIT_6, INTENSITY_1); //initializes the max7219 driver

  delay(2500);
}

//------------------------------------------------

void loop() {
  clock_loop();
}

//------------------------------------------------

// max7219 initialization setup
void max7219_Setup(byte scanLimit, byte intensity) {
  //pinMode(CS_PIN, OUTPUT);  // Sets the CS Pin to output
  DDRB = 0x04; // Sets the CS Pin to output

  SPI.begin();
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0)); //Setups the SPI settings

  max7219_SendData(DISPLAY_TEST, DISPLAY_TEST_OFF); //clears the Display Test
  max7219_shutdown(SHUTDOWN_MODE_ON); //ensures the display is off
  
  max7219_setScanLimit(scanLimit); //sets the display size (digits)
  max7219_setDisplayIntensity(intensity); //sets the display brightness
  
  max7219_DecodeMode(DECODE_MODE_DIGITS_NONE); //sets by default any digit on no-decode-mode
  max7219_Clear(); //Clears the screen NOTE: The clear needs to be after the Decode Mode change FIX THIS
  max7219_shutdown(SHUTDOWN_MODE_OFF);

}

void max7219_DecodeMode(uint8_t digits) {
  max7219_SendData(DECODE_MODE, digits);
}

/* Sends character to a determinated position (0-7) in Decode-Mode
   (Numbers are in BCD encoding)

   Available characters
   CHAR_0 (digit zero)
   CHAR_1 (digit one)
   CHAR_2 (digit two)
   CHAR_3 (digit three)
   CHAR_4 (digit four)
   CHAR_5 (digit five)
   CHAR_6 (digit six)
   CHAR_7 (digit seven)
   CHAR_8 (digit eight)
   CHAR_9 (digit nine)
   CHAR_DASH (char "-")
   CHAR_E (letter "E")
   CHAR_H (letter "H")
   CHAR_L (letter "L")
   CHAR_P (letter "P")
   CHAR_BLANK (blank space " ") */
  
  // TODO: when this command is sent automaticaly set that digit position on Decode-Mode
void max7219_DecodeMode_SendChar(uint8_t digit_position, uint8_t character) {
  if (digit_position <= 7) { //checks if the sent digit_position is within bounds
    if (character <= 15) { //checks if the sent character is within bounds
      max7219_SendData(digit_position, character); //sends the charcter to the desired position
    }
  }
}

/* Sends character to a determinated position (0-7) in No-Decode-Mode
   No-decode mode
*  LED Segments:        A
*                     ----
*                   F|    |B
*                    |  G |
*                     ----
*                   E|    |C
*                    |  D |
*                     ----  o DP
*   Register bits:
*      bit:  7  6  5  4  3  2  1  0
*           DP  A  B  C  D  E  F  G */
void max7219_NoDecodeMode_SendChar(uint8_t digit_position, uint8_t character) {

}

//clears the digits regiters
void max7219_Clear() {
  max7219_SendData(DECODE_MODE, 0xFF); //all digits

  for (uint8_t digit = 1; digit <= 8; digit++) {
    max7219_SendData(digit, CHAR_BLANK);
  }
  // max7219_SendData(DIGIT_0, CHAR_BLANK);
  // max7219_SendData(DIGIT_1, CHAR_BLANK);
  // max7219_SendData(DIGIT_2, CHAR_BLANK);
  // max7219_SendData(DIGIT_3, CHAR_BLANK);
  // max7219_SendData(DIGIT_4, CHAR_BLANK);
  // max7219_SendData(DIGIT_5, CHAR_BLANK);
  // max7219_SendData(DIGIT_6, CHAR_BLANK);
  // max7219_SendData(DIGIT_7, CHAR_BLANK);
}

void max7219_SendData (uint8_t address, uint8_t data) {
  //digitalWrite(CS_PIN, LOW);  // Sets the CS Pin to LOW
  PORTB &= ~(1 << 2); // Sets the PB2 LOW (CS Pin)
  SPI.transfer(address);
  SPI.transfer(data);
  PORTB |= (1 << 2); // Sets the PB2 HIGH (CS Pin)
  //digitalWrite(CS_PIN, HIGH);  // Sets the CS Pin to HIGH
}

void max7219_shutdown (bool turnOff) {
  if (turnOff == false) {
    max7219_SendData(SHUTDOWN_MODE, SHUTDOWN_MODE_ON);
  }
  else { //true
    max7219_SendData(SHUTDOWN_MODE, SHUTDOWN_MODE_OFF);
  }
}

void max7219_displayTest (bool turnOn) {
  if (turnOn == true) {
    max7219_SendData(DISPLAY_TEST, DISPLAY_TEST_ON);
  }
  else { //false
    max7219_SendData(DISPLAY_TEST, DISPLAY_TEST_OFF);
  }
}

// Digits displayed = 1-8
void max7219_setScanLimit (uint8_t numDigits) {
  if (numDigits >= 1 && numDigits <= 8){
    max7219_SendData(SCAN_LIMIT, numDigits);
  }

  // switch (numDigits) {
  //   case 1:
  //     max7219_SendData(SCAN_LIMIT, SCAN_LIMIT_1);
  //     break;
  //   case 2:
  //     max7219_SendData(SCAN_LIMIT, SCAN_LIMIT_2);
  //     break;
  //   case 3:
  //     max7219_SendData(SCAN_LIMIT, SCAN_LIMIT_3);
  //     break;
  //   case 4:
  //     max7219_SendData(SCAN_LIMIT, SCAN_LIMIT_4);
  //     break;
  //   case 5:
  //     max7219_SendData(SCAN_LIMIT, SCAN_LIMIT_5);
  //     break;
  //   case 6:
  //     max7219_SendData(SCAN_LIMIT, SCAN_LIMIT_6);
  //     break;
  //   case 7:
  //     max7219_SendData(SCAN_LIMIT, SCAN_LIMIT_7);
  //     break;
  //   case 8:
  //     max7219_SendData(SCAN_LIMIT, SCAN_LIMIT_8);
  //     break;
  // }
}

// Intensity levels = (min)0 - 15(max)
void max7219_setDisplayIntensity (uint8_t intensity) {
  
  if (intensity >= 0 && intensity <= 15) {
    max7219_SendData(INTENSITY, intensity);
  }
}