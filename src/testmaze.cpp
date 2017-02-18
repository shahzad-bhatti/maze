/**
 * @file testmaze.cpp
 * Performs basic tests of maze.
 *
 * @date April 2007
 * @author Jonathan Ray
 */

#include <iostream>
#include "dsets.h"
#include "maze.h"
#include "epng.h"

int main()
{
    cs225::maze m{3, 5};
    std::cout << "constructor complete" << std::endl;

    auto unsolved = m.draw();
    unsolved.save("images/unsolved.png");
    std::cout << "draw complete" << std::endl;
/*
	m.set_wall(0, 0, cs225::direction::RIGHT, false);
	unsolved = m.draw();
    unsolved.save("images/unsolved1.png");
	m.set_wall(0, 0, cs225::direction::DOWN, false);

	unsolved = m.draw();
    unsolved.save("images/unsolved2.png");
	m.set_wall(1, 0, cs225::direction::RIGHT, false);
	unsolved = m.draw();
    unsolved.save("images/unsolved3.png");
	m.set_wall(1, 0, cs225::direction::DOWN, false);
	unsolved = m.draw();
    unsolved.save("images/unsolved4.png");
	m.set_wall(0, 1, cs225::direction::RIGHT, false);
	unsolved = m.draw();
    unsolved.save("images/unsolved5.png");
	m.set_wall(0, 1, cs225::direction::DOWN, false);
	unsolved = m.draw();
    unsolved.save("images/unsolved6.png");
	m.set_wall(1, 1, cs225::direction::RIGHT, false);
	unsolved = m.draw();
    unsolved.save("images/unsolved7.png");
	m.set_wall(1, 1, cs225::direction::DOWN, false);
	unsolved = m.draw();
    unsolved.save("images/unsolved8.png");
	m.set_wall(0, 0, cs225::direction::DOWN, true);
	unsolved = m.draw();
    unsolved.save("images/unsolved8.png");
*/	
    std::cout << "draw complete1" << std::endl;
	
	
    auto sol = m.solve();
    std::cout << "solve complete" << std::endl;

    auto solved = m.draw_with_solution();
    solved.save("images/solved.png");
    std::cout << "draw_with_solution complete" << std::endl;

    return 0;
}

