#pragma once
#include "message.h"
#include <iostream>

int main() {

	message m;
	uint8_t j = 0;

	m.destination = j++;
	m.origin = j++;
	m.longitude = j++;
	m.type = j++;
	m.PAS = j++;

	for (uint8_t i = 0; i < data_size; ++i, ++j) {
		m.data[i] = j;
	}

	std::cout << m << "\n\n" << (int)j << " -- " << (char)j << "\n\n";

	uint8_t res = 0;
	for (uint16_t i = 1; i < total_msg_size - 1; ++i) {
		res ^= (uint8_t)i;
		std::cout << (int)res << std::endl;
	}

	std::cout << "\n\n";
	set_redundance(m);
	std::cout << "red: " << (int)m.redundance;
	std::cout << "\n" <<(int)res << std::endl <<
	check_redundance(m);

	return EXIT_SUCCESS;
}
