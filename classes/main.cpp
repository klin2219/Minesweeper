// Kevin Lin
// Minesweeper Project
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "Tile.h"
#include <unordered_map>
#include "Random.h"
#include "TextureManager.h"
using namespace std;
using namespace sf;


// Loads the board dimensions: number of columns, rows, and mines
vector<int> BoardInfo(const string& path)
{
    ifstream Data(path);

    if(!Data){
        cout << "Error opening file" << endl;
        exit(1);
    }

    vector<int> dimensions;   int info;

    while(Data >> info){
        dimensions.push_back(info);
        // cout << info << endl;
    }

    return dimensions;
}

// Checks for duplicates, returns true if found
bool CheckDuplicate(vector<int>& mines, int& num){
    for(int i = 0; i < mines.size(); i++){
        if(mines.at(i) == num)
            return true;
    }
    return false;
}

// Returns a randomly generated vector of mine sprites and sets their positions
vector<Sprite> RandomlyGenerateMinePosition(vector<int>& dimension, unordered_map<string, Sprite>& buttons, Sprite& mine){

    // Get a random row
    int xCoord = Random::Int(0, dimension.at(0)) * 32;
    // Get a random column
    int yCoord = Random::Int(0, dimension.at(1)) * 32;

    vector<int> mines;
    vector<Sprite> minePlaces;
    int randomNum = Random::Int(0, dimension.at(0) * dimension.at(1));
    while(mines.size() < dimension.at(2)){
        while(CheckDuplicate(mines, randomNum) != false) {
            randomNum = Random::Int(0, dimension.at(0) * dimension.at(1));
        }
        xCoord = randomNum % dimension.at(0) * 32;
        yCoord = randomNum / dimension.at(0) * 32;
        mine.setPosition(xCoord, yCoord);
        mines.push_back(randomNum);
        minePlaces.push_back(mine);
        randomNum = Random::Int(0, dimension.at(0) * dimension.at(1));
    }
    return minePlaces;
}

// Randomizes the mines for restarting the game
void RandomizeMines(vector<Tile>& tiles, unordered_map<string, Sprite>& images, vector<int>& dimension) {
    vector<Sprite> randomMines = RandomlyGenerateMinePosition(dimension, images, images["mine"]);

    for(int i = 0; i < dimension.at(1); i++){
        for(int j = 0; j < dimension.at(0); j++){
            images["revealed_tile"].setPosition(j* 32, i * 32);
            images["hidden_tile"].setPosition(j * 32, i * 32);
            Tile newTile(images["revealed_tile"], images["hidden_tile"]);

            for(int z = 0; z < randomMines.size(); z++){
                if(randomMines.at(z).getPosition() == images["revealed_tile"].getPosition())
                    newTile.SetMine(randomMines.at(z));
            }
            tiles.push_back(newTile);
        }
    }

}

// Loads a vector of tiles with coordinates for the board
vector<Tile> LoadTiles(vector<int>& dimensions, RenderWindow& window, unordered_map<string, Sprite>& buttons) {

    vector<Sprite> mines = RandomlyGenerateMinePosition(dimensions, buttons, buttons["mine"]);

    // create the tile sprites internally, both revealed and hidden
    vector<Tile> tiles;
    for(int i = 0; i < dimensions.at(1); i++){
        for(int j = 0; j < dimensions.at(0); j++){
            buttons["revealed_tile"].setPosition(j * 32, i * 32);
            buttons["hidden_tile"].setPosition(j * 32, i * 32);
            Tile newTile(buttons["revealed_tile"], buttons["hidden_tile"]);

            for(int z = 0; z < mines.size(); z++){
                if(mines.at(z).getPosition() == buttons["revealed_tile"].getPosition())
                    newTile.SetMine(mines.at(z));
            }
            tiles.push_back(newTile);
        }
    }

    return tiles;
}

// Draws the tiles to the window
void DrawTiles(RenderWindow& window, vector<Tile>& tiles){
    for(auto& this_tile: tiles){
        this_tile.Draw(window);
    }

}

// Loads all the Sprites in images folder
unordered_map<string, Sprite> LoadImages(RenderWindow& window, vector<int>& dimension){
    // create an unordered map to be returned for use in other parts of program
    unordered_map<string, Sprite> buttons;

    // creates the Sprite and stores in the buttons map
    // Makes a sprite and loads a texture in 1 line (instead of 3) using TextureManager class
    buttons["revealed_tile"] =  Sprite(TextureManager::GetTexture("tile_revealed"));
    buttons["hidden_tile"] = Sprite(TextureManager::GetTexture("tile_hidden"));
    buttons["digits"] = Sprite(TextureManager::GetTexture("digits"));
    buttons["face_happy"] = Sprite(TextureManager::GetTexture("face_happy"));
    buttons["face_lose"] = Sprite(TextureManager::GetTexture("face_lose"));
    buttons["face_win"] = Sprite(TextureManager::GetTexture("face_win"));
    buttons["debug"] = Sprite(TextureManager::GetTexture("debug"));
    buttons["test_1"] = Sprite(TextureManager::GetTexture("test_1"));
    buttons["test_2"] = Sprite(TextureManager::GetTexture("test_2"));
    buttons["test_3"] = Sprite(TextureManager::GetTexture("test_3"));
    buttons["flag"] = Sprite(TextureManager::GetTexture("flag"));
    buttons["mine"] = Sprite(TextureManager::GetTexture("mine"));
    for(int i = 1; i <= 8; i++){
        string numPath = "number_"; numPath += to_string(i);
        buttons[numPath] = Sprite(TextureManager::GetTexture(numPath));
    }
    buttons["-"] = Sprite(TextureManager::GetTexture("digits"));
    buttons["-"].setTextureRect(sf::IntRect(10 * 21, 0, 21, 31));
    for(int i = 0; i <= 9; i++){
        string name = "digit_";
        name += to_string(i);
        buttons[name] = Sprite(TextureManager::GetTexture("digits"));
        buttons[name].setTextureRect(sf::IntRect(i * 21,0,21,31));
    }


    return buttons;
}

// Draws the buttons on to the board
void DrawButtons(RenderWindow& window, unordered_map<string, Sprite>& buttons, vector<int>& dimension, int& ScoreCounter, int FlagCount
                , Sprite& hundred, Sprite& tenth, Sprite& one, Sprite& dash){

    // sets the position of Sprite and draws them on to the window
    buttons["face_happy"].setPosition( (dimension.at(0) / 2) * 32 + 34, dimension.at(1) * 32);
    buttons["face_lose"].setPosition( (dimension.at(0) / 2) * 32 + 34, dimension.at(1) * 32);
    buttons["face_win"].setPosition( (dimension.at(0) / 2) * 32 + 34, dimension.at(1) * 32);
    buttons["debug"].setPosition( (dimension.at(0) / 2) * 32 + 162, dimension.at(1) * 32);
    buttons["test_1"].setPosition( (dimension.at(0) / 2) * 32 + 226, dimension.at(1) * 32);
    buttons["test_2"].setPosition( (dimension.at(0) / 2) * 32 + 288, dimension.at(1) * 32);
    buttons["test_3"].setPosition( (dimension.at(0) / 2) * 32 + 352, dimension.at(1) * 32);
    window.draw(buttons["debug"]);
    window.draw(buttons["test_1"]); window.draw(buttons["test_2"]); window.draw(buttons["test_3"]);

    int first_digit = (ScoreCounter - FlagCount) / 100;
    int second_digit = (ScoreCounter - FlagCount) / 10;
    int third_digit = (ScoreCounter - FlagCount) % 10;

    bool Check = false;
    if(first_digit < 0){
        Check = true;
        first_digit *= -1;
    }

    if(second_digit < 0)
        second_digit *= -1;
    if(third_digit < 0)
        third_digit *= -1;

    if(second_digit > 99) {
        second_digit = second_digit % 100;
        if( second_digit > 9){
            second_digit = second_digit % 10;
        }
    }
    if(second_digit == 10)
        second_digit = 0;

    hundred = buttons["digit_" + to_string(first_digit)];
    tenth = buttons["digit_" +to_string(second_digit)];
    one = buttons["digit_" +to_string(third_digit)];

    if(ScoreCounter - FlagCount < -99)
    {

    }
    else if(ScoreCounter - FlagCount < 0){
        hundred = buttons["-"];
    }

    dash = buttons["-"];
    dash.setPosition(0, dimension.at(1) * 32);
    if(Check)
        window.draw(dash);
    hundred.setPosition( 21, dimension.at(1) * 32);
    tenth.setPosition( 42, dimension.at(1) * 32);
    one.setPosition( 63, dimension.at(1) * 32);
    window.draw(hundred); window.draw(tenth); window.draw(one);

}


// Reveals all the tiles
void RevealAll(vector<Tile>& tiles){
    for(int i = 0; i < tiles.size(); i++){
        if(tiles.at(i).CheckIfMine() == true)
            tiles.at(i).Reveal();
    }
}

// Draws a vector of mines for debugging
void DebugMines(vector<Sprite>& mines, vector<int>& dimension, vector<Tile>& tiles, Sprite& mine, bool& mode){
    if(mode == false){
        mode = true;
        mines.clear();
        for(int i = 0; i < tiles.size(); i++){
            if(tiles.at(i).CheckIfMine() == true){
                mine.setPosition(i % dimension.at(0) * 32, i / dimension.at(0) * 32);
                mines.push_back(mine);
            }
        }
    }
    else{
        mode = false;
        mines.clear();
    }

}

// Clears the board of tiles
void ClearBoard(vector<Tile>& board){
    for(int i = 0; i < board.size(); i++)
        board.erase(board.begin(), board.end());

}

// Starts a new board with everything reset AND mines randomized
void Restart(Vector2i& mousePosition, unordered_map<string, Sprite>& buttons, vector<Tile>& tiles, vector<int>& dimension,
             bool& GameOver, vector<Sprite>& debug_mines, bool debug_mode, int& ScoreCounter) {
    // Gets the bounds of both the sad and happy face buttons (they're the same coordinates)
    auto bounds = buttons["face_happy"].getGlobalBounds();
    auto bounds2 = buttons["face_sad"].getGlobalBounds();

    // If either of button is pressed, then ...
    if (bounds.contains(mousePosition.x, mousePosition.y) || bounds2.contains(mousePosition.x,mousePosition.y)) {
        // If the game has been restarted, then "Game" being over is FALSE
        GameOver = false;
        ScoreCounter = dimension.at(2);
        ClearBoard(tiles);
        RandomizeMines(tiles, buttons, dimension);

        if(debug_mode == true) {
            debug_mines.clear();
            for (int i = 0; i < tiles.size(); i++) {
                if (tiles.at(i).CheckIfMine() == true) {
                    buttons["mine"].setPosition(i % dimension.at(0) * 32, i / dimension.at(0) * 32);
                    debug_mines.push_back(buttons["mine"]);
                }
            }
        }
    }

}

// Loads the test board
void LoadTestBoard(string file, vector<Tile>& tiles, unordered_map<string, Sprite>& buttons, Vector2i& mousePosition, int& MineCount, vector<Sprite>& debug_mines, vector<int>& dimension,
                    bool debug_mode){
    string path = "boards/"; path += file + ".brd";

    string buttonName = "test_";
    buttonName += file.substr(file.length() - 1, 1);

    ifstream Board(path);
    auto bounds = buttons[buttonName].getGlobalBounds();
    int rowCount = 0, colCount = 0;

    if(bounds.contains(mousePosition.x, mousePosition.y)) {
        string data, lineFromFile;
        MineCount = 0;

        ClearBoard(tiles);

        vector<int> this_vector;
        while (getline(Board, lineFromFile)) {
            rowCount++;
            for (int i = 0; i < lineFromFile.length(); i++) {
                this_vector.push_back(stoi(lineFromFile.substr(i, 1)));
                colCount++;
            }
        }
        colCount /= rowCount;

        for (int i = 0; i < this_vector.size(); i++) {
            buttons["revealed_tile"].setPosition(i % colCount * 32, i / colCount * 32);
            buttons["hidden_tile"].setPosition(i % colCount * 32, i / colCount * 32);
            Tile newTile(buttons["revealed_tile"], buttons["hidden_tile"]);

            tiles.push_back(newTile);

            if (this_vector.at(i) == 1) {
                buttons["mine"].setPosition(i % colCount * 32, i / colCount * 32);
                tiles.at(i).SetMine(buttons["mine"]);
                MineCount++;

            }
        }

        if(debug_mode == true) {
            debug_mines.clear();
            for (int i = 0; i < tiles.size(); i++) {
                if (tiles.at(i).CheckIfMine() == true) {
                    buttons["mine"].setPosition(i % dimension.at(0) * 32, i / dimension.at(0) * 32);
                    debug_mines.push_back(buttons["mine"]);
                }
            }
        }

    }

}

// Drops a flag on the board
void DropFlag(vector<int>& dimension, Vector2i& mousePosition, unordered_map<string, Sprite>& buttons, vector<Tile>& tiles, bool GameOver, int& flagCount){

    for(int i = 0; i < tiles.size(); i++){
        auto bounds = tiles.at(i).GetHiddenTile().getGlobalBounds();
        if(bounds.contains(mousePosition.x, mousePosition.y)){
            //set position of flag at tile clicked
            buttons["flag"].setPosition(i % dimension.at(0) * 32,i / dimension.at(0) * 32);

            if(GameOver == false) {
                if(tiles.at(i).HasFlag() == true)
                    flagCount++;
                else
                    flagCount--;

                tiles.at(i).Flag();
                tiles.at(i).SetFlag(buttons["flag"]);
            }
        }
    }

}

// ======= FOR ADJACENT REVEALING =========== // ==============================================================
void CheckLeftCorner(vector<Tile>& tiles, const vector<int>& dimension, const int& index, int& mine_count){
    // Check tile to the right of it
    if(tiles.at(index + 1).CheckIfMine() == true)
        mine_count++;
    // Check tile below it
    if(tiles.at(index + dimension.at(0)).CheckIfMine() == true)
        mine_count++;
    // Check tile across it
    if(tiles.at(index + 1 + dimension.at(0)).CheckIfMine() == true)
        mine_count++;
}
void CheckTopRightCorner(vector<Tile>& tiles, const vector<int>& dimension, const int& index, int& mine_count){
    // Check tile to the left of it
    if(tiles.at(index - 1).CheckIfMine() == true)
        mine_count++;
    // Check the tile below it
    if(tiles.at(index + dimension.at(0)).CheckIfMine() == true)
        mine_count++;
    // Check the tile across it
    if(tiles.at(index - 1 + dimension.at(0)).CheckIfMine() == true)
        mine_count++;
}
void CheckBottomLeftCorner(vector<Tile>& tiles, const vector<int>& dimension, const int& index, int& mine_count){
    // Check for the tile to the right of it
    if(tiles.at(index + 1).CheckIfMine() == true)
        mine_count++;
    // Check for the tile above it
    if(tiles.at(index - dimension.at(0)).CheckIfMine() == true)
        mine_count++;
    // Check for the tile across it
    if(tiles.at(index + 1 - dimension.at(0)).CheckIfMine() == true)
        mine_count++;
}
void CheckBottomRightCorner(vector<Tile>& tiles, const vector<int>& dimension, const int& index, int& mine_count){
    // Check for the tile to the left of it
    if(tiles.at(index - 1).CheckIfMine() == true)
        mine_count++;
    // Check for the tile above it
    if(tiles.at(index - dimension.at(0)).CheckIfMine() == true)
        mine_count++;
    // Check for the tile across it
    if(tiles.at(index - 1 - dimension.at(0)).CheckIfMine() == true)
        mine_count++;
}
void CheckLeftEdges(vector<Tile>& tiles, const vector<int>& dimension, const int& index, int& mine_count){
    // Checks for the tile above it
    if(tiles.at(index - dimension.at(0)).CheckIfMine() == true)
        mine_count++;
    // Checks for the tile across to the top the right
    if(tiles.at(index + 1 - dimension.at(0)).CheckIfMine() == true)
        mine_count++;
    // Checks for the tile to the right of it
    if(tiles.at(index + 1).CheckIfMine() == true)
        mine_count++;
    // Checks the tile below it
    if(tiles.at(index + dimension.at(0)).CheckIfMine() == true)
        mine_count++;
    // Checks the tile across to teh bottom right
    if(tiles.at(index + 1 + dimension.at(0)).CheckIfMine() == true)
        mine_count++;
}
void CheckRightEdges(vector<Tile>& tiles, const vector<int>& dimension, const int& index, int& mine_count){
    // Checks the tile above it
    if(tiles.at(index - dimension.at(0)).CheckIfMine() == true)
        mine_count++;
    // Checks the tile to the top left across it
    if(tiles.at(index - 1 - dimension.at(0)).CheckIfMine() == true)
        mine_count++;
    // Checks for the tile to the left of it
    if(tiles.at(index - 1).CheckIfMine() == true)
        mine_count++;
    // Checks for the tile below it
    if(tiles.at(index + dimension.at(0)).CheckIfMine() == true)
        mine_count++;
    // Checks for the tile across it to the top left
    if(tiles.at(index - 1 + dimension.at(0)).CheckIfMine() == true)
        mine_count++;
}
void CheckTopEdges(vector<Tile>& tiles, const vector<int>& dimension, const int& index, int& mine_count){
    // Checks for the tile to the left
    if(tiles.at(index - 1).CheckIfMine() == true)
        mine_count++;
    // Checks for the tile to the right
    if(tiles.at(index + 1).CheckIfMine() == true)
        mine_count++;
    // Checks for the tile below it
    if(tiles.at(index + dimension.at(0)).CheckIfMine() == true)
        mine_count++;
    // Checks for the tile across it to the bottom left
    if(tiles.at(index - 1 + dimension.at(0)).CheckIfMine() == true)
        mine_count++;
    // Checks for the tile across it to the bottom right
    if(tiles.at(index + 1 + dimension.at(0)).CheckIfMine() == true)
        mine_count++;
}
void CheckBottomEdges(vector<Tile>& tiles, const vector<int>& dimension, const int& index, int& mine_count){
    // Checks for the tile to the left
    if(tiles.at(index - 1).CheckIfMine() == true)
        mine_count++;
    // Checks for the tile to the right
    if(tiles.at(index + 1).CheckIfMine() == true)
        mine_count++;
    // Checks for the tile to the across it to top left
    if(tiles.at(index - 1 - dimension.at(0)).CheckIfMine() == true)
        mine_count++;
    // Checks for the tile to across it to top right
    if(tiles.at(index + 1 - dimension.at(0)).CheckIfMine() == true)
        mine_count++;
    // Checks for the tile to the top
    if(tiles.at(index - dimension.at(0)).CheckIfMine() == true)
        mine_count++;
}
void CheckAllDirections(vector<Tile>& tiles, const vector<int>& dimension, const int& index, int& mine_count){
    // Checks for the tile to the left
    if(tiles.at(index - 1).CheckIfMine() == true)
        mine_count++;
    // Checks for the tile to the right
    if(tiles.at(index + 1).CheckIfMine() == true)
        mine_count++;
    // Checks for the tile across to top left
    if(tiles.at(index - 1 - dimension.at(0)).CheckIfMine() == true)
        mine_count++;
    // Checks for the tile across to top right
    if(tiles.at(index + 1 - dimension.at(0)).CheckIfMine() == true)
        mine_count++;
    // Checks for the tile across to top
    if(tiles.at(index - dimension.at(0)).CheckIfMine() == true)
        mine_count++;
    // Checks for the tile across to bottom
    if(tiles.at(index + dimension.at(0)).CheckIfMine() == true)
        mine_count++;
    // Checks for the tile across to bottom left
    if(tiles.at(index - 1 + dimension.at(0)).CheckIfMine() == true)
        mine_count++;
    // Checks for the tile across to bottom right
    if(tiles.at(index + 1 + dimension.at(0)).CheckIfMine() == true)
        mine_count++;
}

// Load the number of mines at this "index" in tiles
int LoadTileNeighbor(vector<Tile>& tiles, const vector<int>& dimension, const int& index){
    int xLoc = index % dimension.at(0);
    int yLoc = index / dimension.at(0);
    int mine_count = 0;

    // Gets the tile at this index
    if(tiles.at(index).CheckIfMine() == false) {

        // Check the top left corner
        if (xLoc == 0 && yLoc == 0)
            CheckLeftCorner(tiles, dimension, index, mine_count);

            // Check the top right corner
        else if (xLoc == dimension.at(0) - 1 && yLoc == 0)
            CheckTopRightCorner(tiles, dimension, index, mine_count);

            // Check bottom left corner
        else if (xLoc == 0 && yLoc == dimension.at(1) - 1)
            CheckBottomLeftCorner(tiles, dimension, index, mine_count);

            // Check for the bottom right corner
        else if (xLoc == dimension.at(0) - 1 && yLoc == dimension.at(1) - 1)
            CheckBottomRightCorner(tiles, dimension, index, mine_count);

            // Checks the left side, excluding corner
        else if (xLoc == 0)
            CheckLeftEdges(tiles, dimension, index, mine_count);

            // Checks for the right side, excluding corner
        else if (xLoc == dimension.at(0) - 1)
            CheckRightEdges(tiles, dimension, index, mine_count);

            // Checks for the top edges, excluding corner
        else if (yLoc == 0)
            CheckTopEdges(tiles, dimension, index, mine_count);

            // Checks for the bottom edges, excluding corner
        else if (yLoc == dimension.at(1) - 1)
            CheckBottomEdges(tiles, dimension, index, mine_count);

            // Checks all eight directions for those that are not corner or edges
        else
            CheckAllDirections(tiles, dimension, index, mine_count);

    }

    return mine_count;
}

// Set the number of mines surroundings
void SetSurrounding(vector<Tile>& tiles, vector<int>& dimension, int numMines, int& index, unordered_map<string, Sprite>& images){
    if(tiles.at(index).CheckIfFlipped() == false && tiles.at(index).HasFlag() == false)
        tiles.at(index).Flip();
    numMines = LoadTileNeighbor(tiles, dimension, index);
    for(int j = 1; j <= 8; j++) {
        if (numMines == j) {
            string mineNumber = "number_";
            mineNumber += to_string(numMines);
            images[mineNumber].setPosition(index % dimension.at(0) * 32, index / dimension.at(0) * 32);
            tiles.at(index).SetMineNumber(images[mineNumber]);
            break;
        }
    }
}

// Reveal adjacent tiles
void RevealAdjacentTiles(vector<Tile>& tiles, int index, vector<int>& dimension, unordered_map<string, Sprite>& images){
    if(tiles.at(index).CheckIfFlipped() == false && tiles.at(index).HasFlag() == false)
        tiles.at(index).Flip();
    int xPos = index % dimension.at(0);
    int yPos = index / dimension.at(0);
    int numMines = 0;
    int newIndex;

    // If not flagged and not revealed and num of adjacent mines = 0,
    // then... recursively reveal the surrounding mines

        // Left corner
        if(xPos == 0 && yPos == 0){
            // RIGHT TILE CHECK
            newIndex = index + 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
                && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // BOT TILE CHECK
            newIndex = index + dimension.at(0);
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // TILE ACROSS Check
            newIndex = index + dimension.at(0) + 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

        }

        // Right corner
        else if(xPos == dimension.at(0) - 1 && yPos == 0){
            // LEFT TILE CHECK
            newIndex = index - 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // BOT TILE CHECK
            newIndex = index + dimension.at(0);
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // TILE ACROSS Check
            newIndex = index + dimension.at(0) - 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);
        }

        // Bottom left corner
        else if(xPos == 0 && yPos == dimension.at(1) - 1){
            // RIGHT TILE CHECK
            newIndex = index + 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // ABOVE TILE CHECK
            newIndex = index - dimension.at(0);
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // TILE ACROSS Check
            newIndex = index - dimension.at(0) + 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);
        }

        // Bottom right corner
        else if(xPos == dimension.at(0) - 1 && yPos == dimension.at(1) - 1){
            // Left TILE CHECK
            newIndex = index - 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // ABOVE TILE CHECK
            newIndex = index - dimension.at(0);
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // TILE ACROSS Check
            newIndex = index - dimension.at(0) - 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);
        }

        // Top edge
        else if(yPos == 0){
            // Left TILE CHECK
            newIndex = index - 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // Right TILE CHECK
            newIndex = index + 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // BELOW TILE CHECK
            newIndex = index + dimension.at(0);
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // TILE ACROSS Check RIGHT
            newIndex = index + dimension.at(0) + 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // TILE ACROSS Check LEFT
            newIndex = index + dimension.at(0) - 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);
        }

        // Bottom edge
        else if(yPos == dimension.at(1) - 1){
            // Left tile
            newIndex = index - 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // Right tile
            newIndex = index + 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // Top Tile
            newIndex = index - dimension.at(0);
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // TILE ACROSS Check RIGHT
            newIndex = index - dimension.at(0) + 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // Tile across top left
            newIndex = index - dimension.at(0) - 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

        }

        // Right edge
        else if(xPos == dimension.at(0) - 1){
            // Left tile
            newIndex = index - 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // Bot tile
            newIndex = index + dimension.at(0);
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // Top Tile
            newIndex = index - dimension.at(0);
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // TILE ACROSS Check LEFT top
            newIndex = index - dimension.at(0) - 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // Tile across check left bot
            newIndex = index + dimension.at(0) - 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

        }

        // Left edge
        else if(xPos == 0){
            // Right tile
            newIndex = index + 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // Bot tile
            newIndex = index + dimension.at(0);
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // Top Tile
            newIndex = index - dimension.at(0);
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // TILE ACROSS Check Right top
            newIndex = index - dimension.at(0) + 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // Tile across check right bot
            newIndex = index + dimension.at(0) + 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

        }

        // For the middle of the board
        else{
            // Right tile
            newIndex = index + 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // Left tile
            newIndex = index - 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // Bot tile
            newIndex = index + dimension.at(0);
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // Top Tile
            newIndex = index - dimension.at(0);
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // TILE ACROSS Check Right top
            newIndex = index - dimension.at(0) + 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // Tile across check right bot
            newIndex = index + dimension.at(0) + 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // TILE ACROSS Check Left top
            newIndex = index - dimension.at(0) - 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

            // Tile across check Left bot
            newIndex = index + dimension.at(0) - 1;
            numMines = LoadTileNeighbor(tiles, dimension, newIndex);
            if(numMines == 0 && tiles.at(newIndex).HasFlag() == false
               && tiles.at(newIndex).CheckIfFlipped() == false)
                RevealAdjacentTiles(tiles, newIndex, dimension, images);
            else
                SetSurrounding(tiles, dimension, numMines, newIndex, images);

        }


    // If flagged || revealed, perform no action
    if(tiles.at(index).HasFlag() == true || tiles.at(index).CheckIfFlipped() == true)
        return;

}

// ======= FOR ADJACENT REVEALING =========== // ==============================================================

// Flips a tile on the board relative to the window
void FlipTile(Vector2i& mousePosition, vector<int>& dimension, vector<Tile>& tiles, bool& GameOver, unordered_map<string, Sprite>& images){
    int numMines = 0;

    if(GameOver == true)
        return;
    else {
        for (int i = 0; i < tiles.size(); i++) {
            auto bounds = tiles.at(i).GetHiddenTile().getGlobalBounds();
            if (bounds.contains(mousePosition.x, mousePosition.y)) {
                cout << "Tile successfully flipped!" << endl;


                if(tiles.at(i).HasFlag() == true){
                    cout << "Tile: " << i << " " << tiles.at(i).HasFlag() << endl;
                    continue;
                }

                if (tiles.at(i).CheckIfMine())
                    GameOver = true;
                // Shows the number of sprites at this location
                else{
                    numMines = LoadTileNeighbor(tiles, dimension, i);
                    for(int j = 1; j <= 8; j++) {
                        if (numMines == j) {
                            string mineNumber = "number_";
                            mineNumber += to_string(numMines);
                            images[mineNumber].setPosition(i % dimension.at(0) * 32, i / dimension.at(0) * 32);
                            tiles.at(i).SetMineNumber(images[mineNumber]);
                            break;
                        }
                    }
                    if(numMines == 0 && tiles.at(i).CheckIfFlipped() == false){
                        RevealAdjacentTiles(tiles, i, dimension, images);
                    }
                }

                if(tiles.at(i).CheckIfFlipped() == false && tiles.at(i).HasFlag() == false)
                    tiles.at(i).Flip();
            }
        }
    }

}


int main()
{
    // creates the window
    vector<int> dimension = BoardInfo("boards/config.cfg");
    RenderWindow window(VideoMode(dimension.at(0) * 32 ,(dimension.at(1) * 32) + 88), "Minesweeper");

    // loads the Tiles and buttons
    unordered_map<string, Sprite> images = LoadImages(window, dimension);
    vector<Tile> tiles = LoadTiles(dimension, window, images);
    vector<Sprite> debug_mines;
    Sprite mineSprite = images["mine"];
    bool debugMode = false, GameOver = false;
    int ScoreCounter = dimension.at(2);
    int flagCount = 0;
    Sprite hundred, tenth, one, dash;

    // Do this until the window is closed
    while (window.isOpen())
    {
        // It has this many "mines" that will be displayed on the digits

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if(event.type == Event::MouseButtonPressed){

                // 1. Get the mouse position relative to the window
                // 2. Get the bounds of sprite in question
                // 3. Check if the mouse position is within those bounds
                if(event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePosition = Mouse::getPosition(window);

                    LoadTestBoard("testboard1",tiles, images, mousePosition, ScoreCounter, debug_mines, dimension, debugMode);
                    LoadTestBoard("testboard2", tiles, images, mousePosition, ScoreCounter,debug_mines, dimension, debugMode);
                    LoadTestBoard("testboard3", tiles, images, mousePosition, ScoreCounter,debug_mines, dimension, debugMode);
                    FlipTile(mousePosition, dimension, tiles, GameOver, images);

                    // If the game is over, reveal all tiles
                    if(GameOver == true)
                        RevealAll(tiles);

                    Restart(mousePosition, images, tiles, dimension, GameOver, debug_mines, debugMode, ScoreCounter);

                    // Debug mode
                    auto bounds = images["debug"].getGlobalBounds();
                    if(bounds.contains(mousePosition.x, mousePosition.y))
                        DebugMines(debug_mines, dimension, tiles, mineSprite, debugMode);


                }
                else if(event.mouseButton.button == Mouse::Right) {
                    Vector2i mousePosition = Mouse::getPosition(window);
                    DropFlag(dimension, mousePosition, images, tiles, GameOver, ScoreCounter);
                }
            }
        }

        // Clear what is on the screen
        window.clear(Color::White);

        // Sets the board with background, cover tiles, and buttons
        DrawTiles(window, tiles);
        DrawButtons(window, images, dimension, ScoreCounter, flagCount, hundred, tenth, one,dash);

        if(GameOver == true)
            window.draw(images["face_lose"]);
        else if(ScoreCounter == 0)
             window.draw(images["face_win"]);
        else
            window.draw(images["face_happy"]);

        // Have the mines go over everything for debug
        for(int i = 0; i < debug_mines.size(); i++)
            window.draw(debug_mines.at(i));

        // Present everything that has been drawn to the screen - call this once
        window.display();
    }

    // Clear out the textures before the program ends
    TextureManager::Clear();
    return 0;
}
