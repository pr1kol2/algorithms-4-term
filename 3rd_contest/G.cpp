#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

const static double kError = 1e-9;

struct Point {
  double x;
  double y;

  Point(double x = 0, double y = 0) : x(x), y(y) {}

  bool operator==(const Point& other) const {
    return std::abs(x - other.x) < kError && std::abs(y - other.y) < kError;
  }

  bool operator<(const Point& other) const {
    if (std::abs(x - other.x) < kError) {
      return y < other.y - kError;
    }
    return x < other.x - kError;
  }

  friend std::istream& operator>>(std::istream& is, Point& point) {
    is >> point.x >> point.y;
    return is;
  }

  friend std::ostream& operator<<(std::ostream& os, const Point& point) {
    os << point.x << ' ' << point.y;
    return os;
  }

  double DistanceSquared(const Point& other) const {
    return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y);
  }

  bool IsFinite() const { return std::isfinite(x) && std::isfinite(y); }
};

double CrossProduct(const Point& a, const Point& b, const Point& c) {
  return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
}

struct Line {
  double a;
  double b;
  double c;

  Line(double a = 0, double b = 0, double c = 0) : a(a), b(b), c(c) {}

  Line(const Point& start, const Point& end) {
    a = start.y - end.y;
    b = end.x - start.x;
    c = start.x * end.y - end.x * start.y;
  }

  friend std::istream& operator>>(std::istream& is, Line& l) {
    is >> l.a >> l.b >> l.c;
    return is;
  }

  bool IsPointInHalfplane(const Point& point) const {
    double val = a * point.x + b * point.y + c;
    return val <= kError;
  }

  Point GetIntersectionWithSegment(const Point& start, const Point& end) const {
    double dx = end.x - start.x;
    double dy = end.y - start.y;
    double denominator = a * dx + b * dy;

    if (std::abs(denominator) < kError) {
      return {std::numeric_limits<double>::infinity(),
              std::numeric_limits<double>::infinity()};
    }

    double t = -(a * start.x + b * start.y + c) / denominator;
    return {start.x + t * dx, start.y + t * dy};
  }
};

std::vector<Point> RemoveDuplicates(const std::vector<Point>& polygon) {
  std::vector<Point> result;

  for (const auto& point : polygon) {
    if (result.empty() || !(point == result.back())) {
      result.push_back(point);
    }
  }

  if (!result.empty() && result.front() == result.back()) {
    result.pop_back();
  }

  return result;
}

std::vector<Point> ClipPolygon(const std::vector<Point>& polygon,
                               const Line& line) {
  std::vector<Point> clipped;

  Point prev = polygon.back();
  bool prev_in = line.IsPointInHalfplane(prev);

  auto push_intersection = [&](const Point& prev, const Point& curr) {
    Point intersection = line.GetIntersectionWithSegment(prev, curr);

    if (intersection.IsFinite()) {
      clipped.push_back(intersection);
    }
  };

  for (const auto& curr : polygon) {
    bool curr_in = line.IsPointInHalfplane(curr);

    if (curr_in) {
      if (!prev_in) {
        push_intersection(prev, curr);
      }
      clipped.push_back(curr);
    } else if (prev_in) {
      push_intersection(prev, curr);
    }

    prev = curr;
    prev_in = curr_in;
  }

  return RemoveDuplicates(clipped);
}

void SortPolygon(std::vector<Point>& polygon) {
  if (!polygon.empty()) {
    std::rotate(polygon.begin(),
                std::min_element(polygon.begin(), polygon.end()),
                polygon.end());
    Point left_bottom = *polygon.begin();

    std::sort(polygon.begin() + 1, polygon.end(),
              [&](const Point& lhs, const Point& rhs) {
                double cross = CrossProduct(left_bottom, lhs, rhs);
                if (std::abs(cross) > kError) {
                  return cross > 0;
                }
                return left_bottom.DistanceSquared(lhs) <
                       left_bottom.DistanceSquared(rhs);
              });
  }
}

std::vector<Point> GetVoronoiCell(const Point& seed,
                                  const std::vector<Point>& seeds, double x_max,
                                  double y_max) {
  std::vector<Point> region{{0, 0}, {x_max, 0}, {x_max, y_max}, {0, y_max}};

  for (const auto& other : seeds) {
    if (seed == other) {
      continue;
    }

    double a = 2 * (other.x - seed.x);
    double b = 2 * (other.y - seed.y);
    double c = seed.x * seed.x + seed.y * seed.y - other.x * other.x -
               other.y * other.y;

    region = ClipPolygon(region, Line(a, b, c));

    if (region.empty()) {
      break;
    }
  }

  SortPolygon(region);

  return region;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int x = 0;
  int y = 0;
  size_t n = 0;
  std::cin >> x >> y >> n;

  std::vector<Point> players(n);
  for (size_t i = 0; i < n; ++i) {
    std::cin >> players[i];
  }

  for (const auto& player : players) {
    auto cell = GetVoronoiCell(player, players, x, y);

    std::cout << cell.size();
    for (const auto& point : cell) {
      std::cout << std::fixed << std::setprecision(8) << ' ' << point;
    }
    std::cout << '\n';
  }
}

