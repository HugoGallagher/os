#include "tasks/syscalls.h"

#include "interface/terminal.h"

void sys_exit(GeneralRegisters r)
{

}
void sys_yield(GeneralRegisters r)
{

}
void sys_msg_send(GeneralRegisters r)
{

}
void sys_msg_receive(GeneralRegisters r)
{

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
    sys_print,
};

void sys_call(GeneralRegisters r)
{
    uint32_t index = r.eax;
    (*syscalls[4])(r);
}
