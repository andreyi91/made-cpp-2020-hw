#pragma once

#include <cmath>
#include <iostream>
#include <vector>

const double EPS = 1e-6;
const double K_MAX = 10e8;
const double PI = 3.1415926;

struct Point {
  double x;
  double y;

  Point();
  Point(double x, double y);
  Point(const Point& a);
  ~Point();

  Point& operator=(const Point& a);
  bool operator==(const Point& a) const;
  bool operator!=(const Point& a) const;
};

class Line {
 public:
  double k;
  double b;

  Line();
  Line(Point a, Point b);
  Line(double k, double b);
  Line(Point& a, double k);
  Line(const Line& a);
  ~Line();

  Line& operator=(const Line& l);
  bool operator==(const Line& l) const;
  bool operator!=(const Line& l) const;
};

class Shape : public Point, public Line {
 public:
  virtual double perimeter() const = 0;
  virtual double area() const = 0;
  virtual bool operator==(const Shape& another) const = 0;
  virtual bool operator!=(const Shape& another) const = 0;

  virtual void rotate(Point center, double angle) = 0;
  virtual void reflex(Point center) = 0;
  virtual void reflex(Line axis) = 0;
  virtual void scale(Point center, double coefficient) = 0;

  virtual ~Shape() = 0;

  Point seg_center(Point& a, Point& b) const;
  double seg_length(Point& a, Point& b) const;
  Line normal(Line& l, Point& c) const;
  Line normal(Point& a, Point& b, Point& c) const;
  Point cross_point(Line& l1, Line& l2) const;
  Point rotate_point(Point c, double ang, Point a) const;
  Point scale_point(Point c, double coef, Point a) const;
  Point reflex_point(Line l, Point a) const;
};

class Polygon : public Shape {
 private:
  std::vector<Point> verts;

 public:
  Polygon();
  Polygon& operator=(const Polygon& p);

  explicit Polygon(const Point points[]);
  explicit Polygon(const std::vector<Point>& points);
  Polygon(const Polygon& copy);
  ~Polygon() override;

  size_t verticesCount() const;
  std::vector<Point> getVertices() const;

  double perimeter() const override;
  double area() const override;
  bool operator==(const Shape& another) const override;
  bool operator!=(const Shape& another) const;
  void rotate(Point center, double angle) override;
  void reflex(Point center) override;
  void reflex(Line axis) override;
  void scale(Point center, double coefficient);
};

class Ellipse : public Shape {
 protected:
  std::pair<Point, Point> centers;
  double dist;

 public:
  Ellipse();
  Ellipse(Point& c1, Point& c2, double dist);
  Ellipse& operator=(const Ellipse& el);
  Ellipse(const Ellipse& copy);
  ~Ellipse() override;

  std::pair<Point, Point> focuses() const;
  double distance() const;
  double eccentricity() const;
  Point center() const;

  double perimeter() const override;
  double area() const override;
  bool operator==(const Shape& another) const override;
  bool operator!=(const Shape& another) const override;
  void rotate(Point center, double angle) override;
  void reflex(Point center) override;
  void reflex(Line axis) override;
  void scale(Point center, double coefficient);
};

class Circle : public Ellipse {
 private:
  Point c;
  double r;

 public:
  Circle();
  Circle(Point& c, double r);
  Circle& operator=(const Circle& ci);
  Circle(const Circle& copy);
  ~Circle() override;

  double radius() const;
  Point center() const;

  double perimeter() const override;
  double area() const override;
  bool operator==(const Shape& another) const override;
  bool operator!=(const Shape& another) const override;
  void rotate(Point center, double angle) override;
  void reflex(Point center) override;
  void reflex(Line axis) override;
  void scale(Point center, double coefficient);
};

class Rectangle : public Polygon {
 private:
  Point a;
  Point b;
  double t;

 public:
  Rectangle();
  Rectangle(Point& a, Point& b, double t);
  Rectangle& operator=(const Rectangle& re);
  Rectangle(const Rectangle& copy);
  ~Rectangle() override;

  Point center();
  std::pair<Line, Line> diagonals();
  double ratio() const;
  std::pair<Point, Point> diag_points() const;

  double perimeter() const override;
  double area() const override;
  bool operator==(const Shape& another) const override;
  bool operator!=(const Shape& another) const override;
  void rotate(Point center, double angle) override;
  void reflex(Point center) override;
  void reflex(Line axis) override;
  void scale(Point center, double coefficient);
};

class Square : public Rectangle {
 private:
  Point a;
  Point b;

 public:
  Square();
  Square(Point& a, Point& b);
  Square& operator=(const Square& sq);
  Square(const Square& copy);
  ~Square() override;

  std::pair<Point, Point> diag() const;
  Circle circumscribedCircle() const;
  Circle inscribedCircle() const;

  double perimeter() const override;
  double area() const override;
  bool operator==(const Shape& another) const override;
  bool operator!=(const Shape& another) const override;
  void rotate(Point center, double angle) override;
  void reflex(Point center) override;
  void reflex(Line axis) override;
  void scale(Point center, double coefficient);
};

class Triangle : public Polygon {
 private:
  Point a;
  Point b;
  Point c;

 public:
  Triangle(Point& a, Point& b, Point& c);
  Triangle& operator=(const Triangle& tr);
  Triangle(const Triangle& copy);
  ~Triangle() override;

  Circle circumscribedCircle();
  Circle inscribedCircle();
  Point centroid();
  Point orthocenter();
  Line EulerLine();
  Circle ninePointsCircle();

  double perimeter() const override;
  double area() const override;
  bool operator==(const Shape& another) const override;
  bool operator!=(const Shape& another) const override;
  void rotate(Point center, double angle) override;
  void reflex(Point center) override;
  void reflex(Line axis) override;
  void scale(Point center, double coefficient);
};