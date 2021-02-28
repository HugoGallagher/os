#pragma once

#include <stdint.h>

#include "interrupts/idt.h"

enum SyscallsID
{
    SYS_EXIT,
    SYS_YIELD,
    SYS_CREATE_PROCESS,
    SYS_GET_SERVER_ID,
    SYS_MSG_SEND,
    SYS_MSG_RECEIVE,
    SYS_MSG_ACK,
    SYS_D_PUT_PIXEL,
    SYS_D_PUT_PIXELS,
    SYS_D_COPY_BUFFER,
};

typedef enum SyscallsID SyscallsID;

void sys_exit(GeneralRegisters r, uint32_t eip, uint32_t esp);
void sys_yield(GeneralRegisters r, uint32_t eip, uint32_t esp);
void sys_create_process(GeneralRegisters r, uint32_t eip, uint32_t esp);
void sys_get_server_id(GeneralRegisters r, uint32_t eip, uint32_t esp);
void sys_msg_send(GeneralRegisters r, uint32_t eip, uint32_t esp);
void sys_msg_receive(GeneralRegisters r, uint32_t eip, uint32_t esp);
void sys_msg_ack(GeneralRegisters r, uint32_t eip, uint32_t esp);
void sys_d_copy_buffer(GeneralRegisters r, uint32_t eip, uint32_t esp);
void sys_d_put_pixel(GeneralRegisters r, uint32_t eip, uint32_t esp);
void sys_d_put_pixels(GeneralRegisters r, uint32_t eip, uint32_t esp);

void sys_call(GeneralRegisters r, uint32_t eip, uint16_t cs, uint32_t f, uint32_t esp, uint16_t ss);
