#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dynamic_array.h"
#include "circle.h"
#include "CD.h"
#include "string.h"
#define get_i d_array->data[i]
#define ARRAY_LENGTH 10
#define PREFIX_LENGTH 1

void task1() {
	double a = 0, b = 0;
	double* p1 = &a;
	double* p2 = &b;
	scanf("%lf", p1);
	scanf("%lf", p2);

	if (*p1 > *p2) {
		*p1 += 3;
		*p2 /= 3;
	}
	else {
		*p1 /= 2;
		*p2 *= 3;
	}
	//free(p1);
	//free(p2);
}

void task2() {
	bool* p1 = NULL;
	bool* p2 = NULL;
	p1 = (bool*)malloc(sizeof(bool));
	p2 = (bool*)malloc(sizeof(bool));

	*p1 = true;
	*p2 = false;
	free(p1);
	free(p2);
}

void print_dynamic_array(dynamic_array* d_array, size_t array_size) {
	for (size_t i = 0; i < array_size; i++) {
		printf("%d ", get_i);
	}
	printf("\n");
}

void task3(dynamic_array *d_array) {
	size_t array_size = get_data_size(d_array);
	int data_at_i;
	for (size_t i = 0; i < array_size; i++) {
		data_at_i = get_i;
		if (data_at_i > 0) {
			get_i *= data_at_i;
		}
	}

	print_dynamic_array(d_array, array_size);
}

void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

void selection_sort(dynamic_array *d_array) {
	int size = get_data_size(d_array), index;
	int* data = d_array->data;
	for (size_t i = 0; i < size - 1; i++) {
		index = i;
		for (size_t j = i + 1; j < size; j++) {
			if (data[j] < data[index]) {
				index = j;
			}
		}
		if (index != i) {
			swap(&data[i], &data[index]);
		}
	}
}

dynamic_array task4(dynamic_array* d_array) {
	size_t size = get_data_size(d_array);
	dynamic_array result;
	result.size = 0;

	int data_at_i;
	for (size_t i = 0; i < size; i++) {
		data_at_i = get_i;
		if (data_at_i < 0) {
			result.size++;
		}
	}
	
	result.data = (int*)malloc(result.size * sizeof(int));
	result.size = 0;
	for (size_t i = 0; i < size; i++) {
		data_at_i = get_i;
		if (data_at_i < 0) {
			result.data[result.size] = 4 * data_at_i;
			result.size++;
		}
	}

	print_dynamic_array(&result, result.size);
	selection_sort(&result);
	printf("sorted\n");
	print_dynamic_array(&result, result.size);
	return result;
}

void get_quadrant_by_param(int* quadrant, int x, int y) {
	if (x * y > 0) {
		if (x > 0) {
			*quadrant = 1;
		}
		else {
			*quadrant = 3;
		}
		return;
	}
	else if (x * y < 0) {
		if (x > 0) {
			*quadrant = 4;
		}
		else {
			*quadrant = 2;
		}
		return;
	}
	else {
		*quadrant = -1;
	}
}

void task5() {
	int* quadrant = (int *)malloc(sizeof(int));
	get_quadrant_by_param(quadrant, 1, 1);
	printf("quadrant %d\n", *quadrant);
	get_quadrant_by_param(quadrant, -2, 3);
	printf("quadrant %d\n", *quadrant);
}

void task6(Circle* circles) {
	int x = -1, y = -1, r = INT_MAX; //min circle
	Circle circle;
	for (size_t i = 0; i < ARRAY_LENGTH; i++) {
		circle = *(circles + i);
		if (circle.radius <= r) {
			x = circle.center.x;
			y = circle.center.y;
			r = circle.radius;
		}
	}

	printf("Minimal radius circle %d %d %d", x, y, r);
}

void swap_CDs(CD* cd1, CD* cd2) {
	CD temp = *cd1;
	*cd1 = *cd2;
	*cd2 = temp;
}

void sort_CDs(CD* cd_arr, size_t length) {
	int size = length, index;
	for (size_t i = 0; i < size - 1; i++) {
		index = i;
		for (size_t j = i + 1; j < size; j++) {
			if (strcmp(cd_arr[j].style, cd_arr[index].style) < 0) {
				index = j;
			}
		}
		if (index != i) {
			swap_CDs(&cd_arr[i], &cd_arr[index]);
		}
	}
}

CD* task7(CD* cd_arr) {
	char* search_prefix = (char*)malloc(PREFIX_LENGTH * sizeof(char));
	scanf("%s", search_prefix);

	int counter = 0;
	for (size_t i = 0; i < ARRAY_LENGTH; i++) {
		if (!strncmp(search_prefix, cd_arr[i].name, PREFIX_LENGTH)) {
			counter++;
		}
	}

	if (!counter) {
		cd_arr = NULL;
	}

	CD* result = (CD *)malloc(sizeof(CD) * counter);
	int j = 0;
	for (size_t i = 0; i < ARRAY_LENGTH; i++) {
		if (!strncmp(search_prefix, cd_arr[i].name, PREFIX_LENGTH)) {
			result[j] = cd_arr[i];
			j++;
		}
	}

	//print
	printf("Before sort\n");
	for (size_t i = 0; i < counter; i++) {
		printf("%s %s, ", result[i].name, result[i].style);
	}
	printf("\n");

	sort_CDs(result, counter);

	printf("After sort\n");
	for (size_t i = 0; i < counter; i++) {
		printf("%s %s, ", result[i].name, result[i].style);
	}
	return result;
}

void init_arr(dynamic_array* d_array) {
	d_array->data = (int*)malloc(6 * sizeof(int));
	d_array->data[0] = 0;
	d_array->data[1] = -1;
	d_array->data[2] = 2;
	d_array->data[3] = -3;
	d_array->data[4] = 4;
	d_array->data[5] = 0;
	d_array->size = 6;
}

Circle* get_circles() {
	Circle* circles = (Circle *)malloc(sizeof(Circle) * ARRAY_LENGTH);
	circles[0] = (Circle){.center = {-190, 1000}, .radius = 10};
	circles[1] = (Circle){ .center = {-19, 1}, .radius = 9 };
	circles[2] = (Circle){ .center = {0, 0}, .radius = 8 };
	circles[3] = (Circle){ .center = {0, 50}, .radius = 10 };
	circles[4] = (Circle){ .center = {50, 9}, .radius = 5 };
	circles[5] = (Circle){ .center = {-10, 7}, .radius = 1 };
	circles[6] = (Circle){ .center = {-14, -20}, .radius = 1000 };
	circles[7] = (Circle){ .center = {3, 2}, .radius = 15 };
	circles[8] = (Circle){ .center = {890, 59}, .radius = 20 };
	circles[9] = (Circle){ .center = {0, 0}, .radius = 99999 };
	return circles;
}

CD* get_cds() {
	CD* cds = (CD *)malloc(sizeof(CD) * ARRAY_LENGTH);
	cds[0] = (CD){ .name = "bad", .musician = "Michael Jackson", .style = "pop-rock", .year = 1999, .duration = 10, .cost = 100};
	cds[1] = (CD){ .name = "thriller", .musician = "Michael Jackson", .style = "pop", .year = 1999, .duration = 10, .cost = 100 };
	cds[2] = (CD){ .name = "happy Nation", .musician = "Ace of Base", .style = "dance-pop", .year = 1999, .duration = 10, .cost = 100 };
	cds[3] = (CD){ .name = "cruel Summer", .musician = "Ace of Base", .style = "euro-dance", .year = 1999, .duration = 10, .cost = 100 };
	cds[4] = (CD){ .name = "victory", .musician = "Modern Talking", .style = "euro-disco", .year = 1999, .duration = 10, .cost = 100 };
	cds[5] = (CD){ .name = "heart And Soul", .musician = "Modern Talking", .style = "new-wave", .year = 1999, .duration = 10, .cost = 100 };
	cds[6] = (CD){ .name = "born A Rockstar : The Collection", .musician = "Neffex", .style = "hip-hop", .year = 1999, .duration = 10, .cost = 100 };
	cds[7] = (CD){ .name = "change: The Collection", .musician = "Neffex", .style = "rap", .year = 1999, .duration = 10, .cost = 100 };
	cds[8] = (CD){ .name = "favourite Worst Nightmare", .musician = "Arctic Monkeys", .style = "indie-rock", .year = 1999, .duration = 10, .cost = 100 };
	cds[9] = (CD){ .name = "hellbilly Deluxe", .musician = "Rob Zombie", .style = "industrial-metal", .year = 1999, .duration = 10, .cost = 100 };
	return cds;
}

int main(int argc, char** argv) {
	task1();
	task2();
	dynamic_array p;
	init_arr(&p);
	task3(&p);
	free(p.data);

	init_arr(&p);
	task4(&p);
	task5();
	/*
	Circle* circles = (Circle*)malloc(sizeof(Circle) * ARRAY_LENGTH);
	int x = -1, y = -1, r = -1; //temp
	printf("x y r\n");
	for (size_t i = 0; i < ARRAY_LENGTH; i++) {
		scanf("%d %d %d\n", &x, &y, &r);
		Circle circle = { .center = {x, y}, .radius = r };
		circles[i] = circle;
	}
	*/
	task6(get_circles());
	task7(get_cds());
	return 0;
}