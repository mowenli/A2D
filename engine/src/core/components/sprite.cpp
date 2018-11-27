//
// Created by selya on 09.11.2018.
//

#include <a2d/core/components/sprite.h>
#include <a2d/core/renderer.h>
#include <a2d/core/object2d.h>

namespace a2d {

Sprite::Sprite() : color(1), texture_region(nullptr), shader(Shader::GetShader("default")) {

}

const pTextureRegion &Sprite::GetTextureRegion() {
    return texture_region;
}

void Sprite::SetTextureRegion(const pTextureRegion &texture_region) {
    this->texture_region = texture_region;
}

void Sprite::Draw(SpriteBatch &sprite_batch) {
    if (!texture_region) return;
    static Vector2f p1, p2, p3, p4;
    float ratio = texture_region->GetRatio();
    p1.Set(0.0f);
    p2.Set(ratio, 0.0f);
    p3.Set(ratio, 1.0f);
    p4.Set(0.0f, 1.0f);
    sprite_batch.Draw(texture_region, shader, p1, p2, p3, p4, GetObject2D()->GetTransformMatrix(), color);
}

void Sprite::OnEnable() {

}

void Sprite::OnDisable() {

}

Sprite::~Sprite() {

}

} //namespace a2d
