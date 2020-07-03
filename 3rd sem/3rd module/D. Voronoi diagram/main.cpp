#include <iostream>
#include <queue>
#include <vector>

using namespace std;

enum EventTypes{place, circle};

struct Point{
    Point(double x, double y): x(x), y(y){}
    double x;
    double y;
};

struct HalfEdge{
    HalfEdge(Point* start, Point* ending): start(start), ending(ending){}
    Point* start;
    Point* ending;
    HalfEdge* twin;
};

struct Parabola{
     Point* site;

     Parabola* leftChild;
     Parabola* rightChild;
     Parabola* parent;
};

class Event{
public:
    Event(Point* site, size_t eventType):eventType(eventType), site(site){}
    size_t eventType;
    Point* site;
    Parabola* parabola;
private:
    bool operator () (const Event* event) const {
        return site->y < event->site->y;
    }
};

struct Comparator{
    bool operator () (const Event* event1, const Event* event2){
        return event1->site->y < event2->site->y;
    }
};

class VoronoiDiagram{
public:
    VoronoiDiagram(const vector<Point*>&);
private:
    priority_queue<Event*, vector<Event*>, Comparator> events;
    void PlaceEvent(const Point*);
    void CircleEvent(const Parabola*);
};

VoronoiDiagram::VoronoiDiagram(const vector<Point*>& sites){
    for (Point* point: sites) {
        events.push(new Event(point, place));
    }

    while (!events.empty()) {
        Event* newEvent = events.top();
        events.pop();
        cout << newEvent->site->x << ' ' << newEvent->site->y << endl;
        if (newEvent->eventType == place) {
            PlaceEvent(newEvent->site);
        } else {
            CircleEvent(newEvent->parabola);
        }
    }
}

void VoronoiDiagram::PlaceEvent(const Point* site){

}

void VoronoiDiagram::CircleEvent(const Parabola* parabola){

}

int main()
{
    size_t N;
    cin >> N;
    vector<Point*> sites;
    for (size_t i = 0; i < N; ++i) {
        double x, y;
        cin >> x >> y;
        sites.push_back(new Point(x, y));
    }
    VoronoiDiagram voronoiDiagram(sites);
    return 0;
}
