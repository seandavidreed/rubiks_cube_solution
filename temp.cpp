void RubiksCube::get_adjacent_edges(RubiksCoord& coord, std::array<RubiksCoord, 4>& adjacent_edges) {
    if (coord[1] == 0) {
        for (int i = 0; i < 2; i++) adjacent_edges[0][i] = (coord[i] + 1) % 2;
        adjacent_edges[0][2] = coord[2];

        for (int i = 0; i < 2; i++) adjacent_edges[1][i] = (coord[i] - 1) % 2;
        adjacent_edges[1][2] = coord[2];

        adjacent_edges[2][0] = coord[0];
        for (int i = 1; i < 3; i++) adjacent_edges[2][i] = (coord[i] + 1) % 2;

        adjacent_edges[3][0] = coord[0];
        for (int i = 1; i < 3; i++) adjacent_edges[3][i] = (coord[i] - 1) % 2;
        return;
    }
    adjacent_edges[0][0] = (coord[0] + 1) % 2;
    adjacent_edges[0][1] = coord[1];
    adjacent_edges[0][2] = (coord[2] + 1) % 2;

    adjacent_edges[1][0] = (coord[0] - 1) % 2;
    adjacent_edges[1][1] = coord[1];
    adjacent_edges[1][2] = (coord[2] - 1) % 2;

    if (coord[0] == 0) {
        adjacent_edges[2][0] = (coord[0] + 1) % 2;
        adjacent_edges[2][1] = (coord[1] + 1) % 2;
        adjacent_edges[2][2] = coord[2];

        adjacent_edges[3][0] = (coord[0] - 1) % 2;
        adjacent_edges[3][1] = (coord[1] - 1) % 2;
        adjacent_edges[3][2] = coord[2];
    }
    else {
        adjacent_edges[2][0] = coord[0];
        adjacent_edges[2][1] = (coord[1] + 1) % 2;
        adjacent_edges[2][2] = (coord[2] + 1) % 2;
        
        adjacent_edges[3][0] = coord[0];
        adjacent_edges[3][1] = (coord[1] - 1) % 2;
        adjacent_edges[3][2] = (coord[2] - 1) % 2;
    }
}

void RubiksCube::set_solved_state() {
    /* EDGES */
    solved_state["wb"] = {0,1,1};       // white/blue edge
    solved_state["wo"] = {-1,0,1};      // white/orange edge
    solved_state["wg"] = {0,-1,1};      // white/green edge
    solved_state["wr"] = {1,0,1};       // white/red edge
    solved_state["bo"] = {-1,1,0};     // blue/orange edge
    solved_state["br"] = {1,1,0};      // blue/red edge
    solved_state["go"] = {-1,-1,0};    // green/orange edge
    solved_state["gr"] = {1,-1,0};     // green/red edge
    solved_state["yb"] = {0,1,-1};      // yellow/blue edge
    solved_state["yo"] = {-1,0,-1};     // yellow/orange edge
    solved_state["yg"] = {0,-1,-1};     // yellow/green edge
    solved_state["yr"] = {1,0,-1};      // yellow/red edge
    /* CORNERS */
    solved_state["wbo"] = {-1,1,1};     // white/blue/orange corner
    solved_state["wog"] = {-1,-1,1};    // white/orange/green corner
    solved_state["wgr"] = {1,-1,1};     // white/green/red corner
    solved_state["wbr"] = {1,1,1};      // white/blue/red corner
    solved_state["ybo"] = {-1,1,-1};    // yellow/blue/orange corner
    solved_state["ybr"] = {1,1,-1};     // yellow/blue/red corner
    solved_state["ygr"] = {1,-1,-1};    // yellow/green/red corner
    solved_state["yog"] = {-1,-1,-1};   // yellow/orange/green corner
}