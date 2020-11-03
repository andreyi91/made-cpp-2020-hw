#pragma once
#include <cstdint>
#include <iostream>

namespace task {

using std::endl;
using std::vector;

vector<double> operator+(const vector<double> &a, const vector<double> &b) {
  vector<double> c(a.size());
  for (size_t i = 0; i < a.size(); ++i) {
    c[i] = a[i] + b[i];
  }
  return c;
}

vector<double> operator-(const vector<double> &a, const vector<double> &b) {
  vector<double> c(a.size());
  for (size_t i = 0; i < a.size(); ++i) {
    c[i] = a[i] - b[i];
  }
  return c;
}

vector<double> operator-(const vector<double> &a) {
  vector<double> c(a.size());
  for (size_t i = 0; i < a.size(); ++i) {
    c[i] = -a[i];
  }
  return c;
}

vector<double> operator+(const vector<double> &a) {
  vector<double> c(a.size());
  for (size_t i = 0; i < a.size(); ++i) {
    c[i] = a[i];
  }
  return c;
}

double operator*(const vector<double> &a, const vector<double> &b) {
  double c = 0;
  for (size_t i = 0; i < a.size(); ++i) {
    c += a[i] * b[i];
  }
  return c;
}

// cross product defined for 3-dimensional vectors
vector<double> operator%(const vector<double> &a, const vector<double> &b) {
  vector<double> c(a.size());
  c[0] = a[2] * b[1] - a[1] * b[2];
  c[1] = a[0] * b[2] - a[2] * b[0];
  c[2] = a[1] * b[0] - a[0] * b[1];
  return c;
}

bool operator||(const vector<double> &a, const vector<double> &b) {
  const double EPS = 1e-8;
  double k;
  for (int i = 0; i < a.size(); ++i) {
    if (b[i] != 0) {
      k = a[i] / b[i];
      break;
    }
  }
  for (int i = 0; i < a.size(); ++i) {
    if (fabs(b[i] * k - a[i]) > EPS) {
      return false;
    }
  }
  return true;
}

bool operator&&(const vector<double> &a, const vector<double> &b) {
  if ((a || b) && (a[0] * b[0] >= 0)) {
    return true;
  } else {
    return false;
  }
}

std::istream &operator>>(std::istream &in, vector<double> &a) {
  int n;
  in >> n;
  double d;

  a.resize(n);
  for (size_t i = 0; i < n; ++i) {
    in >> d;
    a[i] = d;
  }
  return in;
}

std::ostream &operator<<(std::ostream &out, const vector<double> &a) {
  for (size_t i = 0; i < a.size(); ++i) {
    out << a[i] << " ";
  }
  out << endl;
  return out;
}

vector<double> reverse(vector<double> &a) {
  for (size_t i = 0; i < a.size() / 2; ++i) {
    std::swap(a[i], a[a.size() - i - 1]);
  }
  return a;
}

vector<int> operator|(const vector<int> &a, const vector<int> &b) {
  vector<int> c(a.size());
  for (size_t i = 0; i < a.size(); ++i) {
    c[i] = a[i] | b[i];
  }
  return c;
}

vector<int> operator&(const vector<int> &a, const vector<int> &b) {
  vector<int> c(a.size());
  for (size_t i = 0; i < a.size(); ++i) {
    c[i] = a[i] & b[i];
  }
  return c;
}

}  // namespace task
