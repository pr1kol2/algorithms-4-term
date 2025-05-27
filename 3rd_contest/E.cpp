#include <cstdint>
#include <iostream>
#include <unordered_set>
#include <vector>

struct Point {
  int x;
  int y;

  Point() = default;

  Point(int x1, int y1) : x(x1), y(y1) {}

  bool operator<(const Point& p) const {
    return x < p.x || (x == p.x && y < p.y);
  }

  bool operator==(const Point& p) const { return x == p.x && y == p.y; }

  int GetX() const { return x; }

  int GetY() const { return y; }

  std::pair<int, int> AsPair() const { return {x, y}; }

  friend std::istream& operator>>(std::istream& is, Point& p) {
    is >> p.x >> p.y;
    return is;
  }

  friend std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << p.x << ' ' << p.y;
    return os;
  }
};

struct PointHash {
  size_t operator()(Point const& p) const noexcept {
    return (static_cast<uint64_t>(p.GetX()) << 32) ^
           static_cast<uint32_t>(p.GetY());
  }
};

int main() {
  size_t n = 0;
  std::cin >> n;

  std::vector<Point> points_vector(n);
  for (size_t i = 0; i < n; ++i) {
    std::cin >> points_vector[i];
  }

  std::unordered_set<Point, PointHash> points_set;
  points_set.reserve(n * 2);
  for (auto p : points_vector) {
    points_set.insert(p);
  }

  uint64_t diagonal_count = 0;

  for (size_t i = 0; i < n; ++i) {
    auto [x1, y1] = points_vector[i].AsPair();

    for (size_t j = i + 1; j < n; ++j) {
      auto [x2, y2] = points_vector[j].AsPair();

      int mx2 = x1 + x2;
      int my2 = y1 + y2;

      int dx = x2 - x1;
      int dy = y2 - y1;

      if (((mx2 + dy) & 1) || ((my2 - dx) & 1) || ((mx2 - dy) & 1) ||
          ((my2 + dx) & 1)) {
        continue;
      }

      int bx = (mx2 + dy) >> 1;
      int by = (my2 - dx) >> 1;
      int dx2 = (mx2 - dy) >> 1;
      int dy2 = (my2 + dx) >> 1;

      if (points_set.contains({bx, by}) && points_set.contains({dx2, dy2})) {
        ++diagonal_count;
      }
    }
  }

  std::cout << (diagonal_count / 2) << '\n';
}
