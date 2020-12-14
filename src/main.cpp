# include "position.h"

int main()
{
    Position P1(true);
    std::cout << P1 << std::endl;
    std::cout << std::endl;
    std::cout << "FEN notation: " << P1.printFENString() << std::endl;

    Position P2("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    std::cout << "FEN notation: " << P2.printFENString() << std::endl;
    std::cout << "P1 = P2 ? " << ((P1 == P2) ? "True" : "False") << std::endl;

    return 0;
}
