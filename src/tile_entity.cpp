#include "../include/tile_entity.hpp"
#include "../include/tile_map.hpp"
#include <raylib.h>

TileEntity::TileEntity(
    const Vector2&   position,
    const Rectangle& sourceRect,
    const Rectangle& destRect,
    const Texture2D& texture)
    : m_sourceRect(sourceRect),
      m_destRect(destRect),
      m_texture(texture)
{
    this->position = position;
}

void
TileEntity::draw()
{
    DrawTexturePro(
        m_texture,
        m_sourceRect,
        m_destRect,
        {0, 0}, 0, WHITE);
}