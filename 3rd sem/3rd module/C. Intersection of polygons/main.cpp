#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::move;

struct Point{
    Point(double x, double y): x(x), y(y) {}
    double x;
    double y;
    bool operator < (const Point&);
};

bool Point::operator < (const Point& point) {
    if (x == point.x) {
        return y < point.y;
    } else {
        return x < point.x;
    }
}

void CounterClockwiseOrdering(vector<Point>& polygon) {
    size_t minIndex = 0;
    for (size_t i = 1; i < polygon.size(); ++i) {
        if (polygon[i] < polygon[minIndex]) {
            minIndex = i;
        }
    }
    vector<Point> tmp;
    for (size_t i = polygon.size(); i > 0; --i) {
        tmp.push_back(polygon[(minIndex + i) % polygon.size()]);
    }
    polygon = move(tmp);
}

vector<Point> MinkovskiSum(vector<Point>& polygon1, vector<Point>& polygon2) {
    size_t i = 0;
    size_t j = 0;
    polygon1.push_back(polygon1[0]);
    polygon2.push_back(polygon2[0]);
    vector<Point> polygon;
    while (i < polygon1.size() - 1 && j < polygon2.size() - 1) {
        polygon.push_back(Point(polygon1[i].x + polygon2[j].x, polygon1[i].y + polygon2[j].y));
        double vec = (polygon1[i + 1].x - polygon1[i].x) * (polygon2[j + 1].y - polygon2[j].y) - (polygon1[i + 1].y - polygon1[i].y) * (polygon2[j + 1].x - polygon2[j].x);
        if (vec > 0) {
            ++i;
        } else if (vec < 0) {
            ++j;
        } else {
            ++i;
            ++j;
        }
    }
    return polygon;
}

bool IsZeroInside(const vector<Point>& polygon) {
    for (size_t i = 0; i < polygon.size(); ++i) {
        if ((polygon[(i + 1) % polygon.size()].x - polygon[i].x) * (-polygon[i].y) - (polygon[(i + 1) % polygon.size()].y - polygon[i].y) * (-polygon[i].x) < 0) {
            return false;
        }
    }
    return true;
}

int main()
{
    vector<Point> polygon1;
    size_t n;
    cin >> n;
    for (size_t j = 0; j < n; ++j) {
        double x, y;
        cin >> x >> y;
        polygon1.push_back(Point(x, y));
    }
    cin >> n;
    vector<Point> polygon2;
    for (size_t j = 0; j < n; ++j) {
        double x, y;
        cin >> x >> y;
        polygon2.push_back(Point(-x, -y));
    }
    CounterClockwiseOrdering(polygon1);
    CounterClockwiseOrdering(polygon2);
    vector<Point> minkovskiSum = MinkovskiSum(polygon1, polygon2);
    if (IsZeroInside(minkovskiSum)) {
        cout << "YES";
    } else {
        cout << "NO";
    }
    return 0;
}