#ifndef SEVSEG_H
#define SEVSEG_H

#include <Arduino.h>

class SevSegLib
{
public:
  SevSegLib(uint8_t nb_of_elements, uint8_t select_pins_count, uint8_t * select_pins_array,
            uint8_t * seg_pins_array, uint8_t select_active_state, uint8_t seg_active_state)
  {
    this->number_of_elements = nb_of_elements;
    this->active_element = 0;
    this->select_pins_count = select_pins_count;
    this->select_pins_array = select_pins_array;
    this->seg_pins_array = seg_pins_array;
    this->select_active_state = select_active_state;
    this->seg_active_state = seg_active_state;

    for (int idx = 0; idx < select_pins_count; idx++)
    {
      pinMode(select_pins_array[idx], OUTPUT);
    }

    for (int idx = 0; idx < SEGMENT_COUNT; idx++)
    {
      pinMode(seg_pins_array[idx], OUTPUT);
    }

    for (int idx = 0; idx < select_pins_count; idx++)
    {
      digitalWrite(select_pins_array[idx], select_active_state);
    }
  }

  // Number of the segments (a, b, c, d, e, f, g, p)
  constexpr static uint8_t SEGMENT_COUNT = 8;
  // Number of the characters in map_num_to_seg_pins
  constexpr static uint8_t SEG_CHARS_COUNT = 11;

  void selectElement(uint8_t element);
  void selectFirstElement();
  void selectPrevElement();
  void selectNextElement();
  void selectLastElement();
  void displayDigit(uint8_t digit);
  void displayNumber(uint32_t number, long visible_time_ms);
  void setRefreshRateMs(uint32_t refresh_rate_ms);

protected:
  virtual uint8_t getBitValue(uint8_t var, uint8_t bit) = 0;

private:
  constexpr static uint32_t REFRESH_RATE_DEFAULT = 1;
  constexpr static uint8_t SEG_ON = HIGH;
  constexpr static uint8_t SEG_OFF = LOW;

  uint8_t number_of_elements;
  uint8_t active_element;
  uint8_t select_pins_count;
  uint8_t *select_pins_array;
  uint8_t *seg_pins_array;
  uint8_t select_active_state;
  uint8_t seg_active_state;
  uint32_t refresh_rate_ms = REFRESH_RATE_DEFAULT;
  static uint8_t map_num_to_seg_pins[SEG_CHARS_COUNT][SEGMENT_COUNT];
};

class SevSegDecoderStrategy : public SevSegLib
{
public:
  SevSegDecoderStrategy(uint8_t nb_of_elements, uint8_t select_pins_count, uint8_t * select_pins_array,
                        uint8_t * seg_pins_array, uint8_t select_active_state, uint8_t seg_active_state) : 
      SevSegLib(nb_of_elements, select_pins_count, select_pins_array, seg_pins_array, select_active_state, seg_active_state)
  {    
  }

protected:
  uint8_t getBitValue(uint8_t var, uint8_t bit) override;
};

class SevSegCommonPinStrategy : public SevSegLib
{
public:
  SevSegCommonPinStrategy(uint8_t nb_of_elements, uint8_t select_pins_count, uint8_t * select_pins_array,
                          uint8_t * seg_pins_array, uint8_t select_active_state, uint8_t seg_active_state) : 
      SevSegLib(nb_of_elements, select_pins_count, select_pins_array, seg_pins_array, select_active_state, seg_active_state)
  {    
  }

protected:
  uint8_t getBitValue(uint8_t var, uint8_t bit) override;
};

#endif