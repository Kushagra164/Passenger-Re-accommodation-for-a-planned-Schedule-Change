#pragma once
#include<string>
#include<tuple>
#include<cassert>
#include "date.h"
#include "time.h"
using namespace std;

class DateTime {
public:
    Date date;
    Time time;
    DateTime() { }
    DateTime(Date _date,Time _time) : date(_date), time(_time) { assert(time.days == 0); }

    bool operator==(DateTime const& rhs) const {
        return to_tuple() == rhs.to_tuple();
    }
    bool operator!=(DateTime const& rhs) const {
        return to_tuple() != rhs.to_tuple();
    }
    bool operator<(DateTime const& rhs) const {
        return to_tuple() < rhs.to_tuple();
    }
    bool operator<=(DateTime const& rhs) const {
        return to_tuple() <= rhs.to_tuple();
    }
    bool operator>(DateTime const& rhs) const {
        return to_tuple() > rhs.to_tuple();
    }
    bool operator>=(DateTime const& rhs) const {
        return to_tuple() >= rhs.to_tuple();
    }
    DateTime& operator-=(const Time &subTime) {
        time -= subTime;
        date += time.days;
        time.days = 0;
        return *this;
    }
    DateTime& operator+=(const Time &addTime) {
        time += addTime;
        date += time.days;
        time.days = 0;
        return *this;
    }
    Time operator-(DateTime rhs) const{ return Time((*this).time) += Time(0, 0, Date((*this).date) - rhs.date) - rhs.time; }
    DateTime operator-(Time time) const{ return DateTime(*this) -= time; }
    DateTime operator+(Time time) const{ return DateTime(*this) += time; }
    tuple<int, int, int, int, int> to_tuple() const { return {date.year, date.month, date.day, time.hour, time.minute}; }
    string to_string(){ return date.to_string() + " " + time.to_string(); }
};

