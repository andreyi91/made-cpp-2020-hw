#include "geometry.h"

Point::Point() = default;

Point::Point(double x, double y) : x(x), y(y) {}

Point::Point(const Point& a) : x(a.x), y(a.y) {}

Point::~Point() = default;

Point& Point::operator=(const Point& a) {
  if (&a == this) {
    return *this;
  }
  this->x = a.x;
  this->y = a.y;
  return *this;
}

bool Point::operator==(const Point& a) const {
  if (fabs(this->x - a.x) < EPS && fabs(this->y - a.y) < EPS) {
    return true;
  } else {
    return false;
  }
}

bool Point::operator!=(const Point& a) const { return !(*this == a); }

Line::Line() = default;

Line::Line(Point a, Point c) {
  if (fabs(a.x - c.x) < EPS) {
    this->k = K_MAX;
  } else {
    this->k = (c.y - a.y) / (c.x - a.x);
  }
  this->b = c.y - this->k * c.x;
}

Line::Line(double k, double b) : k(k), b(b) {}

Line::Line(Point& a, double k) : k(k), b(a.y - k * a.x) {}

Line::Line(const Line& l) : k(l.k), b(l.b) {}

Line::~Line() = default;

Line& Line::operator=(const Line& l) {
  if (&l == this) {
    return *this;
  }
  this->k = l.k;
  this->b = l.b;
  return *this;
}

bool Line::operator==(const Line& l) const {
  if (fabs(this->k - l.k) < EPS && fabs(this->b - l.b) < EPS) {
    return true;
  } else {
    return false;
  }
}

bool Line::operator!=(const Line& l) const { return !(*this == l); }

Shape::~Shape() = default;

Point Shape::seg_center(Point& a, Point& b) const {
  double xc = (a.x + b.x) / 2;
  double yc = (a.y + b.y) / 2;
  return Point(xc, yc);
}

double Shape::seg_length(Point& a, Point& b) const {
  return pow(pow((b.x - a.x), 2) + pow((b.y - a.y), 2), 0.5);
}

Line Shape::normal(Line& l, Point& c) const {
  double k;
  if (fabs(l.k) < EPS) {
    k = K_MAX;
  } else {
    k = -1 / l.k;
  }
  return Line(c, k);
}

Line Shape::normal(Point& a, Point& b, Point& c) const {
  Line l(a, b);
  return normal(l, c);
}

Point Shape::cross_point(Line& l1, Line& l2) const {
  if (l1.k == l2.k) {
    return Point();
  }
  double x, y;
  x = (l1.b - l2.b) / (l2.k - l1.k);
  y = l1.k * x + l1.b;
  return Point(x, y);
}

Point Shape::rotate_point(const Point c, double ang, const Point a) const {
  double x, y, phi;
  phi = ang * PI / 180;
  x = c.x + (a.x - c.x) * cos(phi) - (a.y - c.y) * sin(phi);
  y = c.y + (a.x - c.x) * sin(phi) + (a.y - c.y) * cos(phi);
  return Point(x, y);
}

Point Shape::scale_point(const Point c, double coef, const Point a) const {
  double x, y;
  x = coef * (a.x - c.x) + c.x;
  y = coef * (a.y - c.y) + c.y;
  return Point(x, y);
}

Point Shape::reflex_point(Line l, Point a) const {
  Line n = normal(l, a);
  Point c = cross_point(l, n);
  return scale_point(c, -1, a);
}

Polygon::Polygon() = default;

Polygon& Polygon::operator=(const Polygon& p) {
  if (&p == this) {
    return *this;
  }
  size_t size = p.verts.size();
  this->verts.resize(size);
  for (size_t i = 0; i < size; ++i) {
    this->verts[i] = p.verts[i];
  }
  return *this;
}

Polygon::Polygon(const Point points[]) {
  size_t size = sizeof(&points) / sizeof(points[0]);
  this->verts.resize(size);
  for (size_t i = 0; i < size; ++i) {
    this->verts[i] = points[i];
  }
}

Polygon::Polygon(const std::vector<Point>& points) {
  this->verts.resize(points.size());
  for (size_t i = 0; i < points.size(); ++i) {
    this->verts[i] = points[i];
  }
}

Polygon::Polygon(const Polygon& copy) { *this = copy; }

Polygon::~Polygon() = default;

size_t Polygon::verticesCount() const { return this->verts.size(); }

std::vector<Point> Polygon::getVertices() const { return this->verts; }

double Polygon::perimeter() const {
  size_t size = this->verts.size();
  double p = 0.;
  Point a, b;
  for (size_t i = 1; i <= size; ++i) {
    a = this->verts[i - 1];
    b = this->verts[i % size];
    p += seg_length(a, b);
  }
  return p;
}

double Polygon::area() const {
  double s = 0;
  size_t size = this->verts.size();
  Point p1, p2;
  for (size_t i = 1; i <= size; ++i) {
    p1 = this->verts[i - 1];
    p2 = this->verts[i % size];
    s += (p1.x - p2.x) * (p1.y + p2.y);
  }
  return 0.5 * fabs(s);
}

bool Polygon::operator==(const Shape& another) const {
  auto* poly = dynamic_cast<const Polygon*>(&another);
  if (poly == nullptr) {
    return false;
  }
  size_t size = this->verts.size();
  if (poly->verts.size() != size) {
    return false;
  }
  int k;
  for (size_t i = 0; i < size; ++i) {
    if (this->verts[0] == poly->verts[i]) {
      if (this->verts[1] == poly->verts[(i + 1) % size]) {
        k = 1;
      } else {
        k = -1;
      }
      for (size_t j = 1; j < size; ++j) {
        if (this->verts[j] != poly->verts[(size + i + k * j) % size]) {
          return false;
        }
      }
      return true;
    }
  }
  return false;
}

bool Polygon::operator!=(const Shape& another) const {
  if (*this == another) {
    return false;
  } else {
    return true;
  }
}

void Polygon::rotate(Point center, double angle) {
  size_t size = this->verts.size();
  for (size_t i = 0; i < size; ++i) {
    this->verts[i] = rotate_point(center, angle, this->verts[i]);
  }
}

void Polygon::reflex(Point center) {
  size_t size = this->verts.size();
  for (size_t i = 0; i < size; ++i) {
    this->verts[i] = scale_point(center, -1, this->verts[i]);
  }
}

void Polygon::reflex(Line axis) {
  size_t size = this->verts.size();
  for (size_t i = 0; i < size; ++i) {
    this->verts[i] = reflex_point(axis, this->verts[i]);
  }
}

void Polygon::scale(Point center, double coefficient) {
  size_t size = this->verts.size();
  for (size_t i = 0; i < size; ++i) {
    this->verts[i] = scale_point(center, coefficient, this->verts[i]);
  }
}

Ellipse::Ellipse() : centers(Point(0., 0.), Point(0., 0.)), dist(0.) {}

Ellipse::Ellipse(Point& c1, Point& c2, double dist)
    : centers(c1, c2), dist(dist) {}

Ellipse& Ellipse::operator=(const Ellipse& el) {
  if (&el == this) {
    return *this;
  }
  this->centers.first = el.centers.first;
  this->centers.second = el.centers.second;
  this->dist = el.dist;
  return *this;
}

Ellipse::Ellipse(const Ellipse& copy) { *this = copy; }

Ellipse::~Ellipse() = default;

std::pair<Point, Point> Ellipse::focuses() const { return this->centers; }

double Ellipse::distance() const { return this->dist; }

double Ellipse::eccentricity() const {
  if (this->dist == 0.) {
    return K_MAX;
  }
  Point c1 = this->focuses().first;
  Point c2 = this->focuses().second;
  return seg_length(c1, c2) / this->dist;
}

Point Ellipse::Ellipse::center() const {
  Point c1 = this->focuses().first;
  Point c2 = this->focuses().second;
  return seg_center(c1, c2);
}

double Ellipse::perimeter() const {
  double a, b, l, t, p;
  a = this->dist / 2.;
  Point f1 = this->centers.first;
  Point f2 = this->centers.second;
  l = seg_length(f1, f2);
  b = pow(a * a - 0.25 * l * l, 0.5);
  t = 3 * pow((a - b) / (a + b), 2);
  p = PI * (a + b) * (1 + t / (10 + pow(4 - t, 0.5)));
  return p;
}

double Ellipse::area() const {
  Point f1 = this->centers.first;
  Point f2 = this->centers.second;
  double l = seg_length(f1, f2);
  double d = this->dist;
  return 0.25 * PI * d * pow(d * d - l * l, 0.5);
}

bool Ellipse::operator==(const Shape& another) const {
  auto* cir = dynamic_cast<const Circle*>(&another);
  if (cir == nullptr) {
    auto* ell = dynamic_cast<const Ellipse*>(&another);
    if (ell == nullptr) {
      return false;
    }
    Point f1 = ell->focuses().first;
    Point f2 = ell->focuses().second;
    if (fabs(this->dist - ell->distance()) < EPS &&
        (this->centers.first == f1 && this->centers.second == f2 ||
         this->centers.first == f2 && this->centers.second == f1)) {
      return true;
    }
    return false;
  }
  if (fabs(0.5 * this->dist - cir->radius()) < EPS &&
      (this->centers.first == this->centers.second &&
       this->centers.first == cir->center())) {
    return true;
  }
  return false;
}

bool Ellipse::operator!=(const Shape& another) const {
  if (*this == another) {
    return false;
  } else {
    return true;
  }
}

void Ellipse::rotate(Point center, double angle) {
  this->centers.first = rotate_point(center, angle, this->centers.first);
  this->centers.second = rotate_point(center, angle, this->centers.second);
}

void Ellipse::reflex(Point center) {
  this->centers.first = scale_point(center, -1, this->centers.first);
  this->centers.second = scale_point(center, -1, this->centers.second);
}

void Ellipse::reflex(Line axis) {
  this->centers.first = reflex_point(axis, this->centers.first);
  this->centers.second = reflex_point(axis, this->centers.second);
}

void Ellipse::scale(Point center, double coefficient) {
  this->centers.first = scale_point(center, coefficient, this->centers.first);
  this->centers.second = scale_point(center, coefficient, this->centers.second);
  this->dist *= coefficient;
}

Circle::Circle() : c(Point(0., 0.)), r(0.) {}

Circle::Circle(Point& c, double r) : c(c), r(r) {}

Circle& Circle::operator=(const Circle& ci) {
  if (&ci == this) {
    return *this;
  }
  this->c = ci.c;
  this->r = ci.r;
  return *this;
}

Circle::Circle(const Circle& copy) { *this = copy; }

Circle::~Circle() = default;

double Circle::radius() const { return this->r; }

Point Circle::center() const { return this->c; }

double Circle::perimeter() const { return 2 * PI * this->r; }

double Circle::area() const { return PI * pow(this->radius(), 2); }

bool Circle::operator==(const Shape& another) const {
  auto* cir = dynamic_cast<const Circle*>(&another);
  if (cir == nullptr) {
    auto* ell = dynamic_cast<const Ellipse*>(&another);
    if (ell == nullptr) {
      return false;
    }
    Point f1 = ell->focuses().first;
    Point f2 = ell->focuses().second;
    if (fabs(2 * this->r - ell->distance()) < EPS &&
        (this->c == f1 && f1 == f2)) {
      return true;
    }
    return false;
  }
  if (fabs(this->r - cir->radius()) < EPS && this->c == cir->center()) {
    return true;
  }
  return false;
}

bool Circle::operator!=(const Shape& another) const {
  if (*this == another) {
    return false;
  } else {
    return true;
  }
}

void Circle::rotate(Point center, double angle) {
  this->c = rotate_point(center, angle, this->c);
}

void Circle::reflex(Point center) {
  this->c = scale_point(center, -1, this->c);
}

void Circle::reflex(Line axis) { this->c = reflex_point(axis, this->c); }

void Circle::scale(Point center, double coefficient) {
  this->c = scale_point(center, coefficient, this->c);
  this->r *= coefficient;
}

Rectangle::Rectangle() = default;

Rectangle::Rectangle(Point& a, Point& b, double t) : a(a), b(b), t(t) {}

Rectangle& Rectangle::operator=(const Rectangle& re) {
  if (&re == this) {
    return *this;
  }
  this->a = re.a;
  this->b = re.b;
  this->t = re.t;
  return *this;
}

Rectangle::Rectangle(const Rectangle& copy) { *this = copy; }

Rectangle::~Rectangle() = default;

Point Rectangle::center() { return seg_center(this->a, this->b); }

std::pair<Line, Line> Rectangle::diagonals() {
  Line l1(this->a, this->b);
  double ratio, tga, k_new, b_new;
  if (this->t < 1) {
    ratio = 1 / this->t;
  } else {
    ratio = t;
  }
  if (ratio != 1) {
    tga = 2 * ratio / (ratio * ratio - 1);
    k_new = (l1.k - tga) / (1 + l1.k * tga);
  } else {
    k_new = -1 / l1.k;
  }
  Point c = this->center();
  Line l2(c, k_new);
  return std::pair<Line, Line>(l1, l2);
}

double Rectangle::ratio() const { return this->t; }

std::pair<Point, Point> Rectangle::diag_points() const {
  Point a = this->a;
  Point b = this->b;
  return std::pair<Point, Point>(a, b);
}

double Rectangle::perimeter() const {
  Point p1 = this->a;
  Point p2 = this->b;
  double l = seg_length(p1, p2);
  return 2 * l * (this->t + 1) / pow(1 + pow(this->t, 2), 0.5);
}

double Rectangle::area() const {
  Point p1 = this->a;
  Point p2 = this->b;
  double l = seg_length(p1, p2);
  return this->t * l * l / (1 + this->t * this->t);
}

bool Rectangle::operator==(const Shape& another) const {
  auto* sq = dynamic_cast<const Square*>(&another);
  if (sq == nullptr) {
    auto* rec = dynamic_cast<const Rectangle*>(&another);
    if (rec == nullptr) {
      return false;
    }
    Point p1 = rec->diag_points().first;
    Point p2 = rec->diag_points().second;
    if (fabs(this->t - rec->ratio()) < EPS &&
        (this->a == p1 && this->b == p2 || this->a == p2 && this->b == p1)) {
      return true;
    }
    return false;
  }
  if (fabs(this->t - 1) < EPS &&
      (this->a == sq->diag().first && this->b == sq->diag().second ||
       this->a == sq->diag().second && this->b == sq->diag().first)) {
    return true;
  }
  return false;
}

bool Rectangle::operator!=(const Shape& another) const {
  if (*this == another) {
    return false;
  } else {
    return true;
  }
}

void Rectangle::rotate(Point center, double angle) {
  this->a = rotate_point(center, angle, this->a);
  this->b = rotate_point(center, angle, this->b);
}

void Rectangle::reflex(Point center) {
  this->a = scale_point(center, -1, this->a);
  this->b = scale_point(center, -1, this->b);
}

void Rectangle::reflex(Line axis) {
  this->a = reflex_point(axis, this->a);
  this->b = reflex_point(axis, this->b);
}

void Rectangle::scale(Point center, double coefficient) {
  this->a = scale_point(center, coefficient, this->a);
  this->b = scale_point(center, coefficient, this->b);
}

Square::Square() = default;

Square::Square(Point& a, Point& b) : a(a), b(b) {}

Square& Square::operator=(const Square& sq) {
  if (&sq == this) {
    return *this;
  }
  this->a = sq.a;
  this->b = sq.b;
  return *this;
}

Square::Square(const Square& copy) { *this = copy; }

Square::~Square() = default;

std::pair<Point, Point> Square::diag() const {
  return std::pair<Point, Point>(this->a, this->b);
}

Circle Square::circumscribedCircle() const {
  double x, y;
  double r;
  Point c((this->b.x + this->a.x) / 2, (this->b.y + this->a.y) / 2);
  x = this->b.x - this->a.x;
  y = this->b.y - this->a.y;
  r = sqrt(pow(x, 2) + pow(y, 2)) / 2;
  return Circle(c, r);
}

Circle Square::inscribedCircle() const {
  double x, y;
  double r;
  Point c((this->b.x + this->a.x) / 2, (this->b.y + this->a.y) / 2);
  x = this->b.x - this->a.x;
  y = this->b.y - this->a.y;
  r = sqrt(2 * (pow(x, 2) + pow(y, 2))) / 4;
  return Circle(c, r);
}

double Square::perimeter() const {
  Point p1 = this->a;
  Point p2 = this->b;
  return 2 * pow(2, 0.5) * seg_length(p1, p2);
}

double Square::area() const {
  Point p1 = this->a;
  Point p2 = this->b;
  return 0.5 * pow(seg_length(p1, p2), 2);
}

bool Square::operator==(const Shape& another) const {
  auto* sq = dynamic_cast<const Square*>(&another);
  if (sq == nullptr) {
    auto* rec = dynamic_cast<const Rectangle*>(&another);
    if (rec == nullptr) {
      return false;
    }
    Point p1 = rec->diag_points().first;
    Point p2 = rec->diag_points().second;
    if (fabs(rec->ratio() - 1) < EPS &&
        (this->a == p1 && this->b == p2 || this->a == p2 && this->b == p1)) {
      return true;
    }
    return false;
  }
  if (this->a == sq->diag().first && this->b == sq->diag().second ||
      this->a == sq->diag().second && this->b == sq->diag().first) {
    return true;
  }
  return false;
}

bool Square::operator!=(const Shape& another) const {
  if (*this == another) {
    return false;
  } else {
    return true;
  }
}

void Square::rotate(Point center, double angle) {
  this->a = rotate_point(center, angle, this->a);
  this->b = rotate_point(center, angle, this->b);
}

void Square::reflex(Point center) {
  this->a = scale_point(center, -1, this->a);
  this->b = scale_point(center, -1, this->b);
}

void Square::reflex(Line axis) {
  this->a = reflex_point(axis, this->a);
  this->b = reflex_point(axis, this->b);
}

void Square::scale(Point center, double coefficient) {
  this->a = scale_point(center, coefficient, this->a);
  this->b = scale_point(center, coefficient, this->b);
}

Triangle::Triangle(Point& a, Point& b, Point& c) : a(a), b(b), c(c){};

Triangle& Triangle::operator=(const Triangle& tr) {
  if (&tr == this) {
    return *this;
  }
  this->a = tr.a;
  this->b = tr.b;
  this->c = tr.c;
  return *this;
}

Triangle::Triangle(const Triangle& copy) { *this = copy; }

Triangle::~Triangle() = default;

Circle Triangle::circumscribedCircle() {
  double s, r;
  s = this->area();
  if (fabs(s) < EPS) {
    return Circle();
  }
  Point c1 = seg_center(this->a, this->b);
  Point c2 = seg_center(this->b, this->c);
  Line l1 = normal(this->a, this->b, c1);
  Line l2 = normal(this->b, this->c, c2);
  Point m = cross_point(l1, l2);
  r = seg_length(m, a);
  return Circle(m, r);
}

Circle Triangle::inscribedCircle() {
  double ab, bc, ca, xc, yc, r;
  ab = seg_length(this->a, this->b);
  bc = seg_length(this->b, this->c);
  ca = seg_length(this->c, this->a);
  if ((ab + bc + ca) < EPS) {
    return Circle();
  }
  xc = (ab * this->c.x + bc * this->a.x + ca * this->b.x) / (ab + bc + ca);
  yc = (ab * this->c.y + bc * this->a.y + ca * this->b.y) / (ab + bc + ca);
  Point ci(xc, yc);
  Line l1 = normal(this->a, this->b, ci);
  Line l2(this->a, this->b);
  Point d = cross_point(l1, l2);
  r = seg_length(ci, d);
  return Circle(ci, r);
}

Point Triangle::centroid() {
  Point m1, m2;
  m1 = seg_center(this->a, this->b);
  m2 = seg_center(this->b, this->c);
  Line l1(this->c, m1), l2(this->a, m2);
  return cross_point(l1, l2);
}

Point Triangle::orthocenter() {
  Line n1, n2;
  n1 = normal(this->a, this->b, this->c);
  n2 = normal(this->b, this->c, this->a);
  return cross_point(n1, n2);
}

Line Triangle::EulerLine() {
  Point ort, cc;
  ort = orthocenter();
  cc = circumscribedCircle().center();
  return Line(cc, ort);
}

Circle Triangle::ninePointsCircle() {
  Point m_ab, m_bc, m_ca;
  m_ab = seg_center(this->a, this->b);
  m_bc = seg_center(this->b, this->c);
  m_ca = seg_center(this->c, this->a);
  Triangle add_tr(m_ab, m_bc, m_ca);
  return add_tr.circumscribedCircle();
}

double Triangle::perimeter() const {
  Point p1 = this->a;
  Point p2 = this->b;
  Point p3 = this->c;
  double l1, l2, l3;
  l1 = seg_length(p1, p2);
  l2 = seg_length(p2, p3);
  l3 = seg_length(p3, p1);
  return l1 + l2 + l3;
}

double Triangle::area() const {
  Point p1 = this->a;
  Point p2 = this->b;
  Point p3 = this->c;
  double l1, l2, l3;
  l1 = seg_length(p1, p2);
  l2 = seg_length(p2, p3);
  l3 = seg_length(p3, p1);
  double p = 0.5 * this->perimeter();
  return pow(p * (p - l1) * (p - l2) * (p - l3), 0.5);
}

bool Triangle::operator==(const Shape& another) const {
  auto* tri = dynamic_cast<const Triangle*>(&another);
  if (tri == nullptr) {
    return false;
  }
  if (this->a == tri->a && this->b == tri->b && this->c == tri->c ||
      this->a == tri->a && this->b == tri->c && this->c == tri->b ||
      this->a == tri->b && this->b == tri->a && this->c == tri->c ||
      this->a == tri->c && this->b == tri->a && this->c == tri->b ||
      this->a == tri->b && this->b == tri->c && this->c == tri->a ||
      this->a == tri->c && this->b == tri->b && this->c == tri->a) {
    return true;
  }
  return false;
}

bool Triangle::operator!=(const Shape& another) const {
  if (*this == another) {
    return false;
  } else {
    return true;
  }
}

void Triangle::rotate(Point center, double angle) {
  this->a = rotate_point(center, angle, this->a);
  this->b = rotate_point(center, angle, this->b);
  this->c = rotate_point(center, angle, this->c);
}

void Triangle::reflex(Point center) {
  this->a = scale_point(center, -1, this->a);
  this->b = scale_point(center, -1, this->b);
  this->c = scale_point(center, -1, this->c);
}

void Triangle::reflex(Line axis) {
  this->a = reflex_point(axis, this->a);
  this->b = reflex_point(axis, this->b);
  this->c = reflex_point(axis, this->c);
}

void Triangle::scale(Point center, double coefficient) {
  this->a = scale_point(center, coefficient, this->a);
  this->b = scale_point(center, coefficient, this->b);
  this->c = scale_point(center, coefficient, this->c);
}