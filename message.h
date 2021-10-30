#pragma once
#include <iostream>
#include <cmath>
#include <assert.h>
#include <array>
#include <string>
#include <vector>
//size of each field

static constexpr uint8_t n = 6; //number of extra fields excluding redundance
static constexpr uint8_t data_size = 256; //number of cghars per message
static constexpr uint16_t total_msg_size = data_size + n;

typedef uint8_t redun_s;
typedef uint8_t dest_s;
typedef uint8_t orig_s;
typedef uint8_t long_s;
typedef uint8_t type_s;
typedef uint8_t PAS_s;
typedef uint16_t total_msg_size_s;

//global constants
constexpr uint8_t n_retry = 3; //number of times the message is tried to be resent in case of error 

struct message {
	// structure of the message:
	// | Redundance | Destination | origin | length | type | data |

	message() = default;
	message(std::vector<unsigned char>& v) { //add destination and pas input
		long_s i = 0;
		for (auto c : v) {
			data[i++] = c;
		}
		//origin = get_pid();
		//destination = _destintion;
		length = v.size();
		//type = 1;
		//PAS = PAS; //14 for eco
	};

	message(std::vector<unsigned char>& v, const int pid_destination) { //add destination and pas input
		long_s i = 0;
		for (auto c : v) {
			data[i++] = c;
		}
		//origin = get_pid();
		destination = pid_destination;
		length = v.size();
		//type = 1;
		//PAS = PAS; //14 for eco
	};

	unsigned char data[data_size]{};
	redun_s redundance{};    //redundance field for error checking
	dest_s destination{};   //final user PID
	orig_s origin{};       //origin user PID
	long_s length{};      //length of the data sent
	type_s type{};       //type of message, either 0, 1, or 2 (Original, ACK, NAK)
	PAS_s PAS{};        //type of comunication, eco or tf. Only eco will be impemented
	static const total_msg_size_s total_size = total_msg_size;

	std::array<uint8_t, total_msg_size> concatenate_message() const;
};

std::array<uint8_t, total_msg_size> message::concatenate_message() const {	
	std::array<uint8_t, total_msg_size> __m;// one block of memory that will contain the full
							 
	unsigned int i = 0;
	__m[i++] = redundance;
	__m[i++] = destination;
	__m[i++] = origin;
	__m[i++] = length;
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
	uint8_t __xor = m.destination xor m.length xor m.origin xor m.PAS xor m.type;
	for (unsigned int i = 0; i < data_size; ++i) {
		__xor ^= m.data[i];
	}
	m.redundance = __xor;
}

bool check_redundance(const message& m) {	
	uint8_t __xor = m.destination xor m.length xor m.origin xor m.PAS xor m.type;
	for (unsigned int i = 0; i < data_size; ++i) {
		__xor ^= m.data[i];
	}
	return m.redundance == __xor;
}

std::ostream& operator<<(std::ostream& os, const message& m) {
	os <<
		"\nDestination: " << (int)m.destination <<
		"\nOrigin: " << (int)m.origin <<
		"\nData longitude: " << (int)m.length <<
		"\nMessage type: " << (int)m.type <<
		"\nPAS: " << (int)m.PAS << "\n";
	for (unsigned char c : m.data) {
		os << c;
	}

	return os;
}

void input_message(std::vector<std::vector<unsigned char>>& w) {
	std::string line;
	std::vector<std::string> v;
	w.clear();

	std::cout << "mensaje:\n";
	bool flag = 0;

	while (std::getline(std::cin, line)) {
		if (flag && line.empty()) {
			v.pop_back(); //remove empty last line
			break;
		}
		else if (line.empty()) {
			flag = 1;
		}
		else {
			flag = 0;
		}
		v.push_back(line);
	}
	
	unsigned long int total_size = 0;
	unsigned long int i = 0;

	for (auto& s : v) {
		total_size += s.size() + 1; //+1 for the '\n' char
	}

	w.resize(total_size / data_size + 1);

	for (auto& s : v) {
		for (unsigned char c : s) {
			w[i++ / data_size].push_back(c);
		}
		w[i++ / data_size].push_back('\n');
	}
	std::cout << "Total message size: " << total_size << std::endl;
}

//---------------------------------------------------------------------------

//Struct for message queue

struct Messages_queue{
	// structure of the message:
	// | Redundancy | Destination | origin | length | type | data |

	Messages_queue() = default;

	unsigned char data[data_size]{};//Data
	redun_s redundance{}; //redundance field for error checking
	dest_s destination{}; //final user PID
	orig_s origin{}; //origin user PID
	long_s length{}; //length of the data sent
	type_s type{}; //type of message, either 0, 1, or 2 (Original, ACK, NAK)
	PAS_s PAS{};
	static const total_msg_size_s total_size = total_msg_size;

	std::array<uint8_t, total_msg_size> concatenate_message() const;

};

//Struct for memory shared

struct Memory_shared {

	unsigned char data[data_size]{}; //Data
	redun_s memo_redundance{}; //redundance field for error checking
	dest_s memo_destination{}; //final user PID
	orig_s memo_origin{}; //origin user PID
	long_s memo_length{}; //length of the data sent
	type_s memo_type{}; //type of message, either 0, 1, or 2 (Original, ACK, NAK)
	PAS_s memo_PAS{};

};