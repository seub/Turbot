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

std::string Tools::convertDoubleToString(const double &x)
{
    std::string s;
    std::stringstream out;
    out << x;
    s = out.str();
    return s;
}

int Tools::stoi(const std::string& str, int* p_value, std::size_t* pos, int base)
{
    // wrapping std::stoi because it may throw an exception

    try {
        *p_value = std::stoi(str, pos, base);
        return 0;
    }

    catch (const std::invalid_argument& ia) {
        //std::cerr << "Invalid argument: " << ia.what() << std::endl;
        return -1;
    }

    catch (const std::out_of_range& oor) {
        //std::cerr << "Out of Range error: " << oor.what() << std::endl;
        return -2;
    }

    catch (const std::exception& e)
    {
        //std::cerr << "Undefined error: " << e.what() << std::endl;
        return -3;
    }
}

std::string Tools::currentDate()
{
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y.%m.%d");
        return oss.str();
}

std::string Tools::currentTime()
{
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%H:%M:%S");
        return oss.str();
}
