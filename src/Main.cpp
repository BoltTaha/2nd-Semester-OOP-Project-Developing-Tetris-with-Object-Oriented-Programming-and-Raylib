#include <iostream>
#include <raylib.h>
#include <vector>
#include <map>
#include <random>
using namespace std;

//struct Color = {red, /green/, /blue/,/intensity/};
const Color a = {51, 51, 51, 255};
const Color b = {47, 140, 50, 255};
const Color c = {255, 182, 193, 255};
const Color d = {220, 115, 17, 255};
const Color e = {28, 28, 28, 255};
const Color f = {128, 0, 128, 255};
const Color h = {255, 255, 255, 255};
const Color i = {10, 70, 220, 255};



vector<Color> get_cell_Colors()  // Member function for returning vector of all colours
{ 
    return {a,b,c,d,e,f,h,i};
}



class position;
class Grid
{

private:
    int Number_of_rows;    // rows of grid array
    int Number_of_columns; // column of grid array
    int cell_size;         // size of each cell of grid in pixels
    vector<Color> colors;  // vector of type Colours to add different colours
    bool checking_row_full(int row);  // this will checking whether a row is full of block
    void clearing_row(int row);  // this will empty a row
    void move_row_down(int row, int NumRows);  // this will move the upper row to down

public:
    Grid(){    // constructor
        Number_of_rows = 20;
        Number_of_columns = 10;
        cell_size = 45;
        Initializes();              // initializing all index of the array to 0
        colors = get_cell_Colors(); // provide the vector of all colours to it.
    }

    int grid[20][10]; // default size : array of 20 by 10 (divided screen into small cells )
    void Initializes();
    void draw(); 
    bool checking_is_Cell_outside(int row, int column);     
    bool Cell_empty(int row, int column); 
    void clearing_allRows();
};




class Block
{

    int Cell_size;      // cell size of blocks
    int rotation_state; // for storing the rotational state of blocks
    vector<Color> colors;

    // these attritubte are helping in changing the origin of the block(moving it)
    int Row_offset;
    int Column_offset;

public:

    Block(){         // constructor
    Cell_size = 45;
    rotation_state = 0;
    colors = get_cell_Colors(); 
    Row_offset = 0;
    Column_offset = 0;
    }

    int ID;   // for distinguish each block
    map<int, vector<position>> Cells; // map storing int () as a key and vector of type position class as a value
    void draw(); // for drawing block on grid
    void rotate(); // for changing rotate state value
    void move(int rows, int columns); // for moving block on grid
    vector<position> Cell_position(); // for returning modified cell position of block
    void undo_rotation(); // for reversing the the rotation 

};

// class of position 
class position
{

public:

    position(int Row, int Column)
    {
       row = Row;
       column = Column;
    }

    int row;
    int column;
};


class game
{
   
    vector<Block> blocks; // the one which will hold all blocks
    Block current_block;
    Block next_block;
    
    void rotate_block(); // for changing the position cell of block 
    bool fitting_block(); // for for overlaping a block over another

public:
      bool gameOver;
    game(){

    // assigning all block to blocks
    blocks = get_all_blocks();
    current_block = Random_block();
    next_block = Random_block();
    gameOver = false;

}
    Grid grid;
    Block Random_block(); 
    vector<Block> get_all_blocks(); // for refilling all blocks
    void draw();   // for displaying grid & current block
    void handling_input(); // for taking input
    void Moving_down();
    void Moving_left();
    void Moving_right();
    bool Is_block_outside();
    void locking_block(); // for if block touches surface, locked it
};



// filling all empty cells with zero
void Grid::Initializes()
{

    for (int i = 0; i < Number_of_rows; i++)
    {
        for (int j = 0; j < Number_of_columns; j++)
        {
            grid[i][j] = 0;
        }
    }
}



// function for drawing rectangle shaped cell on the all index's of grid
void Grid::draw()
{

    for (int row = 0; row < Number_of_rows; row++)
    {

        for (int column = 0; column < Number_of_columns; column++)
        {

            int Cell_value = grid[row][column];

            // Raylib built in function & using it for creating  rcetangle shape for each cells
            DrawRectangle((column * (cell_size + 15)) + 1 /*x-cordinate*/, row * cell_size  + 1/*y-cordinate*/, (cell_size + 15) - 1 /*width*/, cell_size - 1 /*height*/, /*color*/ colors[Cell_value]);
        
        }
    }
}


// for drawing block
void Block::draw()
{

    // vector will get the position of the block through member function cell position
    vector<position> state = Cell_position();

    for (position temp : state)
    { // this loop will assign each element of state container to iteartion

        DrawRectangle((temp.column * (Cell_size + 15)) + 1 /*x-cordinate*/, (temp.row * Cell_size) + 1/*y-cordinate*/, (Cell_size + 15) - 1 /*width*/, Cell_size - 1 /*height*/, colors[ID] /*color*/);
    }
}




class LShape_Block : public Block
{

public:
    LShape_Block()
    {

        ID = 1; // giving each shape a unique id for its color
        // maping each unique key to a specific value(different positions of a block)
        Cells[0] = {position(0, 2), position(1, 0), position(1, 1), position(1, 2)};

        Cells[1] = {position(0, 1), position(1, 1), position(2, 1), position(2, 2)};

        Cells[2] = {position(1, 0), position(1, 1), position(1, 2), position(2, 0)};

        Cells[3] = {position(0, 0), position(0, 1), position(1, 1), position(2, 1)};
        move(0, 3); // setting starting point of  block to upper-center of screen
    }
};



class Jshape_Block : public Block
{

public:
    Jshape_Block()
    {

        ID = 2;
        Cells[0] = {position(0, 0), position(1, 0), position(1, 1), position(1, 2)};

        Cells[1] = {position(0, 1), position(0, 2), position(1, 1), position(2, 1)};

        Cells[2] = {position(1, 0), position(1, 1), position(1, 2), position(2, 2)};

        Cells[3] = {position(0, 1), position(1, 1), position(2, 0), position(2, 1)};
        move(0, 3); // setting starting point of  block to upper-center of screen
    }
};



class Ishape_Block : public Block
{

public:
    Ishape_Block()
    {

        ID = 3;
        Cells[0] = {position(1, 0), position(1, 1), position(1, 2), position(1, 3)};

        Cells[1] = {position(0, 2), position(1, 2), position(2, 2), position(3, 2)};

        Cells[2] = {position(2, 0), position(2, 1), position(2, 2), position(2, 3)};

        Cells[3] = {position(0, 1), position(1, 1), position(2, 1), position(3, 1)};
        move(-1, 3); // setting starting point of  block to upper-center of screen
    }
};



class Oshape_Block : public Block
{

public:
    Oshape_Block()
    {

        ID = 4;
        Cells[0] = {position(0, 0), position(0, 1), position(1, 0), position(1, 1)};
        move(0, 4); // setting starting point of  block to upper-center of screen
    }
};



class Sshape_Block : public Block
{

public:
    Sshape_Block()
    {

        ID = 5;
        Cells[0] = {position(0, 1), position(0, 2), position(1, 0), position(1, 1)};

        Cells[1] = {position(0, 1), position(1, 1), position(1, 2), position(2, 2)};

        Cells[2] = {position(1, 1), position(1, 2), position(2, 0), position(2, 1)};

        Cells[3] = {position(0, 0), position(1, 0), position(1, 1), position(2, 1)};
        move(0, 3); // setting starting point of  block to upper-center of screen
    }
};



class Tshape_Block : public Block
{

public:
    Tshape_Block()
    {

        ID = 6;
        Cells[0] = {position(0, 1), position(1, 0), position(1, 1), position(1, 2)};

        Cells[1] = {position(0, 1), position(1, 1), position(1, 2), position(2, 1)};

        Cells[2] = {position(1, 0), position(1, 1), position(1, 2), position(2, 1)};

        Cells[3] = {position(0, 1), position(1, 0), position(1, 1), position(2, 1)};
        move(0, 3); // setting starting point of  block to upper-center of screen
    }
};



class Zshape_Block : public Block
{

public:
    Zshape_Block()
    {

        ID = 7;
        Cells[0] = {position(0, 0), position(0, 1), position(1, 1), position(1, 2)};

        Cells[1] = {position(0, 2), position(1, 1), position(1, 2), position(2, 1)};

        Cells[2] = {position(1, 0), position(1, 1), position(2, 1), position(2, 2)};
        
        Cells[3] = {position(0, 1), position(1, 0), position(1, 1), position(2, 0)};
        move(0, 3); // setting starting point of  block to upper-center of screen
    }
};



// member function to refill the vector with all blocks
vector<Block> game::get_all_blocks()
{
     return {Ishape_Block(), Jshape_Block(), LShape_Block(), Oshape_Block(), Sshape_Block(), Tshape_Block(),Zshape_Block()};
}




// member function for picking random block at each time
Block game::Random_block()
{

    if (blocks.empty())
    {
        blocks = get_all_blocks();
    }

    // getting random index for randomly picking block
    int Random_block = rand() % blocks.size();

    Block block = blocks[Random_block];

    // ensuring that the picked block will not repeat until all block appeared
    blocks.erase(blocks.begin() + Random_block);
    return block;
}



// function for changing the cordinates of the origin of a block (by adding values in x and y cordinates)
void Block::move(int rows, int columns)
{

    Row_offset += rows;
    Column_offset += columns;

}



vector<position> Block::Cell_position()
{

    vector<position> Tiles = Cells[rotation_state]; // assiging current position of block to a vector
    vector<position> Moved_Tiles;  

    for (position temp : Tiles)
    {
        // new position by adding offsets point in cordinates
        position new_position = position(temp.row + Row_offset, temp.column + Column_offset);
        Moved_Tiles.push_back(new_position); // adding new position in moved tiles vector
    }

    return Moved_Tiles; // return updated position
}





void game::handling_input()
{
    // built in function provides by raylib for getting key tat user enter
    int Key_press = GetKeyPressed();

    switch (Key_press)
    {

    case KEY_DOWN:
        Moving_down();
        break;


    case KEY_RIGHT:
        Moving_right();
        break;

    case KEY_LEFT:
        Moving_left();
        break;

    case KEY_UP:
        rotate_block();
        break;
    }
}



void game::Moving_down()
{
    if (!gameOver)
    {

        current_block.move(1, 0);

        // checking whether block is outside the window screen or is right above another block
        if (Is_block_outside() || fitting_block() == false)
        {
            current_block.move(-1, 0);
            locking_block(); // for locking block at ground(fix it)
        }
    }
}



void game::Moving_left()
{
    if (!gameOver) // means game is not over
    {

        current_block.move(0, -1);
        if (Is_block_outside() || fitting_block() == false)
        {
            current_block.move(0, 1);
        }
    }
}



void game::Moving_right()
{
    if (!gameOver)
    {

        current_block.move(0, 1);
        if (Is_block_outside() || fitting_block() == false)
        {
            current_block.move(0, -1);
        }
    }
}



// for rotating blocks
void game::rotate_block()
{
    if (!gameOver)
    {

        current_block.rotate();
        if (Is_block_outside() || fitting_block() == false)
        {
            current_block.undo_rotation();
        }
    }
}



void Block::rotate()
{
    rotation_state++;
    if (rotation_state == (int)Cells.size())
    {
        rotation_state = 0;
    }
}


void Block::undo_rotation()
{
    rotation_state--; // setting block to previous rotation state

    if (rotation_state == -1)
    {
        rotation_state = Cells.size() - 1;
    }
}




// for checking whether blocks is going outside the window screen
bool Grid::checking_is_Cell_outside(int row, int column)
{
    if (row >= 0 && row < Number_of_rows && column >= 0 && column < Number_of_columns)
    {
        return false;  // means block is inside boundry
    }
    return true; 
}


// member function for not letting block going outsides boundry
bool game::Is_block_outside()
{
    // assigning cells position of current block to tiles
    std::vector<position> Tiles = current_block.Cell_position();

    // and checking whether is that outside or not
    for (position temp : Tiles)
    {
        if (grid.checking_is_Cell_outside(temp.row, temp.column)) // if true means outside
        {
            return true; 
        }
    }
    return false;
}


bool Grid::Cell_empty(int row, int column)
{
    if (grid[row][column] == 0)
    {
        return true;
    }
    return false;
}

// for locking block
bool game::fitting_block()
{
    std::vector<position> Tiles = current_block.Cell_position();

    for (position temp : Tiles)
    {
        if (grid.Cell_empty(temp.row, temp.column) == false)
        {
            return false; // if cells are not empty , returning false
        }
    }
    return true;
}



// for locking block once it touch the ground
void game::locking_block()
{
    vector<position> Tiles = current_block.Cell_position();

    for (position temp : Tiles)
    {
        grid.grid[temp.row][temp.column] = current_block.ID;
    }

    current_block = next_block;

    if (fitting_block() == false)
    { // means cells are not empty
        gameOver = true;
    }
    next_block = Random_block();
    grid.clearing_allRows(); // if row is completed
}


// this will checking whether a row is full of block
bool Grid::checking_row_full(int row)
{
    for (int column = 0; column < Number_of_columns; column++)
    {
        if (grid[row][column] == 0)
        {
            return false; // if any cell is empty , returning false
        }
    }
    return true;
}



// this will empty a row
void Grid::clearing_row(int row)
{
    for (int column = 0; column < Number_of_columns; column++)
    {
        grid[row][column] = 0;
    }
}



// will will the moved the upper row to down
void Grid::move_row_down(int row, int NumRows)
{
    for (int column = 0; column < Number_of_columns; column++)
    {
        // this will move the (row) numRows time and will clear it actual row position
        grid[row + NumRows][column] = grid[row][column]; 
        grid[row][column] = 0;
    }
}




void Grid::clearing_allRows()
{
    int full_completed_row = 0;

    for (int row = Number_of_rows - 1; row >= 0; row--)
    {
        if (checking_row_full(row)) // it means that row is full
        {
            clearing_row(row);
            full_completed_row++;
        }
        else if (full_completed_row > 0)
        {
            move_row_down(row, full_completed_row);
        }
    }
}


void game::draw() // for displaying game
{

    grid.draw();
    current_block.draw();
}

////////////// main ////////////////

double updated_time = 0; // for keeping each  last updated time of the game

// for moving block after a centain interval
bool update(double interval)
{
    double current_time = GetTime(); // GetTime is an raylib function for current time

    if (current_time - updated_time >= interval)
    {
        updated_time = current_time;
        return true;
    }
    return false;
}




int main()
{
    Color black = {0, 0, 0, 255}; // Define black color
    bool gameRunning = false;     // Flag to check if the game is running

    InitWindow(600, 900, "Tetris Game"); // Initialize window
    SetTargetFPS(60);                    // Set frames per second

    game Game; // Create game instance

    // Main game loop
    while (!WindowShouldClose())
    {
        // Check if game is running
        if (gameRunning)
        {
            Game.handling_input(); // Handle user input

            // Update game every 0.25 seconds
            if (update(0.25))
            {
                Game.Moving_down(); // Move block down
            }

            // Draw game
            BeginDrawing();
            ClearBackground(black);
            Game.draw();
            EndDrawing();

            // Check for game over condition
            if (Game.gameOver)
            {
                gameRunning = false; // Stop the game
            }
        }
        else
        {
            if (Game.gameOver)
            {
                // Show game over screen
                BeginDrawing();
                ClearBackground(black);
                DrawText("Game Over", 250, 400, 30, WHITE);
                DrawText("Press Enter to Play Again", 180, 450, 20, WHITE);
                EndDrawing();

                // Check for Enter key press to start or restart game
                if (IsKeyPressed(KEY_ENTER))
                {
                    Game = game();       // Restart the game
                    gameRunning = true;  // Start the game
                }
            }
            else
            {
                // Show start screen
                BeginDrawing();
                ClearBackground(black);
                DrawText("Press Enter to Start", 200, 400, 20, WHITE);
                EndDrawing();

                // Check for Enter key press to start or restart game
                if (IsKeyPressed(KEY_ENTER))
                {
                    Game = game();       // Start the game
                    gameRunning = true;  // Start the game
                }
            }
        }
    }

    CloseWindow(); // Close window
    return 0;
}
