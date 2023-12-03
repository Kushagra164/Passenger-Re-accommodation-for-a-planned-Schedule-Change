#include<bits/stdc++.h>
#include <x86intrin.h>

template <class T> constexpr T floor(const T x, const T y) { T q = x / y, r = x % y; return q - ((x ^ y) < 0 and (r != 0)); }

class Date {
public:
    static constexpr int month_days[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int year, month, day;
    Date () {}
    Date(int d, int m, int y) : year(y), month(m), day(d) {}
    Date(string s){
        vector<string> date = { "" };
        for (int x = 0; x < (int) s.size(); x++){
            (s[x] == '/' or s[x] == '-') ? date.push_back("") : date.back().push_back(s[x]);
        }
        assert((int) date.size() == 3);
        if ((int) date[2].size() == 2) date[2] = "20" + date[2];
        month = stoi(date[0]);
        day = stoi(date[1]);
        year = stoi(date[2]);
    }
    Date(int x){
        year = x * 400 / 146097 + 1;
        day = x - Date(1, 1, year).to_int();
        month = 1;
        while (day >= 28) {
            int k = month_days[month] + (month == 2 && is_leap_year(year) ? 1 : 0);
            if (day < k) break;
            ++month;
            day -= k;
        }
        if (month == 13) {
            ++year;
            month = 1;
        }
        ++day;
    }

    int to_int() {
        int y = (month <= 2 ? year - 1 : year);
        int m = (month <= 2 ? month + 12 : month);
        int d = day;
        return 365 * y + y / 4 - y / 100 + y / 400 + 306 * (m + 1) / 10 + d - 429;
    }

    int weekday() { return (to_int() + 1) % 7; }

    Date& operator++() {
        ++day;
        int lim = month_days[month];
        if (is_leap_year(year) && month == 2) lim = 29;
        if (day <= lim) return (*this);
        day = 1;
        ++month;
        if (month == 13) {
            ++year;
            month = 1;
        }
        return (*this);
    }
    Date operator++(int) {
        Date tmp = *this;
        ++*this;
        return tmp;
    }

    bool operator==(Date const& rhs) const {
        return to_tuple() == rhs.to_tuple();
    }
    bool operator!=(Date const& rhs) const {
        return to_tuple() != rhs.to_tuple();
    }
    bool operator<(Date const& rhs) const {
        return to_tuple() < rhs.to_tuple();
    }
    bool operator<=(Date const& rhs) const {
        return to_tuple() <= rhs.to_tuple();
    }
    bool operator>(Date const& rhs) const {
        return to_tuple() > rhs.to_tuple();
    }
    bool operator>=(Date const& rhs) const {
        return to_tuple() >= rhs.to_tuple();
    }
    Date& operator +=(const int& addDays){
        return *this = Date(to_int() + addDays);
    }
    Date& operator -=(const int& subDays){
        return *this = Date(to_int() - subDays);
    }

    int operator -(Date rhs) { return to_int() - rhs.to_int(); }
    // dd[sep]mm[sep]yyyy
    string to_string(string sep = "/") {
        string y = std::to_string(year);
        string m = std::to_string(month);
        string d = std::to_string(day);
        while ((int) y.size() < 4) y = "0" + y;
        while ((int) m.size() < 2) m = "0" + m;
        while ((int) d.size() < 2) d = "0" + d;
        return d + sep + m + sep + y;
    }

    tuple<int, int, int> to_tuple() const { return {year, month, day}; }

    static bool is_leap_year(int y) {
        if (y % 400 == 0) return true;
        return (y % 4 == 0 && y % 100 != 0);
    }
    static bool is_valid_date(int y, int m, int d) {
        if (!(1 <= m && m <= 12)) return 0;
        int mx = month_days[m];
        if (m == 2 && is_leap_year(y)) ++mx;
        return (1 <= d && d <= mx);
    }
};

class Time{
public:
    int hour, minute;
    int days = 0;
    Time () {}
    Time (string s){
        vector<string> time = { "" };
        for (int x = 0; x < (int) s.size(); x++){
            (s[x] == ':' or s[x] == '-') ? time.push_back("") : time.back().push_back(s[x]);
        }
        assert((int) time.size() == 2);
        hour = std::stoi(time[0]);
        minute = std::stoi(time[1]);
    }
    Time(int h, int m = 0, int d = 0): hour(h), minute(m), days(d) {
        hour += minute / 60;
        minute %= 60;
        days += hour / 24;
        hour %= 24;
    }
    // value of time in hours;
    double value(bool _day = false){ return _day ? days + ((hour + (minute / 60.)) / 24.): days * 24 + hour + (minute / 60.); }

    bool operator==(Time const& rhs) const {
        return to_tuple() == rhs.to_tuple();
    }
    bool operator!=(Time const& rhs) const {
        return to_tuple() != rhs.to_tuple();
    }
    bool operator<(Time const& rhs) const {
        return to_tuple() < rhs.to_tuple();
    }
    bool operator<=(Time const& rhs) const {
        return to_tuple() <= rhs.to_tuple();
    }
    bool operator>(Time const& rhs) const {
        return to_tuple() > rhs.to_tuple();
    }
    bool operator>=(Time const& rhs) const {
        return to_tuple() >= rhs.to_tuple();
    }
    Time& operator-=(Time const& rhs) {
        int dy = days - rhs.days;
        int hr = hour - rhs.hour;
        int mn = minute - rhs.minute;
        long long value = dy * 24 * 60 + hr * 60 + mn;

        days = floor(value, 24 * 60LL);
        value = value - days * 24 * 60;
        hour = value / 60;
        value %= 60;
        minute = value;
        return *this;
    }
    Time& operator+=(Time const& rhs) {
        int dy = days + rhs.days;
        int hr = hour + rhs.hour;
        int mn = minute + rhs.minute;
        long long value = dy * 24 * 60 + hr * 60 + mn;

        days = floor(value, 24 * 60LL);
        value = value - days * 24 * 60;
        hour = value / 60;
        value %= 60;
        minute = value;
        return *this;
    }
    Time operator-(Time rhs) const{ return Time(*this) -= rhs; }
    Time operator+(Time rhs) const{ return Time(*this) += rhs; }
    tuple<int, int, int> to_tuple() const{ return make_tuple(days, hour, minute); }
    string to_string(bool with_days = false){
        string d = "";
        if (with_days) {
            d = std::to_string(days);
            d += ":";
        }
        string h = std::to_string(hour);
        string m = std::to_string(minute);
        while ((int) h.size() < 2) h = "0" + h;
        while ((int) m.size() < 2) m = "0" + m;
        return d + h + ":" + m;
    }
};

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

Date operator+(Date date, int days){ return date += days; }
Date operator-(Date date, int days){ return date -= days; }

