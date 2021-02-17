#include "tasks/syscalls.h"

#include "interface/terminal.h"
#include "tasks/tasks.h"

void sys_exit(GeneralRegisters r)
{
    tm_delete_active_task();
}
void sys_yield(GeneralRegisters r)
{
    tm_enter_next_task();
}
void sys_msg_send(GeneralRegisters r)
{
    uint8_t* data = r.ebx;
    uint32_t len = r.ecx;
    uint32_t dst = r.edx;

    tm_msg_transmit(dst, data, len);
}
void sys_msg_receive(GeneralRegisters r)
{
    uint8_t** p_data = r.ebx;
    uint32_t* p_len = r.ecx;

    tm_msg_get(p_data, p_len);
}
void sys_msg_ack(GeneralRegisters r)
{
    tm_msg_ack();
}
void sys_print(GeneralRegisters r)
{
    char* text = r.ebx;
    uint32_t length = r.ecx;

    terminal_write(text, length);
}

void (*syscalls[])(GeneralRegisters) =
{
    sys_exit,
    sys_yield,
    sys_msg_send,
    sys_msg_receive,
    sys_msg_ack,
    sys_print,
};

void sys_call(GeneralRegisters r)
{
    uint32_t index = r.eax;
    (*syscalls[index])(r);
}
