#include "../include/tile_map.hpp"
#include "../include/asset_manager.hpp"

#include <iostream>
#include <raymath.h>
#include <tmxlite/Layer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/TileLayer.hpp>

TileMap::TileMap(const std::string& tmxFile, AssetManager& assets)
{
    map.load(tmxFile);

    for (auto&& ts : map.getTilesets())
    {
        const auto tsFile        = ts.getImagePath();
        tilesetToTexture[tsFile] = assets.bindTexture(tsFile);
    }

    setPortals();
    setObstacles();
    setInteractables();

    setBaseIndex();
    setStartingPosition();
}

void
TileMap::setBaseIndex()
{
    const auto& layers = map.getLayers();
    for (int i = 0; i < layers.size(); i++)
    {
        if (layers[i]->getName() != "BL")
            continue;

        baseIndex = i;
        break;
    }
}

void
TileMap::setObstacles()
{
    for (auto&& layer : map.getLayers())
    {
        if (layer->getType() != tmx::Layer::Type::Object)
            continue;

        if (layer->getName() != "COLL")
            continue;

        const auto objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();

        for (auto&& obj : objects)
        {
            obstacles.push_back(
                {obj.getAABB().left,
                 obj.getAABB().top,
                 obj.getAABB().width,
                 obj.getAABB().height});
        }
    }
}

void
TileMap::setInteractables()
{
    for (auto&& layer : map.getLayers())
    {
        if (layer->getType() != tmx::Layer::Type::Object)
            continue;

        if (layer->getName() != "INTERACT")
            continue;

        const auto objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();

        for (auto&& obj : objects)
        {
            interactables.push_back(
                {{obj.getAABB().left,
                  obj.getAABB().top,
                  obj.getAABB().width,
                  obj.getAABB().height},
                 obj.getName()});
        }
    }
}

void
TileMap::setPortals()
{
    for (auto&& layer : map.getLayers())
    {
        if (layer->getType() != tmx::Layer::Type::Object)
            continue;

        if (layer->getName() != "TRIGGERS")
            continue;

        const auto objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();

        for (auto&& obj : objects)
        {
            portals.push_back(
                {{obj.getAABB().left,
                  obj.getAABB().top,
                  obj.getAABB().width,
                  obj.getAABB().height},
                 obj.getName()});
        }
    }
}

void
TileMap::setStartingPosition()
{
    for (auto&& layer : map.getLayers())
    {
        if (layer->getType() != tmx::Layer::Type::Object)
            continue;

        if (layer->getName() != "SP")
            continue;

        const auto objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();

        for (auto&& obj : objects)
            startingPoints.push_back({obj.getPosition().x, obj.getPosition().y});
    }
}

const tmx::Tileset*
TileMap::getTilesetFromTile(const tmx::TileLayer::Tile& tile) const
{
    for (auto&& tileset : map.getTilesets())
    {
        if (tile.ID >= tileset.getFirstGID() && tile.ID < tileset.getFirstGID() + tileset.getTileCount())
            return &tileset;
    }

    return nullptr;
}

void
TileMap::drawTileLayer(const tmx::TileLayer& layer) const
{
    const auto& tiles = layer.getTiles();

    for (int i = 0; i < tiles.size(); i++)
    {
        const auto& tile = tiles[i];

        if (tile.ID == 0)
            continue;

        const auto tileset = getTilesetFromTile(tile);
        if (tileset == nullptr)
            continue;

        const int localTileId = tile.ID - tileset->getFirstGID();

        const int tilesetRow = localTileId / tileset->getColumnCount();
        const int tilesetCol = localTileId % tileset->getColumnCount();

        const int tilemapRow = i / map.getTileCount().x;
        const int tilemapCol = i % map.getTileCount().x;

        const float tileW = map.getTileSize().x;
        const float tileH = map.getTileSize().y;

        DrawTexturePro(
            tilesetToTexture.at(tileset->getImagePath()),
            {tilesetCol * tileW, tilesetRow * tileH, tileW, tileH},
            {tilemapCol * tileW, tilemapRow * tileH, tileW, tileH},
            {0, 0}, 0, WHITE);
    }
}

void
TileMap::drawLowerLayers()
{
    const auto& layers = map.getLayers();

    for (int i = 0; i < baseIndex; i++)
    {
        const auto& layer = layers[i];

        switch (layer->getType())
        {
        case tmx::Layer::Type::Tile:
            drawTileLayer(layer->getLayerAs<tmx::TileLayer>());
            break;

        default:
            break;
        }
    }
}

std::vector<TileEntity>
TileMap::getTileEntities() const
{
    std::vector<TileEntity> tileEntities;

    const auto& baseLayer     = map.getLayers()[baseIndex];
    const auto& baseTileLayer = baseLayer->getLayerAs<tmx::TileLayer>();
    const auto& tiles         = baseTileLayer.getTiles();

    for (int i = 0; i < tiles.size(); i++)
    {
        const auto& tile = tiles[i];

        if (tile.ID == 0)
            continue;

        const auto tileset = getTilesetFromTile(tile);
        if (tileset == nullptr)
            continue;

        const int   localTileId = tile.ID - tileset->getFirstGID();
        const int   tilesetRow  = localTileId / tileset->getColumnCount();
        const int   tilesetCol  = localTileId % tileset->getColumnCount();
        const int   tilemapRow  = i / map.getTileCount().x;
        const int   tilemapCol  = i % map.getTileCount().x;
        const float tileW       = map.getTileSize().x;
        const float tileH       = map.getTileSize().y;

        tileEntities.push_back(TileEntity(
            {tilemapCol * tileW + tileW / 2, tilemapRow * tileH + tileH / 2},
            {tilesetCol * tileW, tilesetRow * tileH, tileW, tileH},
            {tilemapCol * tileW, tilemapRow * tileH, tileW, tileH},
            tilesetToTexture.at(tileset->getImagePath())));
    }

    return tileEntities;
}

void
TileMap::drawUpperLayers()
{
    const auto& layers = map.getLayers();

    for (int i = baseIndex + 1; i < layers.size(); i++)
    {
        const auto& layer = layers[i];

        switch (layer->getType())
        {
        case tmx::Layer::Type::Tile:
            drawTileLayer(layer->getLayerAs<tmx::TileLayer>());
            break;

        default:
            break;
        }
    }
}
