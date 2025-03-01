#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_decimal.h"

START_TEST(s21_from_int_to_decimal_test) {
  int value1 = 123;
  s21_decimal result1 = {{0, 0, 0, 0}};
  s21_from_int_to_decimal(value1, &result1);
  ck_assert_int_eq(123, result1.bits[0]);
  ck_assert_int_eq(0, result1.bits[3]);

  int value2 = -56723;
  s21_decimal result2 = {{0, 0, 0, 0}};
  s21_from_int_to_decimal(value2, &result2);
  ck_assert_int_eq(56723, result2.bits[0]);
  ck_assert_int_eq((1 << 31), result2.bits[3]);

  int value3 = 0;
  s21_decimal result3 = {{0, 0, 0, 0}};
  s21_from_int_to_decimal(value3, &result3);
  ck_assert_int_eq(0, result3.bits[0]);
  ck_assert_int_eq(0, result3.bits[3]);
}
END_TEST

Suite *root_suite(void) {
  Suite *s = suite_create("decimal");

  TCase *tc_int_to_dec = tcase_create("int_to_dec");

  tcase_add_test(tc_int_to_dec, s21_from_int_to_decimal_test);
  
  suite_add_tcase(s, tc_int_to_dec);

  return s;
}

int main(void) {
  int failed = 0;
  Suite *s = root_suite();
  SRunner *runner = srunner_create(s);

  srunner_run_all(runner, CK_NORMAL);
  failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (failed == 0) ? 0 : -1;
}
