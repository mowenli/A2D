//
// Created by selya on 11.11.2018.
//

#ifndef CORE_ROOT_COMPONENT_H
#define CORE_ROOT_COMPONENT_H

#include <a2d/core.h>
#include <a2d/core/components/sprite.h>
#include <a2d/core/components/pixel_sprite.h>
#include <a2d/core/texture.h>
#include <a2d/core/components/animator.h>
#include <a2d/core/input.h>
#include <a2d/core/components/pixel_camera.h>

#include <lodepng.h>

#ifdef TARGET_ANDROID
#include <GLES2/gl2.h>
#elif TARGET_DESKTOP
#include <GL/glew.h>
#endif

#include <cmath>
#include <a2d/core/components/events_test.h>
#include "trump.h"

namespace a2d {

class RootComponent : public Component {
public:
    a2d::pObject2D o1;
    a2d::pObject2D o2;
    a2d::pObject2D e;
    float f = 0.0f;

    void Initialize() override {
        auto cam_obj = a2d::Engine::GetRoot()->AddChild(new Object2D);
        a2d::Engine::SetCamera(cam_obj->AddComponent<PixelCamera>());

        auto valeriy = Texture::GetTexture("valeriy");

        o1 = a2d::Engine::GetRoot()->AddChild(new Object2D);
        o1->AddComponent<Sprite>()->SetTextureRegion(new TextureRegion(valeriy, 0, 0, 500, 200));
        o2 = a2d::Engine::GetRoot()->AddChild(new Object2D);
        o2->AddComponent<Sprite>()->SetTextureRegion(new TextureRegion(valeriy));

        o1->GetComponent<Sprite>()->color.Set(0.5, 0.5, 1, 1);
        o2->GetComponent<Sprite>()->color.Set(1, 1, 0, 1);

        auto explosion = Texture::GetTexture("bunny");

        std::vector<Animation::Frame> f_bottom;
        std::vector<Animation::Frame> f_right;
        std::vector<Animation::Frame> f_top;
        std::vector<Animation::Frame> f_left;
        for (int j = 0; j < 6; ++j) {
            f_right.emplace_back(
                    new TextureRegion(explosion, j * explosion->width / 6, 0, explosion->width / 6, explosion->height, Texture::Filtering::NEAREST),
                    0.08f
            );
            f_left.emplace_back(
                    new TextureRegion(explosion, (j + 1) * explosion->width / 6, 0, -explosion->width / 6, explosion->height, Texture::Filtering::NEAREST),
                    0.08f
            );
            /*f_right.emplace_back(
                    new TextureRegion(explosion, j * explosion->width / 6, 2 * explosion->height / 4, explosion->width / 6, explosion->height / 4),
                    0.08f
            );
            f_top.emplace_back(
                    new TextureRegion(explosion, j * explosion->width / 6, 1 * explosion->height / 4, explosion->width / 6, explosion->height / 4),
                    0.08f
            );
            f_left.emplace_back(
                    new TextureRegion(explosion, j * explosion->width / 6, 0 * explosion->height / 4, explosion->width / 6, explosion->height / 4),
                    0.08f
            );*/
        }

        for (int i = 0; i < 100; ++i) {
            for (int j = 0; j < 50; ++j) {
                e = a2d::Engine::GetRoot()->AddChild(new Object2D);
                e->AddComponent<PixelSprite>()->SetTextureRegion(
                        new TextureRegion(explosion, Texture::Filtering::NEAREST));
                e->GetComponent<PixelSprite>()->SetScaleFactor(2);
                auto a = e->AddComponent<Animator>();
                a->AddAnimation("bottom", new Animation(f_bottom));
                a->AddAnimation("right", new Animation(f_right));
                a->AddAnimation("top", new Animation(f_top));
                a->AddAnimation("left", new Animation(f_left));
                e->scale.Set(0.7f);
                e->SetLayer(4);
                e->AddComponent<Trump>();
                e->position.Set(i * 10, j * 10);
            }
        }

        //Engine::GetRoot()->AddComponent<a2d::EventsTest>();
    }

    void Update() override {
        f += a2d::Engine::GetDeltaTime();
        /*if (std::sin(f * 100) < 0) {
            o1->SetLayer(0);
            o2->SetLayer(1);
        } else {
            o1->SetLayer(1);
            o2->SetLayer(0);
        }*/

        a2d::Engine::GetLogger()->info("{}", 1 / a2d::Engine::GetDeltaTime());

        o1->GetComponent<Sprite>()->color.w = std::sin(f * 0.5f);
        o2->GetComponent<Sprite>()->color.w = std::sin(f * 0.25f);
        o1->position.Set(std::sin(f) * 3, 0.5f);
        o2->position.Set(-std::sin(f) * 3, -0.5f);
    }
};

}

#endif //CORE_ROOT_COMPONENT_H
