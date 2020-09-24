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

    vector<double> operator+(           //??? should it perform type conversion?
            const vector<double> &a)
    {
        vector<double> c;
        for (size_t i = 0; i < a.size(); ++i)
        {
            c.push_back(a[i]);
        }
        return c;
    }

    vector<double> operator*(
            const vector<double> &a,
            const vector<double> &b)
    {
        vector<double> c;
        for (size_t i = 0; i < a.size(); ++i)
        {
            c.push_back(a[i] * b[i]);
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
        vector<double> c;
        bool collin = true;

        c = a % b;
        for (size_t i = 0; i < c.size(); ++i)
        {
            if ( c[i] != 0 )
            {
                collin = false;
            }
        }
        return collin;
    }

    bool operator&&(
            const vector<double> &a,
            const vector<double> &b)
    {
        vector<double> c;
        bool codir = false;
        if (a || b)
        {
            codir = (a[0] / b[0]) < 0;      // may be check for b[0] != 0
        }
        return codir;
    }


} // namespace task
