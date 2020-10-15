#pragma once

#include <iostream>
#include <vector>
#include <iterator>
#include <cmath>


const double EPS = 1e-6;

struct Point {
    double x;
    double y;

    Point(double x, double y);
    Point(const Point &a);
    ~Point();

    bool operator ==(const Point &a) const;
    bool operator !=(const Point &a) const;

};


class Line {
private :

    double k;
    double b;

public:

    Line(Point& a, Point& b);
    Line(double k, double b);
    Line(Point& a, double k);
    Line(const Line &a);
    ~Line ();

    bool operator ==(const Line &l) const;
    bool operator !=(const Line &l) const;
};


class Shape {
public:
/*
    virtual double perimeter() const = 0;
    virtual double area() const = 0;
    virtual bool operator==(const Shape& another) const = 0;

    virtual void rotate(Point center, double angle) = 0;
    virtual void reflex(Point center) = 0;
    virtual void reflex(Line axis) = 0;
    virtual void scale(Point center, double coefficient) = 0;
*/
};


class Polygon : public Shape {
private:
    std::vector<Point> vertices;

public:
    Polygon(const Point points []);
    Polygon(const std::vector<Point> points);
    ~Polygon();

    size_t verticesCount() const;
    std::vector<Point> getVertices() const;

};


class Ellipse : public Shape {
private:
    std::pair<Point, Point> centers;
    double dist;

public:
    Ellipse(Point& c1, dPoint& c2, double dist);
    ~Ellipse();

    std::pair<Point,Point> focuses() const;
    double eccentricity() const;
    Point center() const;

};










