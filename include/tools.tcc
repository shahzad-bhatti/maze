/**
 * @file tools.tcc
 * Definition of utility functions on graphs.
 */

#include <limits>
#include <queue>

#include "dsets.h"
#include "tools.h"
#include "features/edge_weights.h"
namespace cs225
{
namespace tools
{

template <class Graph>
typename Graph::edge_weight_type min_weight_edge(Graph& g)
{
	// initializing min weight and edge with min weight
    using weight_t = typename Graph::edge_weight_type;
    weight_t min = std::numeric_limits<weight_t>::max();
    edge min_edge{g.start_vertex(), g.start_vertex()};

	// marking all vertices as UNEXPLORED
    vertex_map<vertex_state> vtx_mark;
    for (const auto& v : g.vertices())
        vtx_mark[v] = vertex_state::UNEXPLORED;

	// marking all edges as UNEXPLORED
	edge_map<edge_state> edg_mark;
    for (const auto& e : g.edges())
        edg_mark[e] = edge_state::UNEXPLORED;
	
	// This for loop makes sure that all vertices are visited even in multi-component graph.
	for (const auto& u: g.vertices())
	{
		// If statement checks for vertices in other components.
		const auto& mrk = vtx_mark[u];
		if (mrk == vertex_state::UNEXPLORED)
		{
			std::queue<vertex> q;
			q.push(u);
			vtx_mark[u] = vertex_state::VISITED;
			
			// BFS starts here.
			while (!q.empty())
			{
				auto v = q.front();
				q.pop();
				for (const auto& w: g.adjacent(v))
				{
					if (vtx_mark[w] == vertex_state::UNEXPLORED)
					{
						vtx_mark[w] = vertex_state::VISITED;
						edg_mark[{v,w}] = edge_state::DISCOVERY;
						q.push(w);					
					}
					else if (edg_mark[{v,w}] == edge_state::UNEXPLORED)
					{						
						edg_mark[{v,w}] = edge_state::CROSS;
					}
					
					// updating the min_edge and min weight.
					if (g.edge_weight({v,w}) < min)
					{
						min = g.edge_weight({v,w});
						min_edge = {v,w};
					}		
				}
			}
		}	
	}
	g.edge_label(min_edge, edge_state::MIN);  // setting MIN label
	return min;
}

template <class Graph>
uint64_t shortest_path_length(Graph& g, vertex start, vertex end)
{
	// initializing vertex state and vertex distance from start.
    vertex_map<vertex_state> vtx_mark;
    vertex_map<int> vtx_dist;
    for (const auto& v : g.vertices())
    {
        vtx_mark[v] = vertex_state::UNEXPLORED;
        vtx_dist[v] = -1;
	}
	
	// marking all edges as UNEXPLORED
	edge_map<edge_state> edg_mark;
    for (const auto& e : g.edges())
        edg_mark[e] = edge_state::UNEXPLORED;
	
	std::queue<vertex> q;
	q.push(start);
	vtx_mark[start] = vertex_state::VISITED;
	vtx_dist[start] = 0;			
	bool end_reached = false;
	
	//BFS starts here.
	while (!q.empty())
	{
		auto v = q.front();
		q.pop();
		for (const auto& w: g.adjacent(v))
		{
			auto mk = vtx_mark[w];
			if (mk == vertex_state::UNEXPLORED)
			{
				vtx_mark[w] = vertex_state::VISITED;
				edg_mark[{v,w}] = edge_state::DISCOVERY;
				q.push(w);
				vtx_dist[w] = vtx_dist[v] + 1;					
			}
			else if (edg_mark[{v,w}] == edge_state::UNEXPLORED)
				edg_mark[{v,w}] = edge_state::CROSS;
			if (w == end) 		// if end vertex is reached break out of the loops.
			{
				end_reached = true;	
				break;
			}					
		}
		if (end_reached)
			break;
	}
	// if end vertex does not exist or is in another component throw an exception
	if (vtx_dist[end] == -1)
		throw std::runtime_error{"end is not reachable from start."};
	
	// labeling MINPATH edges.
	auto curr = end;	
	while(curr !=start)
	{
		for (const auto& v: g.adjacent(curr))
		{
			if (edg_mark[{v,curr}] == edge_state::DISCOVERY && vtx_dist[curr] - vtx_dist[v] == 1)
			{
				g.edge_label({v,curr}, edge_state::MINPATH);
				curr = v;
				break;
			}
		}
	}
	return vtx_dist[end];
}

template <class Graph>
void mark_mst(Graph& g)
{
    for (const auto& e : minimum_spanning_tree(g))
        g.edge_label(e, edge_state::MST);
}

template <class Graph>
edge_set minimum_spanning_tree(const Graph& g)
{
    // sort all the edges
    struct weighted_edge
    {
        edge arc;
        typename Graph::edge_weight_type weight;
    };
    std::vector<weighted_edge> pq;
    for (const auto& e : g.edges())
        pq.push_back({e, g.edge_weight(e)});
    std::sort(pq.begin(), pq.end(),
              [&](const weighted_edge& e1, const weighted_edge& e2)
              {
                  return e1.weight < e2.weight;
              });

//	edge_map<edge_state> edg_mark;
//    for (const auto& e : g.edges())
//        edg_mark[e] = edge_state::UNEXPLORED;

	// Mapping each vertex a unique number. This number is used as a reference for disjoint sets.
	vertex_map<uint64_t> vtx_number;
	uint64_t numb = 0;
	for (const auto& v : g.vertices())
        vtx_number[v] = numb++;

    edge_set ret;		// edge_set to be returned
    dsets vtx_set;
    vtx_set.add_elements(numb);
    uint64_t mst_edgs = 0;		// number of edges in the mst. 
    for (const auto& pair : pq)
    {
    	if (vtx_set.find(pair.arc.source) != vtx_set.find(pair.arc.dest))
    	{
    		vtx_set.merge(vtx_number[pair.arc.source], vtx_number[pair.arc.dest]);
//    		edg_mark[pair.arc] = edge_state::MST;
    		ret.insert(pair.arc);
    		++mst_edgs;	
    	}
    	if (mst_edgs == numb-1) 	// if number of edges are equal to |V| - 1, break. 
    	 	break;    
    }
	return ret;

	
}
}
}
