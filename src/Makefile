CFLAGS=-Wall -Werror -Wextra -std=c11
GCOV_FLAGS=-fprofile-arcs -ftest-coverage

SOURCE = $(wildcard s21_*.c)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CHECK_LIB = -lcheck -lsubunit -lm -lrt -lpthread
endif
ifeq ($(UNAME_S),Darwin)
	CHECK_LIB = -lcheck -lpthread
endif

all: s21_decimal

s21_decimal:
	gcc $(CFLAGS) -o s21_decimal.out s21_decimal.c && ./s21_decimal.out


clang:
	clang-format -i *.c *.h

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem --checkers-report=cppcheck.report *.c

valgrind: clean test
	valgrind --tool=memcheck --leak-check=yes ./s21_test.out

clean:
	rm -f *.gcda *.gcno *.out *.o *.a s21_test *.info *.report
	rm -rf report

test:
	gcc $(CFLAGS) ${SOURCE} -o s21_test.out $(CHECK_LIB) && ./s21_test.out

s21_decimal.a:
	gcc -c s21_decimal.c && ar r s21_decimal.a s21_decimal.o

gcov_report: clean lcov ${SOURCE}
	gcc ${GCOV_FLAGS} ${SOURCE} ${CHECK_LIB} -o s21_test 
	./s21_test
	lcov -t "s21_test" -o s21_test.info -c -d .
	genhtml -o report s21_test.info
	open ./report/index.html

lcov:
ifeq ("", "$(shell PATH=$(PATH) which lcov)")
	$(error Need to install lcov)
endif
