#pragma once
#include "message.h"
#include <iostream>

int main() {

	message m;
	uint16_t j = 0;

	m.destination = j++;
	m.origin = j++;
	m.longitud = j++;
	m.type = j++;
	m.PAS = j++;

	for (uint8_t i = 0; i < data_s; ++i) {
		m.data[i] = j++;
	}

	std::cout << m << "\n\n" << --j << char(10) << (char)j;

	uint8_t res = 0;
	for (uint16_t i = 0; i < 256; ++i) {
		res ^= i;
		std::cout << (int)res << std::endl;
	}

	std::cout << (int)res << std::endl;
	check_redundance(m);

	return EXIT_SUCCESS;
}
