#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

typedef struct 
{
    int bits[4];
} s21_decimal;

typedef struct 
{
    int bits[5];
} s21_BIG_decimal;

typedef enum s21_decimal_sign {
    POSITIVE = 0,
    NEGATIVE = 1
} s21_decimal_sign;

typedef enum s21_success {
    OK = 0,
    ERROR = 1
} s21_success;

typedef enum s21_bool {
    FALSE = 0,
    TRUE = 1
} s21_bool;

int getBit(s21_decimal decimal, int pos);
int getSign(s21_decimal decimal);
s21_decimal s21_decimal_null(void);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
// int s21_from_decimal_to_int(s21_decimal src, int *dst);
int getPower(s21_decimal value);
s21_decimal s21_decimal_zero(void);
int s21_is_equal(s21_decimal *leftOp, s21_decimal *rightOp);
int divten(s21_decimal *value);
int s21_normalize(s21_decimal *value_1, s21_decimal *value_2);
int s21_truncate(s21_decimal value, s21_decimal *result);
int changing_power(s21_decimal* value, int new_index);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int get_BIG_bit(s21_BIG_decimal value, int index);
int multiten(s21_BIG_decimal *value);
int s21_print_two(s21_decimal value);
int s21_round(s21_decimal value, s21_decimal* result);
int s21_floor(s21_decimal value, s21_decimal* result);


#endif  // S21_DECIMAL_H
