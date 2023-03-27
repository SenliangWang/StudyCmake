#include <iostream>
#include "man.h"

int main(int argc, char *argv[])
{
    man  m(3);
	std::cout<<m.getAge();
    return 0;
}