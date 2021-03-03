#include "tasks/syscalls.h"

#include "interface/terminal.h"
#include "tasks/tasks.h"

void sys_exit(GeneralRegisters r, uint32_t eip, uint32_t esp)
{
    tm_delete_active_task();
}
void sys_yield(GeneralRegisters r, uint32_t eip, uint32_t esp)
{
    tm_save_registers(r, eip, esp);
    tm_enter_next_task();
}
void sys_create_process(GeneralRegisters r, uint32_t eip, uint32_t esp)
{
    char* path = r.ebx;
    uint32_t len = r.ecx;
    tm_create_task(false, path, len);
}
void sys_get_server_id(GeneralRegisters r, uint32_t eip, uint32_t esp)
{
    uint16_t* p_id = r.ebx;
    ServerType s_id = r.ecx;
    p_id = tm_get_server_id(s_id);
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
void sys_d_copy_buffer(GeneralRegisters r, uint32_t eip, uint32_t esp)
{
    if (tm_get_current_task() != tm_get_server_id(SV_DISPLAY))
        return;

    char* data = r.ebx;
    for (uint32_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++){
        uint32_t x = i % VGA_WIDTH;
        uint32_t y = (i - x) % VGA_WIDTH;
        terminal_put_char_at(x, y, data[i]);
    }
}
void sys_d_put_pixel(GeneralRegisters r, uint32_t eip, uint32_t esp)
{
    if (tm_get_current_task() != tm_get_server_id(SV_DISPLAY))
        return;

    uint32_t x = r.ebx;
    uint32_t y = r.ecx;
    uint32_t data = r.edx;

    terminal_put_char_at(x, y, data);
}
void sys_d_put_pixels(GeneralRegisters r, uint32_t eip, uint32_t esp)
{
//    if (tm_get_current_task() != tm_get_server_id(SV_DISPLAY))
//        return;

    uint32_t x = r.ebx;
    uint32_t y = r.ecx;
    char* data = r.edx;
    uint32_t length = r.esi;

    for (uint32_t i = 0; i < length; i++)
    {
        uint32_t o_x = x + i % VGA_WIDTH;
        uint32_t o_y = y + ((i - o_x) % VGA_WIDTH);
        terminal_put_char_at(o_x, o_y, data[i]);
    }
}

void (*syscalls[])(GeneralRegisters, uint32_t, uint32_t) =
{
    sys_exit,
    sys_yield,
    sys_create_process,
    sys_get_server_id,
    sys_msg_send,
    sys_msg_receive,
    sys_msg_ack,
    sys_d_copy_buffer,
    sys_d_put_pixel,
    sys_d_put_pixels,
};

void sys_call(GeneralRegisters r, uint32_t eip, uint16_t cs, uint32_t f, uint32_t esp, uint16_t ss)
{
//    terminal_hex(r.eax);
    uint32_t index = r.eax;
    (*syscalls[index])(r, eip, esp);
}
