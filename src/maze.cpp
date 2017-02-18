/**
 * @file maze.cpp
 * Definition of a maze class.
 *
 * @author Chase Geigle
 * @date Summer 2014
 */

#include <random>

#include "tools.h"
//#include "tools_provided.tcc"
#include "maze.h"
#include <iostream>
namespace cs225
{

std::ostream& operator<<(std::ostream& os, direction dir)
{
    switch (dir)
    {
        case direction::RIGHT:
            return os << "RIGHT";
        case direction::DOWN:
            return os << "DOWN";
        case direction::LEFT:
            return os << "LEFT";
        case direction::UP:
            return os << "UP";
    }
}

maze::maze(uint64_t width, uint64_t height) 
	: width_{width}, height_{height}
{
	std::vector<edge> edges;
   	for (uint64_t i = 0; i < width_ * height_; ++i)
       	vertex_.push_back(graph_.add_vertex());
        
    for (uint64_t i = 0; i < width_ * height_ - 1 ; ++i)
    {
		if (i < (height_ - 1) * width_)
			edges.push_back(graph_.add_edge(vertex_[i], vertex_[i+width_]));   		
    	
    	if (i % (width_) != width_ - 1)
    		edges.push_back(graph_.add_edge(vertex_[i], vertex_[i+1]));   			
    }

    std::random_device rd;
    std::mt19937_64 rng{rd()};
    std::shuffle(edges.begin(), edges.end(), rng);
    
    dsets vtx_set;
    vtx_set.add_elements(height_ * width);

	vertex_map<uint64_t> vtx_number;
	uint64_t numb = 0;
	for (const auto& v : vertex_)
        vtx_number[v] = numb++;

//	cs225::tools::save_png(graph_, "temp");

    
    for (size_t i = 0; i < edges.size(); ++i)
    {
    	if (vtx_set.find(edges[i].source) != vtx_set.find(edges[i].dest))
    		vtx_set.merge(vtx_number[edges[i].source], vtx_number[edges[i].dest]);
		else
			graph_.remove_edge(edges[i].source, edges[i].dest);
    }
//    cs225::tools::save_png(graph_, "temp2");
    
}

bool maze::can_travel(uint64_t x, uint64_t y, direction dir) const
{
	if (x >= width_ || y >= height_)
		throw std::out_of_range{"Index out of bounds."};
		
    if (x == 0 && y == 0 && dir == direction::UP)
    	return true;
    
    if (dir == direction::RIGHT  &&  x!=width_ - 1)
   		return graph_.has_edge(vertex_[y*width_ + x], vertex_[y*width_ + x + 1]);	
    
    else if (dir == direction::DOWN && y!=height_ -1)
    	return graph_.has_edge(vertex_[y*width_ + x], vertex_[y*width_ + x + width_]);	
    
    else if (dir == direction::LEFT && x!=0)
    	return graph_.has_edge(vertex_[y*width_ + x], vertex_[y*width_ + x - 1]);	
    
    else if (dir == direction::UP && y!=0)
		return graph_.has_edge(vertex_[y*width_ + x], vertex_[y*width_ + x - width_]);	
    
    else
        return false;
}

void maze::set_wall(uint64_t x, uint64_t y, direction dir, bool exists)
{
	auto idx = x + (y*width_);		// index of the vertex.
	if (!exists)	// i.e if the wall doesn't exist, then add an edge.
	{
	    if (dir == direction::RIGHT && (x != width_ - 1) && 
	    		!graph_.has_edge(vertex_[idx], vertex_[idx+1]))
	       	graph_.add_edge(vertex_[idx], vertex_[idx+1]);	
	    
	    else if (dir == direction::DOWN && idx < (height_ - 1) * width_  &&
	    			!graph_.has_edge(vertex_[idx], vertex_[idx + width_]))
	       	graph_.add_edge(vertex_[idx], vertex_[idx + width_]);	
	}
	else	// if the wall exists, then remove the edge.
	{
		if (dir == direction::RIGHT && (idx % width_ != width_ - 1) &&
	    			graph_.has_edge(vertex_[idx], vertex_[idx+1]))
    		graph_.remove_edge(vertex_[idx], vertex_[idx+1]);	

	    else if (dir == direction::DOWN && idx < (height_ - 1) * width_ &&
					graph_.has_edge(vertex_[idx], vertex_[idx + width_]))
	    	graph_.remove_edge(vertex_[idx], vertex_[idx + width_]);	
	}
}

std::vector<direction> maze::solve()
{
// initializing vertex state and vertex distance from start.
    vertex_map<int> vtx_dist;
    for (const auto& v : vertex_)
        vtx_dist[v] = -1;
		
	std::queue<vertex> q;
	q.push(vertex_[0]);
	vtx_dist[vertex_[0]] = 0;			
	
	//BFS starts here.
	while (!q.empty())
	{
		auto v = q.front();
		q.pop();
		for (const auto& w: graph_.adjacent(v))
		{
			if (vtx_dist[w] == -1)
			{
				q.push(w);
				vtx_dist[w] = vtx_dist[v] + 1;					
			}
		}				
	}
	// checking if all vertices are visited. delete this
	for (const auto& v : vertex_)
		if (vtx_dist[v] == -1)
			throw std::runtime_error{"all vertices are not visited"};
	
	// finding the vertex with largest distance.
	int max = -1;
	auto max_vertex = vertex_[0];
	for (uint64_t i = (height_ - 1) * width_; i < height_*width_; ++i)
	{
		if (vtx_dist[vertex_[i]] > max)
		{
			max = vtx_dist[vertex_[i]];
			max_vertex = vertex_[i];
		}	
	}
	
	// populating the return vetor ret.	
	std::vector<direction> ret{static_cast<size_t>(max)};
	size_t counter = max;
	auto curr = max_vertex;	
	while(curr !=vertex_[0])
	{
		for (const auto& v: graph_.adjacent(curr))
		{
			if (vtx_dist[curr] - vtx_dist[v] == 1)
			{
				if (v-1 == curr)
					ret[--counter] = direction::LEFT;
				else if (v+1 == curr && width_ > 1)
					ret[--counter] = direction::RIGHT;
				else if (v-width_ == curr)
					ret[--counter] = direction::UP;
				else if (v+width_ == curr)
					ret[--counter] = direction::DOWN;			 
				curr = v;
				break;
			}
		}
	}
	return ret;
}

epng::png maze::draw() const
{
	epng::rgba_pixel black{0, 0, 0};
	epng::png ret{width_*10 + 1, height_*10 + 1};
	
	for (size_t i = 10; i < ret.width(); ++i) 	//drawing top wall
		*ret(i,0) = black;

	for (size_t i = 0; i < ret.width(); ++i)	//drawing bottom wall
		*ret(i, ret.height()-1) = black;

	for (size_t i = 0; i < ret.height(); ++i)  	//drawing left and right wall
	{
		*ret(0,i) = black;
		*ret(ret.width()-1, i) = black;
	}
	
	for (uint64_t i = 0; i < width_ * height_ - 1 ; ++i)
    {
		if (i < (height_ - 1) * width_)
		{
			if (!graph_.has_edge(vertex_[i], vertex_[i+width_]))
			{
				for (size_t k = 0; k <= 10; ++ k)
					*ret( (i%width_)*10 + k, ((i/width_)+1)*10 ) = black;
			}			   		
    	}
    	if (i % width_ != width_ - 1)
    	{
			if (!graph_.has_edge(vertex_[i], vertex_[i+1]))
			{
				for (size_t k = 0; k <= 10; ++k)
					*ret( ((i%width_)+1)*10 , (i/width_)*10 + k ) = black;
			}    	
    	}
    }		
	return ret;	
}

epng::png maze::draw_with_solution()
{
//    std::cout << __func__ << " : " << __LINE__ << std::endl;
    auto result = draw();
    auto soln = solve();
    epng::rgba_pixel red{255, 0, 0};
    size_t x = 5;
    size_t y = 5;
    for(auto d : soln)
    {
      	if (d == direction::RIGHT)
    	{
    		for (size_t k = 0; k <= 10; ++k)
    			*result(x+k,y) = red;
			x+=10;
	   	}
    	else if (d == direction::LEFT)
    	{
    		for (size_t k = 0; k <= 10; ++k)
    			*result(x-k,y) = red;
			x-=10;    		
	   	}
    	else if (d == direction::DOWN)
    	{
    		for (size_t k = 0; k <= 10; ++k)
    			*result(x,y+k) = red;
    		y+=10;
       	}
    	else if (d == direction::UP)
    	{
    		for (size_t k = 0; k <= 10; ++k)
    			*result(x,y-k) = red;
    		y-=10;    			
       	}
    }
    for (int k = -4; k<=4; ++k)
    	*result(x-k,y+5) = epng::rgba_pixel{255, 255, 255};

    return result;
}

}	//end namespace
