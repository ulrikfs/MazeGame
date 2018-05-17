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
  int _running = false;
  vector<vector<char> > grid;
  person player;
  bool done = false;
  bool lose = false;
  int _fireDelay = 0;

  Maze() {
    resize_grid(40);
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
      delay_fire(9);

    grid[player.y][player.x] = 'p';
  }


  void expand_fire();
  void delay_fire(int delay);
  void reset();

  void resize_grid(int dimension);

  void shuffle_grid();

};


int main() {

  srand(time(NULL));

  int timer = 1;
  
  Maze game;

  bool three = true;
  int expand = 3;


  char input;
  while (game.done == false) {

    if ( !game._running ){
      do {
        cout << "Welcome to the Maze Game\n";
        cout << "1. Start Game\n";
        cout << "2. Change Difficulty\n";
        cout << "3. Quit\n";
        
        cin >> input;
        switch(input) {
          case '1': game._running = true;
                    game.initialize_grid();
                    game.initialize_player(1, 0, 0);
                    game.print_grid();
                    break;
          case '2': break;
          case '3': 
              std::cout << "Good-bye!\n";
              exit(0);
              break;
        }
      } while (input != '1' && cin >> input && input != 'q');
    }
    else {
      game.print_grid();
    }
    cin >> input;
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
      game.shuffle_grid();
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


void Maze::shuffle_grid() {
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
  print_grid();
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

void Maze::resize_grid(int dimension) {
  grid.resize(dimension);
  for (auto& row : grid) {
    row.resize(dimension);
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
}
