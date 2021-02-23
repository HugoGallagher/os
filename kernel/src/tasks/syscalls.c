#include "tasks/syscalls.h"

#include "interface/terminal.h"
#include "tasks/tasks.h"

void sys_exit(GeneralRegisters r, uint32_t eip, uint32_t esp)
{
    tm_delete_active_task();
}
void sys_yield(GeneralRegisters r, uint32_t eip, uint32_t esp)
{
    terminal_writestring("Yielded\n");
    tm_save_registers(r, eip, esp);
    tm_enter_next_task();
}
void sys_msg_send(GeneralRegisters r, uint32_t eip, uint32_t esp)
{
    uint8_t* data = r.ebx;
    uint32_t len = r.ecx;
    uint32_t dst = r.edx;

    tm_msg_transmit(dst, data, len);
}
void sys_msg_receive(GeneralRegisters r, uint32_t eip, uint32_t esp)
{
    uint8_t** p_data = r.ebx;
    uint32_t* p_len = r.ecx;

    tm_msg_get(p_data, p_len);
}
void sys_msg_ack(GeneralRegisters r, uint32_t eip, uint32_t esp)
{
    tm_msg_ack();
}
void sys_print(GeneralRegisters r, uint32_t eip, uint32_t esp)
{
    char* text = r.ebx;
    uint32_t length = 14;

    terminal_write(text, length);
}
void sys_print_hex(GeneralRegisters r, uint32_t eip, uint32_t esp)
{
    terminal_writestring("print_hex called: ");
    terminal_writehex(r.ebx);
}

void (*syscalls[])(GeneralRegisters, uint32_t, uint32_t) =
{
    sys_exit,
    sys_yield,
    sys_msg_send,
    sys_msg_receive,
    sys_msg_ack,
    sys_print,
    sys_print_hex
};

void sys_call(GeneralRegisters r, uint32_t eip, uint16_t cs, uint32_t f, uint32_t esp, uint16_t ss)
{
    uint32_t index = r.eax;
    (*syscalls[index])(r, eip, esp);
}
