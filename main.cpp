# include "position.h"

int main()
{
    Position P;
    std::cout << P.printString() << std::endl;
    std::cout << std::endl;
    std::cout << "Material count:" << P.materialCount() << std::endl;
    std::cout << std::endl;
    std::cout << P.printFENString() << std::endl;

    return 0;
}
