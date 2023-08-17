#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include "asset_manager.hpp"
#include "tile_entity.hpp"

#include <raylib.h>
#include <string>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Tileset.hpp>
#include <vector>

class TileMap
{
private:
    int                                        baseIndex;
    tmx::Map                                   map;
    std::unordered_map<std::string, Texture2D> tilesetToTexture;

    void setPortals();
    void setInteractables();
    void setObstacles();
    void setBaseIndex();
    void setStartingPosition();

    void                drawTileLayer(const tmx::TileLayer& layer) const;
    const tmx::Tileset* getTilesetFromTile(const tmx::TileLayer::Tile& tile) const;

public:
    std::vector<std::pair<Rectangle, std::string>> interactables  = {};
    std::vector<std::pair<Rectangle, std::string>> portals        = {};
    std::vector<Rectangle>                         obstacles      = {};
    std::vector<Vector2>                           startingPoints = {};

    TileMap(const std::string& tmxFile, AssetManager& assets);

    void                    load(const std::string& tmxFile);
    std::vector<TileEntity> getTileEntities() const;
    void                    drawLowerLayers();
    void                    drawUpperLayers();
};

#endif