struct Cell
{
  int x;
  int y;
  int walls[4];
  int dist;
};

struct Grid
{
  int rows;
  int cols;
  Cell grid[9][9];
};

int absolute(int num)
{
  if (num > 0)
  {
    return num;
  }
  else
  {
    return -num;
  }
}

Grid make_grid()
{
  Grid grid;
  grid.cols = 9;
  grid.rows = 9;
  for (int i = 0; i < grid.cols; i++)
  {
    for (int j = 0; j < grid.rows; j++)
    {
      grid.grid[i][j].x = i;
      grid.grid[i][j].y = j;
      grid.grid[i][j].walls[0] = false;
      grid.grid[i][j].walls[1] = false;
      grid.grid[i][j].walls[2] = false;
      grid.grid[i][j].walls[3] = false;
      if (i == 0)
      {
        grid.grid[i][j].walls[3] = true;
      }
      else if (i == grid.cols - 1)
      {
        grid.grid[i][j].walls[1] = true;
      }
      if (j == 0)
      {
        grid.grid[i][j].walls[0] = true;
      }
      else if (j == grid.rows - 1)
      {
        grid.grid[i][j].walls[2] = true;
      }
      grid.grid[i][j].dist = absolute(i - 4) + absolute(j - 4);
    }
  }
  return grid;
}

int set_wall(Grid *grid, int x, int y, int wall_no)
{
  if (grid->grid[x][y].walls[wall_no])
  {
    return 0;
  }
  grid->grid[x][y].walls[wall_no] = true;
  if (wall_no == 0)
  {
    grid->grid[x][y - 1].walls[2] = true;
  }
  else if (wall_no == 1)
  {
    grid->grid[x + 1][y].walls[3] = true;
  }
  else if (wall_no == 2)
  {
    grid->grid[x][y + 1].walls[3] = true;
  }
  else if (wall_no == 3)
  {
    grid->grid[x - 1][y].walls[3] = true;
  }
}

int print_grid(Grid *grid)
{
  for (int j = 0; j < grid->rows; j++)
  {
    for (int i = 0; i < grid->cols; i++)
    {
      Serial.print(grid->grid[i][j].dist);
      Serial.print(" ");
    }
    Serial.println(" ");
  }
}

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.println("");
  Grid grid = make_grid();
  print_grid(&grid);
  while(true);
}
