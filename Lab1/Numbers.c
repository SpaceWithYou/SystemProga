#include "Numbers.h"


Numbers getNumbersFromInput() {
	int a = -1, b = -1, c = -1, d = -1;
	scanf("%d %d %d %d", &a, &b, &c, &d);
	Numbers numbers = { a, b, c, d };
	return numbers;
}