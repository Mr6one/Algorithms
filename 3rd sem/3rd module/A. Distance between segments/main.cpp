#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

struct Point {
    Point(double x, double y, double z): x(x), y(y), z(z) {}
    Point() {}
    double x;
    double y;
    double z;
};

struct Vector: Point {
    Vector(const Point&, const Point&);
    Vector() {}

    Vector operator * (double);
    friend Vector operator + (const Vector&, const Vector&);
    friend Vector operator - (const Vector&, const Vector&);
};

Vector::Vector(const Point& from, const Point& to) {
    x = to.x - from.x;
    y = to.y - from.y;
    z = to.z - from.z;
}

Vector Vector::operator * (double scalar) {
    return Vector(Point(0, 0, 0), Point(x * scalar, y * scalar, z * scalar));
}

Vector operator + (const Vector& vec1, const Vector& vec2) {
    return Vector(Point(0, 0, 0), Point(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z));
}

Vector operator - (const Vector& vec1, const Vector& vec2) {
    return Vector(Point(0, 0, 0), Point(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z));
}

double DotProduct(const Vector& vector1, const Vector& vector2) {
    return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}

vector<Vector> CreateVectors(const vector<Point>& points) {
    vector<Vector> vectors(3);
    vectors[0] = Vector(points[0], points[1]);
    vectors[1] = Vector(points[2], points[3]);
    vectors[2] = Vector(points[2], points[0]);
    return vectors;
}

double FindDistance(const vector<Point>& points) {
    vector<Vector> vectors = CreateVectors(points);
    double A = DotProduct(vectors[0], vectors[0]);
    double B = DotProduct(vectors[0], vectors[1]);
    double C = DotProduct(vectors[2], vectors[0]);
    double D = DotProduct(vectors[1], vectors[1]);
    double E = DotProduct(vectors[2], vectors[1]);
    double area = A * D - B * B;
    double volume = B * E - C * D;
    double param1;
    double param2;
    if (area == 0) {
        param1 = 0;
    } else {
        param1 = volume / area;
        if (param1 >= 1) {
            param1 = 1;
        } else if (param1 <= 0) {
            param1 = 0;
        }
    }
    if (D == 0) {
        param2 = 0;
    } else {
        param2 = (B * param1 + E) / D;
    }

    if (param2 <= 0) {
        param2 = 0;
    } else if (param2 >= 1) {
        param2 = 1;
    }

    if ((B * param2 - C) <= 0) {
        param1 = 0;
    } else if ((B * param2 - C) >= A) {
        param1 = 1;
    } else {
        if (A == 0){
            param1 = 0;
        } else {
            param1 = (B * param2 - C) / A;
        }
    }
    double distance = DotProduct(vectors[2] + vectors[0] * param1 - vectors[1] * param2, vectors[2] + vectors[0] * param1 - vectors[1] * param2);
    return  sqrt(distance);
}

int main() {
    vector<Point> points(4);
    for (size_t i = 0; i < 4; ++i) {
        double x, y, z;
        cin >> x >> y >> z;
        points[i] = Point(x, y, z);
    }
    printf("%.8f", FindDistance(points));
    return 0;
}
