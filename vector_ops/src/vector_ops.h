#pragma once
#include <vector>
#include <iostream>
#include <cstdint>

using std::cin;
using std::cout;
using std::endl;
using std::vector;


namespace task {

// Your code here...

vector<int> scan_vec () {
	size_t N;
	cin >> N;

	vector<int> arr;

	for (size_t i = 0; i < N; ++ i) {
		int x;
		cin >> x;
		arr.push_back(x);
	}
	return arr;

}

void print_vec (vector<int> x) {
	for (size_t i = 0; i < x.size(); ++ i) {
		cout << x[i] << " ";
	}

	cout << endl;
}

int main() {
	vector<int> arr = scan_vec();
	print_vec(arr);
	//arr.push_back(123);
	//arr.push_back(456);
	//arr.pop_back();


	return 0;
}


}  // namespace task
