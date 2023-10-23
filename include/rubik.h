#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <map>

#pragma once

using RubiksCoord = std::array<int, 3>;
using RubiksMap = std::map<RubiksCoord, RubiksCoord>;

typedef struct Edge {
    RubiksCoord coord;
    char move;
} Edge;

class RubiksCube {
public:
    RubiksCube();

    void printState() const;
    void solveWhiteCross();
    void solveLayer2Edges();
private:
    RubiksMap state;
    RubiksMap orientations;

    int getEuclideanDistance(RubiksCoord unsolved, RubiksCoord solved) const;
    RubiksCoord getLocation(RubiksCoord cube) const;
    void makeMove(char move);
    void solveEdge(RubiksCoord key, std::array<char, 3> lockedFaces);
    void adjustOrientation(RubiksCoord cube);

    /* STATIC DECLARATIONS */
    static std::map<char, std::array<RubiksCoord, 8>> faces;
    static Edge* edgify(RubiksCoord c, char move);
    static std::map<RubiksCoord, std::array<Edge*, 4>> edgeAdjacencies;
    /* END STATIC DECLARATIONS */
};