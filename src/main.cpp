# include "position.h"

int main()
{
    Position P1;
    std::cout << P1 << std::endl;
    std::cout << std::endl;
    std::cout << "Material count: " << P1.materialCount() << std::endl;
    std::cout << std::endl;
    std::cout << "FEN notation: " << P1.exportFENString() << std::endl;

    Position P2;
    P2.setFromFENString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    std::cout << "FEN notation: " << P2.exportFENString() << std::endl;
    std::cout << "P1 = P2 ? " << ((P1 == P2) ? "True" : "False") << std::endl;

    return 0;
}
