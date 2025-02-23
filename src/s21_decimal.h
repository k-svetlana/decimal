#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

typedef struct {
  int bits[4];
} s21_decimal;

typedef enum s21_decimal_sign { POSITIVE = 0, NEGATIVE = 1 } s21_decimal_sign;

typedef enum s21_success { OK = 0, ERROR = 1 } s21_success;

int getBit(s21_decimal decimal, int pos);
int getSign(s21_decimal decimal);
s21_decimal s21_decimal_null(void);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
// int s21_from_decimal_to_int(s21_decimal src, int *dst);
int power(s21_decimal value);
int s21_get_scale(s21_decimal *value);
int power_etsy(s21_decimal value);

#endif  // S21_DECIMAL_H