#include<string>
#include<vector>
using namespace std;

template <class T> constexpr T floor(const T x, const T y) { T q = x / y, r = x % y; return q - ((x ^ y) < 0 and (r != 0)); }

class Time{
public:
    int hour, minute;
    int days = 0;
    Time () {}
    
    //HH:MM
    Time (string s){
        vector<string> time = { "" };
        for (int x = 0; x < (int) s.size(); x++){
            (s[x] == ':' or s[x] == '-') ? time.push_back("") : time.back().push_back(s[x]);
        }
        hour = std::stoi(time[0]);
        minute = std::stoi(time[1]);
    }

    Time(int h, int m = 0, int d = 0): hour(h), minute(m), days(d) {
        hour += minute / 60;
        minute %= 60;
        days += hour / 24;
        hour %= 24;
    }

    // _day? time in day: time in hours
    double value(bool _day = false){ 
        return _day ? days + ((hour + (minute / 60.)) / 24.): days * 24 + hour + (minute / 60.); 
    }

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

        days = floor(value, 24 * 60ll);
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

        days = floor(value, 24 * 60ll);
        value = value - days * 24 * 60;
        hour = value / 60;
        value %= 60;
        minute = value;
        return *this;
    }
    Time operator *=(int rhs) {
        Time tmp = *this;
        while (--rhs) *this += tmp;
        return *this;

    }
    Time operator-(Time rhs) const{ return Time(*this) -= rhs; }
    Time operator+(Time rhs) const{ return Time(*this) += rhs; }
    Time operator*(int rhs) const{ return Time(*this) *= rhs; }
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

Time abs(Time t){ return t < Time(0, 0) ? Time(0, 0) - t: t; }