// Directed_Graph.hpp
#ifndef DIRECTED_GRAPH_HPP
#define DIRECTED_GRAPH_HPP

#include <exception>
#include <functional>
#include <list>
#include <map>
#include <utility>
#include <vector>
#include <queue>
#include <limits>

class DigraphException : public std::runtime_error
{
public:
    DigraphException(const std::string& reason);
};


inline DigraphException::DigraphException(const std::string& reason)
    : std::runtime_error{reason}
{
}


template <typename EdgeInfo>
struct DigraphEdge
{
    int fromVertex;
    int toVertex;
    EdgeInfo einfo;
};


template <typename VertexInfo, typename EdgeInfo>
struct DigraphVertex
{
    VertexInfo vinfo;
    std::list<DigraphEdge<EdgeInfo>> edges;
};


template <typename VertexInfo, typename EdgeInfo>
class Digraph
{
public:

    Digraph();
    ~Digraph() noexcept;
    
    Digraph(const Digraph& d);
    Digraph(Digraph&& d) noexcept;
    
    Digraph& operator=(const Digraph& d);
    Digraph& operator=(Digraph&& d) noexcept;

    std::vector<int> vertices() const;
    std::vector<std::pair<int, int>> edges() const;
    std::vector<std::pair<int, int>> edges(int vertex) const;
    
    VertexInfo vertexInfo(int vertex) const;
    EdgeInfo edgeInfo(int fromVertex, int toVertex) const;
    
    void addVertex(int vertex, const VertexInfo& vinfo);
    void addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo);

    void removeVertex(int vertex);
    void removeEdge(int fromVertex, int toVertex);

    int vertexCount() const noexcept;
    int edgeCount() const noexcept;
    int edgeCount(int vertex) const;
    
    bool isStronglyConnected() const;
    std::map<int, int> findShortestPaths(
        int startVertex,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const;
    
private:
    std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> graph;
    void depthFirst(VertexInfo vertex, std::vector<int>& tracker) const;
};


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph()
{
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(const Digraph& d)
    : graph{ d.graph }
{
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(Digraph&& d) noexcept
{
    std::swap(graph, d.graph);
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::~Digraph() noexcept
{
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(const Digraph& d)
{
    if(this != &d) {
        
        graph.clear();
        std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> temp(d.graph);
        graph = temp;
    }

    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(Digraph&& d) noexcept
{
    if(this != &d) {
        
        std::swap(graph, d.graph);
    }

    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<int> Digraph<VertexInfo, EdgeInfo>::vertices() const
{
    std::vector<int> vertices;

    for(auto i = graph.begin(); i != graph.end(); ++i) {
        
        vertices.push_back(i->first);
    }

    return vertices;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges() const
{
    std::vector<std::pair<int, int>> edges;

    for(auto i = graph.begin(); i != graph.end(); ++i) {
        
        for(auto j = i->second.edges.begin(); j != 
            i->second.edges.end(); ++j) {
            
            std::pair<int, int> current_edge(j->fromVertex, j->toVertex);
            edges.push_back(current_edge);
        }
    }
    
    return edges;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::
    edges(int vertex) const
{
    if(graph.count(vertex) == 0) {
        
        throw DigraphException{ "Vertex does not exist!" };
    }

    std::vector<std::pair<int, int>> edges_;
    for(auto i = graph.begin(); i != graph.end(); ++i) {

        if(i->first == vertex) {
            
            //For any existing edges, add them to the vector.
            for(auto j = i->second.edges.begin(); j !=
                i->second.edges.end(); ++j) {    

                std::pair<int, int> current_edge(j->fromVertex, j->toVertex); 
                edges_.push_back(current_edge);
            }
        }
    }

    return edges_;
}


template <typename VertexInfo, typename EdgeInfo>
VertexInfo Digraph<VertexInfo, EdgeInfo>::vertexInfo(int vertex) const
{
    if(graph.count(vertex) == 0) {
        
        throw DigraphException{ "Vertex does not exist!" };
    }

    return graph.at(vertex).vinfo;
}


template <typename VertexInfo, typename EdgeInfo>
EdgeInfo Digraph<VertexInfo, EdgeInfo>::edgeInfo(int fromVertex, int toVertex) const
{
    if(graph.count(fromVertex) == 0 || graph.count(toVertex) == 0) {
        
        throw DigraphException{ "Vertice(s) do not exist!" };
    }
    
    for(auto i = graph.begin(); i != graph.end(); ++i) {
        
        if(i->first == fromVertex) {
            
            for(auto j = i->second.edges.begin(); j != 
                i->second.edges.end(); ++j) {
                    
                if(j->toVertex == toVertex) {
                    
                    return j->einfo;
                }
            }
        }
    }
    
    throw DigraphException{ "Edge does not exist!" };
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addVertex(int vertex, const VertexInfo& vinfo)
{
    DigraphVertex<VertexInfo, EdgeInfo> current_vertex{ vinfo };
    
    for(auto i = graph.begin(); i != graph.end(); ++i) {
        
        if(i->first == vertex) {
            
            throw DigraphException{"Vertex already exists!"};
        }
    }
    
    graph.insert(std::pair<int, DigraphVertex<VertexInfo, 
        EdgeInfo>>(vertex, current_vertex));
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addEdge(int fromVertex, 
    int toVertex, const EdgeInfo& einfo)
{
    if(graph.count(fromVertex) == 0 || graph.count(toVertex) == 0) {
        
        throw DigraphException{ "Vertice(s) do not exist!" };
    }

    for(auto i = graph.begin(); i != graph.end(); ++i) {
        
        if(i->first == fromVertex) {
            
            for(auto j = i->second.edges.begin(); j != 
                i->second.edges.end(); ++j) {
                    
                if(j->toVertex == toVertex) {
                    
                    throw DigraphException{ "Edge already exists!" };
                }
            }
            
            //Add a DigraphEdge struct to the graph.
            i->second.edges.push_back(DigraphEdge<EdgeInfo>{
                fromVertex, toVertex, einfo });
        }
    }
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeVertex(int vertex)
{
    if(graph.count(vertex) == 0) {
        
        throw DigraphException{ "Vertex does not exist!" };
    }

    for(auto i = graph.begin(); i != graph.end(); ++i) {

        if(i->first == vertex) {

            i->second.edges.clear(); //Removes all out-degree edges
        }                           //from the vertex.

        for(auto j = i->second.edges.begin(); j != 
            i->second.edges.end(); ++j) {
            
            if(j->toVertex == vertex) {
                
                j = i->second.edges.erase(j); //Removes all in-degree edges
            }                                //to the vertex.
        }
    }

    graph.erase(vertex); //Removes the vertex itself.
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeEdge(int fromVertex, int toVertex)
{
    if(graph.count(fromVertex) == 0 || graph.count(toVertex) == 0) {
        
        throw DigraphException{ "Vertice(s) do not exist!" };
    }
    
    bool edgeExists = false;
    for(auto i = graph.begin(); i != graph.end(); ++i) {
        
        if(i->first == fromVertex) {
            
            for(auto j = i->second.edges.begin(); j !=
                i->second.edges.end(); ++j) {
                
                if(j->toVertex == toVertex) {
                    
                    j = i->second.edges.erase(j);
                    edgeExists = true;
                }
            }
        }
    }

    if(edgeExists == false) {
        
        throw DigraphException{ "Edge does not exist!"};
    }
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::vertexCount() const noexcept
{
    return graph.size();
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount() const noexcept
{
    unsigned int count = 0;
    for(auto i = graph.begin(); i != graph.end(); ++i) {
        
        for(auto j = i->second.edges.begin(); j !=
            i->second.edges.end(); ++j) {
            
            count++;
        }
    }
    
    return count;
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount(int vertex) const
{
    if(graph.count(vertex) == 0) {
        
        throw DigraphException{ "Vertex does not exist!" };
    }

    unsigned int count = 0;
    for(auto i = graph.begin(); i != graph.end(); ++i) {
        
        if(i->first == vertex) {
            
            for(auto j = i->second.edges.begin(); j !=
                i->second.edges.end(); ++j) {
                
                count++;
            }
        }
    }

    return count;
}


template <typename VertexInfo, typename EdgeInfo>
bool Digraph<VertexInfo, EdgeInfo>::isStronglyConnected() const
{
    //For each vertex in the graph.
    for(auto i = graph.begin(); i != graph.end(); ++i) {
        
        std::vector<int> tracker;
        depthFirst(i->first, tracker);

        if(vertexCount() != tracker.size()) {
            
            return false;
        }
    }

    return true;
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::depthFirst(VertexInfo vertex, 
    std::vector<int>& tracker) const
{
    bool strong_connected = false;

    for(auto i = graph.begin(); i != graph.end(); ++i) {
        
        if(i->first == vertex) {
        
            for(unsigned int j = 0; j < tracker.size(); ++j) {
            
                if(tracker.at(j) == vertex) {
                    
                    strong_connected = true;
                }
            }

            if(strong_connected == false) {
                
                tracker.push_back(i->first);

                //For each vertex said vertex points to.
                for(auto k = i->second.edges.begin(); k !=
                    i->second.edges.end(); ++k) {
                    
                    //Recursive.
                    depthFirst(k->toVertex, tracker);
                }
            }
        }
    }
}


template <typename VertexInfo, typename EdgeInfo>
std::map<int, int> Digraph<VertexInfo, EdgeInfo>::findShortestPaths(
    int startVertex,
    std::function<double(const EdgeInfo&)> edgeWeightFunc) const
{
    //Indicates whether the shortest path to said vertex is known.
    std::vector<bool> kv(vertexCount() + 1, false);
    //Tracks the preceeding vertex from the starting vertex to said vertex.
    std::map<int, int> pv;
    //Tracks the weights/distance of shortest path.
    std::vector<int> dv(vertexCount() + 1, 2147000000);

    //'-1' represents unknown.
    for(auto i = graph.begin(); i != graph.end(); ++i) {

        pv.insert(std::pair<int, int>(i->first, -1));
    }
    
    pv.at(startVertex) = startVertex;
    dv.at(startVertex) = 0;
    
    auto compare = [] (std::pair<int, double> left, std::pair<int, double>
        right) { return left.second > right.second; };
    std::priority_queue<std::pair<int, double>, std::vector<std::pair<int,
        double>>, decltype(compare)> pq(compare);

    pq.push(std::make_pair(0, startVertex));
    
    while(!pq.empty()) {
        
        int vertex = pq.top().second;
        pq.pop();

        if(kv.at(vertex) == false) {
            
            kv.at(vertex) = true;

            for(auto i = graph.begin(); i != graph.end(); ++i) {
                
                if(i->first == vertex) {
                    
                    for(auto j = i->second.edges.begin(); j !=
                        i->second.edges.end(); ++j) {
                        
                        if(dv.at(j->toVertex) > dv.at(vertex) + 
                            edgeWeightFunc(j->einfo)) {
                            
                            dv.at(j->toVertex) = dv.at(vertex) +
                                edgeWeightFunc(j->einfo);
                            
                            std::map<int, int>::iterator it = 
                                pv.find(j->toVertex);
                                
                            if(it != pv.end()) {
                                    
                                it->second = vertex;
                            }
                           
                            pq.push(std::make_pair(dv.at(j->toVertex), 
                                j->toVertex));
                        }
                    }
                }
            }
        }
    }
  
    //Any vertex w/o a predecessor, the value is the copy of the vertex.
    for(auto i = pv.begin(); i != pv.end(); ++i) {
        
        if(i->second == -1) {
            
            i->second = i->first;
        }
    }

    return pv;
}

#endif // DIRECTED_GRAPH_HPP
