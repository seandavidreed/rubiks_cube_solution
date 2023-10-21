#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <map>
#include <cmath>
#include "../include/rubik.h"

/* OPERATOR OVERLOADS */
// Overload operator<< for RubiksCoord for use in RubiksMap printing.
std::ostream& operator<<(std::ostream& os, const RubiksCoord& array) {
    os << "{ ";
    for (auto elem: array) {
        os << elem << ' ';
    }
    os << '}' << std::endl;
    return os;
}

// Overload operator<< for RubiksMap for use in printing.
std::ostream& operator<<(std::ostream& os, const RubiksMap& rm) {
    for (auto t: rm) {
        os << t.first << ": " << t.second << std::endl;
    }
    return os;
}

// Overload operator!= for use in solve_primary_face().
bool operator!=(const RubiksCoord& left, const RubiksCoord& right) {
    for (int i = 0; i < 3; i++) {
        if (left[i] != right[i]) {
            return true;
        }
    }
    return false;
}

/* END OPERATOR OVERLOADS */

/***************************************************************************/

/* CONSTRUCTORS */

RubiksCube::RubiksCube() {
    set_unsolved_state();
    /* THESE INITIALIZATIONS ARE FOR TESTING. TO BE REPLACED BY IMAGE INPUT*/
    Face* up = new Face({"wbo", "wo", "ygo", "br", "ybr", "wg", "wgr", "yo"});
    Face* front = new Face({"wgr", "wg", "ybr", "wb", "ybo", "bo", "wbr", "yr"});
    Face* down = new Face({"wbr", "bo", "ybo", "wr", "wgo", "go", "ygr", "yb"});
    Face* left = new Face({"wbo", "yo", "wgr", "yr", "wbr", "yb", "ygr", "gr"});
    Face* right = new Face({"ybr", "br", "ygo", "yg", "wgo", "wr", "ybo", "wb"});
    Face* back = new Face({"ygo", "wo", "wbo", "gr", "ygr", "go", "wgo", "yg"});
    
    // Aligning each face with cubes from neighboring faces
    up->affected_faces[0] = back->head->next->next;
    up->affected_faces[1] = right->head->next->next;
    up->affected_faces[2] = front->head->next->next;
    up->affected_faces[3] = left->head->next->next;

    front->affected_faces[0] = up->head->prev->prev;    
    front->affected_faces[1] = right->head->prev->prev;
    front->affected_faces[2] = down->head->next->next;
    front->affected_faces[3] = left->head->next->next->next->next;

    down->affected_faces[0] = front->head->prev->prev;
    down->affected_faces[1] = right->head->prev->prev;
    down->affected_faces[2] = back->head->prev->prev;
    down->affected_faces[3] = left->head->prev->prev;

    left->affected_faces[0] = up->head;
    left->affected_faces[1] = front->head;
    left->affected_faces[2] = down->head;
    left->affected_faces[3] = back->head->next->next->next->next;

    right->affected_faces[0] = up->head->next->next->next->next;
    right->affected_faces[1] = back->head;
    right->affected_faces[2] = down->head->next->next->next->next;
    right->affected_faces[3] = front->head->next->next;

    back->affected_faces[0] = up->head->next->next;
    back->affected_faces[1] = left->head;
    back->affected_faces[2] = down->head->prev->prev;
    back->affected_faces[3] = right->head->next->next->next->next;

    faces['U'] = up;
    faces['F'] = front;
    faces['D'] = down;
    faces['L'] = left;
    faces['R'] = right;
    faces['B'] = back;
};

Face::Face(std::array<std::string, 8> cubes) {
    Node* ptr = new Node;
    head = ptr;
    ptr->prev = nullptr;
    ptr->next = nullptr;
    ptr->cube = cubes[0];

    for (int i = 1; i < 8; i++) {
        ptr->next = new Node;
        ptr->next->prev = ptr;
        ptr->next->next = head;
        head->prev = ptr->next;
        ptr = ptr->next;
        ptr->cube = cubes[i];
    }
}

/* END CONSTRUCTORS */

/***************************************************************************/

/* STATIC DEFINITIONS */

RubiksMap RubiksCube::solved_state = {
    /* EDGES */
    {"wb", {0,1,1}},    
    {"wo", {-1,0,1}},
    {"wg", {0,-1,1}}, 
    {"wr", {1,0,1}},
    {"bo", {-1,1,0}},
    {"br", {1,1,0}}, 
    {"go", {-1,-1,0}},
    {"gr", {1,-1,0}},
    {"yb", {0,1,-1}},
    {"yo", {-1,0,-1}},
    {"yg", {0,-1,-1}},
    {"yr", {1,0,-1}}, 
    /* CORNERS */
    {"wbo", {-1,1,1}}, 
    {"wgo", {-1,-1,1}},
    {"wgr", {1,-1,1}},
    {"wbr", {1,1,1}},
    {"ybo", {-1,1,-1}},
    {"ybr", {1,1,-1}},
    {"ygr", {1,-1,-1}},
    {"ygo", {-1,-1,-1}} 
};

Edge* RubiksCube::edgify(RubiksCoord c, char face, char turn) {
    Edge* edge = new Edge;
    edge->coord = c;
    edge->move.first = face;
    edge->move.second = turn;
    return edge;
}

std::map<RubiksCoord, std::array<Edge*, 4>> RubiksCube::edge_adjacencies = {
    {{0,1,1}, {edgify({1,0,1}, 'U', 'R'), edgify({1,1,0}, 'B', 'L'), edgify({-1,1,0}, 'B', 'R'), edgify({-1,0,1}, 'U', 'L')}},
    {{1,0,1}, {edgify({0,1,1}, 'U', 'L'), edgify({0,-1,1}, 'U', 'R'), edgify({1,-1,0}, 'R', 'L'), edgify({1,1,0}, 'R', 'R')}},
    {{0,-1,1}, {edgify({1,0,1}, 'U', 'L'), edgify({-1,0,1}, 'U', 'R'), edgify({1,-1,0}, 'F', 'R'), edgify({-1,-1,0}, 'F', 'L')}},
    {{-1,0,1}, {edgify({0,1,1}, 'U', 'R'), edgify({0,-1,1}, 'U', 'L'), edgify({-1,1,0}, 'L', 'L'), edgify({-1,-1,0}, 'L', 'R')}},
    {{-1,1,0}, {edgify({0,1,1}, 'B', 'L'), edgify({-1,0,1}, 'L', 'R'), edgify({0,1,-1}, 'B', 'R'), edgify({-1,0,-1}, 'L', 'L')}},
    {{1,1,0}, {edgify({1,0,1}, 'R', 'L'), edgify({0,1,1}, 'B', 'R'), edgify({1,0,-1}, 'R', 'R'), edgify({0,1,-1}, 'B', 'L')}},
    {{1,-1,0}, {edgify({1,0,1}, 'R', 'R'), edgify({0,-1,1}, 'F', 'L'), edgify({0,-1,-1}, 'F', 'R'), edgify({1,0,-1}, 'R', 'L')}},
    {{-1,-1,0}, {edgify({-1,0,1}, 'L', 'L'), edgify({0,-1,1}, 'F', 'R'), edgify({-1,0,-1}, 'L', 'R'), edgify({0,-1,-1}, 'F', 'L')}},
    {{0,1,-1}, {edgify({-1,1,0}, 'B', 'L'), edgify({1,0,-1}, 'D', 'L'), edgify({-1,0,-1}, 'D', 'R'), edgify({1,1,0}, 'B', 'R')}},
    {{1,0,-1}, {edgify({1,-1,0}, 'R', 'R'), edgify({0,1,-1}, 'D', 'R'), edgify({1,1,0}, 'R', 'L'), edgify({0,-1,-1}, 'D', 'L')}},
    {{0,-1,-1}, {edgify({1,0,-1}, 'D', 'R'), edgify({1,-1,0}, 'F', 'L'), edgify({-1,-1,0}, 'F', 'R'), edgify({-1,0,-1}, 'D', 'L')}},
    {{-1,0,-1}, {edgify({0,-1,-1}, 'D', 'R'), edgify({-1,-1,0}, 'L', 'L'), edgify({-1,1,0}, 'L', 'R'), edgify({0,1,-1}, 'D', 'L')}}
};

/* END STATIC DEFINITIONS */

/***************************************************************************/

/* FUNCTION DEFINITIONS */

void RubiksCube::print_solved_state() {
    std::cout << solved_state << std::endl;
}

void RubiksCube::print_unsolved_state() {
    std::cout << unsolved_state << std::endl;
}

int RubiksCube::get_euclidean_distance(RubiksCoord unsolved, RubiksCoord solved) {
    double euclidean = 0;
    RubiksCoord result;
    for (int i = 0; i < 3; i++) {
        euclidean += pow((solved[i] - unsolved[i]), 2);
        result[i] = solved[i] - unsolved[i];
    }
    euclidean = sqrt(euclidean);
    return std::floor(euclidean);
}

void Face::cycle_list(char cycle, int times) {
    // This function cycles through the doubly-linked circular linked list
    // attribute in the Face class.
    if (cycle == 'L') {
        for (int i = 0; i < times; i++) this->head = this->head->prev;
    }
    else {
        for (int i = 0; i < times; i++) this->head = this->head->next;
    }
}

void RubiksCube::cube_swap(Face* face, Face::Node* ptr) {
    // Update coordinate of cube
    std::cout << face->head->cube << ' ' << ptr->cube << std::endl;
    unsolved_state.at(ptr->cube) = unsolved_state.at(face->head->cube);

    // Transfer cube name from moved to affected face
    ptr->cube = face->head->cube;
}

void RubiksCube::make_move(Face* face, char turn) {
    if (turn == 'L') face->cycle_list('R', 2);
    else face->cycle_list('L', 2);

    // Update affected faces by replacing their adjacent cubes
    // with the newly-moved cubes in the face in question. 
    for (Face::Node* ptr : face->affected_faces) {
        cube_swap(face, ptr);
        face->cycle_list('R', 1);
        cube_swap(face, ptr->prev);
        face->cycle_list('R', 1);
        cube_swap(face, ptr->prev->prev);
    }
}

int RubiksCube::solve_primary_face() {
    // See which edges are out of place
    print_unsolved_state();
    while (unsolved_state.at("wo") != solved_state.at("wo")) {
        
        RubiksCoord unsolved = unsolved_state.at("wo");
        RubiksCoord solved = solved_state.at("wo");
        
        int distance = get_euclidean_distance(unsolved, solved);
        RubiksCoord best_move;
        for (auto x : edge_adjacencies.at(unsolved)) {
            int new_distance = get_euclidean_distance(x->coord, solved);
            if (new_distance < distance) {
                best_move = x->coord;
                distance = new_distance;
            }
        }

        // Find out what move to make
        // Eventually factor in - minimal disturbance of solved edges
        Edge *move = nullptr;
        for (auto x : edge_adjacencies.at(unsolved_state.at("wo"))) {
            if (x->coord == best_move) {
                move = x;
                break;
            }
        }
        
        // Make the move
        std::cout << move->move.first << ' ' << move->move.second << std::endl;
        make_move(faces[move->move.first], move->move.second); 
        
        print_unsolved_state();
        break;
    }
    // while (unsolved_state["wo"] != solved_state["wo"]) {

    // }
    // while (unsolved_state["wg"] != solved_state["wg"]) {

    // }
    // while (unsolved_state["wr"] != solved_state["wr"]) {

    // }

    return 0;
}

/* TEMPORARY - WILL BE REPLACED BY IMAGE SEGMENTATION INPUT */

void RubiksCube::set_unsolved_state() {
    unsolved_state["wb"] = {1,-1,0};       // white/blue edge
    unsolved_state["wo"] = {0,1,1};      // white/orange edge
    unsolved_state["wg"] = {0,-1,1};      // white/green edge
    unsolved_state["wr"] = {1,0,-1};       // white/red edge
    unsolved_state["bo"] = {0,-1,-1};     // blue/orange edge
    unsolved_state["br"] = {1,0,1};      // blue/red edge
    unsolved_state["go"] = {0,1,-1};    // green/orange edge
    unsolved_state["gr"] = {-1,1,0};     // green/red edge
    unsolved_state["yb"] = {-1,0,-1};      // yellow/blue edge
    unsolved_state["yo"] = {-1,0,1};     // yellow/orange edge
    unsolved_state["yg"] = {1,1,0};     // yellow/green edge
    unsolved_state["yr"] = {-1,-1,0};      // yellow/red edge
    /* CORNERS */
    unsolved_state["wbo"] = {-1,1,1};     // white/blue/orange corner
    unsolved_state["wgo"] = {1,1,-1};    // white/orange/green corner
    unsolved_state["wgr"] = {-1,-1,1};     // white/green/red corner
    unsolved_state["wbr"] = {-1,-1,-1};      // white/blue/red corner
    unsolved_state["ybo"] = {1,-1,-1};    // yellow/blue/orange corner
    unsolved_state["ybr"] = {1,-1,1};     // yellow/blue/red corner
    unsolved_state["ygr"] = {-1,1,-1};    // yellow/green/red corner
    unsolved_state["ygo"] = {1,1,1};   // yellow/orange/green corner
}

/* END FUNCTION DEFINITIONS */