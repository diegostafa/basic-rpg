#ifndef TILE_ENTITY_HPP
#define TILE_ENTITY_HPP

#include "drawable.hpp"
#include <string>

class TileEntity : public Drawable
{
private:
    const Rectangle m_sourceRect;
    const Rectangle m_destRect;
    const Texture2D m_texture;

public:
    TileEntity() = default;

    TileEntity(
        const Vector2&   position,
        const Rectangle& sourceRect,
        const Rectangle& destRect,
        const Texture2D& texture);

    virtual void draw() override;
};

#endif