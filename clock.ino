//variables to store time
uint8_t hours_2 = 1;
uint8_t hours_1 = 9;

uint8_t minutes_2 = 1;
uint8_t minutes_1 = 0;

uint8_t seconds_2 = 0;
uint8_t seconds_1 = 0;

void clock_loop() {
  max7219_DecodeMode(DECODE_MODE_ALL_DIGITS);
  if (seconds_1 == 10) {
    seconds_1 = 0;
    
    seconds_2++;

    if (seconds_2 == 6) {
      seconds_2 = 0;
      seconds_1 = 0;

      minutes_1++;

      if (minutes_1 == 10){
        minutes_1 = 0;

        minutes_2++;

        if (minutes_2 == 6){
          minutes_2 = 0;
          minutes_1 = 0;
        
          hours_1++;

          if (hours_1 == 10) {
            hours_1 = 0;
            hours_2++;
          }
          else if (hours_2 == 2 && hours_1 == 4) {
            hours_2 = 0;
            hours_1 = 0; //midnight reached
          }
        }
      }
    }
  }
  else {
    print_time();
    seconds_1++;
    delay(990);
  }
}

void print_time() {
  if (hours_2 == 0) {
    max7219_DecodeMode_SendChar(DIGIT_0, CHAR_BLANK);
    max7219_DecodeMode_SendChar(DIGIT_1, hours_1);
  }
  else {
    max7219_DecodeMode_SendChar(DIGIT_0, hours_2);
    max7219_DecodeMode_SendChar(DIGIT_1, hours_1);
  }
  max7219_DecodeMode_SendChar(DIGIT_2, minutes_2);
  max7219_DecodeMode_SendChar(DIGIT_3, minutes_1);
  max7219_DecodeMode_SendChar(DIGIT_4, seconds_2);
  max7219_DecodeMode_SendChar(DIGIT_5, seconds_1);
}
