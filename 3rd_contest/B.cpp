#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

struct Point {
  int64_t x;
  int64_t y;

  Point() = default;

  Point(int64_t x1, int64_t y1) : x(x1), y(y1) {}

  bool operator<(const Point& p) const {
    return x < p.x || (x == p.x && y < p.y);
  }

  friend std::istream& operator>>(std::istream& is, Point& p) {
    is >> p.x >> p.y;
    return is;
  }

  friend std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << p.x << ' ' << p.y;
    return os;
  }
};

struct Polygon {
  std::vector<Point> vertices;

  Polygon() = default;

  explicit Polygon(size_t n) : vertices(n) {}

  friend std::istream& operator>>(std::istream& is, Polygon& polygon) {
    for (size_t i = 0; i < polygon.vertices.size(); ++i) {
      is >> polygon.vertices[i];
    }
    return is;
  }

  void SetVertices(const std::vector<Point>& new_vertices) {
    vertices = new_vertices;
  }

  double Area() {
    int64_t area = 0;

    for (size_t i = 0; i < vertices.size(); ++i) {
      Point current = vertices[i];
      Point next = vertices[(i + 1) % vertices.size()];
      area += current.x * next.y - current.y * next.x;
    }

    double result = 0.5 * static_cast<double>(area);

    return std::abs(result);
  }
};

int64_t CrossProduct(const Point& a, const Point& b, const Point& c) {
  return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
}

std::vector<Point> JarvisScan(std::vector<Point>& points) {
  size_t n = points.size();
  size_t k = 0;
  std::vector<Point> h(2 * n);

  std::sort(points.begin(), points.end());

  for (size_t i = 0; i < n; ++i) {
    while (k >= 2 && CrossProduct(h[k - 2], h[k - 1], points[i]) <= 0) {
      --k;
    }
    h[k++] = points[i];
  }

  for (size_t i = n - 1, t = k + 1; i > 0; --i) {
    while (k >= t && CrossProduct(h[k - 2], h[k - 1], points[i - 1]) <= 0) {
      --k;
    }
    h[k++] = points[i - 1];
  }

  h.resize(k - 1);

  return h;
}

void PrintConvexHull(const std::vector<Point>& hull) {
  std::cout << hull.size() << '\n';

  std::cout << hull[0] << '\n';
  for (size_t i = 1; i < hull.size(); ++i) {
    if (hull.size() < 3) {
      std::cout << hull[i] << '\n';
    } else if (CrossProduct(hull[0], hull[1], hull[2]) > 0) {
      std::cout << hull[hull.size() - i] << '\n';
    } else if (CrossProduct(hull[0], hull[1], hull[2]) == 0) {
      continue;
    } else {
      std::cout << hull[i] << '\n';
    }
  }
}

int main() {
  size_t n = 0;
  std::cin >> n;

  Polygon polygon(n);
  std::cin >> polygon;

  std::vector<Point> hull = JarvisScan(polygon.vertices);

  PrintConvexHull(hull);

  polygon.SetVertices(hull);

  std::cout << std::fixed << std::setprecision(1) << polygon.Area() << '\n';

  return 0;
}
