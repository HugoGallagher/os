#include "tasks/tasks.h"

#include "tasks/tss.h"
#include "lib/mem.h"

void tm_init(TaskManager* tm, uint16_t c)
{
    tm->tss = kmalloc(sizeof(TSS));
    bzero(tm->tss, sizeof(TSS));
    tm->tss->ss0 = 0x10;

    tm->count = c;
    tm->tasks = kmalloc(sizeof(Task) * c);
};
