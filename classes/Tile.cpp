#include "Tile.h"

// ============= CTOR ==================

Tile::Tile(Sprite revealed, Sprite hidden)
{
    // create the tile sprites internally, both revealed and hidden
    // Makes a sprite and loads a texture in 1 line (instead of 3) using TextureManager class
    isRevealed = false;
    spriteRevealed = revealed;
    spriteHidden = hidden;
    isMine = false;
}

// ============= SETTERS ==================

void Tile::Draw(RenderWindow &window)
{
    if(isRevealed) {
        window.draw(spriteRevealed);
        if(isMine)
            window.draw(spriteMine);
        else
            window.draw(spriteNumMines);
    }
    else {
        window.draw(spriteHidden);
        if(isFlag)
            window.draw(spriteFlag);
    }
}

void Tile::Reveal() { isRevealed = true;}

void Tile::Hide(){ isRevealed = false;}

void Tile::Flag(){ isFlag = !isFlag;}

void Tile::SetMine(Sprite& mine){ spriteMine = mine; isMine = true; }

void Tile::SetMineNumber(Sprite& number) { spriteNumMines = number; }

void Tile::Flip(){ isRevealed = !isRevealed; }

void Tile::SetFlag(Sprite& flag){ spriteFlag = flag; }

// ============= ACCESSORS ==================

Sprite Tile::GetHiddenTile() { return spriteHidden;}

Sprite Tile::GetRevealedTile() { return spriteRevealed;}

bool Tile::CheckIfMine() {return isMine; }

bool Tile::CheckIfFlipped() { return isRevealed; }

bool Tile::HasFlag() { return isFlag;}
