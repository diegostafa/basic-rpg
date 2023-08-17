#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include <nlohmann/json.hpp>
#include <raylib.h>
#include <string>
#include <unordered_map>

typedef unsigned int TextureId;
typedef unsigned int SpriteSheetId;

class AssetManager
{
private:
    std::unordered_map<std::string, TextureId> m_texFileToTexId;
    std::unordered_map<TextureId, Texture2D>   m_texIdToTexture;

    std::unordered_map<std::string, nlohmann::json> m_fileToSpriteJson;

public:
    ~AssetManager();

    Texture2D bindTexture(const std::string& texFile);
    void      releaseTexture(Texture2D texture);
    void      releaseAllTextures();

    nlohmann::json bindSpriteJson(const std::string& spriteJsonFile);
    void           releaseSpriteJson(const std::string& spriteJsonFile);
    void           releaseAllSpriteJson();

    void releaseAllAssets();
};

#endif