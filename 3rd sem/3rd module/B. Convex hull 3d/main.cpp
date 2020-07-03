#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <stdio.h>

using namespace std;

const double inf = numeric_limits<double>::max();
const double angle = 0.0000001;

struct Point{
    Point(double x, double y, double z, size_t number): x(x), y(y), z(z), number(number), prev(nullptr), next(nullptr) {}
    Point(){}
    double x;
    double y;
    double z;
    size_t number;
    Point* prev;
    Point* next;
    void Act();
};

double CrossProduct(const Point*, const Point*, const Point*);

Point* firstPointOfHull;
bool Comparator(Point*, Point*);

void Point::Act() {
    if (prev->next == this) {
        prev->next = next;
        next->prev = prev;
    } else {
        prev->next = this;
        next->prev = this;
    }
}

struct Face{
    Face(size_t firstPoint, size_t secondPoint, size_t thirdPoint): points(3){
        vector<size_t> tmp = {firstPoint, secondPoint, thirdPoint};
        int minIndex = 0;
        for (size_t i = 1; i < tmp.size(); ++i) {
            if (tmp[i] < tmp[minIndex]) {
                minIndex = i;
            }
        }
        for (size_t i = 0; i < tmp.size(); ++i) {
            points[i] = tmp[(minIndex + i) % 3];
        }
    }
    vector<size_t> points;

    bool operator < (const Face&) const;
};

bool Face::operator < (const Face& face) const {
    if (points[0] == face.points[0]) {
        if (points[1] == face.points[1]) {
            return points[2] < face.points[2];
        } else {
            return points[1] < face.points[1];
        }
    } else {
        return points[0] < face.points[0];
    }
}

double TimeCalculation(const Point* firstPoint, const Point* secondPoint, const Point* thirdPoint) {
    if (firstPoint == nullptr || secondPoint == nullptr || thirdPoint == nullptr) {
        return inf;
    }
    return ((secondPoint->x - firstPoint->x) * (thirdPoint->z - secondPoint->z) -
            (secondPoint->z - firstPoint->z) * (thirdPoint->x - secondPoint->x)) /
            CrossProduct(firstPoint, secondPoint, thirdPoint);
}

class ConvexHull {
public:
    ConvexHull(vector<Point*>&);
    void Voronoi(vector<Point*>&);
private:
    set<Face> faces;
    vector<Point*> BuildConvexHull(size_t, size_t, vector<Point*>&);
};

vector<Point*> ConvexHull::BuildConvexHull(size_t left, size_t right, vector<Point*>& points) {
    if (right - left <= 1) {
        vector<Point*> result;
        return result;
    }
    size_t mid = (left + right) / 2;
    vector<vector<Point*>> convexHull = {BuildConvexHull(left, mid, points), BuildConvexHull(mid, right, points)};
    vector<Point*> result;
    Point* u = points[mid - 1];
    Point* v = points[mid];
    while (true) {
        if (CrossProduct(u->prev, u, v) < 0) {
            u = u->prev;
        } else if (CrossProduct(u, v, v->next) < 0) {
            v = v->next;
        } else {
            break;
        }
    }
    size_t i = 0, j = 0;
    double currentTime = -inf;
    while(true) {
        vector<double> time(6, inf);
        if (i < convexHull[0].size()) {
            time[0] = TimeCalculation(convexHull[0][i]->prev, convexHull[0][i], convexHull[0][i]->next);
        }
        if (j < convexHull[1].size()) {
            time[1] = TimeCalculation(convexHull[1][j]->prev, convexHull[1][j], convexHull[1][j]->next);
        }
        time[2] = TimeCalculation(u->prev, u, v);
        time[3] = TimeCalculation(u, u->next, v);
        time[4] = TimeCalculation(u, v, v->next);
        time[5] = TimeCalculation(u, v->prev, v);
        int minIndex = -1;
        double newTime = inf;
        for (size_t k = 0; k < time.size(); ++k) {
            if (time[k] < newTime && time[k] > currentTime) {
                minIndex = k;
                newTime = time[k];
            }
        }
        if (minIndex == -1 || newTime >= inf) {
            break;
        }
        if (minIndex == 0) {
            if (convexHull[0][i]->x < u->x) {
                result.push_back(convexHull[0][i]);
            }
            convexHull[0][i]->Act();
            ++i;
        } else if (minIndex == 1) {
            if (convexHull[1][j]->x > v->x) {
                result.push_back(convexHull[1][j]);
            }
            convexHull[1][j]->Act();
            ++j;
        } else if (minIndex == 2) {
            result.push_back(u);
            u = u->prev;
        } else if (minIndex == 3) {
            u = u->next;
            result.push_back(u);
        } else if (minIndex == 4) {
            result.push_back(v);
            v = v->next;
        } else if (minIndex == 5) {
            v = v->prev;
            result.push_back(v);
        }
        currentTime = newTime;
    }
    u->next = v;
    v->prev = u;
    for (int k = result.size() - 1; k >= 0; --k) {
        if (result[k]->x <= u->x || result[k]->x >= v->x) {
            result[k]->Act();
            if (result[k] == u) {
                u = u->prev;
            }
            if (result[k] == v) {
                v = v->next;
            }
        } else {
            u->next = result[k];
            v->prev = result[k];
            result[k]->prev = u;
            result[k]->next = v;
            if (result[k]->x <= points[mid - 1]->x) {
                u = result[k];
            } else {
                v = result[k];
            }
        }
    }

    return result;
}

ConvexHull::ConvexHull(vector<Point*>& points) {
    vector<Point*> convexHull = BuildConvexHull(0, points.size(), points);
    for (Point* point: convexHull) {
        if (CrossProduct(point->prev, point, point->next) > 0) {
            faces.insert(Face(point->prev->number, point->next->number, point->number));
        } else {
            faces.insert(Face(point->prev->number, point->number, point->next->number));
        }
        point->Act();
    }
    for (Point* point: points) {
        point->z = -point->z;
        point->next = nullptr;
        point->prev = nullptr;
    }
    convexHull = BuildConvexHull(0, points.size(), points);
    for (Point* point: convexHull) {
        if (CrossProduct(point->prev, point, point->next) < 0) {
            faces.insert(Face(point->prev->number, point->next->number, point->number));
        } else {
            faces.insert(Face(point->prev->number, point->number, point->next->number));
        }
        point->Act();
    }
}

void ConvexHull::Voronoi(vector<Point*>& sites) {
    set<pair<int, int>> edges;
    for (Face face: faces) {
        for (size_t i = 0; i < face.points.size(); ++i) {
            if (face.points[i] > face.points[(i + 1) % face.points.size()]) {
                edges.insert(make_pair(face.points[(i + 1) % face.points.size()], face.points[i]));
            } else {
                edges.insert(make_pair(face.points[i], face.points[(i + 1) % face.points.size()]));
            }
        }
    }

    vector<int> edgesNumbers(sites.size());
    for (pair<int, int> edge: edges) {
        ++edgesNumbers[edge.first];
        ++edgesNumbers[edge.second];
    }

    size_t minIndex = 0;
    for (size_t i = 1; i < sites.size(); ++i) {
        if (sites[i]->x < sites[minIndex]->x || (sites[i]->x == sites[minIndex]->x && sites[i]->y < sites[minIndex]->y)) {
            minIndex = i;
        }
    }
    firstPointOfHull = sites[minIndex];
    sort(sites.begin(), sites.end(), Comparator);

    vector<Point*> convexHull = {sites[0], sites[1]};
    for (size_t i = 2; i < sites.size(); ++i) {
        while (CrossProduct(convexHull[convexHull.size() - 2], convexHull[convexHull.size() - 1], sites[i]) < 0) {
            convexHull.pop_back();
        }
        convexHull.push_back(sites[i]);
    }
    vector<bool> isInConvexHull(sites.size());
    for (Point* point: convexHull) {
        isInConvexHull[point->number] = true;
    }

    int innerPoints = 0;
    int degree = 0;
    for (size_t i = 0; i < sites.size(); ++i) {
        if (!isInConvexHull[i]) {
            ++innerPoints;
            degree += edgesNumbers[i];
        }
    }
    if (innerPoints == 0) {
        cout << 0.0;
    } else {
        printf("%.6f\n", static_cast<double>(degree) / innerPoints);
    }
}

void Rotation(Point* point) {
    double x = cos(angle) * point->x + sin(angle) * point->z;
    double z = -sin(angle) * point->x + cos(angle) * point->z;
    point->x = x;
    point->z = z;

    z = cos(angle) * point->z + sin(angle) * point->y;
    double y = -sin(angle) * point->z + cos(angle) * point->y;
    point->z = z;
    point->y = y;

    x = cos(angle) * point->x + sin(angle) * point->y;
    y = -sin(angle) * point->x + cos(angle) * point->y;
    point->x = x;
    point->y = y;
}

double CrossProduct(const Point* firstPoint, const Point* secondPoint, const Point* thirdPoint) {
    if (firstPoint == nullptr || secondPoint == nullptr || thirdPoint == nullptr) {
        return inf;
    }
    return (secondPoint->x - firstPoint->x) * (thirdPoint->y - secondPoint->y) - (secondPoint->y - firstPoint->y) * (thirdPoint->x - secondPoint->x);
}

double Distance(Point* firstPoint, Point* secondPoint){
    return sqrt(pow(secondPoint->x - firstPoint->x,2) + pow(secondPoint->y - firstPoint->y, 2));
}

bool Comparator(Point* firstPoint, Point* secondPoint) {
    if (CrossProduct(firstPointOfHull, firstPoint, secondPoint) == 0) {
        return Distance(firstPointOfHull, firstPoint) < Distance(firstPointOfHull, secondPoint);
    } else {
        return CrossProduct(firstPointOfHull, firstPoint, secondPoint) > 0;
    }
}

bool Comp(Point* firstPoint, Point* secondPoint) {
    return firstPoint->x < secondPoint->x;
}

int main()
{
    vector<Point*> sites;
    int j = 0;
	double x, y;
	int number;
	cin >> number;
    for (int i = 0; i < number; ++i) {
        cin >> x >> y;
        Point* site = new Point(x, y, x * x + y * y, j);
        Rotation(site);
        sites.push_back(site);
        ++j;
    }
	sort(sites.begin(), sites.end(), Comp);
    ConvexHull convexHull(sites);
    convexHull.Voronoi(sites);
    return 0;
}
