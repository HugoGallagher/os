#include "lib.h"

int main()
{
    print("hello from 1!\n", 14);
    char* message = "program 1 says hello!";
    msg_send(1, message, 21);
}
