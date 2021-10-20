#pragma once
#include <iostream>

//size of each field

constexpr uint8_t data_s = 251; //251 so the full message sent is 2^11 bits size. This will be helpfull for bit masking
typedef uint8_t redun_s;
typedef uint8_t dest_s;
typedef uint8_t orig_s;
typedef uint8_t long_s;
typedef uint8_t type_s;


//global constants
constexpr uint8_t n_retry = 3; //number of times the message is tried to be resent in case of error
constexpr uint8_t local_buffer_size = 10; //maximum number of characters in the data to send is local_buffer_size * data_s
 

struct message {
	// structure of the message:
	// | Redundancia | Destino | origen | longitud | type | data |
	unsigned char* data[data_s];
	redun_s redundance; //redundance field for error checking
	dest_s destination; //final user ID
	orig_s origin; //origin user ID
	long_s longitud; //longitud of the data sent
	type_s type; //type of message, either 0, 1, or 2 (Original, ACK, NAK)
};

struct entity {
	const message* _messages[local_buffer_size]; //constant pointers to non constant messages


};
