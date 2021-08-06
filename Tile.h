#include <SFML/Graphics.hpp>
#include <vector>
using namespace std;
using namespace sf;

class Tile
{
    bool isRevealed = false, isMine = false, isFlag = false;
    Sprite spriteRevealed, spriteHidden;
    Sprite spriteMine, spriteFlag;
    Sprite spriteNumMines;

public:
    // CTOR
    Tile(Sprite revealed, Sprite hidden);

    // SETTERS
    void Draw(RenderWindow& window);
    void SetMineNumber(Sprite& number);
    void Flag();
    void Flip();
    void SetFlag(Sprite &flag);
    void SetMine(Sprite& mine);
    void Reveal();
    void Hide();

    // ACCESSORS
    Sprite GetHiddenTile();
    Sprite GetRevealedTile();
    bool CheckIfMine();
    bool CheckIfFlipped();
    bool HasFlag();

};
