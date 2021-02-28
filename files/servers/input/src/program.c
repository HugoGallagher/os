#include "libos/libos.h"

static uint64_t keys;

int main()
{
    /*
    while (1)
    {
        uint8_t* data;
        uint32_t len;

        msg_get(&data, &len);

        if (len != 0)
        {
            uint8_t sc = data[1];
            if (sc & 0x80)
                keys |= 1 << sc;
            else
                keys &= ~(1 << sc);
        }

        msg_ack();
    }
    */
}
