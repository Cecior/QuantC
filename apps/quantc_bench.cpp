#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "QuantC bench: received" << argc - 1 << " argument(s)\n";

    for (int i = 1; i < argc; i++)
    {
        std::cout << "argv[" << i << "] = " << argv[i] << "\n";
    }
    return 0;
}