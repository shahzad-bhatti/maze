/**
 * @file dsets.cpp
 * Definition of a dsets class.
 *
 * @author Shahzad Bhatti
 * @date Summer 2015
 */

#include "dsets.h"

void dsets::add_elements(uint64_t num)
{
	for (uint64_t i = 0; i < num ; ++i )
		arr_.push_back(-1);
}
	
uint64_t dsets::find(uint64_t elem)
{
	if (arr_[elem] < 0)
		return elem;
	
	return arr_[elem] = find(arr_[elem]);	
}

void dsets::merge(uint64_t	a, uint64_t	b)
{
	auto root1 = find(a);
	auto root2 = find(b);
	if (root1 == root2) 	// if in the same set
		return;
	if (arr_[root1] <= arr_[root2])
	{
		arr_[root1] += arr_[root2];
		arr_[root2] = root1;
	}	
	else
	{
		arr_[root2] += arr_[root1];
		arr_[root1] = root2;
	}	
}


