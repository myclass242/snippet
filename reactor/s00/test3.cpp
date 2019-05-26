//
// Created by zy on 5/26/19.
//

#include "Eventloop.h"

int main(void)
{
    muduo::Eventloop loop1;
    loop1.loop();

    muduo::Eventloop loop2;
    loop2.loop();

    return 0;
}
