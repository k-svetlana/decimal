#ifndef SRC_TESTS_TEST_H_
#define SRC_TESTS_TEST_H_

#include <check.h>

#include "./../s21_decimal.h"


#define TEST_CONVERSION_OK 0
#define TEST_CONVERSION_ERROR 1

Suite *from_int_to_decimal_suite(void);
void test_from_int_to_decimal(int number, s21_decimal decimal_check);

#endif  // SRC_TESTS_TEST_H_