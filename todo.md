To do

Questions:
    Sur la class position, il y a une fonction reset. Est ce qu'on ne veut pas plutot une class immutable?

Proposition de classes:

Position:
    - generatelegalmoves() -> vector< move >
    - applymove(move) -> position
    - islegal(move) -> bool
    - ismate() -> bool
    - ispat() -> bool
    - isdraw() -> bool (avec le probleme de du draw par repetition)
    - fromFEM(FENfile) -> bool (valid or not)
    - static newboard() -> position

Positionevaluator:
    -virtual evaluate(Position)

BasicEvaluator(Positionevaluator):
    piecevalues

MoveGenerator:
    Returns a move. Has the job of deciding how long it should think.
        -Maybe the time decision is decoupled method or not (this move is the best lets not lose time)
    Probably has some cached memory not to explore already explored.
        -Further down the line -> cache manager, pruning
    methods 
        -virtual proposemove(position,timeRules) -> move

BranchComparator:
    Tells the MinMax move generator which branch is more interesting
    operator < (Position*, Position*) -> bool
    *Can have a bunch of additional parameters:
        - Evaluation
        - depth of position in the three


MoveGeneratorbyMinMax(MoveGenerator):
    - takes an evaluator as parameter:
    - takes a branchComparator as as parameter
    
Remarque :
Quand on voudra optimiser, quand on calcule l'arbre des coups, je pense que ça vaut le coup de passer en "kc chess" (king capture allowed)
