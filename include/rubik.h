#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <map>

#pragma once

using RubiksCoord = std::array<int, 3>;
using RubiksMap = std::map<std::string, RubiksCoord>;

class Edge {
public:
    RubiksCoord coord;
    std::pair<char, char> move;
};

class Face {
public:
    class Node {
    public:
        std::string cube;
        Node* prev;
        Node* next;
    };
    Face(std::array<std::string, 8> cubes);
    Node* head;
    std::array<Node*, 4> affected_faces;
    void cycle_list(char cycle, int times);
};

class RubiksCube {
public:
    RubiksCube();
    void print_solved_state();
    void print_unsolved_state();
    void set_unsolved_state();
    int solve_primary_face();
    void make_move(Face* face, char turn);
    void cube_swap(Face* face, Face::Node* ptr);
    RubiksMap unsolved_state;
private:
    std::map<char, Face*> faces;
    int get_euclidean_distance(RubiksCoord unsolved, RubiksCoord solved);
    /* STATIC DECLARATIONS */
    static RubiksMap solved_state;
    static Edge* edgify(RubiksCoord c, char face, char turn);
    static std::map<RubiksCoord, std::array<Edge*, 4>> edge_adjacencies;
    /* END STATIC DECLARATIONS */
};