#include "geometry.h"

Point::Point(double x, double y) : x(x), y(y) {};

Point::Point(const Point &a) : x(a.x), y(a.y) {};

Point::~Point() {};

bool Point::operator ==(const Point& a) const {
    if (abs(this->x - a.x) < EPS && abs(this->y - a.y) < EPS ) {
        return true;
    }
    else {
        return false;
    }
};

bool Point::operator !=(const Point& a) const {
    if (abs(this->x - a.x) < EPS && abs(this->y - a.y) < EPS ) {
        return false;
    }
    else {
        return true;
    }
};


Line::Line(Point& a, Point& c) : k((c.y - a.y) / (c.x - a.x)),
                                 b(c.y - this->k * c.x) {};

Line::Line(double k, double b) : k(k), b(b) {};

Line::Line(Point& a, double k) : k(k), b(a.y - k * a.x) {};

Line::Line(const Line &l) : k(l.k), b(l.b) {};

Line::~Line() {};

bool Line::operator ==(const Line &l) const {
    if (abs(this->k - l.k) < EPS && abs(this->b - l.b) < EPS ) {
        return true;
    }
    else {
        return false;
    }
};

bool Line::operator !=(const Line& l) const {
    if (abs(this->k - l.k) < EPS && abs(this->b - l.b) < EPS ) {
        return false;
    }
    else {
        return true;
    }
};


Polygon::Polygon(const Point points []) {
    size_t size = sizeof(&points) / sizeof(points[0]);
    for (size_t i = 0; i < size; ++ i) {
        this->vertices.push_back(points[i]);
    }
};

Polygon::Polygon(const std::vector<Point> points) {
    for (size_t i = 0; i < points.size(); ++ i) {
        this->vertices.push_back(points[i]);
    }
};

Polygon::~Polygon() {};

size_t Polygon::verticesCount() const {
    return this->vertices.size();
};

std::vector<Point> Polygon::getVertices() const {
    return this->vertices;
};


Ellipse::Ellipse(Point& c1, Point& c2, double dist) :
                centers(c1, c2),
                dist(dist) {};

Ellipse::~Ellipse() {};

std::pair<Point,Point> Ellipse::focuses() const {
    return this->centers;
};

double Ellipse::Ellipse::eccentricity() const {
    Point c1 = this->centers.first;
    Point c2 = this->centers.second;
    return sqrt(pow((c2.x - c1.x), 2) + pow((c2.y - c1.y), 2)) / this->dist;
};

Point Ellipse::Ellipse::center() const {
    Point c1 = this->centers.first;
    Point c2 = this->centers.second;
    return Point((c1.x + c2.x) / 2, (c1.y + c2.y) / 2);
};







int main() {
    Point a(0., 0.);
    Point b(0., 1.);
    Point c(1., 1.);
    Point d(1., 0.);
    std::cout << "Points " << (a == b) << std::endl;

    Line l(2., 1.);
    Line m(2., 1.);
    Line l1(a, 2.);
    Line l2(a, b);
    std::cout << "Lines " << (m == l) << std::endl;
    std::cout << "Lines " << (l1 != l2) << std::endl;

    Polygon p({a, b, c, d});
    std::vector<Point> vec = {a, b, c, d};
    Polygon p1(vec);
    std::cout << "Polygon " << p.verticesCount() << std::endl;
    std::cout << "Polygon " << p1.verticesCount() << std::endl;
    std::cout << "Polygon " << p.getVertices()[2].x << std::endl;


}
