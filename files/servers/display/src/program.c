#include "libos/libos.h"

// the id of the task controlling the display
uint16_t current_task;

static uint8_t display_buffer[80*25];

int main()
{
    const char* asdf = "asdfasdfasdf\n";
    const char* asdf2 = "cvbcvb\n";

    d_put_pixels(0, 0, asdf, 13);
    d_put_pixels(0, 0, asdf2, 7);
    get_server_id(0x5678, 0x1234);
//    get_server_id(&current_task, SV_DISPLAY);

//    while (1)
//    {
//        uint8_t* data;
//        uint32_t len;
//
//        msg_get(&data, &len);
//
//        if (len != 0)
//        {
//            if (data[0] == current_task)
//            {
//                MessageID code = data[1];
//                if (code == MID_D_PUT_PIXEL)
//                    d_put_pixel(data[2], data[3], data[4]);
//                else if (code == MID_D_PUT_PIXELS)
//                    d_put_pixels(data[2], data[3], data[4], data[len-1]);
//            }
//        }
//
//        msg_ack();
//    }
}
