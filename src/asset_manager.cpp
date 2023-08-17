#include "../include/asset_manager.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

AssetManager::~AssetManager()
{
    releaseAllAssets();
}

Texture2D
AssetManager::bindTexture(const std::string& texFile)
{
    if (m_texFileToTexId.count(texFile))
    {
        const auto texId = m_texFileToTexId[texFile];

        if (m_texIdToTexture.count(texId))
            return m_texIdToTexture[texId];
        else
            m_texFileToTexId.erase(texFile);
    }

    if (!std::filesystem::exists(texFile))
        throw std::runtime_error("TEXTURE NOT FOUND: '" + texFile + "'");

    const auto img = LoadImage(texFile.c_str());
    const auto tex = LoadTextureFromImage(img);
    UnloadImage(img);

    m_texFileToTexId[texFile] = tex.id;
    m_texIdToTexture[tex.id]  = tex;

    return tex;
}

void
AssetManager::releaseTexture(Texture2D texture)
{
    const auto texId = texture.id;

    if (!m_texIdToTexture.count(texId))
        return;

    UnloadTexture(m_texIdToTexture[texId]);
    m_texIdToTexture.erase(texId);
}

void
AssetManager::releaseAllTextures()
{
    for (auto&& texIdAndTexture : m_texIdToTexture)
        UnloadTexture(texIdAndTexture.second);

    m_texFileToTexId.clear();
    m_texIdToTexture.clear();
}

nlohmann::json
AssetManager::bindSpriteJson(const std::string& spriteJsonFile)
{
    if (m_fileToSpriteJson.count(spriteJsonFile))
        return m_fileToSpriteJson[spriteJsonFile];

    if (!std::filesystem::exists(spriteJsonFile))
        throw std::runtime_error("FILE NOT FOUND: '" + spriteJsonFile + "'");

    m_fileToSpriteJson[spriteJsonFile] = nlohmann::json::parse(std::ifstream(spriteJsonFile));

    return m_fileToSpriteJson[spriteJsonFile];
}

void
AssetManager::releaseSpriteJson(const std::string& spriteJsonFile)
{
    if (!m_fileToSpriteJson.count(spriteJsonFile))
        return;

    m_fileToSpriteJson.erase(spriteJsonFile);
}

void
AssetManager::releaseAllSpriteJson()
{
    m_fileToSpriteJson.clear();
}

void
AssetManager::releaseAllAssets()
{
    releaseAllTextures();
    releaseAllSpriteJson();
}
