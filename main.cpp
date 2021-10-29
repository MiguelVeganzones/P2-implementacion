#pragma once
#include "message.h"
#include <iostream>

int main0() {

	message m;
	uint8_t j = 0;

	m.destination = j++;
	m.origin = j++;
	m.length = j++;
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


int main() {
	uint8_t t{0};
	std::vector<std::vector<unsigned char>> v{};
	input_message(v);

	int i = 0; 

	for (auto& s : v) {
		message m(s);
		std::cout << "=================================\nMessage "<<i++<<": \n" << m << "\n========================================\n";
	}

	std::cout << '\n' << t << std::endl;
	
}

//ignorar
//Campos con un tamaño especifico de bits
// 
//struct A {
//	unsigned char a : 4; //entero de 4 bits, aunque en memoria ocupe 8
//	unsigned char b : 5;
//	A() : a{}, b{} {};
//};
//
//struct B {
//	unsigned char a; //entero de 8 bits, aunque en memoria ocupe 8
//	unsigned char b;
//	B() : a{}, b{} {};
//};
//
//int main() {
//	std::cout << "Tamaño en bytes de A: " << sizeof(A) << "\nTamaño en bytes de B: " << sizeof(B) << "\n";
//	int n = 17, i;
//	A a;
//	B b;
//	for (i = 0; i < n; ++i) {
//		++a.a;
//		++a.b;
//		++b.a;
//		++b.b;
//	}
//
//	std::cout << "A: " << (int)a.a << ", " << (int)a.b << "\nB: " << (int)b.a << ", " << (int)b.b << std::endl;
//}