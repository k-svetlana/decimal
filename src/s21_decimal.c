#include "s21_decimal.h"

#include <math.h>
#include <stdio.h>
#include <limits.h>

int main() {
  s21_decimal decimal;
  decimal.bits[0] = 0b01110001111110110000100001000011;
  decimal.bits[1] = 0b00000000000000000000000100011111;
  decimal.bits[2] = 0;
  decimal.bits[3] = 0b10000000000000100000000000000000;
  s21_decimal b;
  b.bits[0] = 0b01110001111110110000100001000011;
  b.bits[1] = 0b00000000000000000000000100011111;
  b.bits[2] = 0;
  b.bits[3] = 0b10000000000000100000000000000000;
  // printf("%d\n", getSign(decimal));
  // printf("%d\n", getPower(decimal));
  printf("%d\n", s21_is_equal(&decimal, &b));
  printf("%d\n", s21_is_not_equal(&decimal, &b));

  // s21_decimal decimal = {{35}};
  // s21_decimal b = {{-35}};

  //ПРОВЕРКА decimal_to_float
  // float dst;
  // s21_from_decimal_to_float(decimal, &dst);
  // printf("%f\n", dst);

  // s21_print_two(decimal);
  // printf("\n");
  // // div_ten_signed(&decimal);
  // // s21_print_two(decimal);
  // s21_decimal result;
  // s21_truncate(decimal, &result);
  // s21_print_two(result);

  return 0;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int truncate = OK;
  if (!result) {
    truncate = ERROR;
  }
  *result = value;
  int scale = getPower(value);

  for (int i = 0; i < scale; i++) {
    div_ten_signed(result);
  }

  result->bits[3] &= 0x80000000;
  return truncate;
}

// use of hexadecimal mask 0xFF800000
// Expanding 0xFF800000 to binary:
// 1111 1111 1000 0000 0000 0000 0000 0000
// Bits 31-24 (1111 1111 1) are kept as they are.
// Bits 23-0 are cleared to zero.
// E.g. both of these set exponent to zero:
// result->bits[3] &= 0x80000000;
// result->bits[3] &= ~(0x00FF0000);

// взято у etsy
int s21_print_two(s21_decimal value) {
  int ret = 0;
  for (int k = 3; k >= 0; k--){
    for (int i = 31; i >= 0; --i){
      printf("%d", (value.bits[k] >> i) & 1);
    }
    printf("\n");
  }
  return ret;
}


int s21_is_equal(s21_decimal *leftOp, s21_decimal *rightOp) {
  int isEqual = TRUE;
  
  if (!leftOp || !rightOp) {
    isEqual = FALSE;
  } else {
    int leftIsZero = (leftOp->bits[0] == 0 && leftOp->bits[1] == 0 && leftOp->bits[2] == 0);
    int rightIsZero = (rightOp->bits[0] == 0 && rightOp->bits[1] == 0 && rightOp->bits[2] == 0);

    if (leftIsZero && rightIsZero) {
      isEqual = TRUE;
    } else {
      s21_normalize(leftOp, rightOp);
      for (int bitsIndex = 0; bitsIndex < 4; bitsIndex++) {
        if (leftOp->bits[bitsIndex] != rightOp->bits[bitsIndex]) {
          isEqual = FALSE;
        }
      }
    }
  }
  return isEqual;
}

int s21_is_not_equal(s21_decimal *leftOp, s21_decimal *rightOp) {
  return !s21_is_equal(leftOp, rightOp);
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int success = OK;

  if (!dst) {
    success = ERROR;
  } else {
    *dst = 0;
    double tmp = 0.0;

    for (int i = 0; i < 96; i++) {
      if (getBit(src, i) != 0) {
        tmp += pow(2.0, i);
        // printf("%lf\n", tmp);
      }
    }
    int scale = getPower(src);
    tmp /= pow(10.0, scale);
    *dst = (float)tmp;

    if (getSign(src) == NEGATIVE) {
      *dst *= -1;
    }
  }
  return success;
}

int getBit(s21_decimal decimal, int index) {
  return (decimal.bits[index / 32] >> index % 32) & 1;
}

int getSign(s21_decimal decimal) {
  return getBit(decimal, 127);
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int success = OK;
  
  if (!dst) {  // check that dest address is valid to avoid segm fault
    success = ERROR;
  } else {
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
  }
  return success;
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

//взято у etsy
int changing_power(s21_decimal* value, int new_index){
  int ret = 0;
  int minus = getSign(*value);
  value->bits[3] = new_index << 16;
  value->bits[3] |= minus << 31;
  return ret;
}

int getPower(s21_decimal decimal) {
    int res = 0;
    for (int i = 0; i < 8; i++) {
        if ((decimal.bits[3] >> (i+16)) & 1) {
            res += pow(2, i);
        }
    }
    return res;
}

// void s21_normalize(s21_decimal *left, s21_decimal *right) {
//   while (getPower(*left) > getPower(*right)) {
//       divten(right);
//   }
//   while (getPower(*right) > getPower(*left)) {
//       divten(left);
//   }
// }

int s21_normalize(s21_decimal *value_1, s21_decimal *value_2){
  int ret = 0;
  int pow1 = getPower(*value_1);
  int pow2 = getPower(*value_2);
  s21_BIG_decimal temp = {{0, 0, 0, 0, 0}};
  if (pow1 != pow2) {
    int extrpow = pow1 < pow2 ? pow2 - pow1 : pow1 - pow2;
    for (int i = 0; i < 3; i++){
      temp.bits[i] = pow1 < pow2 ? value_1->bits[i]: value_2->bits[i];
    }
    for (int i = 0; i < extrpow; i++){
      if (multiten(&temp) == 1) {
        ret = pow1 < pow2 ? 1: 2;
        break;
      }
    }
    if (ret == 0) {
      for (int i = 0; i < 3; i++){
        if(pow1 < pow2) value_1->bits[i] = temp.bits[i];
        else value_2->bits[i] = temp.bits[i];
      }
      changing_power(pow1 < pow2 ? value_1: value_2, pow1 < pow2 ? pow2 : pow1);
    }
  }
  return ret;
}

int multiten(s21_BIG_decimal *value){
  int ret = 0;
  s21_BIG_decimal temp = {{0, 0, 0, 0, 0}};
  int carry = 0;
  for (int i = 0; i < 97; i++) {
    int bit = get_BIG_bit(*value, i);
    int res = (bit * 10) + carry;
    temp.bits[i / 32] |= (res & 1) << (i % 32);
    carry = res >> 1;
  }
  if (temp.bits[3] == 0){
    for (int i = 0; i < 4; i++){
      value->bits[i] = temp.bits[i];
    }
  }
  else ret = 1;
  return ret;
}

int get_BIG_bit(s21_BIG_decimal value, int index){
  return (value.bits[index / 32] >> index % 32) & 1;
}

int div_ten_signed(s21_decimal *value) {
  s21_BIG_decimal temp = {{0, 0, 0, 0, 0}};
  unsigned carry = 0;

  for (int i = 95; i >= 0; i--) {
    carry <<= 1;
    carry |= getBit(*value, i);
    if (carry >= 10) {
      carry -= 10;
      temp.bits[i/32] |= (1 << (i % 32));
    }
  }
  for (int i = 0; i < 3; i++) {
    value->bits[i] = temp.bits[i];
  }
  return 0;
}

int s21_floor(s21_decimal value, s21_decimal* result) {
  *result = s21_decimal_zero();
  s21_truncate(value, result);
  if ((sign(value) == NEGATIVE && value.bits[3] & 0x00FF0000) != 0) {
      s21_sub(*result, ((s21_decimal){{1, 0, 0, 0}}), result);
  }
  return 0;
}

int s21_round(s21_decimal value, s21_decimal* result) {
  if ((value.bits[3] & 0x00FF0000) != 0) {
    if (sign(value))
      s21_sub(value, ((s21_decimal){{5, 0, 0, 65536}}), &value);
    else
      s21_add(value, ((s21_decimal){{5, 0, 0, 65536}}), &value);
    s21_truncate(value, result);
  } else {
    *result = value;
  }
  return 0;
}
