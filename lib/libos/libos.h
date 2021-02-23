#pragma once

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

void exit();
void yield();
void msg_send(uint32_t dst, uint8_t* data, uint32_t len);
void msg_get(uint8_t** p_data, uint32_t* p_len);
void msg_ack();
void print(char* text, uint32_t length);
void print_hex(uint32_t num);
