#include "libos/libos.h"

int main()
{
    uint16_t d_s;
    get_server_id(&d_s, SV_DISPLAY);

    char text[] = { MID_D_PUT_PIXELS, 'h', 'e', 'l', 'l', 'o', '!', '\n'};
    uint32_t len = 8;
    msg_send(d_s, text, len);

//    while (1)
//    {
//        print("hello from 1!\n", 1);
//    }
}
