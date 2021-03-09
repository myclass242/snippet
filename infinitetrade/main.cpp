#include "test.h"
#include <iostream>


int main(void)
{
    testFindAllCycles1();
    std::cout << "-----------------------------------------" << std::endl;
    testFindAllCycles2();
    std::cout << "-----------------------------------------" << std::endl;
    testFindAllTrades();
    std::cout << "-----------------------------------------" << std::endl;
    testFindAllTrades2();
    std::cout << "-----------------------------------------" << std::endl;
    testFindAllTrads3();
    std::cout << "-----------------------------------------" << std::endl;
    testFindAllTrads_configFile();

    return 0;
}