#include <iostream>
#include <random>

int main(int arg, char **args)
{
    std::cout << std::random_device()() << std::endl;
}