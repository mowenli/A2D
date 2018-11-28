//
// Created by selya on 28.11.2018.
//

#include <a2d/core/bitmap_font.h>

namespace a2d {

BitmapFont::Character::Character() :
texture_region(nullptr), x(0), y(0), width(0), height(0), ratio(0) {

}

BitmapFont::Character::Character(const a2d::pTextureRegion &texture_region, int x, int y, int width, int height) :
texture_region(texture_region), x(x), y(y), width(width), height(height), ratio((float)width / height) {

}

const BitmapFont::Character *BitmapFont::GetCharacter(unsigned long char_code) const {
    auto i = characters.find(char_code);
    if (i == characters.end()) return nullptr;
    return &i->second;
}

int BitmapFont::GetLineHeight() const {
    return line_height;
}

BitmapFont::BitmapFont(const std::vector<unsigned char> &ttf, int size) {
    FT_Face face;
    FT_New_Memory_Face(GetFreeTypeLibrary(), &ttf[0], ttf.size(), 0, &face);
    FT_Set_Pixel_Sizes(face, 0, size);

    int face_height = (int)std::ceil((float)face->height / 64);
    int face_width = (int)std::ceil((float)face->max_advance_width * face->num_glyphs / 64);
    TextureBuffer tb(face_width, face_height * 3);
    pTexture texture = new Texture(tb.width, tb.height, nullptr);
    tb.Fill(0, 0, 0, 0);
    int current_x = 0;

    unsigned int glyph_index;
    unsigned long char_code = FT_Get_First_Char(face, &glyph_index);
    FT_GlyphSlot g = face->glyph;
    while (glyph_index) {
        FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);

        for (int x = 0; x < g->bitmap.width; ++x) {
            for (int y = 0; y < g->bitmap.rows; ++y) {
                unsigned char value = g->bitmap.buffer[(g->bitmap.rows - y - 1) * g->bitmap.width + x];
                tb.SetPixel(current_x + g->bitmap_left + x, face_height - g->bitmap.rows + g->bitmap_top + y,
                            255, 255, 255, value);
            }
        }

        characters[char_code] = Character(
                new TextureRegion(
                        texture,
                        current_x, 0,
                        g->advance.x / 64, face_height * 3
                ),
                0,
                face_height - g->bitmap.rows + g->bitmap_top,
                g->advance.x / 64,
                face_height
        );

        current_x += g->advance.x / 64;

        char_code = FT_Get_Next_Char(face, char_code, &glyph_index);
    }

    line_height = face_height;
    texture->SetData(tb.data);
}

FT_Library BitmapFont::GetFreeTypeLibrary() {
    static FT_Library ft_library;
    if (!ft_library) {
        FT_Init_FreeType(&ft_library);
    }
    return ft_library;
}

} //namespace a2d

