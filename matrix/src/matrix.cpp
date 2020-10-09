#include "matrix.h"

using namespace task;

// Your code goes here...

Matrix::Matrix() : rows(1), cols(1) {
  this->matr = new double *[this->rows];
  this->matr[0] = new double[this->cols];
  this->matr[0][0] = 1.;
};

Matrix::Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
  this->matr = new double *[rows];
  for (size_t i = 0; i < rows; ++i) {
    this->matr[i] = new double[cols];
    for (size_t j = 0; j < cols; ++j) {
      if (i == j) {
        this->matr[i][j] = 1.;
      } else {
        this->matr[i][j] = 0.;
      }
    }
  }
};

Matrix::Matrix(const Matrix &copy) : rows(copy.rows), cols(copy.cols) {
  this->matr = new double *[this->rows];
  for (size_t i = 0; i < this->rows; ++i) {
    this->matr[i] = new double[this->cols];
    for (size_t j = 0; j < this->cols; ++j) {
      this->matr[i][j] = copy.matr[i][j];
    }
  }
};

Matrix::~Matrix() {
  if (matr != NULL) {
    for (size_t i = 0; i < this->rows; ++i) {
      delete[] matr[i];
    }
    delete[] matr;
  }
};

Matrix &Matrix::operator=(const Matrix &a) {
  if (&a == this) {
    return *this;
  }
  if (this->rows != a.rows || this->cols != a.cols) {
    for (size_t i = 0; i < this->rows; ++i) {
      delete[] this->matr[i];
    }
    delete[] this->matr;
    this->rows = a.rows;
    this->cols = a.cols;
    this->matr = new double *[this->rows];
    for (size_t i = 0; i < this->rows; ++i) {
      this->matr[i] = new double[this->cols];
    }
  }
  for (size_t i = 0; i < this->rows; ++i) {
    for (size_t j = 0; j < this->cols; ++j) {
      this->matr[i][j] = a.matr[i][j];
    }
  }
  return *this;
};

double &Matrix::get(size_t row, size_t col) {
  if (row < 0 || row >= this->rows || col < 0 || col >= this->cols) {
    throw OutOfBoundsException();
  }
  return this->matr[row][col];
};

const double &Matrix::get(size_t row, size_t col) const {
  if (row < 0 || row >= this->rows || col < 0 || col >= this->cols) {
    throw OutOfBoundsException();
  }
  return this->matr[row][col];
};

void Matrix::set(size_t row, size_t col, const double &value) {
  if (row < 0 || row >= this->rows || col < 0 || col >= this->cols) {
    throw OutOfBoundsException();
  }
  this->matr[row][col] = value;
};

void Matrix::resize(size_t new_rows, size_t new_cols) {
  if (new_rows < 0 || new_cols < 0) {
    throw OutOfBoundsException();
  }
  Matrix new_matr = Matrix(new_rows, new_cols);
  for (size_t k = 0; k < new_cols * new_rows; ++k) {
    if (k < this->rows * this->rows) {
      new_matr[k / new_cols][k % new_cols] =
          this->matr[k / this->cols][k % this->cols];
    } else {
      new_matr[k / new_cols][k % new_cols] = 0.;
    }
  }
  *this = new_matr;
};

double *Matrix::operator[](size_t row) {
  if (row >= 0 && row < this->rows) {
    return this->matr[row];
  } else {
    throw OutOfBoundsException();
  }
};

const double *Matrix::operator[](size_t row) const {
  if (row >= 0 && row < this->rows) {
    return this->matr[row];
  } else {
    throw OutOfBoundsException();
  }
};

Matrix &Matrix::operator+=(const Matrix &a) {
  if (this->rows != a.rows || this->cols != a.cols) {
    throw SizeMismatchException();
  }
  for (size_t i = 0; i < this->rows; ++i) {
    for (size_t j = 0; j < this->cols; ++j) {
      this->matr[i][j] = this->matr[i][j] + a.matr[i][j];
    }
  }
  return *this;
};

Matrix &Matrix::operator-=(const Matrix &a) {
  if (this->rows != a.rows || this->cols != a.cols) {
    throw SizeMismatchException();
  }
  for (size_t i = 0; i < this->rows; ++i) {
    for (size_t j = 0; j < this->cols; ++j) {
      this->matr[i][j] = this->matr[i][j] - a.matr[i][j];
    }
  }
  return *this;
};

Matrix &Matrix::operator*=(const Matrix &a) {
  if (this->cols != a.rows) {
    throw SizeMismatchException();
  }
  Matrix M = Matrix(this->rows, a.cols);
  double d;
  for (size_t i = 0; i < this->rows; ++i) {
    for (size_t j = 0; j < a.cols; ++j) {
      d = 0.;
      for (size_t k = 0; k < this->cols; ++k) {
        d += this->matr[i][k] * a.matr[k][j];
      }
      M[i][j] = d;
    }
  }
  *this = M;
  return *this;
};

Matrix &Matrix::operator*=(const double &number) {
  for (size_t i = 0; i < this->rows; ++i) {
    for (size_t j = 0; j < this->cols; ++j) {
      this->matr[i][j] = this->matr[i][j] * number;
    }
  }
  return *this;
};

Matrix Matrix::operator+(const Matrix &a) const {
  if (this->rows != a.rows || this->cols != a.cols) {
    throw SizeMismatchException();
  }
  Matrix curr = *this;
  return curr += a;
};

Matrix Matrix::operator-(const Matrix &a) const {
  if (this->rows != a.rows || this->cols != a.cols) {
    throw SizeMismatchException();
  }
  Matrix curr = *this;
  return curr -= a;
};

Matrix Matrix::operator*(const Matrix &a) const {
  if (this->cols != a.rows) {
    throw SizeMismatchException();
  }
  Matrix tmp = *this;
  return tmp *= a;
};

Matrix Matrix::operator*(const double &a) const {
  Matrix tmp = *this;
  return tmp *= a;
};

Matrix Matrix::operator-() const {
  Matrix tmp = *this;
  return tmp -= tmp * 2;
};

Matrix Matrix::operator+() const {
  Matrix tmp = *this;
  return tmp;
};

double Matrix::det() const {
  if (this->rows != this->cols) {
    throw SizeMismatchException();
  }
  if (this->rows == 1) {
    return this->matr[0][0];
  }
  if (this->rows == 2) {
    return this->matr[0][0] * this->matr[1][1] -
           this->matr[0][1] * this->matr[1][0];
  }
  double d = 0.;
  int f = 1;
  for (size_t i = 0; i < this->cols; ++i) {
    Matrix M = Matrix(this->rows - 1, this->cols - 1);
    for (size_t k = 0; k < this->rows - 1; ++k) {
      for (size_t j = 0; j < this->cols - 1; ++j) {
        if (j < i) {
          M.matr[k][j] = this->matr[k + 1][j];
        } else {
          M.matr[k][j] = this->matr[k + 1][j + 1];
        }
      }
    }
    d += f * this->matr[0][i] * M.det();
    f = -f;
  }
  return d;
};

Matrix Matrix::transposed() const {
  Matrix T = Matrix(this->cols, this->rows);
  for (size_t i = 0; i < this->rows; ++i) {
    for (size_t j = 0; j < this->cols; ++j) {
      T[j][i] = this->matr[i][j];
    }
  }
  return T;
};

void Matrix::transpose() { *this = this->transposed(); };

double Matrix::trace() const {
  if (this->rows != this->cols) {
    throw SizeMismatchException();
  }
  double t = 0.;
  for (size_t i = 0; i < this->rows; ++i) {
    t += this->matr[i][i];
  }
  return t;
};

std::vector<double> Matrix::getRow(size_t row) {
  if (row < 0 || row >= this->rows) {
    throw OutOfBoundsException();
  }
  std::vector<double> vec_row;
  for (size_t j = 0; j < this->cols; ++j) {
    vec_row.push_back(this->matr[row][j]);
  }
  return vec_row;
};

std::vector<double> Matrix::getColumn(size_t column) {
  if (column < 0 || column >= this->cols) {
    throw OutOfBoundsException();
  }
  std::vector<double> vec_col;
  for (size_t i = 0; i < this->rows; ++i) {
    vec_col.push_back(this->matr[i][column]);
  }
  return vec_col;
};

bool Matrix::operator==(const Matrix &a) const {
  for (size_t i = 0; i < this->rows; ++i) {
    for (size_t j = 0; j < this->cols; ++j) {
      double dif = this->matr[i][j] - a[i][j];
      if (dif > EPS || dif < -EPS) {
        return false;
      }
    }
  }
  return true;
};

bool Matrix::operator!=(const Matrix &a) const {
  for (size_t i = 0; i < this->rows; ++i) {
    for (size_t j = 0; j < this->cols; ++j) {
      double dif = this->matr[i][j] - a[i][j];
      if (dif > EPS || dif < -EPS) {
        return true;
      }
    }
  }
  return false;
};

size_t task::Matrix::ncols() const { return this->cols; };

size_t task::Matrix::nrows() const { return this->rows; };

Matrix task::operator*(const double &a, const Matrix &b) {
  Matrix M = b * a;
  return M;
};

std::ostream &task::operator<<(std::ostream &output, const Matrix &matrix) {
  for (size_t i = 0; i < matrix.nrows(); ++i) {
    for (size_t j = 0; j < matrix.ncols(); ++j) {
      output << matrix[i][j] << " ";
    }
    output << std::endl;
  }
  return output;
};

std::istream &task::operator>>(std::istream &input, Matrix &matrix) {
  size_t rows, cols;
  input >> rows >> cols;
  matrix = Matrix(rows, cols);
  double d;
  for (size_t i = 0; i < matrix.nrows(); ++i) {
    for (size_t j = 0; j < matrix.ncols(); ++j) {
      input >> d;
      matrix[i][j] = d;
    }
  }
  return input;
};