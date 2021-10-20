#pragma once
#include "message.h"

static constexpr uint8_t local_buffer_size = 10;

struct entity {
	const message* _messages[local_buffer_size]; //constant pointers to non constant messages
};