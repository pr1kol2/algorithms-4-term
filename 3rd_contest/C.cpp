#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <numbers>
#include <vector>

static const double kPi = std::numbers::pi_v<double>;

template <typename T>
struct Point {
  T x;
  T y;

  Point() = default;

  Point(T x1, T y1) : x(x1), y(y1) {}

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

template <typename T>
struct Vector {
  T x;
  T y;

  Vector() = default;

  Vector(T x1, T y1) : x(x1), y(y1) {}

  Vector(T x1, T y1, T x2, T y2) {
    x = x2 - x1;
    y = y2 - y1;
  }

  Vector(Point<T> start, Point<T> end) {
    x = end.x - start.x;
    y = end.y - start.y;
  }

  T LengthSquared() const { return x * x + y * y; }

  T CrossProduct(const Vector& other) const {
    return x * other.y - y * other.x;
  }

  T ScalarProduct(const Vector& other) const {
    return x * other.x + y * other.y;
  }

  void Upscale(T scale) {
    x *= scale;
    y *= scale;
  }

  friend std::istream& operator>>(std::istream& is, Vector& v) {
    Point<T> start;
    Point<T> end;
    is >> start >> end;
    v.x = end.x - start.x;
    v.y = end.y - start.y;
    return is;
  }
};

template <typename T>
struct Lighthouse {
  Point<T> pos;
  int angle;

  Lighthouse() = default;

  Lighthouse(T x, T y, int a) : pos(x, y), angle(a) {}

  friend std::istream& operator>>(std::istream& is, Lighthouse& l) {
    is >> l.pos >> l.angle;
    return is;
  }
};

Vector<double> RotateVector(const Vector<double>& v, int angle_degrees) {
  double angle_radians = angle_degrees * kPi / 180.0l;

  double sin = std::sin(angle_radians);
  double cos = std::cos(angle_radians);

  double rotated_x = v.x * cos - v.y * sin;
  double rotated_y = v.x * sin + v.y * cos;

  return Vector<double>(rotated_x, rotated_y);
}

double NextBoyPos(const Point<double>& boy_pos,
                   const Lighthouse<double>& lighhouse,
                   int r) {
  Vector<double> left_ray(lighhouse.pos, boy_pos);
  Vector<double> right_ray = RotateVector(left_ray, lighhouse.angle);

  if (right_ray.y * lighhouse.pos.y >= 0) {
    return r;
  }

  right_ray.Upscale(std::abs(lighhouse.pos.y / right_ray.y));

  return right_ray.x + lighhouse.pos.x;
}

int main() {
  size_t n = 0;
  int l = 0;
  int r = 0;

  std::cin >> n >> l >> r;

  std::vector<Lighthouse<double>> lights(n);
  for (size_t i = 0; i < n; ++i) {
    std::cin >> lights[i];
  }

  double distance = r - l;
  std::bitset<20> all_set_bitset;
  for (size_t i = 0; i < n; ++i) {
    all_set_bitset.set(i);
  }
  uint32_t all_set = all_set_bitset.to_ulong();

  std::vector<double> dp(all_set + 1);

  for (uint32_t mask = 0; mask < all_set; ++mask) {
    for (size_t j = 0; j < n; ++j) {
      std::bitset<20> next_mask(mask);

      if (next_mask[j]) {
        continue;
      }

      next_mask.flip(j);

      double next_pos = NextBoyPos({dp[mask] + l, 0}, lights[j], r);

      dp[next_mask.to_ulong()] = std::min(
          std::max(next_pos - l, dp[next_mask.to_ulong()]), distance);
    }
  }

  std::cout << std::fixed << std::setprecision(6)
            << *std::max_element(dp.begin(), dp.end()) << '\n';
}
