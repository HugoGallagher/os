#pragma once

#include <stdint.h>

#include "interrupts/idt.h"

void sys_exit(GeneralRegisters r, uint32_t eip, uint32_t esp);
void sys_yield(GeneralRegisters r, uint32_t eip, uint32_t esp);
void sys_msg_send(GeneralRegisters r, uint32_t eip, uint32_t esp);
void sys_msg_receive(GeneralRegisters r, uint32_t eip, uint32_t esp);
void sys_print(GeneralRegisters r, uint32_t eip, uint32_t esp);

void sys_call(GeneralRegisters r, uint32_t eip, uint16_t cs, uint32_t f, uint32_t esp, uint16_t ss);
