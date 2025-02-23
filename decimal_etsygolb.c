#include "decimal.h"

// Отрицательные дес
// Float
// Больше или меньше 96 бит
// Больше или меньне 79 октиллинов...

// Как понять что у нас ноль в конце float?

int get_bit(s21_decimal value, int index);
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_print_two(s21_decimal value);
int power(s21_decimal value);
int sign(s21_decimal value);
int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int multiten(s21_BIG_decimal *value);
int get_BIG_bit(s21_BIG_decimal value, int index);
int normalization(s21_decimal *value_1, s21_decimal *value_2);
int changing_power(s21_decimal* value, int new_index);
int signB(s21_BIG_decimal value);
int divten(s21_decimal *value);
int anti_normalization(s21_decimal *value_1, s21_decimal *value_2);
void changing_sign(s21_decimal* value);

int main(void) {
  
  s21_decimal first;
  // first.bits[0] = 1414214;
  first.bits[0] = 346346; 
  first.bits[1] = 4;
  first.bits[2] = 0;
  first.bits[3] = 0b00000000000000010000000000000000;

  s21_decimal second;
  // second.bits[0] = 16346342;
  second.bits[0] = 3463;
  second.bits[1] = 2;
  second.bits[2] = 0;
  second.bits[3] = 0b00000000000000010000000000000000;
  s21_decimal result;
  result.bits[0] = 0; 
  result.bits[1] = 0;
  result.bits[2] = 0;
  result.bits[3] = 0;

  printf("FIRST\n");
  s21_print_two(first);
  printf("SECOND\n");
  s21_print_two(second);

  printf("RESULT = %d\n", s21_add(second, first, &result));
  s21_print_two(result);
  printf("%d\n", result.bits[0]);
  // printf("FIRST\n");
  // s21_print_two(first);
  // printf("SECOND\n");
  // s21_print_two(second);
  // printf("FIRST\n");
  // s21_print_two(first);
  // printf("SECOND\n");
  // s21_print_two(second);
  // printf("%u\n", (unsigned)second.bits[0]);
  result.bits[0] = 0; 
  result.bits[1] = 0;
  result.bits[2] = 0;
  result.bits[3] = 0;
  printf("RESULT = %d\n", s21_sub(second, first, &result));
  s21_print_two(result);
  printf("%d\n", result.bits[0]);
  // printf("POWER: %d\n", power(first));
  // printf("POWER: %d\n", power(second));
  // printf("%d\n", s21_is_less(first, second));
  // printf("%d\n", s21_is_greater_or_equal(first, second));
  // printf("%d\n", normalization(&first, &second));
  // s21_print_two(first);
  // s21_print_two(second);
  // printf("%d\n", second.bits[0]);
  // printf("%d\n", 0b111);
  // anti_normalization(&first, &second);
  // // s21_print_two(first);
  // // printf("%d\n", first.bits[0]);
  // changing_sign(&first);
  // s21_print_two(first);

  return 0;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int ret = 0;
  int extr = 0;
  s21_BIG_decimal temp = {{0, 0, 0, 0, 0}};
  if (sign(value_1) != sign(value_2)) {
    changing_sign(&value_2);
    ret = s21_sub(value_1, value_2, result);
  }
  else {
    if (normalization(&value_1, &value_2) != 0) {
      anti_normalization(&value_1, &value_2);
    }
    for (int i = 0; i < 97; i++){
      int res = get_bit(value_1, i) + get_bit(value_2, i) + extr;
      temp.bits[i/32] |= (res & 1) << i%32;
      extr = res >> 1; 
    }
    // чтобы узнать + или - в result нужна функция сравнения decimal без знака
    if (temp.bits[3] == 0){
      result->bits[3] = value_1.bits[3];
      for (int i = 0; i < 3; i++){
        result->bits[i] = temp.bits[i];
      }
    }
    else ret = 1;
    if (sign(value_1) == 1 && ret == 1) ret = 2;
  }
  return ret;
}


int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int ret = 0;
  int extr = 0;
  s21_BIG_decimal temp = {{0, 0, 0, 0, 0}};
  if (sign(value_1) != sign(value_2)) {
    changing_sign(&value_2);
    ret = s21_add(value_1, value_2, result);
  }
  else {
    if (normalization(&value_1, &value_2) != 0) {
      anti_normalization(&value_1, &value_2);
    }
    int less = s21_is_less(value_1, value_2);
    if (sign(value_1) == 1) less = !less;
    s21_decimal* minuend = less ? &value_2: &value_1;
    s21_decimal* subtrahend = less ? &value_1: &value_2;
    for (int i = 0; i < 97; i++){
      int res = get_bit(*minuend, i) - get_bit(*subtrahend, i) - extr;
      if (res < 0){
        res += 2;
        extr = 1;
      }
      else extr = 0;
      temp.bits[i/32] |= (res & 1) << i%32;
    }
    // чтобы узнать + или - в result нужна функция сравнения decimal без знака
    if (temp.bits[3] == 0){
      result->bits[3] = value_1.bits[3];
      changing_sign(result);
      for (int i = 0; i < 3; i++){
        result->bits[i] = temp.bits[i];
      }
    }
    else ret = 1;
    if (sign(value_1) == 1 && ret == 1) ret = 2;
  }
  return ret;
}

int power(s21_decimal value) {
  int res = 0;
  int power = 1;
  for (int i = 0; i < 8; i++){
    res += ((value.bits[3] >> (i+16)) & 1)*power;
    power *= 2;
  }
  return res;
}

int is_devided_ten(s21_decimal value) {
  return (value.bits[0] % 10) == 0;
}

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int ret;
  int min1 = sign(value_1);
  int min2 = sign(value_2);
  if (min1 == min2){
    int norm = normalization(&value_1, &value_2);
    if (norm == 0) {
      int i = 2;
      while (!(i < 0) && value_1.bits[i] == value_2.bits[i]){
        i--;
      }
      ret = (unsigned)value_1.bits[i] < (unsigned)value_2.bits[i];
      if (min1 == 1) ret = !ret;
    }
    else ret = norm == 2;
  }
  else ret = min1 == 1;
  return ret;
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2){
  return !s21_is_less(value_1, value_2);
}

int normalization(s21_decimal *value_1, s21_decimal *value_2){
  int ret = 0;
  int pow1 = power(*value_1);
  int pow2 = power(*value_2);
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


int anti_normalization(s21_decimal *value_1, s21_decimal *value_2){
  int ret = 0;
  int pow1 = power(*value_1);
  int pow2 = power(*value_2);
  int extrpow = pow1 < pow2 ? pow2 - pow1 : pow1 - pow2;
  for (int i = 0; i < extrpow; i++){
    divten(pow1 > pow2 ? value_1: value_2);
  }
  changing_power(pow1 > pow2 ? value_1: value_2, pow1 > pow2 ? pow2 : pow1);
  return ret;
}

int sign(s21_decimal value){
  return value.bits[3] < 0;
}
int signB(s21_BIG_decimal value){
  return value.bits[3] < 0;
}

int changing_power(s21_decimal* value, int new_index){
  int ret = 0;
  int minus = sign(*value);
  value->bits[3] = new_index << 16;
  value->bits[3] |= minus << 31;
  return ret;
}

void changing_sign(s21_decimal* value){
  value->bits[3] = !sign(*value) << 31 | power(*value) << 16;
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

int divten(s21_decimal *value) {
  int ret = 0;
  s21_BIG_decimal temp = {{0, 0, 0, 0, 0}};
  unsigned remainder = 0;

  for (int i = 95; i >= 0; i--) {
    remainder <<= 1;
    remainder |= get_bit(*value, i);
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

int get_bit(s21_decimal value, int index){
  return (value.bits[index / 32] >> index % 32) & 1;
}
int get_BIG_bit(s21_BIG_decimal value, int index){
  return (value.bits[index / 32] >> index % 32) & 1;
}