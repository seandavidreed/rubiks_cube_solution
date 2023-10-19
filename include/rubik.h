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
    Node* zero_ptr;
    std::array<Node*, 4> affected_faces;
};

class RubiksCube {
public:
    RubiksCube();
    void print_solved_state();
    void set_unsolved_state();
    int solve_primary_face();
private:
    RubiksMap unsolved_state;
    static RubiksMap solved_state;
    std::map<std::string, Face*> faces;
    static std::map<RubiksCoord, std::array<Edge*, 4>> edge_adjacencies;
    static Edge* edgify(RubiksCoord c, char face, char turn);
    std::pair<std::string, std::string> calculate_turn(RubiksCoord& start, RubiksCoord& end);
    int get_euclidean_distance(RubiksCoord unsolved, RubiksCoord solved);
};