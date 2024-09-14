#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Numbers.h"
#include "IntervalNumbers.h"

#define RESULT_MSG "'D'; 'ћен€ зовут %s %s'; 191, Ц437; 138.76, Ц961.753; 993.\n"
#define PRINT_MSG1 "Print Your LastName:\n"
#define PRINT_MSG2 "Print Your FirstName:\n"
#define PRINT_MSG3 "Print a b c d:\n"
#define PRINT_MSG4 "Print k1 k2 k3:\n"
#define PRINT_MSG5 "Print n:\n"
#define PRINT_MSG6 "Print text:\n"
#define PRINT_MSG7 "Print word:\n"
#define ERR_MSG "There no two words in text!"

#define CHECK_MACRO(x) !(x % k1) && !(x > k2 && x < k3)
#define LENGTH 10
#define LENGTH_ARRAY_1 5
#define LENGTH_ARRAY_2 6
#define TEXT_LENGTH 255

//Get First And LastName
void task1(char** FirstName, char** LastName) {
	printf(PRINT_MSG1);
	scanf("%s", FirstName);
	printf(PRINT_MSG2);
	scanf("%s", LastName);
	printf(RESULT_MSG, FirstName, LastName);
}

//get Sum
long task2(Numbers* numbers, IntervalNumbers* interval) {
	long sum = 0;
	int a = numbers->a, b = numbers->b, c = numbers->c, d = numbers->d;
	int k1 = interval->k1, k2 = interval->k2, k3 = interval->k3;
	
	if (CHECK_MACRO(a)) {
		sum += a;
	}
	if (CHECK_MACRO(b)) {
		sum += b;
	}
	if (CHECK_MACRO(c)) {
		sum += c;
	}
	if (CHECK_MACRO(d)) {
		sum += d;
	}

	return sum;
}

int getRandInt() {
	return rand();
}

int* getM() {
	static int M[LENGTH] = { 0 };
	for (int i = 0; i < LENGTH; i++) {
		M[i] = getRandInt();
	}
	return M;
}

//Get average of random LENGTH-array
double task3_1(int* m) {
	double res = 0;
	for (int i = 0; i < LENGTH; i++) {
		res += m[i];
	}
	return res / LENGTH;
}

int* getA() {
	static int A[LENGTH_ARRAY_1] = { 0 };
	for (int i = 0; i < LENGTH_ARRAY_1; i++) {
		A[i] = getRandInt();
	}
	return A;
}

int* getB() {
	static int B[LENGTH_ARRAY_2] = { 0 };
	for (int i = 0; i < LENGTH_ARRAY_2; i++) {
		B[i] = getRandInt();
	}
	return B;
}

void swap(int* arr, int i, int j) {
	arr[i] = arr[i] ^ arr[j];
	arr[j] = arr[i] ^ arr[j];
	arr[i] = arr[i] ^ arr[j];
}

void sort(int* arr, int n) {
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				swap(arr, j, j + 1);
			}
		}
	}
}

int* task3_2(int* A, int* B) {
	int static C[LENGTH_ARRAY_1 + LENGTH_ARRAY_2] = { 0 };
	int totalLength = LENGTH_ARRAY_1 + LENGTH_ARRAY_2;
	for (int i = 0; i < LENGTH_ARRAY_1; i++) {
		C[i] = A[i];
	}
	for (int i = LENGTH_ARRAY_1; i < totalLength; i++) {
		C[i] = B[i - LENGTH_ARRAY_1];
	}
	sort(C, totalLength);
	return C;
}

//Get first Fibonacci number bigger than n
long task4(int n) {
	long f1 = 1, f2 = 1, temp;
	while (f2 < n) {
		temp = f1;
		f1 = f2;
		f2 += temp;
	}
	return f2;
}

int checkWhiteSpace(char c) {
	return c == ' ' || c == '\t' ? 1 : 0;
}

int checkEndOfString(char c) {
	return c == '\n' || c == '\0' ? 1 : 0;
}

void skipUntilNewLine() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF);
}

//Insert word between two first words in text
void task5() {
	char text[TEXT_LENGTH], word[TEXT_LENGTH], result[2 * TEXT_LENGTH + 2], c;
	//get text with whitespaces and word (no whitespaces)
	printf(PRINT_MSG6);
	scanf("%c", &c);
	scanf("%[^\n]s", text);
	skipUntilNewLine();
	printf(PRINT_MSG7);
	scanf("%s", word);
	
	int i = -1, j = 0;
	//find first word
	while (!checkEndOfString(c = text[++i]) && checkWhiteSpace(c))
		result[i] = c;
	if (checkEndOfString(c)) {
		printf("%s", ERR_MSG);
		return;
	}

	while (!checkEndOfString(c = text[i])) {
		if (checkWhiteSpace(c)) {
			//Check that second word exists
			//There j = 0 at the beginning 
			while (checkWhiteSpace(c = text[i + j++]));
			if (checkEndOfString(c)) {
				printf("%s", ERR_MSG);
				return;
			}
			//Copy word to result
			result[i] = text[i];
			strcpy((result + i + 1), word);
			j = strlen(word);
			result[i + j + 1] = text[i];
			break;
		}
		else {
			//Copy 'till meet whitespace
			result[i] = text[i];
		}
		i++;
	}

	strcpy((result + i + j + 2), (text + i + 1));

	printf("%s\n", result);
}

//debug
void printIntArray(int* arr, int n) {
	for (int i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int main(int argc, char** argv) {
	//Task1
	char FirstName[100], LastName[100];
	task1(&FirstName, &LastName);
	//Task2
	printf(PRINT_MSG3);
	Numbers numbers = getNumbersFromInput();
	printf(PRINT_MSG4);
	IntervalNumbers interval = getIntervalFromInput();
	printf("%ld\n", task2(&numbers, &interval));
	//Task3
	srand(time(NULL));
	int* M = getM();
	printIntArray(M, LENGTH);
	printf("M avg = %3.6f\n", task3_1(M));
	int* A = getA();
	int* B = getB();
	printIntArray(A, LENGTH_ARRAY_1);
	printIntArray(B, LENGTH_ARRAY_2);
	int* C = task3_2(A, B);
	printIntArray(C, LENGTH_ARRAY_1 + LENGTH_ARRAY_2);
	//Task4
	int n = 0;
	printf(PRINT_MSG5);
	scanf("%d", &n);
	printf("%ld\n", task4(n));
	//Task5
	task5();
	return 0;
}