#pragma once

#include "types.h"

void msg_send(uint32_t dst, uint8_t* data, uint32_t len);
void msg_get(uint8_t** p_data, uint32_t* p_len);
void msg_ack();
void print(char* text, uint32_t length);
