#include "../include/sprite_animation.hpp"
#include "../include/asset_manager.hpp"
#include <fstream>
#include <iostream>

SpriteAnimation::SpriteAnimation(const std::string& imageFile, const std::string& dataFile, AssetManager& assets)
{
    m_spriteSheet  = assets.bindTexture(imageFile);
    m_frameData    = assets.bindSpriteJson(dataFile)["frames"];
    m_totalFrames  = m_frameData.size();
    m_currentFrame = 0;

    setCurrentSpriteFrame();
}

void
SpriteAnimation::incrementFrameCounter()
{
    m_currentFrame++;
    if (m_currentFrame == m_totalFrames)
        m_currentFrame = 0;
}

void
SpriteAnimation::setCurrentSpriteFrame()
{
    auto it = m_frameData.cbegin();
    std::advance(it, m_currentFrame);
    auto child = *it;

    frameSize =
        {child["frame"]["x"].get<float>(),
         child["frame"]["y"].get<float>(),
         child["frame"]["w"].get<float>(),
         child["frame"]["h"].get<float>()};

    bbox =
        {child["spriteSourceSize"]["x"].get<float>(),
         child["spriteSourceSize"]["y"].get<float>(),
         child["spriteSourceSize"]["w"].get<float>(),
         child["spriteSourceSize"]["h"].get<float>()};
}

void
SpriteAnimation::update()
{
    if (m_frameTimer.start().update().hasElapsed())
    {
        incrementFrameCounter();
        setCurrentSpriteFrame();
        m_frameTimer.reset();
    }
}

void
SpriteAnimation::draw(const Vector2& position, unsigned char alpha) const
{
    const Vector2 bboxPos = {position.x - bbox.width / 2, position.y - bbox.height / 2};

    DrawTextureRec(
        m_spriteSheet,
        frameSize,
        {bboxPos.x - bbox.x,
         bboxPos.y - bbox.y},
        {255, 255, 255, alpha});
}

void
SpriteAnimation::setFrameAnimationSpeed(int milliseconds)
{
    m_frameTimer.duration = milliseconds;
}