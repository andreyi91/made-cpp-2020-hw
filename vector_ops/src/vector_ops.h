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

	vector<int> operator+(
		const vector<int> &a,
		const vector<int> &b)
	{
		vector<int> c;

		for (size_t i = 0; i < a.size(); ++i)
		{
			c.push_back(a[i] + b[i]);
		}

		return c;
	}

} // namespace task
