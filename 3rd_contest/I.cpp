#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

template <typename T>
struct Point {
  T x;
  T y;

  Point() = default;

  Point(T x1, T y1) : x(x1), y(y1) {}

  bool operator<(const Point& p) const {
    return x < p.x || (x == p.x && y < p.y);
  }

  Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }

  Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }

  Point operator-() const { return Point(-x, -y); }

  friend std::istream& operator>>(std::istream& is, Point& p) {
    is >> p.x >> p.y;
    return is;
  }

  friend std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << p.x << ' ' << p.y;
    return os;
  }
};

template <typename T>
T CrossProduct(const Point<T>& a, const Point<T>& b, const Point<T>& c) {
  return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
}

template <typename T>
T CrossProduct(const Point<T>& a, const Point<T>& b) {
  return a.x * b.y - a.y * b.x;
}

template <typename T>
struct Polygon {
  std::vector<Point<T>> vertices;

  Polygon() : vertices() {}

  explicit Polygon(size_t n) : vertices(n) {}

  explicit Polygon(const std::vector<Point<T>>& points) : vertices(points) {}

  size_t size() const { return vertices.size(); }

  Point<T>& operator[](size_t index) { return vertices[index]; }

  const Point<T>& operator[](size_t index) const { return vertices[index]; }

  Polygon operator-() const {
    Polygon result(vertices.size());
    for (size_t i = 0; i < vertices.size(); ++i) {
      result[i] = -vertices[i];
    }
    return result;
  }

  friend std::istream& operator>>(std::istream& is, Polygon& polygon) {
    // !!! starting from 1
    for (size_t i = 1; i < polygon.vertices.size(); ++i) {
      is >> polygon.vertices[i];
    }
    return is;
  }
};

struct BIT {
  size_t size;
  std::vector<int64_t> tree;

  BIT(size_t n) : size(n), tree(n + 1, 0) {}

  void Update(size_t i, int64_t value) {
    for (; i <= size; i += i & -i) {
      tree[i] += value;
    }
  }

  // [1:i]
  int64_t Sum(size_t i) const {
    int64_t result = 0;
    for (; i > 0; i -= i & -i) {
      result += tree[i];
    }
    return result;
  }

  // [l:r]
  int64_t Sum(size_t l, int r) const {
    if (l > r) {
      return 0;
    }
    return Sum(r) - Sum(l - 1);
  }
};

void PrintAreas(const Polygon<int64_t>& poly, const BIT& cross_products,
                size_t N) {
  size_t m = 0;
  size_t n = 0;
  std::cin >> m >> n;

  int64_t cross_sum = 0;
  if (m > n) {
    cross_sum = cross_products.Sum(m, N) + cross_products.Sum(1, n - 1);
  } else {
    cross_sum = cross_products.Sum(m, n - 1);
  }
  cross_sum += CrossProduct(poly[n], poly[m]);

  int64_t area_1_twice = std::abs(cross_sum);
  int64_t total_area_twice = std::abs(cross_products.Sum(N));
  int64_t area_2_twice = total_area_twice - area_1_twice;

  long double area_1 = static_cast<long double>(area_1_twice) / 2;
  long double area_2 = static_cast<long double>(area_2_twice) / 2;
  std::cout << std::fixed << std::setprecision(1)
            << std::min(area_1, area_2) << ' '
            << std::max(area_1, area_2) << '\n';
}

void ChangeVertice(Polygon<int64_t>& poly, BIT& cross_products, size_t N) {
  size_t n = 0;
  Point<int64_t> new_vertice;
  std::cin >> n >> new_vertice;

  size_t prev = (n == 1 ? N : n - 1);
  size_t next = (n == N ? 1 : n + 1);

  int64_t old_cross_1 = CrossProduct(poly[prev], poly[n]);
  int64_t old_cross_2 = CrossProduct(poly[n], poly[next]);

  poly[n] = new_vertice;

  int64_t new_cross_1 = CrossProduct(poly[prev], poly[n]);
  int64_t new_cross_2 = CrossProduct(poly[n], poly[next]);

  cross_products.Update(prev, new_cross_1 - old_cross_1);
  cross_products.Update(n, new_cross_2 - old_cross_2);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  size_t N = 0;
  std::cin >> N;

  Polygon<int64_t> poly(N + 1);
  std::cin >> poly;

  BIT cross_products(N);
  for (size_t i = 1; i <= N; i++) {
    size_t j = (i == N ? 1 : i + 1);
    cross_products.Update(i, CrossProduct(poly[i], poly[j]));
  }

  int Q = 0;
  std::cin >> Q;

  for (size_t i = 0; i < Q; ++i) {
    int type = 0;
    std::cin >> type;
    if (type == 1) {
      PrintAreas(poly, cross_products, N);
    } else if (type == 2) {
      ChangeVertice(poly, cross_products, N);
    }
  }
}
