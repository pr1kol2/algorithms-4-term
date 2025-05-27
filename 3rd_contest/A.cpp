#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>

struct Point {
  double x;
  double y;

  Point() = default;

  Point(double x1, double y1) : x(x1), y(y1) {}

  friend std::istream& operator>>(std::istream& is, Point& p) {
    is >> p.x >> p.y;
    return is;
  }
};

struct Line {
  double a;
  double b;
  double c;

  Line() = default;

  Line(double a1, double b1, double c1) : a(a1), b(b1), c(c1) {}

  Line(Point start, Point end) {
    a = start.y - end.y;
    b = end.x - start.x;
    c = start.x * end.y - end.x * start.y;
  }

  friend std::istream& operator>>(std::istream& is, Line& l) {
    is >> l.a >> l.b >> l.c;
    return is;
  }
};

struct Vector {
  double x;
  double y;

  Vector() = default;

  Vector(double x1, double y1) : x(x1), y(y1) {}

  Vector(double x1, double y1, double x2, double y2) {
    x = x2 - x1;
    y = y2 - y1;
  }

  Vector(Point a, Point b) {
    x = b.x - a.x;
    y = b.y - a.y;
  }

  double Length() const { return std::sqrt(x * x + y * y); }

  friend std::istream& operator>>(std::istream& is, Vector& v) {
    double x1 = 0;
    double y1 = 0;
    double x2 = 0;
    double y2 = 0;
    is >> x1 >> y1 >> x2 >> y2;
    v.x = x2 - x1;
    v.y = y2 - y1;
    return is;
  }
};

double CrossProduct(const Vector& a, const Vector& b) {
  return a.x * b.y - a.y * b.x;
}

double ScalarProduct(const Vector& a, const Vector& b) {
  return a.x * b.x + a.y * b.y;
}

double DistanceToLine(const Point& point, const Line& line) {
  return std::fabs(line.a * point.x + line.b * point.y + line.c) /
         std::sqrt(line.a * line.a + line.b * line.b);
}

double DistanceToSegment(const Point& start, const Point& end,
                         const Point& point) {
  Vector ab(start, end);
  Vector ac(start, point);
  Vector bc(end, point);
  Vector ba(end, start);

  if (ScalarProduct(ac, ab) <= 0) {
    return ac.Length();
  }

  if (ScalarProduct(bc, ba) <= 0) {
    return bc.Length();
  }

  Line line(start, end);

  return DistanceToLine(point, line);
}

bool IsPointOnLine(const Point& a, const Point& b, const Point& c) {
  Vector ab(a, b);
  Vector ac(a, c);

  return std::fabs(CrossProduct(ab, ac)) == 0;
}

bool IsPointOnSegment(const Point& a, const Point& b, const Point& c) {
  if (!IsPointOnLine(a, b, c)) {
    return false;
  }

  Vector ab(a, b);
  Vector ac(a, c);

  double scalar_product = ScalarProduct(ab, ac);
  if (scalar_product < 0) {
    return false;
  }

  double ab_length_squared = ab.x * ab.x + ab.y * ab.y;

  return scalar_product <= ab_length_squared;
}

bool IsSegmentCrossesLine(const Point& a, const Point& b, const Point& c,
                          const Point& d) {
  Vector ab(a, b);
  Vector ac(a, c);
  Vector ad(a, d);

  return CrossProduct(ab, ac) * CrossProduct(ab, ad) <= 0;
}

bool IntersectSegments(const Point& a, const Point& b, const Point& c,
                       const Point& d) {
  Vector ab(a, b);
  Vector ac(a, c);
  Vector ad(a, d);
  Vector cd(c, d);
  Vector ca(c, a);
  Vector cb(c, b);

  if (CrossProduct(ab, ac) == 0 && CrossProduct(ab, ad) == 0 &&
      CrossProduct(cd, ca) == 0 && CrossProduct(cd, cb) == 0) {
    return IsPointOnSegment(a, b, c) || IsPointOnSegment(a, b, d);
  }

  return IsSegmentCrossesLine(a, b, c, d) && IsSegmentCrossesLine(c, d, a, b);
}

double SegmentsDistance(const Point& a, const Point& b, const Point& c,
                        const Point& d) {
  if (IntersectSegments(a, b, c, d)) {
    return 0;
  }

  return std::min<double>({DistanceToSegment(a, b, c), DistanceToSegment(a, b, d),
                   DistanceToSegment(c, d, a), DistanceToSegment(c, d, b)});
}

int main() {
  Point a;
  Point b;
  Point c;
  Point d;
  std::cin >> a >> b >> c >> d;

  std::cout << std::setprecision(7) << SegmentsDistance(a, b, c, d) << '\n';

  return 0;
}
