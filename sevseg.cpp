/*
  SevSegLib

  7-segment display library

  Supports two strategies:
  1. Decoders/Demultiplextors (e.g. SNx4HCT138: 3 input (select) pins; 8 output pins)
  2. Common anode/cathode pin (one pin per 7-segment element)

  2024
  Martin Karamihov
*/

#include "sevseg.h"

/* xy: x = array index (0-7); y = segment (a, b, c, d, e, f, g, p)
*  0a_
*5f | | 1b
* 6g -
*4e |_| 2c
*   3d . 7p
*/
uint8_t SevSegLib::map_num_to_seg_pins[SEG_CHARS_COUNT][SEGMENT_COUNT] = {
  { SEG_ON,  SEG_ON,  SEG_ON,  SEG_ON,  SEG_ON,  SEG_ON,  SEG_OFF, SEG_ON },  /*0*/
  { SEG_OFF, SEG_ON,  SEG_ON,  SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF, SEG_ON },  /*1*/
  { SEG_ON,  SEG_ON,  SEG_OFF, SEG_ON,  SEG_ON,  SEG_OFF, SEG_ON,  SEG_ON },  /*2*/
  { SEG_ON,  SEG_ON,  SEG_ON,  SEG_ON,  SEG_OFF, SEG_OFF, SEG_ON,  SEG_ON },  /*3*/
  { SEG_OFF, SEG_ON,  SEG_ON,  SEG_OFF, SEG_OFF, SEG_ON,  SEG_ON,  SEG_ON },  /*4*/
  { SEG_ON,  SEG_OFF, SEG_ON,  SEG_ON,  SEG_OFF, SEG_ON,  SEG_ON,  SEG_ON },  /*5*/
  { SEG_ON,  SEG_OFF, SEG_ON,  SEG_ON,  SEG_ON,  SEG_ON,  SEG_ON,  SEG_ON },  /*6*/
  { SEG_ON,  SEG_ON,  SEG_ON,  SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF, SEG_ON },  /*7*/
  { SEG_ON,  SEG_ON,  SEG_ON,  SEG_ON,  SEG_ON,  SEG_ON,  SEG_ON,  SEG_ON },  /*8*/
  { SEG_ON,  SEG_ON,  SEG_ON,  SEG_ON,  SEG_OFF, SEG_ON,  SEG_ON,  SEG_ON },  /*9*/
  { SEG_ON,  SEG_OFF, SEG_OFF, SEG_ON,  SEG_ON,  SEG_ON,  SEG_ON,  SEG_ON }   /*E*/
};

uint8_t SevSegDecoderStrategy::getBitValue(uint8_t var, uint8_t bit)
{
  return (((var) & (1 << (bit))) >> (bit));
}

uint8_t SevSegCommonPinStrategy::getBitValue(uint8_t var, uint8_t bit)
{
  return (var == bit);
}

void SevSegLib::displayDigit(uint8_t digit)
{
  for (int idx = 0; idx < SEGMENT_COUNT; idx++) {
    digitalWrite(seg_pins_array[idx], map_num_to_seg_pins[digit][idx] ? seg_active_state : !seg_active_state);
  }
}

void SevSegLib::selectElement(uint8_t element)
{
  if (element < number_of_elements)
  {
    active_element = element;
  }
  else
  {
    active_element = 0;
  }

  for (int idx = 0; idx < select_pins_count; idx++)
  {
    digitalWrite(select_pins_array[idx], getBitValue(active_element, idx) ? !select_active_state : select_active_state);
  }
}

void SevSegLib::selectFirstElement()
{
  selectElement(0);
}

void SevSegLib::selectPrevElement()
{
  if ((active_element > 0) && (active_element <= number_of_elements))
  {
    active_element--;
  }
  else
  {
    active_element = number_of_elements - 1;
  }

  selectElement(active_element);
}

void SevSegLib::selectNextElement()
{
  active_element++;
  if (active_element >= number_of_elements)
  {
    active_element = 0;
  }

  selectElement(active_element);
}

void SevSegLib::selectLastElement()
{
  selectElement(number_of_elements - 1);
}

void SevSegLib::displayNumber(uint32_t number, int32_t visible_time_ms)
{
  uint8_t active_digit;
  uint32_t max_visible_number = pow(10, number_of_elements) - 1;

  selectLastElement();

  if (number <= max_visible_number)
  {
    while (visible_time_ms > 0)
    {
      uint32_t tmp_number = number;
      selectLastElement();

      do
      {
        active_digit = tmp_number % 10;
        tmp_number = tmp_number / 10;

        displayDigit(active_digit);
        delay(refresh_rate_ms);
        visible_time_ms -= refresh_rate_ms;
        selectPrevElement();
      } while (tmp_number > 0);
    }
  }
  else
  {
    selectFirstElement();
    displayDigit(SEG_CHARS_COUNT - 1);
    delay(visible_time_ms);
  }
}

void SevSegLib::setRefreshRateMs(uint32_t refresh_rate_ms)
{
  this->refresh_rate_ms = refresh_rate_ms;
}
