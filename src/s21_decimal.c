#include "s21_decimal.h"

#include <math.h>
#include <stdio.h>

int main() {
  s21_decimal decimal;
  decimal.bits[0] = 0xFFFF;
  decimal.bits[1] = 2;
  decimal.bits[2] = 0;
  decimal.bits[3] = 0b10000000000001010000000000000000;
  // printf("%d\n", getBit(decimal, 3));
  // printf("%d\n", getSign(decimal));
  printf("%d\n", power(decimal));
  printf("%d\n", s21_get_scale(&decimal));
  printf("%d\n", power_etsy(decimal));
  return 0;
}

int getBit(s21_decimal decimal, int pos) {
  return (decimal.bits[pos / 32] >> ((pos % 32) - 1)) & 1;
}

int getSign(s21_decimal decimal) { return getBit(decimal, 127); }

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  if (!dst) {  // check that dest address is valid to avoid segm fault
    return ERROR;
  }

  *dst = s21_decimal_null();  // set to NULL
  int sign;

  if (src < 0) {
    sign = NEGATIVE;
    if (src != -2147483648) {
      // except minimal int value to avoid overflow.
      // -2147483648 has no positive equivalent
      // because 2147483648 exceeds int's max (2147483647)
      src = -src;
    }
  } else {
    sign = POSITIVE;
  }

  dst->bits[0] = src;
  dst->bits[3] |= sign << 31;

  return OK;
}

// int s21_from_decimal_to_int(s21_decimal src, int *dst) {
//     //If there is a fractional part in a decimal number,
//     //it should be discarded (for example, 0.9 is converted to 0).
//     //НУЖНО СДЕЛАТЬ TRUNCATE

//     if (!dst) { // check that dest address is valid to avoid segm fault
//         return ERROR;
//     }

//     *dst = 0; // set to NULL
//     for (int i = 0; i < 96; i++) {
//         *dst += pow(2, i);
//     }
// }

s21_decimal s21_decimal_null(void) {  // set the whole decimal to null
  s21_decimal res = {0};              // Initialize all bits to null
  return res;
}

// int s21_truncate(s21_decimal value, s21_decimal *result) {
//     *result = s21_decimal_null(); // set to NULL

// }


// почему первая из этих трех функций видит только первую единицу?
int power(s21_decimal decimal) {
  int scale = 0;
  int num = 0;
  for (int i = 17; i < 25; i++) {
    num += pow(((decimal.bits[3] >> i) & 1), 2);
  }
  return scale;
}

int s21_get_scale(s21_decimal *value) {
  int mask = 0b11111111;
  return (mask << 16 & value->bits[3]) >> 16;
}

int power_etsy(s21_decimal value) {
  int res = 0;
  int power = 1;
  for (int i = 0; i < 8; i++){
    res += ((value.bits[3] >> (i+16)) & 1)*power;
    power *= 2;
  }
  return res;
}
