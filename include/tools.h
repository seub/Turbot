#ifndef TOOLS_H
#define TOOLS_H


#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <memory>
#include <ctime>
#include <random>
#include <chrono>

#include "types.h"


namespace Tools
{

int intRound(double x);
double randDouble(double min, double max);
int stoi(const std::string& str, int* p_value, std::size_t* pos = 0, int base = 10);
void currentDate(uint &year, uint &month, uint &day);

template <typename T> T exponentiation(T base, int power)
{
    T res = 1;
    if (power >= 0)
    {
        T pow = base;
        while (power > 0)
        {
            if (power & 1)
            {
                res = res*pow;
            }
            pow = pow*pow;
            power >>= 1;
        }
    }
    else
    {
        throw("ERROR in Tools::exponentiation: no negative exponents!");
    }
    return res;
}

std::string convertToString(int i);

template <typename T> std::vector<uint> findInList(const T &x, const std::vector<T> &V)
{
    std::vector<uint> outputIndices;
    for (uint i=0; i<V.size(); i++)
    {
        if (V[i] == x)
        {
            outputIndices.push_back(i);
        }
    }
    return outputIndices;
}

template <typename T> bool isInList(const T &x, const std::vector<T> &V)
{
    for (const auto &v : V)
    {
        if (v == x)
        {
            return true;
        }
    }
    return false;
}

template <typename T> bool containsDuplicates(const std::vector<T> & V)
{
    uint i,j, N=V.size();
    for (i=0; i!=N-1; ++i)
    {
        for (j=i+1; j!=N; ++j)
        {
            if (V[i] == V[j])
            {
                return true;
            }
        }
    }
    return false;
}

template <typename T> bool containsTriplicates(const std::vector<T> & V)
{
    uint i, j, count, N=V.size();

    for (i=0; i!=N; ++i)
    {
        count = 1;
        for (j=0; j!=i; ++j)
        {
            if (V[j] == V[i])
            {
                ++count;
                if (count==3) {return true;}
            }
        }
    }
    return false;
}


template <typename T> bool haveCommonElements(const std::vector<T> & V1, const std::vector<T> & V2)
{
    uint i, j, N1 = V1.size(), N2 = V2.size();
    for (i=0; i!=N1; i++)
    {
        for (j=0; j!=N2; j++)
        {
            if (V1[i] == V2[j])
            {
                return true;
            }
        }
    }
    return false;
}

template <typename T> int sign(T val)
{
    return (T(0) < val) - (val < T(0));
}

template<typename T> void pop_front(std::vector<T> &vector)
{
    assert(!vector.empty());
    vector.erase(vector.begin());
}

}

template<typename T> std::ostream & operator<<(std::ostream &out, const std::vector<T> &V)
{
    out << "[";
    for (uint i = 0; i+1 < V.size(); ++i)
    {
        out << V[i] << ", ";
    }
    if (!V.empty())
    {
        out << V.back();
    }
    out << "]";
    return out;
}

template<typename T> std::vector<T> operator+(std::vector<T> V, const T &x)
{
    for (auto &v : V)
    {
        v += x;
    }
    return V;
}

template<typename T, typename F> std::vector<T> operator*(const F & f, std::vector<T> V)
{
    for (auto &v : V)
    {
        v = f*v;
    }
    return V;
}



#endif // TOOLS_H
