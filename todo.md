TODO

BUGS:
I fixed a bug but the program still crashes sometimes, need to figure out the issue and solve it.


Question :
    Sur la class position, il y a une fonction reset. Est ce qu'on ne veut pas plutot une class immutable?
Réponse :
    Je crois que y a pas vraiment de classe "immutable", mais on peut faire en sorte que 1. toutes les méthodes publiques de la classe sont const, et 2. on peut utiliser
    le mot-clé const quand on définit un élément de la classe.





Proposition de classes:

Positionevaluator:
    (classe virtuelle)

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

Idée :
claim draw, resign, offer draw ne drevraient pas faire partie de Move, c'est une action séparée. Elle fera partie d'une classe "Game"!
