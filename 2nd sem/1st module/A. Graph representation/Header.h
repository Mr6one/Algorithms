#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <vector>

struct IGraph {
    virtual ~IGraph() {}
    virtual void AddEdge(int from, int to) = 0;
	virtual int VerticesCount() const  = 0;
    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0;
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0;
};

#endif // HEADER_H_INCLUDED
