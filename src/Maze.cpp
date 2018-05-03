#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>

using namespace std;

class person {
public:
    int speed;
    int x;
    int y;

    void init(int speed_in, int x_in, int y_in) {
      speed = speed_in;
      x = x_in;
      y = y_in;
    }

    void up() {
      y = y - speed;
    }
    void down(){
      y = y + speed;
    }
    void left(){
      x = x - speed;
    }
    void right(){
      x = x + speed;
    }
};


class Maze {
public:
  vector<vector<char> > grid;
  person player;
  bool done = false;
  bool lose = false;
  int _fireDelay = 0;

  Maze(int dimension) {
    grid.resize(dimension);
    for (auto& row : grid) {
      row.resize(dimension);
    }

    initialize_grid();
  }

  void player_right();
  void player_down();
  void player_left();
  void player_up();

  void print_grid();
  void initialize_grid();

  void check_for_win();

  void reset_current_char() {
    grid[player.y][player.x] = '.';
  }

  void initialize_player(int speed, int x, int y) {
    player.init(speed, x, y);
    set_player_location();
  }

  void set_player_location() {
    if (grid[player.y][player.x] == '&')
      delay_fire(10);

    grid[player.y][player.x] = 'p';


    print_grid();
  }


  void expand_fire();
  void delay_fire(int delay);
  void reset();

  void reinitialize();

};


int main() {

  srand(time(NULL));

  int timer = 1;
  
  Maze game(40);
  game.initialize_player(1, 0, 0);

  bool three = true;
  int expand = 3;



  char input;
  while (game.done == false && cin >> input && input != 'q') {
    if (input == 'w') {
      // Move the player up.
      game.player_up();
    }
    if (input == 's') {
      game.player_down();
    }
    if (input == 'a') {
      game.player_left();
    }
    if (input == 'd') {
      game.player_right();
    }
    if (input == 'r')
    {
      game.reset();
    }
    if (input == 'e'){
      game.reinitialize();
    }
    timer = timer + 1;

    game.expand_fire();
  

    if (game.lose == true) {
      cout << "you lose!" << endl;
      game.print_grid();
      // TODO: ask if they want to play again?
      cout << "Do you want to play again? [y/n]: ";
      char answer;
      
      while (cin >> answer) {
        if (answer == 'y') {
          game.reset();
          game.lose = false;
          cout << "you lost and we reprinted the grid!" << endl;
          break;
        }
        else if (answer == 'n') {
          cout << "Thanks for playing!" << endl;
          return 0;
        }

        cout << "Not valid input, try again!" << endl;
      }

    }
  }
  if (game.done == true) {
    cout << "Congratulations" << endl;
    cout << "Done! :)" << endl;
  }
  

  return 0;
}





// --------------------------------------------------------------------------


void Maze::reinitialize() {
  // A string containing the possible
  // floor tiles.
  string possible = "......#####";

  for (auto& row : grid) {
      for (auto& el : row) {
        if (el == '#' || el == '.') {
            el = possible[rand() % 11];
          }
      }
  }



  print_grid();
}

void Maze::reset() {
  initialize_grid();

  initialize_player(player.speed , 0, 0);
}


void Maze::expand_fire() {

  if (_fireDelay > 0) {
    _fireDelay -= 1;
    return;
  }

  for (int row = grid.size() - 1; row >= 0; --row) {
    for (int col = 0; col < grid.size(); ++col) {
      if (grid[row][col] == 'f') {

        if (row != grid.size() - 1 && grid[row + 1][col] != '#' && grid[row + 1][col] != '&') {
          grid[row + 1][col] = 'f';
        }
        if (row != 0 && grid[row - 1][col] != '#' && grid[row - 1][col] != '&') {
          grid[row - 1][col] = 'f';
        }

        if (col != 0 && grid[row][col-1] != '#' && grid[row][col-1] != '&') {
          grid[row][col-1] = 'f';
        }
        if (col != grid.size()-1 && grid[row][col+1] != '#' && grid[row][col+1] != '&') {
          grid[row][col + 1] = 'f';
        }
      }
    }
  }
}

void Maze::delay_fire(int delay) {
  _fireDelay += delay;
}

void Maze::player_right() {
  int potential = player.x + player.speed;
  if (potential > grid.size()-1) {
    cout << "Can't go that way!" << endl;
    return;
  }
  if (grid[player.y][potential] == '#') {
    cout << "Player can't move that way! Try again." << endl;
    return;
  }

  else if (grid[player.y][potential] == 'f') {
    lose = true;
  }

  reset_current_char();
  player.right();
  set_player_location();
  check_for_win();
}


void Maze::player_left() {
  int potential = player.x - player.speed;
  if (potential < 0) {
    cout << "Can't go that way!" << endl;
    return;
  }
  if (grid[player.y][potential] == '#') {
    cout << "Player can't move that way! Try again." << endl;
    return;
  }
    else if (grid[player.y][potential] == 'f') {
      lose = true;
  }

  reset_current_char();
 player.left();
 set_player_location();
 check_for_win();
}


void Maze::player_up() {
  int potential = player.y - player.speed;
  // Check if the player would move out of bounds.
  if (potential < 0) {
    cout << "Can't go that way!" << endl;
    return;
  }
  // Check if player will move to fire or a wall.
  if (grid[potential][player.x] == '#') {
    cout << "Player can't move that way! Try again." << endl;
    return;
  }
  else if (grid [potential][player.x] == 'f') {
    lose = true;
}

  reset_current_char();
  player.up();
  set_player_location();
  check_for_win();
}


void Maze::player_down() {
  int potential = player.y + player.speed;
  if (potential > grid.size()-1) {
    cout << "Can't go that way!" << endl;
    return;
  }
  if (grid[potential][player.x] == '#') {
    cout << "Player can't move that way! Try again." << endl;
    return;
  }
  else if (grid [potential][player.x] == 'f') {
    lose = true;
}

  reset_current_char();
  player.down();
   set_player_location();
   check_for_win();
}


// Print a two-dimensional vector.
void Maze::print_grid() {
    for (auto& row : grid) {
        for (auto el : row) {
            if (el == 'f')
              cout << "\033[31mf \033[0m";
            else if (el == 'p')
              cout << "\033[35mp \033[0m";
            else if (el == '#')
              cout << "\033[36m# \033[0m";
            else if (el == '&')
              cout << "& ";
            else
              cout << "  ";
              //cout << "\033[30m. \033[0m";
        }
        cout << endl;
    }
    cout << " ------------------------------------------ " << endl;
}


void Maze::check_for_win() {
  if (player.x == grid.size() - 1 && player.y == grid.size() - 1){
    done = true;
  }
}



// Initialize a random grid for the game map.
// Try to make it solvable everytime?
void Maze::initialize_grid() {

    // A string containing the possible
    // floor tiles.
    string possible = "......####";

    for (auto& row : grid) {
        for (auto& el : row) {
            el = possible[rand() % 10];
        }
    }

    grid[rand() % grid.size()][rand() % grid.size()] = '&';
    grid[rand() % grid.size()][rand() % grid.size()] = '&';

    grid[0][0] = '.';
    grid[grid.size() - 1][grid.size() - 1] = '.';
    grid[0][grid.size() - 1] = 'f';

    print_grid();
}
