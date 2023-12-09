#include<vector>
#include<string>
#include<cassert>
using namespace std;

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

Date operator+(Date date, int days){ return date += days; }
Date operator-(Date date, int days){ return date -= days; }