#pragma once
#include "cpu.h"
#include "idt.h"

void isr_pushed(struct pushed_values* val);

void irq_handler(struct pushed_values* val);


void isr_install(void);

void exception_handler(void);

