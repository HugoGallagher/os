#include "lib.h"

extern void asm_msg_send(uint32_t dst, uint8_t* data, uint32_t len);
extern void asm_msg_get(uint8_t** p_data, uint32_t* p_len);
extern void asm_msg_ack();
extern void asm_print(char* text, uint32_t length);

void msg_send(uint32_t dst, uint8_t* data, uint32_t len)
{
    asm_msg_send(dst, data, len);
}
void msg_get(uint8_t** p_data, uint32_t* p_len)
{
    asm_msg_get(p_data, p_len);
}
void msg_ack()
{
    asm_msg_ack();
}
void print(char* text, uint32_t length)
{
    asm_print(text, length);
}
