#pragma once
#include <iostream>
#include <cmath>
#include <string>
#include <vector>

//#include <sys/ipc.h>
//#include <sys/msg.h>

//size of each field

//size of each field

int msgrcv(int a, void* b, size_t c, long d) { return 1; }
int msgsnd(int a, void* b, size_t c, long d) { return 1; }

static constexpr uint8_t n = 6; //number of extra fields excluding redundance
static constexpr uint8_t data_size = 255; //number of cghars per message
static constexpr uint16_t total_msg_size = data_size + n;
static constexpr uint8_t message_units_queue = 20;
static constexpr uint16_t queue_block_size = data_size * message_units_queue;

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

	message(dest_s _destination, orig_s _origin, long_s _length, type_s _type, PAS_s _PAS) :
		destination{ _destination }, origin{ _origin }, length{ _lenght }, type{ _type }, PAS{ _PAS }
	{

	}

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
	PAS_s PAS{};        //type of comunication, eco or tf. Only eco will be implemented
	static const total_msg_size_s total_size = total_msg_size;
};

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

message mutate(message m) {
	message ret;
	for (auto& c : m.data) {
		c = toupper(c);
	}
	return ret;
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

void input_message(char* ret, size_t size = queue_block_size) {
	std::string line;
	std::vector<std::string> v;

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

	if (total_size > queue_block_size) { std::cout << "mensaje demasiado grande\n"; exit(EXIT_FAILURE); }
	for (auto& s : v) {
		for (unsigned char c : s) {
			ret[i++] = c;
		}
		ret[i++] = '\n';
	}
	std::cout << "Total message size: " << total_size << std::endl;
}

//---------------------------------------------------------------------------
//struct for queue
struct data_queue {

	long message_type;

	PAS_s PAS;
	orig_s origin;
	dest_s destination;

	char data_block[queue_block_size];
	inline size_t size() { return sizeof(data_queue) - sizeof(long); }
};

inline void user_send_message(long type, int ID_cola, int PID_user) {
	data_queue queue;
	queue.message_type = type;

	dest_s destination;
	std::cout << "Introduce el PID del usuario de destino\n";
	std::cin >> destination;

	PAS_s pas;
	std::cout << "\nIntroduce el tipo de comunicaci?n: \nPara eco: 14\n";
	std::cin >> queue.PAS;

	if (pas != 14) {
		exit(EXIT_FAILURE);
	}
	queue.origin = PID_user;
	input_message(queue.data_block);

	msgsnd(ID_cola, (data_queue*)&queue, queue.size(), 1L);
}

inline std::vector<message> entity_read_queue_msg(int ID_cola) {
	data_queue queue;
	msgrcv(ID_cola, (data_queue*)&queue, queue.size(), 1L);

	std::vector<message> ret;

	int16_t i = 0;

	for (auto c : queue.data_block) {
		if (i % data_size == 0)ret.push_back(message());

		ret[i / data_size].data[i % data_size] = c;
	}
}

inline void entity_send_queue_msg(int ID_cola, std::vector<message> messages) {
	data_queue queue;
	queue.destination = messages[0].destination;
	queue.message_type = 2L;
	queue.origin = messages[0].origin;
	queue.PAS = messages[0].PAS;
	uint16_t i = 0;
	for (auto& message : messages) {
		if (i >= queue_block_size) { std::cout << "Message size error\n";  exit(EXIT_FAILURE); }
		for (auto c : message.data) {
			queue.data_block[i++] = c;
		}
	}
	msgsnd(ID_cola, (data_queue*)&queue, queue.size(), 2L);
}

inline void user_read_queue_msg(int ID_cola) {
	data_queue queue;
	msgrcv(ID_cola, (data_queue*)&queue, queue.size(), 2L);
	for (auto c : queue.data_block)std::cout << c;
}

//Struct for shared memory

struct shared_mem {
	uint8_t err_count = 0;
	message _message;
};

inline void entity_send_to_shared(shared_mem* _shared_mem, message m) {
	if (m.type == 2) {
		if (_shared_mem->err_count++ >= n_retry) {
			std::cout << "Mensaje recibido erroneamente demasiadas veces";
			exit(EXIT_FAILURE);
		}
	}
	else if (m.type == 1) { _shared_mem->err_count = 0; }

	_shared_mem->_message = m;
}

inline message entity_read_from_shared(shared_mem* _shared_mem) {
	return _shared_mem->_message;
}
