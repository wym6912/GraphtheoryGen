#include <bits/stdc++.h>

using namespace std;

const double pi = acos(-1.0);

template <typename T>
inline void __assert__(const T &condition, const string &message)
{
    if (! condition)
    {
        std :: cerr << message << std :: endl;
        exit(2);
    }
}

inline double __cos__(const double &f)
{
	return cos(f / 180.0 * pi);
}

inline double __sin__(const double &f)
{
	return sin(f / 180.0 * pi);
}

template <typename T>
inline string EatBread(const T &bread)
{
	stringstream ss;
	string res;
	ss << bread;
	ss >> res;
	return res;
}
