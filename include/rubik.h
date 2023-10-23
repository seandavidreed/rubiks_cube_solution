#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <map>

#pragma once

using RubiksCoord = std::array<int, 3>;
using RubiksMap = std::map<RubiksCoord, RubiksCoord>;

class Edge {
public:
    RubiksCoord coord;
    char move;
};

class RubiksCube {
public:
    RubiksCube();
    void printState() const;
    void solvePrimaryEdge(RubiksCoord key);
    int solveWhiteCross();
    
    //void overwrite_cube(Face::Node* node, Face::Node* affected_node);
    RubiksMap state;
private:
    int getEuclideanDistance(RubiksCoord unsolved, RubiksCoord solved) const;
    RubiksCoord getLocation(RubiksCoord cube) const;
    void makeMove(char move);
    /* STATIC DECLARATIONS */
    static std::map<char, std::array<RubiksCoord, 8>> faces;
    static Edge* edgify(RubiksCoord c, char move);
    static std::map<RubiksCoord, std::array<Edge*, 4>> edgeAdjacencies;
    /* END STATIC DECLARATIONS */
};