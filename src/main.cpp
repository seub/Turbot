# include "position.h"
# include "mover.h"

int main()
{
    try
    {
        uint j=0, k=0;
        uint kmax=1; //kmax=10000;
        bool test = true, test1, test2, test3;
        while(k<kmax && test)
        {
            Position P(true);
            j=0;
            bool white=true;
            while (j<200)
            {
                Mover M(&P);
                std::vector<Move> legalMoves = M.getlegalMoves();
                if (legalMoves.empty()) {break;}
                Move m=legalMoves[(uint (clock())) % legalMoves.size()];
                MovePGN movepgn(m, &M);
                if (white) {std::cout << P.getMoveNumber() << ". " << movepgn;}
                else{std::cout << " " << movepgn << " ";}

                MovePGN movepgn2;
                test1 = MovePGN::fromPGN(movepgn2, movepgn.toPGN(), &M);
                test2 = (movepgn==movepgn2);
                test3 = (((Move) movepgn2)==m);
                test = test1 && test2 && test3;
                if (!test)
                {
                    std::cout << "Test failed!" << std::endl;
                    std::cout << "The position was " << P.toFENstring() << std::endl;
                    std::cout << "The move played was " << m << std::endl;
                    std::cout << "Its PGN move conversion was " << Move(movepgn.getOrigin(),movepgn.getTarget()) << std::endl;
                    std::cout << "Its PGN string was " << movepgn << std::endl;
                    std::cout << "Its conversion back to a PGN move was a success? " << ((test1) ? "Yes" : "No") << std::endl;
                    std::cout << "Its conversion back to a PGN move was " << movepgn2 << std::endl;
                    std::cout << "The comparison between the first PGN conversion and the conversion back from PGN was a success? " << ((test2) ? "Yes" : "No") << std::endl;
                    throw("test failed");
                }

                P = M.applyKCMove(m);
                ++j;
                white = !white;
            }
            std::cout << std::endl;
            std::cout << P.toFENstring() << std::endl;
            std::cout << "Check position on Lichess: " << P.toLichessURL() << std::endl;
            ++k;
        }
        std::cout << "success? " << ((k==kmax) ? "Yes" : "No") << std::endl;

    }
    catch (char const *errorMessage)
    {
        std::cout << "CAUGHT EXCEPTION: " << std::endl;
        std::cout << errorMessage << std::endl;
    }


    return 0;
}
