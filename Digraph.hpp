// Digraph.hpp
//
// ICS 46 Winter 2021
// Project #5: Rock and Roll Stops the Traffic
//
// This header file declares a class template called Digraph, which is
// intended to implement a generic directed graph.  The implementation
// uses the adjacency lists technique, so each vertex stores a linked
// list of its outgoing edges.
//
// Along with the Digraph class template is a class DigraphException
// and a couple of utility structs that aren't generally useful outside
// of this header file.
//
// In general, directed graphs are all the same, except in the sense
// that they store different kinds of information about each vertex and
// about each edge; these two types are the type parameters to the
// Digraph class template.

#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <exception>
#include <functional>
#include <list>
#include <map>
#include <utility>
#include <vector>
#include <queue>
#include <limits>

// DigraphExceptions are thrown from some of the member functions in the
// Digraph class template, so that exception is declared here, so it
// will be available to any code that includes this header file.

class DigraphException : public std::runtime_error
{
public:
    DigraphException(const std::string& reason);
};


inline DigraphException::DigraphException(const std::string& reason)
    : std::runtime_error{reason}
{
}

// A DigraphEdge lists a "from vertex" (the number of the vertex from which
// the edge points), a "to vertex" (the number of the vertex to which the
// edge points), and an EdgeInfo object.  Because different kinds of Digraphs
// store different kinds of edge information, DigraphEdge is a struct template.

template <typename EdgeInfo>
struct DigraphEdge
{
    int fromVertex;
    int toVertex;
    EdgeInfo einfo;
};

// A DigraphVertex includes two things: a VertexInfo object and a list of
// its outgoing edges.  Because different kinds of Digraphs store different
// kinds of vertex and edge information, DigraphVertex is a struct template.

template <typename VertexInfo, typename EdgeInfo>
struct DigraphVertex
{
    VertexInfo vinfo;
    std::list<DigraphEdge<EdgeInfo>> edges;
};

// Digraph is a class template that represents a directed graph implemented
// using adjacency lists.  It takes two type parameters:
//
// * VertexInfo, which specifies the kind of object stored for each vertex
// * EdgeInfo, which specifies the kind of object stored for each edge
//
// You'll need to implement the member functions declared here; each has a
// comment detailing how it is intended to work.
//
// Each vertex in a Digraph is identified uniquely by a "vertex number".
// Vertex numbers are not necessarily sequential and they are not necessarily
// zero- or one-based.

template <typename VertexInfo, typename EdgeInfo>
class Digraph
{
public:
    // The default constructor initializes a new, empty Digraph so that
    // contains no vertices and no edges.
    Digraph();

    // The copy constructor initializes a new Digraph to be a deep copy
    // of another one (i.e., any change to the copy will not affect the
    // original).
    Digraph(const Digraph& d);

    // The move constructor initializes a new Digraph from an expiring one.
    Digraph(Digraph&& d) noexcept;

    // The destructor deallocates any memory associated with the Digraph.
    ~Digraph() noexcept;

    // The assignment operator assigns the contents of the given Digraph
    // into "this" Digraph, with "this" Digraph becoming a separate, deep
    // copy of the contents of the given one (i.e., any change made to
    // "this" Digraph afterward will not affect the other).
    Digraph& operator=(const Digraph& d);

    // The move assignment operator assigns the contents of an expiring
    // Digraph into "this" Digraph.
    Digraph& operator=(Digraph&& d) noexcept;

    // vertices() returns a std::vector containing the vertex numbers of
    // every vertex in this Digraph.
    std::vector<int> vertices() const;

    // edges() returns a std::vector of std::pairs, in which each pair
    // contains the "from" and "to" vertex numbers of an edge in this
    // Digraph.  All edges are included in the std::vector.
    std::vector<std::pair<int, int>> edges() const;

    // This overload of edges() returns a std::vector of std::pairs, in
    // which each pair contains the "from" and "to" vertex numbers of an
    // edge in this Digraph.  Only edges outgoing from the given vertex
    // number are included in the std::vector.  If the given vertex does
    // not exist, a DigraphException is thrown instead.
    std::vector<std::pair<int, int>> edges(int vertex) const;

    // vertexInfo() returns the VertexInfo object belonging to the vertex
    // with the given vertex number.  If that vertex does not exist, a
    // DigraphException is thrown instead.
    VertexInfo vertexInfo(int vertex) const;

    // edgeInfo() returns the EdgeInfo object belonging to the edge
    // with the given "from" and "to" vertex numbers.  If either of those
    // vertices does not exist *or* if the edge does not exist, a
    // DigraphException is thrown instead.
    EdgeInfo edgeInfo(int fromVertex, int toVertex) const;

    // addVertex() adds a vertex to the Digraph with the given vertex
    // number and VertexInfo object.  If there is already a vertex in
    // the graph with the given vertex number, a DigraphException is
    // thrown instead.
    void addVertex(int vertex, const VertexInfo& vinfo);

    // addEdge() adds an edge to the Digraph pointing from the given
    // "from" vertex number to the given "to" vertex number, and
    // associates with the given EdgeInfo object with it.  If one
    // of the vertices does not exist *or* if the same edge is already
    // present in the graph, a DigraphException is thrown instead.
    void addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo);

    // removeVertex() removes the vertex (and all of its incoming
    // and outgoing edges) with the given vertex number from the
    // Digraph.  If the vertex does not exist already, a DigraphException
    // is thrown instead.
    void removeVertex(int vertex);

    // removeEdge() removes the edge pointing from the given "from"
    // vertex number to the given "to" vertex number from the Digraph.
    // If either of these vertices does not exist *or* if the edge
    // is not already present in the graph, a DigraphException is
    // thrown instead.
    void removeEdge(int fromVertex, int toVertex);

    // vertexCount() returns the number of vertices in the graph.
    int vertexCount() const noexcept;

    // edgeCount() returns the total number of edges in the graph,
    // counting edges outgoing from all vertices.
    int edgeCount() const noexcept;

    // This overload of edgeCount() returns the number of edges in
    // the graph that are outgoing from the given vertex number.
    // If the given vertex does not exist, a DigraphException is
    // thrown instead.
    int edgeCount(int vertex) const;

    // isStronglyConnected() returns true if the Digraph is strongly
    // connected (i.e., every vertex is reachable from every other),
    // false otherwise.
    bool isStronglyConnected() const;

    // findShortestPaths() takes a start vertex number and a function
    // that takes an EdgeInfo object and determines an edge weight.
    // It uses Dijkstra's Shortest Path Algorithm to determine the
    // shortest paths from the start vertex to every other vertex
    // in the graph.  The result is returned as a std::map<int, int>
    // where the keys are vertex numbers and the value associated
    // with each key k is the precedessor of that vertex chosen by
    // the algorithm.  For any vertex without a predecessor (e.g.,
    // a vertex that was never reached, or the start vertex itself),
    // the value is simply a copy of the key.
    std::map<int, int> findShortestPaths(
        int startVertex,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const;


private:
    // Add whatever member variables you think you need here.  One
    // possibility is a std::map where the keys are vertex numbers
    // and the values are DigraphVertex<VertexInfo, EdgeInfo> objects.


    // You can also feel free to add any additional member functions
    // you'd like (public or private), so long as you don't remove or
    // change the signatures of the ones that already exist.

    std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> graph;
   
    void depthFirst(VertexInfo vertex, std::vector<int>& tracker) const;
};



// You'll need to implement the member functions below.  There's enough
// code in place to make them compile, but they'll all need to do the
// correct thing instead.

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

#endif // DIGRAPH_HPP
