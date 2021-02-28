#pragma once

typedef unsigned long long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

enum ServerType
{
    SV_DISPLAY,
    SV_INPUT,
    SV_FILESYSTEM,
};
enum MessageID
{
    D_PUT_PIXEL,
    D_PUT_PIXELS,
    I_GET_INPUT_MAP,
};

typedef enum ServerType ServerType;
typedef enum MessageID MessageID;

void exit();
void yield();
void create_process(char* path, uint32_t path_size);
void get_server_id(uint16_t* p_data, ServerType sv);
void msg_send(uint32_t dst, uint8_t* data, uint32_t len);
void msg_get(uint8_t** p_data, uint32_t* p_len);
void msg_ack();
void d_copy_buffer(char* data);
void d_put_pixel(uint32_t x, uint32_t y, uint32_t data);
void d_put_pixels(uint32_t x, uint32_t y, uint8_t* data, uint32_t length);

uint8_t sc_to_ascii(uint8_t sc);
