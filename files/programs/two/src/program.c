#include "lib.h"

int main()
{
    print("hello from 2!\n", 14);
    uint8_t* data;
    uint32_t len;
    msg_get(&data, &len);
    print(data, len);
}