/**
 * @file dsets.h
 * Declaration of the dsets class.
 *
 * @author Shahzad Bhatti	
 * @date Summer 2015
 */

#ifndef CS225_DSETS_H_
#define CS225_DSETS_H_

#include <cstdint>
#include <vector>

/*
* Each dsets object represents a family of disjoint sets, where each element has an integer index.
* It is implemented with as an up-trees stored in a single vector of ints. Specifically, path 
* compression and union-by-size are used. Each place in the vector represents a node. 
* (Note that this means that the elements in our universe are indexed starting at 0.)
* A nonnegative number is the index of the parent of the current node; a negative number in a 
* root node is the negative of the set size.
*/

class dsets{

  public:
  	/*
  	* No constructor is provided for the class, system provided default constructors will be used.
  	*/
  
  	/*
  	* Explicitly stating that default destructor will be used.
  	*/
	~dsets() = default;

	/*
	* Creates n unconnected root nodes at the end of the vector. 
	* @Parameters num - The number of nodes to create
	*/
	void add_elements(uint64_t num);
	
	/*
	* This function should compress paths and works as described in lecture.
	* @Returns the index of the root of the up-tree in which the parameter element resides.
	*/
	uint64_t find(uint64_t elem);	

	/*
	* This function is implemented using union-by-size.
	* This function finds the roots of its arguments before combining the trees. If the two sets are the
	* same size, breaks the tie by making the root of the second argument point at the root of the 		 		* first. 
	* @Parameters
	* a	Index of the first element to union
	* b	Index of the second element to union
	*/	
	void merge(uint64_t	a, uint64_t	b);
	  	std::vector<int64_t> arr_;	//The storage vector for sets. 	
  private:

};

#endif

