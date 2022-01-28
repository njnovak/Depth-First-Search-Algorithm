/**
 * @file mouse.h
 * @author Jerry Pittman, Jr., Nicholas Novak, Orlandis Smith
 *  (jpittma1@umd.edu, nnovak@umd.edu, osmith15@umd.edu)
 * @brief RWA2_Group_2_2
 * @brief The file contains the Mouse class
 * @version 3.0
 * @date 2021-10-24
 *
 * @copyright Copyright (c) 2021
 *
 */

 /*! \mainpage Maze search algorithm
  *
  * This project consists of searching a path in a maze
  * and then task a mouse (robot) to follow the path.
  * - \subpage searchingPathPage "Searching a path"
  *
  * - \subpage followingPathPage "Following a path"
  *
  */

  /*! \page searchingPathPage Searching a path
   *
   * The search algorithm used for searching a path in a maze relies on
   * the depth-first search (DFS) approach. This algorithm is implemented in rwa2::Mouse::search_maze()
   *
   */

   /*! \page followingPathPage Following a path
    *
    * To follow a path generated by DFS, methods from the class API (api/api.h) must be used to interact
    * with the micromouse simulator.
    * - Methods of the API class are documented <a href="https://github.com/mackorone/mms#summary">here</a>.
    */


#ifndef MICROMOUSE_H
#define MICROMOUSE_H

#include "../node/node.h"
#include "../util/util.h"
#include <array>
#include <stack>
#include <string>
#include <vector>
#include <utility>

namespace rwa2 {
    /**
     * @brief This class is used to compute a path and execute the path.
     *
     */
    class Mouse {
        public:

        std::stack< std::array <int, 2> > node_stack;
        std::vector< std::array <int, 2> > node_list;
        std::array< std::array <int, 16>, 16> pos_array;
        std::array<int, 2> parent_node;
        std::array<std::array<Node, 16>, 16> node_info;
        std::array<std::array<Node, 16>, 16> node_master;
        std::array<std::array<bool, 16>, 16> explored_node;
        std::array<std::array<bool, 16>, 16> visited_node;
        bool path_blocked = true;
        
        
        /**
         * @brief Construct a new MicroMouse object
         *
         * The robot is always at (0,0) and facing NORTH when the simulator starts
         */
        Mouse() :m_x{ 0 }, m_y{ 0 }, m_direction{ direction::NORTH } {
            //initialize the maze by placing around the perimeter of the maze
            for (int x = 0; x < m_maze_width; x += 1) {
                for (int y = 0; y < m_maze_height; y += 1) {
                    m_maze.at(x).at(y).set_wall(direction::NORTH, (y == m_maze_height - 1));
                    m_maze.at(x).at(y).set_wall(direction::EAST, (x == m_maze_width - 1));
                    m_maze.at(x).at(y).set_wall(direction::SOUTH, (y == 0));
                    m_maze.at(x).at(y).set_wall(direction::WEST, (x == 0));
                }
            }
        }
    
        /**
         * @brief this method visually sets the walls in the simulator
         * 
         */
        void display_walls();
        
        /**
         * @brief To not have to do std::cerr to print
         * 
         * @param text to be printed
         */
        void log(const std::string& text);
        
        /**
         * @brief Determines if a node exists
         * 
         * @param n_list 
         * @param c_list 
         * @return true 
         * @return false 
         */
        bool node_exists(std::vector<std::array<int,2>> n_list, std::array<int,2> c_list);

        /**
         * @brief For setting walls in maze around the robot
         * 
         * @param x 
         * @param y 
         * @param direction 
         */
        void findWalls(int x, int y, int direction);
        
        /**
        * @brief follow path function
        * Moves mouse along the route
        * @param route reverse of stack
        * @param current_pos from current node of mouse
        * @return current_node
        */
        bool follow_path(std::stack<std::array<int,2>> route, std::array<int,2> goal_node);

        /**
         * @brief Determines if at the goal node
         * 
         * @param pres_pos 
         * @param goal_pos 
         * @return true 
         * @return false
         */
        bool at_goal_node(std::array<int,2> pres_pos, std::array<int,2> goal_pos);

        /**
        * @brief Updated method for searching the maze -- This version shortens the code a bit and should run with less errors
        * Note: the recursive function call still needs to be updated as m is improperly defined.
        *
        * @param n present node
        * @param g goal node
        * @return true 
        * @return false 
        */
        bool search_maze(std::array<int,2> n, std::array<int,2> g);
            
        /**
        * @brief Make the mouse move forward
        * 
        */
        void move_forward();
        
        /**
        * @brief Make the mouse rotate 90 deg CCW
        *
        */
        void turn_left();
        
        /**
         * @brief Make the mouse rotate 90 deg CW
         *
         */
        void turn_right();

        //Accessors for m_x, m_y, m_direction
        //m_maze_height, and m_maze_width
        const int& get_x() const {
            return m_x;
        }
        
        const int& get_y() const {
            return m_y;
        }
        
        const int& get_direction() const {
            return m_direction;
        }

        const int& get_maze_height() const {
            return m_maze_height;
        }

        const int& get_maze_width() const {
            return m_maze_width;
        }


        private:
        static const int m_maze_width{ 16 }; //width of the maze
        static const int m_maze_height{ 16 };//height of the maze
        int m_x; //x position of the robot in the maze
        int m_y; //y position of the robot in the maze
        int m_direction; //direction of the robot in the maze
        std::array<std::array<Node, m_maze_width>, m_maze_height> m_maze; //2D array maze object
    };
}
#endif