# include "position.h"
# include "mover.h"

int main()
{
    try
    {
        Position P(true);

        uint j=0;
        bool white=true;
        bool test = true;
        while (j<160)
        {
            Mover M(&P);
            std::vector<Move> legalMoves = M.getlegalMoves();
            if (legalMoves.empty()) {std::cout << "break" << std::endl; break;}
            Move m=legalMoves[(uint (clock())) % legalMoves.size()];
            MovePGN movepgn(m, &M);
            if (white) {std::cout << P.getMoveNumber() << ". " << movepgn;}
            else{std::cout << " " << movepgn << " ";}

            MovePGN movepgn2;
            test &= MovePGN::fromPGN(movepgn2, movepgn.toPGN(), &M);
            test &= (movepgn==movepgn2);

            P = M.applyKCMove(m);
            ++j;
            white = !white;
        }
        std::cout << std::endl;

        std::cout << "test = " << test << std::endl;

        std::cout << P.toFENstring() << std::endl;
        std::cout << "Check position on Lichess: " << P.toLichessURL() << std::endl;

    }
    catch (char const *errorMessage)
    {
        std::cout << "CAUGHT EXCEPTION: " << std::endl;
        std::cout << errorMessage << std::endl;
    }


    return 0;
}
