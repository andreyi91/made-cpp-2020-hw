#pragma once
#include <iostream>
#include <cstdint>

namespace task
{

	// Your code here...

	using std::cin;
	using std::cout;
	using std::endl;
	using std::vector;

	vector<double> operator+(
		const vector<double> &a,
		const vector<double> &b)
	{
		vector<double> c;
		for (size_t i = 0; i < a.size(); ++i)
		{
			c.push_back(a[i] + b[i]);
		}
		return c;
	}

    vector<double> operator-(
            const vector<double> &a,
            const vector<double> &b)
    {
        vector<double> c;
        for (size_t i = 0; i < a.size(); ++i)
        {
            c.push_back(a[i] - b[i]);
        }
        return c;
    }

    vector<double> operator-(
            const vector<double> &a)
    {
        vector<double> c;
        for (size_t i = 0; i < a.size(); ++i)
        {
            c.push_back( - a[i]);
        }
        return c;
    }

    vector<double> operator+(
            const vector<double> &a)
    {
        vector<double> c;
        for (size_t i = 0; i < a.size(); ++i)
        {
            c.push_back(a[i]);
        }
        return c;
    }

    double operator*(
            const vector<double> &a,
            const vector<double> &b)
    {
        double c = 0;
        for (size_t i = 0; i < a.size(); ++i)
        {
            c += a[i] * b[i];
        }
        return c;
    }

    // cross product defined for 3-dimensional vectors
    vector<double> operator%(
            const vector<double> &a,
            const vector<double> &b)
    {
        vector<double> c;
        c.push_back(a[2] * b[1] - a[1] * b[2]);
        c.push_back(a[0] * b[2] - a[2] * b[0]);
        c.push_back(a[1] * b[0] - a[0] * b[1]);
        return c;
    }

    bool operator||(
            const vector<double> &a,
            const vector<double> &b)
    {
        const double EPS = 1e-8;
        double k = a[0] / b[0];
        for (int i = 0; i < a.size(); ++i)
        {
            if (fabs(a[i] / b[i] - k) > EPS)
            {
                return false;
            }
        }
        return true;
    }

    bool operator&&(
            const vector<double> &a,
            const vector<double> &b)
    {
        if ((a || b) && (a[0] * b[0] >= 0))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    std::istream& operator>>(
            std::istream &in,
            vector<double> &a)
    {
        int n;
        in >> n;
        double d;

        a.resize(n);
        for (size_t i = 0; i < n; ++i)
        {
            in >> d;
            a[i] = d;
        }
        return in;
    }

    std::ostream& operator<<(
            std::ostream &out,
            const vector<double> &a)
    {
        for (size_t i = 0; i < a.size(); ++i)
        {
            out << a[i] << " ";
        }
        out << endl;
        return out;
    }

    vector<double> reverse(
            vector<double> &a)
    {
	    double temp;
        for (size_t i = 0; i < a.size() / 2; ++i)
        {
            std::swap(a[i], a[a.size() - i - 1]);
        }
        return a;
    }

    vector<int> operator|(
            const vector<int> &a,
            const vector<int> &b)
    {
        vector<int> c;
        for (size_t i = 0; i < a.size(); ++i)
        {
            c.push_back(a[i] | b[i]);
        }
        return c;
    }

    vector<int> operator&(
            const vector<int> &a,
            const vector<int> &b)
    {
	    vector<int> c;
        for (size_t i = 0; i < a.size(); ++i)
        {
            c.push_back(a[i] & b[i]);
        }
        return c;
    }

} // namespace task
