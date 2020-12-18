# include "position.h"
# include "mover.h"

int main()
{
    try
    {
    Position P(true);

    uint j=0;
    while (j<100)
    {
        Mover M(&P);
        std::vector<Move> legalMoves = M.getlegalMoves();
        if (legalMoves.empty()) break;
        Move m=legalMoves.front();
        std::cout << P.getMoveNumber() << ". " << m << std::endl;
        P = M.applyKCMove(m);
        ++j;
    }

    std::cout << P.printFENString() << std::endl;

    }
    catch (char const *errorMessage)
    {
        std::cout << "CAUGHT EXCEPTION: " << std::endl;
        std::cout << errorMessage << std::endl;
    }


    return 0;
}
