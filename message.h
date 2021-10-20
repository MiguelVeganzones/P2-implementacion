#pragma once
#include <iostream>
#include <cmath>
#include <assert.h>
#include <array>*
//size of each field

constexpr uint16_t total_msg_size = 256 + 1;
constexpr uint8_t n = 6; //number of extra fields excluding redundance
constexpr uint8_t data_s = total_msg_size - n + 1; //251 so the full message sent without redundance is 2^11 bits size. This will be helpfull for bit masking


typedef uint8_t redun_s;
typedef uint8_t dest_s;
typedef uint8_t orig_s;
typedef uint8_t long_s;
typedef uint8_t type_s;
typedef uint8_t PAS_s;
typedef uint16_t total_msg_size_s;

//global constants
constexpr uint8_t n_retry = 3; //number of times the message is tried to be resent in case of error
constexpr uint8_t local_buffer_size = 10; //maximum number of characters in the data to send is local_buffer_size * data_s
 

struct message {
	// structure of the message:
	// | Redundancia | Destino | origen | longitud | type | data |
	unsigned char data[data_s];
	redun_s redundance; //redundance field for error checking
	dest_s destination; //final user ID
	orig_s origin; //origin user ID
	long_s longitud; //longitud of the data sent
	type_s type; //type of message, either 0, 1, or 2 (Original, ACK, NAK)
	PAS_s PAS; 
	const total_msg_size_s total_size = total_msg_size;

	std::array<uint8_t, total_msg_size -1> concatenate_message_for_redundance() const;

};

std::array<uint8_t, total_msg_size - 1> message::concatenate_message_for_redundance() const {
	unsigned int n = total_msg_size - 1;
	assert((n & (n - 1)) == 0); //assert final message size is a power of 2
	
	std::array<uint8_t, total_msg_size - 1> __m;// one block of memory that will contain the full
							    //message wiithout the redundance filed	
	unsigned int i = 0;    
	__m[i++] = destination;
	__m[i++] = origin;
	__m[i++] = longitud;
	__m[i++] = type;
	__m[i++] = PAS;
	for (unsigned int j = 0; i < total_msg_size -1; ++i, ++j) {
		__m[i] = data[j];
	}
	return __m;
}

struct entity {
	const message* _messages[local_buffer_size]; //constant pointers to non constant messages
};

bool check_destination(const message& m) {
	return m.destination != 0;
}

bool check_redundance(const message& m) {	
	const unsigned int n = total_msg_size - 1;
	std::array<uint8_t, total_msg_size - 1> __m = m.concatenate_message_for_redundance();
	unsigned int stride = n / 2;

	unsigned int _size = sizeof(redun_s) * 8;
	for (; stride < _size; stride / 2) {
		for (unsigned int i = 0; i < stride / _size; ++i) {
			__m[i] ^= __m[i + stride];
		}
	}
	std::cout << (int)__m[0];
	return m.redundance == __m[0];
}

std::ostream& operator<<(std::ostream& os, const message& m) {
	os
		<< m.destination << " "
		<< m.origin << " "
		<< m.longitud << " "
		<< m.type << " "
		<< m.PAS << " ";
	for (char c : m.data) {
		os << c << " ";
	}

	return os;
}