#include "s21_decimal.h"

#include <math.h>
#include <stdio.h>
#include <limits.h>

int main() {
  s21_decimal a = {{35}};
  s21_decimal b = {{35}};
  s21_decimal decimal;
  decimal.bits[0] = 1103;
  decimal.bits[1] = 0;
  decimal.bits[2] = 0;
  decimal.bits[3] = 0b10000000000000010000000000000000;
  // printf("%d\n", getBit(decimal, 3));
  // printf("%d\n", getSign(decimal));
  printf("%d\n", power(decimal));
  printf("%d\n", s21_is_equal(&a, &b));
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

  *dst = s21_decimal_zero();  // set to NULL
  int sign = POSITIVE;

  if (src < 0) {
    sign = NEGATIVE;
    if (src != INT_MIN) {
      // except minimal int value to avoid overflow.
      // -2147483648 has no positive equivalent
      // because 2147483648 exceeds int's max (2147483647)
      src = -src;
    }
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

s21_decimal s21_decimal_zero(void) {
  s21_decimal res = {0};// Initialize all bits to null
  return res;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int truncate = OK;
  if (!result) {
    truncate = ERROR;
  }
  *result = value;
  int scale = power(value);

  for (int i = 0; i < scale; i++) {
    divten(result);
  }
  changing_power(result, 0);
  return truncate;
}

//взято у etsy
int changing_power(s21_decimal* value, int new_index){
  int ret = 0;
  int minus = getSign(*value);
  value->bits[3] = new_index << 16;
  value->bits[3] |= minus << 31;
  return ret;
}

// s21_decimal zeroPower(s21_decimal decimal) {
//   int res;
//   for (int i = 0; i < 8; i++) {
//     res = (decimal.bits[3] >> (i+16)) & 0;
//   }
// }

int power(s21_decimal decimal) {
    int res = 0;
    for (int i = 0; i < 8; i++) {
        if ((decimal.bits[3] >> (i+16)) & 1) {
            res += pow(2, i);
        }
    }
    return res;
}

int s21_is_equal(s21_decimal *leftOp, s21_decimal *rightOp) {
    int isEqual = TRUE;

    if (!leftOp || !rightOp) {
        isEqual = FALSE;
    }

    if (isEqual && getSign(*leftOp) != getSign(*rightOp)) {
        isEqual = FALSE;
    }

    if (isEqual) {
        s21_normalize(leftOp, rightOp);
    }

    if (isEqual && power(*leftOp) != power(*rightOp)) {
        isEqual = FALSE;
    }

    for (int i = 0; i < 3 && isEqual; i++) {  
        if (leftOp->bits[i] != rightOp->bits[i]) {
            isEqual = FALSE;
        }
    }

    return isEqual;
}

void s21_normalize(s21_decimal *left, s21_decimal *right) {
  while (power(*left) > power(*right)) {
      divten(right);
  }
  while (power(*right) > power(*left)) {
      divten(left);
  }
}

//взято у etsy
int divten(s21_decimal *value) {
  int ret = 0;
  s21_BIG_decimal temp = {{0, 0, 0, 0, 0}};
  unsigned remainder = 0;

  for (int i = 95; i >= 0; i--) {
    remainder <<= 1;
    remainder |= getBit(*value, i);
    if (remainder >= 10) {
      remainder -= 10;
      temp.bits[i/32] |= (1 << i%32);
    }
  }
  if (temp.bits[3] == 0){
    for (int i = 0; i < 4; i++){
      value->bits[i] = temp.bits[i];
    }
  }
  return ret;
}
