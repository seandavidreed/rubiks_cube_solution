#include <iostream>
#include "../include/rubik.h"

int main() {
    RubiksCube cube;
    //cube.print_solved_state();
    //cube.print_unsolved_state();
    cube.solve_primary_face();
    return 0;
}