#pragma once

typedef struct
{
	int* data;
	size_t size;
} dynamic_array;

size_t get_data_size(dynamic_array *d_array) {
	return d_array->size;
}