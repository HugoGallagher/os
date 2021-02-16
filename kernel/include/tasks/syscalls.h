#pragma once

#include <stdint.h>

#include "interrupts/idt.h"

void sys_exit(GeneralRegisters r);
void sys_yield(GeneralRegisters r);
void sys_msg_send(GeneralRegisters r);
void sys_msg_receive(GeneralRegisters r);
void sys_print(GeneralRegisters r);

void sys_call(GeneralRegisters r);
