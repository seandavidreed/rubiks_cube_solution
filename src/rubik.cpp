#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <map>
#include <cmath>
#include <cctype>
#include "../include/rubik.h"

/* GLOBAL VARIABLES */

/* EDGES */
const RubiksCoord WB = {0,1,1};    
const RubiksCoord WO = {-1,0,1};
const RubiksCoord WG = {0,-1,1}; 
const RubiksCoord WR = {1,0,1};
const RubiksCoord BO = {-1,1,0};
const RubiksCoord BR = {1,1,0};
const RubiksCoord GO = {-1,-1,0};
const RubiksCoord GR = {1,-1,0};
const RubiksCoord YB = {0,1,-1};
const RubiksCoord YO = {-1,0,-1};
const RubiksCoord YG = {0,-1,-1};
const RubiksCoord YR = {1,0,-1}; 
/* CORNERS */
const RubiksCoord WBO = {-1,1,1}; 
const RubiksCoord WGO = {-1,-1,1};
const RubiksCoord WGR = {1,-1,1};
const RubiksCoord WBR = {1,1,1};
const RubiksCoord YBO = {-1,1,-1};
const RubiksCoord YBR = {1,1,-1};
const RubiksCoord YGR = {1,-1,-1};
const RubiksCoord YGO = {-1,-1,-1}; 

std::map<RubiksCoord, std::string> names = {
    {{0,1,1}, "WB"},
    {{-1,0,1}, "WO"},
    {{0,-1,1}, "WG"},
    {{1,0,1}, "WR"},
    {{-1,1,0}, "BO"},
    {{1,1,0}, "BR"},
    {{-1,-1,0}, "GO"},
    {{1,-1,0}, "GR"},
    {{0,1,-1}, "YB"},
    {{-1,0,-1}, "YO"},
    {{0,-1,-1}, "YG"},
    {{1,0,-1}, "YR"},

    {{-1,1,1}, "WBO"},
    {{-1,-1,1}, "WGO"},
    {{1,-1,1}, "WGR"},
    {{1,1,1}, "WBR"},
    {{-1,1,-1}, "YBO"},
    {{1,1,-1}, "YBR"},
    {{1,-1,-1}, "YGR"},
    {{-1,-1,-1}, "YGO"}
};

/* END GLOBAL VARIABLES */

/***************************************************************************/

/* OPERATOR OVERLOADS */
// Overload operator<< for RubiksCoord for use in RubiksMap printing.
std::ostream& operator<<(std::ostream& os, const RubiksCoord& array) {
    os << "{ ";
    for (auto elem: array) {
        os << elem << ' ';
    }
    os << '}';
    return os;
}

// Overload operator<< for RubiksMap for use in printing.
std::ostream& operator<<(std::ostream& os, const RubiksMap& rm) {
    for (auto t: rm) {
        os << names[t.first] << " ---> " << names[t.second] << std::endl;
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
    state[WB] = GO;
    state[WO] = BR;
    state[WG] = YB;
    state[WR] = YO;
    state[BO] = WG;
    state[BR] = YR;
    state[GO] = WO;
    state[GR] = YG;
    state[YB] = GR;
    state[YO] = WB;
    state[YG] = WR;
    state[YR] = BO;
    /* CORNERS */
    state[WBO] = WBO;
    state[WGO] = YGO;
    state[WGR] = WGR;
    state[WBR] = YBO;
    state[YBO] = YBR;
    state[YBR] = WGO;
    state[YGR] = WBR;
    state[YGO] = YGR;
};

/* END CONSTRUCTORS */

/***************************************************************************/

/* STATIC DEFINITIONS */

Edge* RubiksCube::edgify(RubiksCoord c, char move) {
    Edge* edge = new Edge;
    edge->coord = c;
    edge->move = move;
    return edge;
}

std::map<RubiksCoord, std::array<Edge*, 4>> RubiksCube::edgeAdjacencies = {
    {WB, {edgify(WR, 'U'), edgify(BR, 'b'), edgify(BO, 'B'), edgify(WO, 'u')}},
    {WR, {edgify(WB, 'u'), edgify(WG, 'U'), edgify(GR, 'r'), edgify(BR, 'R')}},
    {WG, {edgify(WR, 'u'), edgify(WO, 'U'), edgify(GR, 'F'), edgify(GO, 'f')}},
    {WO, {edgify(WB, 'U'), edgify(WG, 'u'), edgify(BO, 'l'), edgify(GO, 'L')}},
    {BO, {edgify(WB, 'b'), edgify(WO, 'L'), edgify(YB, 'B'), edgify(YO, 'l')}},
    {BR, {edgify(WR, 'r'), edgify(WB, 'B'), edgify(YR, 'R'), edgify(YB, 'b')}},
    {GR, {edgify(WR, 'R'), edgify(WG, 'f'), edgify(YG, 'F'), edgify(YR, 'r')}},
    {GO, {edgify(WO, 'l'), edgify(WG, 'F'), edgify(YO, 'L'), edgify(YG, 'f')}},
    {YB, {edgify(BO, 'b'), edgify(YR, 'd'), edgify(YO, 'D'), edgify(BR, 'B')}},
    {YR, {edgify(GR, 'R'), edgify(YB, 'D'), edgify(BR, 'r'), edgify(YG, 'd')}},
    {YG, {edgify(YR, 'D'), edgify(GR, 'f'), edgify(GO, 'F'), edgify(YO, 'd')}},
    {YO, {edgify(YG, 'D'), edgify(GO, 'l'), edgify(BO, 'L'), edgify(YB, 'd')}}
};

std::map<char, std::array<RubiksCoord, 8>> RubiksCube::faces = {
    {'U', {WBO, WB, WBR, WR, WGR, WG, WGO, WO}},
    {'F', {WGO, WG, WGR, GR, YGR, YG, YGO, GO}},
    {'L', {WBO, WO, WGO, GO, YGO, YO, YBO, BO}},
    {'R', {WGR, WR, WBR, BR, YBR, YR, YGR, GR}},
    {'B', {WBR, WB, WBO, BO, YBO, YB, YBR, BR}},
    {'D', {YGO, YG, YGR, YR, YBR, YB, YBO, YO}}
};

/* END STATIC DEFINITIONS */

/***************************************************************************/

/* FUNCTION DEFINITIONS */

void RubiksCube::printState() const {

    std::cout << state << std::endl;
}

int RubiksCube::getEuclideanDistance(RubiksCoord unsolved, RubiksCoord solved) const {
    double euclidean = 0;
    RubiksCoord result;
    for (int i = 0; i < 3; i++) {
        euclidean += pow((solved[i] - unsolved[i]), 2);
        result[i] = solved[i] - unsolved[i];
    }
    euclidean = sqrt(euclidean);
    return std::floor(euclidean);
}

RubiksCoord RubiksCube::getLocation(RubiksCoord cube) const {
    auto iter = state.cbegin();
    while (iter != state.cend()) {
        if (iter->second == cube) break;
        iter++;
    }
    return iter->first;
}

void RubiksCube::makeMove(char move) {

    std::array<RubiksCoord, 8>& coords = faces.at(toupper(move));
    std::pair<RubiksCoord, RubiksCoord> temp;

    if (islower(move)) {  
        temp.first = state.at(coords[0]);
        temp.second = state.at(coords[1]);

        state.at(coords[0]) = state.at(coords[2]);
        state.at(coords[1]) = state.at(coords[3]);
        state.at(coords[2]) = state.at(coords[4]);
        state.at(coords[3]) = state.at(coords[5]);
        state.at(coords[4]) = state.at(coords[6]);
        state.at(coords[5]) = state.at(coords[7]);
        state.at(coords[6]) = temp.first;
        state.at(coords[7]) = temp.second;
    }
    else {
        temp.first = state.at(coords[6]);
        temp.second = state.at(coords[7]);

        state.at(coords[6]) = state.at(coords[4]);
        state.at(coords[7]) = state.at(coords[5]);
        state.at(coords[4]) = state.at(coords[2]);
        state.at(coords[5]) = state.at(coords[3]);
        state.at(coords[2]) = state.at(coords[0]);
        state.at(coords[3]) = state.at(coords[1]);
        state.at(coords[0]) = temp.first;
        state.at(coords[1]) = temp.second;
    }

}

void RubiksCube::solveEdge(RubiksCoord key, std::array<char, 3> lockedFaces = {'A','A','A'}) {
    RubiksCoord location = getLocation(key);
    auto notLocked = [](char c, std::array<char, 3> lf) -> bool {
        if (toupper(c) == lf[0]) return false;
        if (toupper(c) == lf[1]) return false;
        if (toupper(c) == lf[2]) return false;
        return true;
    };

    while (state.at(key) != key) {
        int distance = getEuclideanDistance(location, key);
        Edge *move;
        for (auto x : edgeAdjacencies.at(location)) {
            int new_distance = getEuclideanDistance(x->coord, key);
            if (new_distance <= distance && notLocked(x->move, lockedFaces)) {
                move = x;
                distance = new_distance;
            }
        }
        // Make the move
        std::cout << "PERFORMING MOVE:  " << move->move << std::endl << std::endl;
        makeMove(move->move); 
        location = move->coord;
    }
}

void adjustOrientation(RubiksCoord cube) {}

void RubiksCube::solveWhiteCross() {
    printState();
    
    solveEdge(WO);
    solveEdge(WB, {'L'});
    solveEdge(WR, {'L','B'});
    solveEdge(WG, {'L','B','R'});

    // adjustOrientation(WO);
    // adjustOrientation(WB);
    // adjustOrientation(WR);
    // adjustOrientation(WG);
    
    printState();
}

void RubiksCube::solveLayer2Edges() {
    printState();

    solveEdge(BO, {'U'});
    solveEdge(BR, {'U'});
    solveEdge(GR, {'U'});
    solveEdge(GO, {'U'});

    printState();
}

/* END FUNCTION DEFINITIONS */