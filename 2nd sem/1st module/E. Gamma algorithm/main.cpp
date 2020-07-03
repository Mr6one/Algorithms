#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class ListGraph{
public:
    ListGraph(int n){
        graph.resize(n);
    }

    void Add(int, int);
    int VerticesCount();
    void GetNextVertices(int, vector<int>&);
    void RemoveEdge(int, int);
    bool IsVertBelongToSeg(int, vector<int>&);
private:
    vector<vector<int>> graph;
};

void ListGraph::Add(int from, int to){
        graph[from].push_back(to);
        graph[to].push_back(from);
}

int ListGraph::VerticesCount(){
    return graph.size();
}

void ListGraph::GetNextVertices(int vertex, vector<int>& vertices){
    vertices=graph[vertex];
}

void ListGraph::RemoveEdge(int to, int from){
    auto point1=find(graph[from].begin(), graph[from].end(), to);
    auto point2=find(graph[to].begin(), graph[to].end(), from);
    if (point1!=graph[from].end()){
        graph[from].erase(point1);
    }
    if (point2!=graph[to].end()){
        graph[to].erase(point2);
    }
}

bool ListGraph::IsVertBelongToSeg(int i, vector<int>& segment){
    auto point=find(segment.begin(), segment.end(), i);
    if (point!=segment.end()){
        return true;
    } else {
        return false;
    }
}

void DeleteCycle(ListGraph& graph, vector<int>& cycle){
    vector<vector<int>> deleted(graph.VerticesCount());
    for (size_t i=0; i<cycle.size()-1; ++i){
        deleted[cycle[i]].push_back(cycle[i+1]);
    }
    deleted[cycle[cycle.size()-1]].push_back(cycle[0]);

    for (size_t i=0; i<deleted.size(); ++i){
        for (size_t j=0; j<deleted[i].size(); ++j){
            graph.RemoveEdge(i, deleted[i][j]);
        }
    }
}

bool CycleSearchInOneComp(ListGraph& graph, int i, int numComp, vector<int>& components,
    vector<bool>& visited, vector<int>& parents, int& cycle_begin, int& cycle_end)
{
    vector<int> vertices;
    graph.GetNextVertices(i, vertices);
    for (int vertex: vertices){
        if (!visited[vertex] && components[vertex]==numComp){
            visited[vertex]=true;
            parents[vertex]=i;
            if (CycleSearchInOneComp(graph, vertex, numComp, components, visited, parents, cycle_begin, cycle_end)){
                return true;
            }
        } else if (parents[i]!=vertex && components[vertex]==numComp){
            cycle_end=i;
            cycle_begin=vertex;
            return true;
        }
    }
    return false;
}

bool CycleSearchInOneComp(ListGraph& graph, int numComp, vector<int>& components, vector<int>& cycle, vector<int>& contactsVert, vector<int>& parents){
    vector<bool> visited(graph.VerticesCount());
    int cycle_begin;
    int cycle_end;
    for (int i=0; i<graph.VerticesCount(); ++i){
        if (!visited[i] && components[i]==numComp){
            visited[i]=true;
            if (CycleSearchInOneComp(graph, i, numComp, components, visited, parents, cycle_begin, cycle_end)){
                if (contactsVert[0]!=-1){
                    for (int j=cycle_end; j!=cycle_begin; j=parents[j]){
                        cycle.push_back(j);
                        contactsVert[j]=1;
                    }
                    cycle.push_back(cycle_begin);
                    contactsVert[cycle_begin]=1;

                    reverse(cycle.begin(), cycle.end());
                    DeleteCycle(graph, cycle);
                }

                return true;
            }
        }
    }
    return false;
}

bool CycleSearch(ListGraph& graph){
    vector<int> parents(graph.VerticesCount());
    vector<int> components(graph.VerticesCount());
    for (size_t i=0; i<components.size(); ++i){
        components[i]=0;
    }

    vector<int> cycle;
    vector<int> contactsVert(1, -1);
    return CycleSearchInOneComp(graph, 0, components, cycle, contactsVert, parents);
}

void BridgeDelete(ListGraph& graph, vector<vector<int>>& deleted, int i, vector<int>& bridgeFunction, int& time, vector<int>& timeIn, vector<int>& visited, int p){
    timeIn[i]=bridgeFunction[i]=++time;

    vector<int> vertices;
    graph.GetNextVertices(i, vertices);

    for (int vertex: vertices){
        if (vertex==p){
            continue;
        }
        if (!visited[vertex]){
            visited[vertex]=true;
            BridgeDelete(graph, deleted, vertex, bridgeFunction, time, timeIn, visited, i);

            bridgeFunction[i]=min(bridgeFunction[i], bridgeFunction[vertex]);
            if (bridgeFunction[vertex]>timeIn[i]){
                deleted[i].push_back(vertex);
            }
        } else {
            bridgeFunction[i]=min(bridgeFunction[i], timeIn[vertex]);
        }
    }
}

void BridgeDelete(ListGraph& graph, vector<vector<int>>& deleted){
    vector<int> bridgeFunction(graph.VerticesCount());
    vector<int> visited(graph.VerticesCount());
    vector<int> timeIn(graph.VerticesCount());
    int time=-1;

    for (int i=0; i<graph.VerticesCount(); ++i){
        if (!visited[i]){
            visited[i]=true;
            BridgeDelete(graph, deleted, i, bridgeFunction, time, timeIn, visited, -1);
        }
    }
}

void BridgeDelete(ListGraph& graph){
    vector<vector<int>> deleted(graph.VerticesCount());
    BridgeDelete(graph, deleted);

    for (size_t i=0; i<deleted.size(); ++i){
        for (size_t j=0; j<deleted[i].size(); ++j){
            graph.RemoveEdge(i, deleted[i][j]);
        }
    }
}

void ComponentsCount(ListGraph& graph, int i, vector<int>& components, vector<bool>& visited, int& componentNumber){
    components[i]=componentNumber;

    vector<int> vertices;
    graph.GetNextVertices(i, vertices);

    for (int vertex: vertices){
        if (!visited[vertex]){
            visited[vertex]=true;
            ComponentsCount(graph, vertex, components, visited, componentNumber);
        }
    }
}

void ComponentsCount(ListGraph& graph, vector<int>& components, int& componentNumber){
    components.resize(graph.VerticesCount());
    vector<bool> visited(graph.VerticesCount());
    for (int i=0; i<graph.VerticesCount(); ++i){
        if (!visited[i]){
            visited[i]=true;
            ComponentsCount(graph, i, components, visited, componentNumber);
            ++componentNumber;
        }
    }
}

void SegmentsSearch(ListGraph& graph, int i, vector<int>& contactsVert, vector<vector<int>>& segments){
    vector<int> vertices;
    vector<int> vertex(2);
    graph.GetNextVertices(i, vertices);
    for (int vert: vertices){
        if (contactsVert[vert]==1 && vert>i){
            vertex[0]=i; vertex[1]=vert;
            segments.push_back(vertex);
        }
    }
}

void SegmentsSearch(ListGraph& graph, vector<bool>& vis, vector<int>& visited, int i, vector<int>& vert,  int numComp, vector<int>& components, vector<int>& contactsVert){
    visited[i]=true;
    vert.push_back(i);

    vector<int> vertices;
    graph.GetNextVertices(i, vertices);

    for (int vertex: vertices){
        if (components[vertex]==numComp){
            if (!visited[vertex] && contactsVert[vertex]==0){
                SegmentsSearch(graph, vis, visited, vertex, vert, numComp, components, contactsVert);
            } else if (contactsVert[vertex]==1){
                if (!vis[vertex]){
                    vis[vertex]=true;
                    vert.push_back(vertex);
                }
            }
        }
    }
}

void SegmentsSearch(ListGraph& graph, vector<vector<int>>& segments,  int numComp, vector<int>& components, vector<int>& contactsVert){
    vector<int> visited(graph.VerticesCount());
    for (int i=0; i<graph.VerticesCount(); ++i){
        vector<bool> vis(graph.VerticesCount());
        if (!visited[i] && components[i]==numComp){
            if (contactsVert[i]==1){
                SegmentsSearch(graph, i, contactsVert, segments);
            } else {
                vector<int> vert;
                SegmentsSearch(graph, vis, visited, i, vert, numComp, components, contactsVert);
                segments.push_back(vert);
            }
        }
    }
}

bool FindChain(ListGraph& graph, int i, vector<vector<int>>& deleted, vector<int>& segment, vector<int>& contactsVert, vector<int>& path, vector<int>& parents, vector<bool>& visited){
    visited[i]=true;
    path.push_back(i);

    vector<int> vertices;
    graph.GetNextVertices(i, vertices);

    for (int vertex: vertices){
        if (graph.IsVertBelongToSeg(vertex, segment)){
            deleted[i].push_back(vertex);
            if (!visited[vertex] && contactsVert[vertex]==0){
                parents[vertex]=i;
                if (FindChain(graph, vertex, deleted, segment, contactsVert, path, parents, visited)){
                    return true;
                }
            } else if (contactsVert[vertex]==1){
                if (!visited[vertex]){
                    path.push_back(vertex);
                    return true;
                } else if (visited[vertex] && parents[i]!=vertex){
                    path.push_back(vertex);
                    return true;
                }
            }
        }
    }
}

void FindChain(ListGraph& graph, vector<int>& segment, vector<int>& contactsVert, vector<int>& path, int contactVert){
    vector<bool> visited(graph.VerticesCount());
    vector<int> parents(graph.VerticesCount());
    vector<vector<int>> deleted(graph.VerticesCount());

    FindChain(graph, contactVert, deleted, segment, contactsVert, path, parents, visited);

    for (size_t i=0; i<deleted.size(); ++i){
        for (size_t j=0; j<deleted[i].size(); ++j){
            graph.RemoveEdge(i, deleted[i][j]);
        }
    }
}

void FacesForSegment(vector<vector<int>>& facesForSegment, vector<vector<int>>& contactsVertForSegment, vector<vector<int>>& faces, int i){
    bool result;
    for (size_t j=0; j<faces.size(); ++j){
        for (int vert: contactsVertForSegment[i]){
            auto point=find(faces[j].begin(), faces[j].end(), vert);
            if (point!=faces[j].end()){
                result=true;
            } else{
                result=false;
                break;
            }
        }
        if (result==true){
            facesForSegment[i].push_back(j);
        }
    }
}

void ChainLaying(vector<vector<int>>& faces, vector<vector<int>>& facesForSegment, vector<int>& chain, int minimum){
    vector<vector<int>> new_faces(2);

    vector<int> reversed=chain;
    reverse(reversed.begin(), reversed.end());

    int face=facesForSegment[minimum][0];
    size_t i=0;
    if (chain[0]!=chain[chain.size()-1]){
        for (int j=0; j<1; ++j){
            while (faces[face][i]!=chain[0] && faces[face][i]!=chain[chain.size()-1]){
                new_faces[j].push_back(faces[face][i]);
                ++i;
            }
            if (faces[face][i]==chain[0]){
                for (int vertex: chain){
                    new_faces[j].push_back(vertex);
                }
            } else if (faces[face][i]==chain[chain.size()-1]){
                for (int vertex: reversed){
                    new_faces[j].push_back(vertex);
                }
            }
        }

        new_faces[1].pop_back();
        ++i;
        while (i<faces[face].size()){
            new_faces[0].push_back(faces[face][i]);
            ++i;
        }

    } else {
        while (faces[face][i]!=chain[0] && faces[face][i]!=chain[chain.size()-1]){
            new_faces[0].push_back(faces[face][i]);
            ++i;
        }
        for (int vertex: chain){
            new_faces[0].push_back(vertex);
            new_faces[1].push_back(vertex);
        }
        new_faces[0].pop_back();
        new_faces[1].pop_back();
        ++i;
        while (i<faces[face].size()){
            faces[0].push_back(faces[face][i]);
            ++i;
        }
    }

    faces[face].clear();
    for (int vert: new_faces[1]){
        faces[face].push_back(vert);
    }
    faces.push_back(new_faces[0]);
}

bool One_Step_In_Gamma_Algorithm(vector<vector<int>>& faces, vector<int>& contactsVert, ListGraph& graph, int numComp, vector<int>& components){
    vector<vector<int>> segments;
    SegmentsSearch(graph, segments, numComp, components, contactsVert);

    while (!segments.empty()){
        vector<vector<int>> contactsVertForSegment(segments.size());
        vector<vector<int>> facesForSegment(segments.size());

        for (size_t i=0; i<segments.size(); ++i){
            for (int vert: segments[i]){
                if (contactsVert[vert]==1){
                    contactsVertForSegment[i].push_back(vert);
                }
            }
            FacesForSegment(facesForSegment, contactsVertForSegment, faces, i);

        }

        int minimum=0;
        for (size_t i=0; i<facesForSegment.size(); ++i){
            if (facesForSegment[i].size()<facesForSegment[minimum].size()){
                minimum=i;
            }
        }

        if (facesForSegment[minimum].size()==0){
            return false;
        }

        vector<int> chain;
        FindChain(graph, segments[minimum], contactsVert, chain, contactsVertForSegment[minimum][0]);

        for (int vert: chain){
            contactsVert[vert]=1;
        }

        ChainLaying(faces, facesForSegment, chain, minimum);

        segments.clear();
        SegmentsSearch(graph, segments, numComp, components, contactsVert);

    }
    return true;
}

bool Gamma_Algorithm(ListGraph& graph, vector<int>& components, int n, int i){
    vector<int> parents(n);
    vector<int> contactsVert(n, 0);
    vector<int> cycle;
    vector<vector<int>> faces;

    if (CycleSearchInOneComp(graph, i, components, cycle, contactsVert, parents)){
        faces.push_back(cycle);
        faces.push_back(cycle);

        if (!One_Step_In_Gamma_Algorithm(faces, contactsVert, graph, i, components)){
            return false;
        }
    }
    return true;
}

int main()
{
    int n,m;
    cin>>n>>m;
    ListGraph graph(n);
    for (int i=0; i<m; ++i){
        int from, to;
        cin>>from>>to;
        graph.Add(from, to);
    }

    if (!CycleSearch(graph)){
        cout<<"YES";
        return 0;
    }

    BridgeDelete(graph);

    vector<int> components(n);
    int componentNumber=0;
    ComponentsCount(graph, components, componentNumber);

    for (int i=0; i<componentNumber; ++i){
        if (!Gamma_Algorithm(graph, components, n, i)){
            cout<<"NO";
            return 0;
        }
    }

    cout<<"YES";
    return 0;
}
