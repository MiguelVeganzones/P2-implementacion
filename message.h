#pragma once
#include <iostream>
#include <cmath>
#include <assert.h>
#include <array>
//size of each field

static constexpr uint8_t n = 6; //number of extra fields excluding redundance
static constexpr uint8_t data_size = 255; //number of cghars per message
static constexpr uint16_t total_msg_size = data_size + n;

typedef uint8_t redun_s;
typedef uint8_t dest_s;
typedef uint8_t orig_s;
typedef uint8_t long_s;
typedef uint8_t type_s;
typedef uint8_t PAS_s;
typedef uint16_t total_msg_size_s;
typedef long queue_direction_s;

//global constants
constexpr uint8_t n_retry = 3; //number of times the message is tried to be resent in case of error 

struct message {
	// structure of the message:
	// | Redundance | Destination | origin | length | type | data |

	message() = default;

	unsigned char data[data_size]{};
	redun_s redundance{}; //redundance field for error checking
	dest_s destination{}; //final user PID
	orig_s origin{}; //origin user PID
	long_s longitude{}; //length of the data sent
	type_s type{}; //type of message, either 0, 1, or 2 (Original, ACK, NAK)
	PAS_s PAS{};
	static const total_msg_size_s total_size = total_msg_size;

	std::array<uint8_t, total_msg_size> concatenate_message() const;

};

std::array<uint8_t, total_msg_size> message::concatenate_message() const {	
	std::array<uint8_t, total_msg_size> __m;// one block of memory that will contain the full
							 
	unsigned int i = 0;
	__m[i++] = redundance;
	__m[i++] = destination;
	__m[i++] = origin;
	__m[i++] = longitude;
	__m[i++] = type;
	__m[i++] = PAS;
	for (uint8_t j = 0; i < total_msg_size; ++i, ++j) {
		__m[i] = data[j];
	}
	return __m;
}

bool check_destination(const message& m) {
	return m.destination != 0;
}

void set_redundance(message& m) {
	uint8_t __xor = m.destination xor m.longitude xor m.origin xor m.PAS xor m.type;
	for (unsigned int i = 0; i < data_size; ++i) {
		__xor ^= m.data[i];
	}
	m.redundance = __xor;
}

bool check_redundance(const message& m) {	
	uint8_t __xor = m.destination xor m.longitude xor m.origin xor m.PAS xor m.type;
	for (unsigned int i = 0; i < data_size; ++i) {
		__xor ^= m.data[i];
	}
	return m.redundance == __xor;
}

std::ostream& operator<<(std::ostream& os, const message& m) {
	os <<
		"\nDestination: " << (int)m.destination <<
		"\nOrigin: " << (int)m.origin <<
		"\nData longitude: " << (int)m.longitude <<
		"\nMessage type: " << (int)m.type <<
		"\nPAS: " << (int)m.PAS << "\n";
	for (unsigned char c : m.data) {
		os << c << " ";
	}

	return os;
}


//---------------------------------------------------------------------------

//Struct for message queue

struct Messages_queue{
	// structure of the message:
	// | Redundancy | Destination | origin | length | type | data |

	Messages_queue() = default;

	unsigned char data[data_size]{};//Data
	queue_direction_s queue_direction{};//Indicate the queue direction
	//redun_s redundance{}; //redundance field for error checking
	dest_s destination{}; //final user PID
	orig_s origin{}; //origin user PID
	long_s longitude{}; //length of the data sent
	//type_s type{}; //type of message, either 0, 1, or 2 (Original, ACK, NAK)
	static const total_msg_size_s total_size = total_msg_size;

	std::array<uint8_t, total_msg_size> concatenate_message() const;

};

//Struct for memory shared

struct Memory_shared {

	Memory_shared() = default;

	unsigned char data[data_size]{}; //Data
	redun_s memo_redundance{}; //redundance field for error checking
	dest_s memo_destination{}; //final user PID
	orig_s memo_origin{}; //origin user PID
	long_s memo_longitude{}; //length of the data sent
	type_s memo_type{}; //type of message, either 0, 1, or 2 (Original, ACK, NAK)

};