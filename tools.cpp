#include "tools.h"



int Tools::intRound(double x)
{
    return (x < 0) ? int(x - 0.5) : int(x + 0.5);
}



double Tools::randDouble(double min, double max)
{
    return min + static_cast <double> (rand()) / ( static_cast <double> (RAND_MAX/(max - min)));
}

std::string Tools::convertToString(int i)
{
    std::string s;
    std::stringstream out;
    out << i;
    s = out.str();
    return s;
}
