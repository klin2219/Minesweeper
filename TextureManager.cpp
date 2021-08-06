#include "TextureManager.h"

// "redeclare" the variable, so this file knows it exists and can use it
unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(const string& fileName)
{
    string path = "images/";
    path += fileName + ".png";

    textures[fileName].loadFromFile(path);
}

sf::Texture& TextureManager::GetTexture(string textureName)
{
    // if texture doesn't exist, load it
    if(textures.find(textureName) == textures.end()) // we didn't find it
        LoadTexture(textureName);

    return textures[textureName];
}



void TextureManager::Clear()
{
    textures.clear(); // get rid of all stored objects
}