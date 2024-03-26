#include "sevseg.h"

#define NUMBER_OF_ELEMENTS 6

#define SELECT_ACTIVE_STATE (LOW)
#define SEGMENT_ACTIVE_STATE (LOW)

#define SELECT_PINS_COUNT 3

static uint8_t select_pins_array[SELECT_PINS_COUNT] = {
  2, // SELECT_PIN_A
  3, // SELECT_PIN_B
  4  // SELECT_PIN_C
};

static uint8_t seg_pins_array[SevSegLib::SEGMENT_COUNT] = {
  5,  // SEG_A
  6,  // SEG_B
  7,  // SEG_C
  8,  // SEG_D
  9,  // SEG_E
  10, // SEG_F
  11, // SEG_G
  12  // SEG_P
};

SevSegDecoderStrategy sevseg(NUMBER_OF_ELEMENTS, SELECT_PINS_COUNT, select_pins_array, seg_pins_array, SELECT_ACTIVE_STATE, SEGMENT_ACTIVE_STATE);

// the setup function runs once when you press reset or power the board
void setup() {
}

// the loop function runs over and over again forever
void loop() {
  // From the first element -> forward
  sevseg.selectFirstElement();

  for (int digit = 0; digit < SevSegLib::SEG_CHARS_COUNT; digit++)
  {
    sevseg.displayDigit(digit);
    delay(1000);
    sevseg.selectNextElement();
  }

  // From the first element -> backward
  sevseg.selectFirstElement();

  for (int digit = 0; digit < SevSegLib::SEG_CHARS_COUNT; digit++)
  {
    sevseg.displayDigit(digit);
    delay(1000);
    sevseg.selectPrevElement();
  }

  // From the last element -> backward
  sevseg.selectLastElement();

  for (int digit = 0; digit < SevSegLib::SEG_CHARS_COUNT; digit++)
  {
    sevseg.displayDigit(digit);
    delay(1000);
    sevseg.selectPrevElement();
  }

  // From the last element -> forward
  sevseg.selectLastElement();

  for (int digit = 0; digit < SevSegLib::SEG_CHARS_COUNT; digit++)
  {
    sevseg.displayDigit(digit);
    delay(1000);
    sevseg.selectNextElement();
  }

  sevseg.displayNumber(123456, 2000);

  sevseg.setRefreshRateMs(50);
  sevseg.displayNumber(987654, 2000);

  sevseg.setRefreshRateMs(100);
  sevseg.displayNumber(345678, 2000);

  sevseg.setRefreshRateMs(1);
}

