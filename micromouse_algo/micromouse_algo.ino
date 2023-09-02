struct Cell {
    short x;
    short y;
    bool walls[4];
    short dist;
};

#define NUM_CELLS 8

struct Grid {
    short rows;
    short cols;
    Cell grid[NUM_CELLS][NUM_CELLS];
};

int absolute(int num) {
    if (num > 0) {
        return num;
    } else {
        return -num;
    }
}

Grid make_grid() {
    Grid grid;
    grid.cols = NUM_CELLS;
    grid.rows = NUM_CELLS;
    for (int i = 0; i < grid.cols; i++) {
        for (int j = 0; j < grid.rows; j++) {
            grid.grid[i][j].x = i;
            grid.grid[i][j].y = j;
            grid.grid[i][j].walls[0] = false;
            grid.grid[i][j].walls[1] = false;
            grid.grid[i][j].walls[2] = false;
            grid.grid[i][j].walls[3] = false;
            if (i == 0) {
                grid.grid[i][j].walls[3] = true;
            } else if (i == grid.cols - 1) {
                grid.grid[i][j].walls[1] = true;
            }
            if (j == 0) {
                grid.grid[i][j].walls[0] = true;
                Serial.println("lol");
            } else if (j == grid.rows - 1) {
                grid.grid[i][j].walls[2] = true;
            }
            grid.grid[i][j].dist = absolute(2 * i - NUM_CELLS + 1) / 2 +
                                   absolute(2 * j - NUM_CELLS + 1) / 2;
        }
    }
    return grid;
}

short set_wall(Grid *grid, short x, short y, short wall_no) {
    if (grid->grid[x][y].walls[wall_no] == true) {
        return 0;
    }
    grid->grid[x][y].walls[wall_no] = true;
    if (wall_no == 0) {
        grid->grid[x][y - 1].walls[2] = true;
    } else if (wall_no == 1) {
        grid->grid[x + 1][y].walls[3] = true;
    } else if (wall_no == 2) {
        grid->grid[x][y + 1].walls[0] = true;
    } else if (wall_no == 3) {
        grid->grid[x - 1][y].walls[1] = true;
    }
}

short run_floodfill(Grid *grid, short *curr_cell) {
    short x = curr_cell[0];
    short y = curr_cell[1];
    short min_dist = NUM_CELLS * NUM_CELLS;
    short min_cell[2] = {0, 0};
    if (!grid->grid[x][y].walls[0]) {
        if (min_dist > grid->grid[x][y - 1].dist) {
            min_dist = grid->grid[x][y - 1].dist;
            min_cell[0] = x;
            min_cell[1] = y - 1;
        }
    }
    if (!grid->grid[x][y].walls[1]) {
        if (min_dist > grid->grid[x + 1][y].dist) {
            min_dist = grid->grid[x + 1][y].dist;
            min_cell[0] = x + 1;
            min_cell[1] = y;
        }
    }
    if (!grid->grid[x][y].walls[2]) {
        if (min_dist > grid->grid[x][y + 1].dist) {
            min_dist = grid->grid[x][y + 1].dist;
            min_cell[0] = x;
            min_cell[1] = y + 1;
        }
    }
    if (!grid->grid[x][y].walls[3]) {
        if (min_dist > grid->grid[x - 1][y].dist) {
            min_dist = grid->grid[x - 1][y].dist;
            min_cell[0] = x - 1;
            min_cell[1] = y;
        }
    }
    if (min_dist < grid->grid[x][y].dist) {
        curr_cell[0] = min_cell[0];
        curr_cell[1] = min_cell[1];
    } else {
        short queue[2][64];
        short start = 0;
        short end = 0;
        queue[0][start] = curr_cell[0];
        queue[1][start] = curr_cell[1];
        end++;
        int num = 0;
        while (end != start) {
            num++;
            if (grid->grid[x][y].dist == 0) {
                return 1;
            }
            short x = queue[0][start];
            short y = queue[1][start];
            start++;
            if (start >= 64) {
                start = 0;
            }

            short min_dist = NUM_CELLS * NUM_CELLS;
            short to_add[2][4];
            short to_add_len = 0;
            short min_cell[2] = {0, 0};
            if (!grid->grid[x][y].walls[0]) {
                if (min_dist > grid->grid[x][y - 1].dist) {
                    min_dist = grid->grid[x][y - 1].dist;
                    min_cell[0] = x;
                    min_cell[1] = y - 1;
                }
                to_add[0][to_add_len] = x;
                to_add[1][to_add_len] = y - 1;
                to_add_len++;
            }
            if (!grid->grid[x][y].walls[1]) {
                if (min_dist > grid->grid[x + 1][y].dist) {
                    min_dist = grid->grid[x + 1][y].dist;
                    min_cell[0] = x + 1;
                    min_cell[1] = y;
                }
                to_add[0][to_add_len] = x + 1;
                to_add[1][to_add_len] = y;
                to_add_len++;
            }
            if (!grid->grid[x][y].walls[2]) {
                if (min_dist > grid->grid[x][y + 1].dist) {
                    min_dist = grid->grid[x][y + 1].dist;
                    min_cell[0] = x;
                    min_cell[1] = y + 1;
                }
                to_add[0][to_add_len] = x;
                to_add[1][to_add_len] = y + 1;
                to_add_len++;
            }
            if (!grid->grid[x][y].walls[3]) {
                if (min_dist > grid->grid[x - 1][y].dist) {
                    min_dist = grid->grid[x - 1][y].dist;
                    min_cell[0] = x - 1;
                    min_cell[1] = y;
                }
                to_add[0][to_add_len] = x - 1;
                to_add[1][to_add_len] = y;
                to_add_len++;
            }
            if (grid->grid[x][y].dist <= min_dist) {
                grid->grid[x][y].dist = min_dist + 1;
                for (short i = 0; i < to_add_len; i++) {
                    queue[0][end] = to_add[0][i];
                    queue[1][end] = to_add[1][i];
                    end++;
                    if (end >= 64) {
                        end = 0;
                    }
                }
            }
        }
        // Serial.println(num);
    }
    return 0;
}

short print_grid(Grid *grid, short *curr_cell) {
    for (short j = 0; j < grid->rows; j++) {
        for (short i = 0; i < grid->cols; i++) {
            if (grid->grid[i][j].walls[0]) {
                Serial.print(" __");
            } else {
                Serial.print("   ");
            }
        }
        Serial.println(" ");
        if (grid->grid[0][j].walls[3]) {
            Serial.print("|");
        } else {
            Serial.print(" ");
        }
        for (short i = 0; i < grid->cols; i++) {
            if (i == curr_cell[0] && j == curr_cell[1]) {
                Serial.print("<>");
            } else {
                Serial.print(grid->grid[i][j].dist);
                if (grid->grid[i][j].dist < 10) {
                    Serial.print(" ");
                }
            }
            if (grid->grid[i][j].walls[1]) {
                Serial.print("|");
            } else {
                Serial.print(" ");
            }
        }
        Serial.println(" ");
    }
    for (short i = 0; i < grid->cols; i++) {
        Serial.print(" __");
    }
    Serial.println(" ");

    return 0;
}

short curr_cell[2];
Grid grid;

void setup() {
    Serial.begin(115200);
    grid = make_grid();
    curr_cell[0] = 0;
    curr_cell[1] = 0;
    set_wall(&grid, 4, 0, 2);
    set_wall(&grid, 3, 0, 2);
    set_wall(&grid, 2, 0, 2);
    set_wall(&grid, 1, 0, 2);
    set_wall(&grid, 0, 0, 2);
}

void loop() {
    Serial.println("");
    print_grid(&grid, curr_cell);
    while (true) {
        Serial.println("");
        if (run_floodfill(&grid, curr_cell)) {
            break;
        }
        delay(1000);
        print_grid(&grid, curr_cell);
        Serial.print(curr_cell[0]);
        Serial.print(" ");
        Serial.println(curr_cell[1]);
    }
    while (true) {
        ;
    }
}
