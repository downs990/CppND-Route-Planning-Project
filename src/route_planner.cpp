#include "route_planner.h"
#include <algorithm>

RouteModel::Node start_node;
RouteModel::Node end_node;

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest
    // nodes to the starting and ending coordinates. Store the nodes you 
    // find in the RoutePlanner's start_node and end_node attributes.
    start_node = &m_Model.FindClosestNode(start_x, start_y);
    end_node = &m_Model.FindClosestNode(end_x, end_y);

}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {

    float hValue = node->distance(*end_node);
    return hValue;

}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - For each node in current_node.neighbors, add the neighbor to 
//          open_list and set the node's visited attribute to true.
void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {

    current_node->FindNeighbors();

    for(RouteModel::Node *current_neighbor : current_node->neighbors){
        current_neighbor->parent = current_node;
        current_neighbor->h_value = CalculateHValue(current_neighbor);
        current_neighbor->g_value = current_node->g_value + current_node->distance(*current_neighbor); 
 
        open_list.push_back(current_neighbor);
        current_neighbor->visited = true;
    }
}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

RouteModel::Node *RoutePlanner::NextNode() { 

    std::sort(open_list.begin(), open_list.end(), 
    [](const RouteModel::Node *my_left, const RouteModel::Node *my_right){
        return my_left->g_value + my_left->h_value < my_right->g_value + my_right->h_value;
    });


    RouteModel::Node* my_next_node = open_list[0];
    open_list.erase(open_list.begin());
    return my_next_node;
}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f; 
    std::vector<RouteModel::Node> path_found;

    while(current_node->parent != nullptr){
        path_found.push_back(*current_node);
        
        // Manhattan distance ( NOT Euclidean )
        double distanceFromParent = current_node->distance(*current_node->parent);
        distance += distanceFromParent;
        current_node = current_node->parent; 
    }
 
    path_found.push_back(*current_node);

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    reverse(path_found.begin(), path_found.end());

    std::cout << "Path Front (x,y):" << path_found.front().x << " " << path_found.front().y << "\n";
    std::cout << "Path Back (x,y):" << path_found.back().x << " " << path_found.back().y << "\n";
    
    std::cout << "Start Node (x,y):" << start_node->x << " " << start_node->y << "\n";
    std::cout << "End Node (x,y):" << end_node->x << " " << end_node->y << "\n";
    
	// finally working !!!!
    return path_found;

}



// TODO 7: Write the A* Search algorithm here.. 
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() { 
   
   
    RouteModel::Node *current_node = start_node;
    open_list.push_back(current_node);
    current_node->visited = true;
 
    while(open_list.size() > 0){
        AddNeighbors(current_node);
        current_node = NextNode();

        if(current_node == end_node){
            m_Model.path = ConstructFinalPath(current_node); 
            break;
        }

    }

     
 

}
