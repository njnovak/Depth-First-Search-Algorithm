/**
 * @file main.cpp
 * @author Jerry Pittman, Jr., Nicholas Novak, Orlandis Smith
 *  (jpittma1@umd.edu, nnovak@umd.edu, osmith15@umd.edu)
 * @brief RWA2_Group_2_2
 * @version 3.0
 * @date 2021-10-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <iostream>
#include <array>
//--Initialize the walls, Mouse, and Maze--//
#include "../include/mouse/mouse.h" 
#include "../include/api/api.h"
#include "../include/node/node.h"

void log(const std::string& text){
	std::cerr << text <<'\n';
}

int main() {
	rwa2::Mouse mouse;
	rwa2::Node node;

	std::array <int, 2> current_node {};
	// std::stack<std::array<int, 2>> local_path{};
	current_node = {mouse.get_x(), mouse.get_y()}; //(0,0)
	int current_direction=mouse.get_direction(); //North=0

	bool at_goal_node_search {}; //undefined until run through search.maze

	log("Running...Clearing all colors & text..");
	
	//-Clear color of all tiles-//
	API::clearAllColor(); 

	//--Set color and text for start-> white--//
	API::setColor(current_node.at(0),current_node.at(1),'w');
	API::setText(current_node.at(0),current_node.at(1),"Start");
	
	//--Set color and text of goal->Blue--//
	//---------Goal is G4 (8,8)-----------//
	std::array <int, 2> goal_node{8,8}; //Set to Goal 4
	API::setColor(goal_node.at(0),goal_node.at(1),'b');
	API::setText(goal_node.at(0),goal_node.at(1),"G4");

	//--Update maze walls in the program--//
	log("\nCalling Display walls..");
	mouse.display_walls(); 
	log("\nFinished Display walls. Calling set walls.");
	
	while (current_node!= goal_node) {
		//--Track visited nodes--//
		mouse.visited_node[current_node[0]][current_node[1]] = true;

		//--Determine walls around mouse, then set them--//
		mouse.findWalls(current_node.at(0),current_node.at(1),current_direction);
		// log("\nFinished Find and Set walls..");

		mouse.node_list.clear();
		mouse.node_list.push_back(current_node);

		//---DFS Algorithm to find path to Goal----//
		//-----Initial call to search & Re-search--//
		//-------if path blocked-------------------//
		if(mouse.path_blocked) { 
			
			log("\nReseting Colors and Text..");
			
			//Reset colors & text
			API::clearAllColor(); 
			API::setColor(0,0,'w');
			API::setText(0,0,"Start");
			API::setColor(goal_node.at(0),goal_node.at(1),'b');
			API::setText(goal_node.at(0),goal_node.at(1),"G4");
			API::setColor(current_node.at(0),current_node.at(1),'w');
			API::setText(current_node.at(0),current_node.at(1),"last");

			log("\nCalling Search Maze..");
			at_goal_node_search=mouse.search_maze(current_node, goal_node);

			if(at_goal_node_search) {  //
				API::setColor(current_node.at(0),current_node.at(1),'w'); //color path
			}
			mouse.path_blocked=false;
		}

		//--Create Reverse stack for pathway--
		// log("\nCreating Reverse Stack..");
		std::stack<std::array<int,2>> rev_stack {};
					
		int j=mouse.node_stack.size(); //Variable used to get the size of the stack for reversing.
					
		for(int i = 0; i < j; i++){
			rev_stack.push(mouse.node_stack.top());
			mouse.node_stack.pop();
		}
		
		log("\nMoving Mouse along path..");
		API::setColor(current_node.at(0),current_node.at(1),'w'); //color path
		bool wall_hit = mouse.follow_path(rev_stack, goal_node); //Move to next node

		log("\nUpdating Mouse position..");
		
		//Update parent_node, current position, and direction
		mouse.parent_node = current_node;
		current_node={mouse.get_x(), mouse.get_y()};
		current_direction=mouse.get_direction();

		
	} //end of While Loop
	std::cerr <<"\nNo path found!!\n";
}
