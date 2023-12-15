#pragma once
#include <iostream>
#include <string>
#include <map>
#include "../Utils/constants.h"
using namespace std;

class CityPair
{
public:
    string srcCity, destCity;
    CityPair(string _src, string _dest) : srcCity(_src), destCity(_dest) {}
    CityPair() {}
    CityPair &operator=(const CityPair &other)
    {
        if (other.srcCity.empty())
            return *this;
        if (srcCity.empty())
        {
            if ((other.srcCity.length() != CITY_CODE_LENGTH) || 
                (other.destCity.length() != CITY_CODE_LENGTH))
            {
                cout << "Inconsistent city code length" << endl;
                cout << (srcCity) << " " << (destCity) << " " << (other.srcCity) << " " << (other.destCity) << endl;
            }
            srcCity = other.srcCity;
            destCity = other.destCity;
        }
        else
        {
            if ((srcCity != other.srcCity) || (destCity != other.destCity))
            {
                cout << "Inconsistency in flight numbers" << endl;
            }
        }
        return *this;
    }
    bool operator<(const CityPair &other) const
    {
        return make_pair(srcCity, destCity) < make_pair(other.srcCity, other.destCity);
    }
};
map<int, vector<CityPair>> flightNumberMap;