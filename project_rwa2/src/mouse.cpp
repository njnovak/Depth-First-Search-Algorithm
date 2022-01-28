/**
 * @file mouse.cpp
 * @author Jerry Pittman, Jr., Nicholas Novak, Orlandis Smith
 *  (jpittma1@umd.edu, nnovak@umd.edu, osmith15@umd.edu)
 * @brief RWA2_Group_2_2
 * @version 3.0
 * @date 2021-11-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "../include/mouse/mouse.h"
#include "../include/node/node.h"
#include "../include/api/api.h"  
#include "../include/util/util.h"
#include <string>
#include <stack>
#include <vector>
#include <array>
#include <utility>
#include <algorithm>
#include <iostream>

void rwa2::Mouse::display_walls() { //Given Function
    for (int x = 0; x < m_maze_width; x += 1) {
        for (int y = 0; y < m_maze_height; y += 1) {
            if (m_maze.at(x).at(y).is_wall(direction::NORTH)) {
                //display this wall in the simulator
                API::setWall(x, y, 'n');
            }
                
            if (m_maze.at(x).at(y).is_wall(direction::EAST)) {
                //display this wall in the simulator
                API::setWall(x, y, 'e');
            }
                
            if (m_maze.at(x).at(y).is_wall(direction::SOUTH)) {
                //display this wall in the simulator
                API::setWall(x, y, 's');
            }
                
            if (m_maze.at(x).at(y).is_wall(direction::WEST)) {
                //display this wall in the simulator
                API::setWall(x, y, 'w');
            }
            //display the number of walls surrounding the current node
            API::setText(x, y, std::to_string(m_maze.at(x).at(y).compute_number_of_walls()));
        }
    }
}

bool rwa2::Mouse::node_exists(std::vector<std::array<int,2>> n_list, std::array<int,2> c_node){
	// Use this to see if a node has been previously visitied in the vector of nodes, n_list.
    for(int i = 0; i < n_list.size(); i++){
		if(n_list[i] == c_node){
			return false;
}
	}
	return true;
}

bool rwa2::Mouse::search_maze(std::array<int, 2> n, std::array<int, 2> g){
	API::setColor(n[0],n[1],'g');
    API::setColor(g[0],g[1],'b');
	API::setText(g[0],g[1],"G4");
	node_list.push_back(n);

    // Is the mouse at the goal? If so, leave this search. Otherwise, add thennode to the visited stack.
	if(n != g){
		if(node_stack.empty()){
			node_stack.push(n);}}
	else{
		std::cerr<<"\nFound the Goal! \n";
		return true;}
	

	if(!node_stack.empty()) //while the stack is not empty
	{
		const int m_row = n[1];
		const int m_col = n[0];

		const int g_row = g[1];
		const int g_col = g[0];

		rwa2::Node check_wall;
	
		//North=0; East=1, South=2, West=3
		// Checking North for a wall or visited node
		if (!m_maze[n[0]][n[1]].is_wall(0) && node_exists(node_list, std::array<int,2>{n[0],n[1]+1}))
		{
			n[1]+=1; 
			node_stack.push(n);
		}
		// Checking East for a wall or visited node
		else if (!m_maze[n[0]][n[1]].is_wall(1) && node_exists(node_list, std::array<int,2>{n[0]+1,n[1]}))
		{
			n[0]+=1;	
			node_stack.push(n);
		}
		//Checking South for a wall or visited node
		
		else if (!m_maze[n[0]][n[1]].is_wall(2) && node_exists(node_list, std::array<int,2>{n[0],n[1]-1}))
		{
			n[1]-=1;
			node_stack.push(n);
		}
		//Checking West for a wall or visited node
	
		else if (!m_maze[n[0]][n[1]].is_wall(3) && node_exists(node_list, std::array<int,2>{n[0]-1,n[1]}))
		{
			n[0]-=1;
			node_stack.push(n);
		}

        // Insurance case for an empty stack.
		else if (!node_stack.empty()) {
			node_stack.pop();
		}

        // Exit the function if no direction is found.
		else{ return false;}
	}

        // Recursively call search maze while the goal node is not reached and the stack is not empty.
		if (!node_stack.empty()) {
				n = node_stack.top();
				search_maze(n, g);
			} else{return false;}
}

void rwa2::Mouse::turn_left() {
    API::turnLeft();
}
 
void rwa2::Mouse::turn_right() {
    API::turnRight();
}

void rwa2::Mouse::move_forward() {
    API::moveForward(1);
}

bool rwa2::Mouse::follow_path(std::stack<std::array<int,2>> route, std::array<int,2> goal_node){
	int x{}, y{}; //Intialize x and y positions.
	int curr_dir {}; //North=0; East=1; South=2; West=3
	int next_direction{}; //Initialize the next direction.

   

	std::array<int, 2> node_curr{}, node_next{}; //Create an array of the current and next positions.
    
    //--Set current and next node--//
    node_curr = route.top();
	// std::cerr << "\nCurrent Node set to X: " <<node_curr.at(0) << "  Y: " <<node_curr.at(1);
    route.pop();
    node_next = route.top();
	// std::cerr << "\nNext Node set to X: " <<node_next.at(0) << "  Y: " <<node_next.at(1);

    //--Determine to-go direction---//
    x = node_next[0] - node_curr[0];
    y = node_next[1] - node_curr[1];
    curr_dir = Mouse::get_direction();

    if(x==-1 && y == 0) next_direction = 3; //North=0; East=1; South=2; West=3
    else if(x == 1 && y == 0) next_direction = 1;
    else if(x == 0 && y ==-1) next_direction = 2;
    else if(x == 0 && y == 1) next_direction = 0;

    int x_1 = node_curr[0];
    int y_1 = node_curr[1];

    if (node_next==goal_node) {
			API::setColor(node_next.at(0), node_next.at(1), 'r');//Recolor to red
			std::cerr<<"\nGoal node is next.\n";
			
	}

    //---Move mouse then update location and direction
    if(curr_dir == 0){  //Facing North
        if(next_direction == 0) {

            // Check for walls, move if none seen. Then update the position.
            if(!API::wallFront()){ Mouse::move_forward();
                m_y+=1;
                }

            //If there's a wall ahead, set the wall in the mouse's memory, then update its position and exit the function. 
            else{ Mouse::path_blocked = true;
                m_maze.at(x_1).at(y_1).set_wall(0,true);
                m_direction = {direction::NORTH};
                return true;}
        }
        
        else if(next_direction == 2) {
            Mouse::turn_left();
            Mouse::turn_left();
            if(!API::wallFront()){
                Mouse::move_forward();
                //std::cerr << "\nTurned South!";
                m_y-=1;
                m_direction = {direction::SOUTH};}
            else{m_maze.at(x_1).at(y_1).set_wall(2,true);
                return true;}

		}
        else if(next_direction == 1) {
            if(!API::wallRight()) {
                Mouse::turn_right();
                Mouse::move_forward();
				//std::cerr << "\nTurned East!";
                m_direction = {direction::EAST};
                m_x+=1;
            }else{ Mouse::path_blocked = true;
                m_maze.at(x_1).at(y_1).set_wall(1,true);
                return true;}
        }
        else if(next_direction == 3) {
            if(!API::wallLeft()) {
                Mouse::turn_left();
                Mouse::move_forward();
				//std::cerr << "\nTurned West!";
                m_direction = {direction::WEST};
                m_x-=1;
            }else {Mouse::path_blocked = true;
                m_maze.at(x_1).at(y_1).set_wall(3,true);
                return true;}
        }
        
    }
    else if(curr_dir == 2) { //Facing South
        if(next_direction == 0) {
            Mouse::turn_left();
            Mouse::turn_left();
            if(!API::wallFront()){
                Mouse::move_forward();
                // std::cerr << "\nTurned North!"; //Need wall detection?
                m_y+=1;
                m_direction = {direction::NORTH};}
                
            else{m_maze.at(x_1).at(y_1).set_wall(0,true);
                return true;}}
    
        else if(next_direction == 2) {
            if(!API::wallFront()){
                Mouse::move_forward();
                m_y-=1;
                m_direction = {direction::SOUTH};}
                
            else{ Mouse::path_blocked = true;
                m_maze.at(x_1).at(y_1).set_wall(2,true);
                return true;}
        }
        else if(next_direction == 1) {
            if(!API::wallLeft()) {
                Mouse::turn_left();
                Mouse::move_forward();
                m_x+=1;
                m_direction = {direction::EAST};}
            else{ Mouse::path_blocked = true;
                m_maze.at(x_1).at(y_1).set_wall(1,true);
                return true;}
        }
        else if(next_direction == 3) {
            if(!API::wallRight()){
                Mouse::turn_right();
                Mouse::move_forward();
                m_x-=1;
                m_direction = {direction::WEST};
            }else{ Mouse::path_blocked = true;
                m_maze.at(x_1).at(y_1).set_wall(3,true);
                return true;}
        }
    }
    else if(curr_dir ==1) { //Facing East
        if(next_direction == 0) {
            if(!API::wallLeft()) {
                Mouse::turn_left();
                Mouse::move_forward();
                m_y+=1;
                m_direction = {direction::NORTH};
            }else{ Mouse::path_blocked = true;
                m_maze.at(x_1).at(y_1).set_wall(0,true);
                return true;}
        }
        else if(next_direction == 2) {
            if(!API::wallRight()) {
                Mouse::turn_right();
                Mouse::move_forward();
                m_y-=1;
                m_direction = {direction::SOUTH};
            }else{ Mouse::path_blocked = true;
                m_maze.at(x_1).at(y_1).set_wall(2,true);
                return true;}
        }
        else if(next_direction == 1) {
            if(!API::wallFront()){ Mouse::move_forward();
                m_x+=1;
                m_direction = {direction::EAST};}
            else{ Mouse::path_blocked = true;
                m_maze.at(x_1).at(y_1).set_wall(1,true);
                return true;}
        }
        else if(next_direction == 3) {
            Mouse::turn_left();
            Mouse::turn_left();
            Mouse::move_forward();(node_next[0], node_next[1], next_direction);
            m_x-=1;
            m_direction = {direction::WEST};
        }
    }
    else if(curr_dir ==3) { //Facing West
        if(next_direction == 0) {
            if(!API::wallRight()) {
                Mouse::turn_right();
                Mouse::move_forward();
                m_y+=1;
                m_direction = {direction::NORTH};
            }else{ Mouse::path_blocked = true;
                m_maze.at(x_1).at(y_1).set_wall(0,true);
                return true;}
        }
        else if(next_direction == 2) {
            if(!API::wallLeft()) {
                Mouse::turn_left();
                Mouse::move_forward();
                m_y-=1;
                m_direction = {direction::SOUTH};
            }else{ Mouse::path_blocked = true;
                m_maze.at(x_1).at(y_1).set_wall(2,true);
                return true;}
        }
        else if(next_direction == 1) {
            Mouse::turn_left();
            Mouse::turn_left();
            Mouse::move_forward();
            m_x+=1;
            m_direction = {direction::EAST};
        }
        else if(next_direction == 3) {
            if(!API::wallFront()){ Mouse::move_forward();
                m_x-=1;
                m_direction = {direction::WEST};}
            else{ Mouse::path_blocked = true;
                m_maze.at(x_1).at(y_1).set_wall(3,true);
                return true;}
        }
    }
    follow_path(route, goal_node);
}


void rwa2::Mouse::findWalls(int x, int y, int direction) {

    // Used to see what walls are around a given node in a given direction.
	if(direction == 0){ //North=0
        if(API::wallFront()){
            API::setWall(x,y,'N');

        }
        if(API::wallRight()){
            API::setWall(x,y,'E');
        }
        if(API::wallLeft()){
            API::setWall(x,y,'W');
        }
    }
    if(direction == 1){//East=1
        if(API::wallFront()){
            API::setWall(x,y,'e');

        }
        if(API::wallRight()){
            API::setWall(x,y,'s');
        }
        if(API::wallLeft()){
            API::setWall(x,y,'n');
        }
    }
    if(direction == 3){// West=3
        if(API::wallFront()){
            API::setWall(x,y,'w');
        }
        if(API::wallRight()){
            API::setWall(x,y,'n');
        }
        if(API::wallLeft()){
            API::setWall(x,y,'s');
        }
    }
    if(direction == 2){//South=2
        if(API::wallFront()){
            API::setWall(x,y,'s');
        }
        if(API::wallRight()){
            API::setWall(x,y,'w');
        }
        if(API::wallLeft()){
            API::setWall(x,y,'e');

        }
    }
}