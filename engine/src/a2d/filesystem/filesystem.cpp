//
// Created by selya on 27.12.2018.
//

#include <a2d/filesystem/filesystem.h>

#ifdef TARGET_ANDROID
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif

#include <fstream>
#include <string>
#include <codecvt>

namespace a2d {

#ifdef TARGET_ANDROID
AAssetManager *a2d::FileSystem::asset_manager = nullptr;

void FileSystem::Initialize(void *asset_manager) {
    FileSystem::asset_manager = (AAssetManager *)asset_manager;
}
#endif

std::vector<unsigned char> FileSystem::LoadRaw(const std::string &path) {
#ifdef TARGET_ANDROID
    auto asset = AAssetManager_open(asset_manager, path.c_str(), AASSET_MODE_UNKNOWN);
    auto size = AAsset_getLength(asset);
    std::vector<unsigned char> v(size);
    AAsset_read (asset, &v[0], size);
    AAsset_close(asset);
    return v;
#elif TARGET_DESKTOP
    std::ifstream file("resources/" + path, std::ios::binary);
    if ((file.rdstate() & std::ifstream::failbit) != 0) {
        std::vector<unsigned char> a;
        return a;
    }
    return std::vector<unsigned char>(
            (std::istreambuf_iterator<char>(file)),
            (std::istreambuf_iterator<char>())
    );
#endif
}

std::string FileSystem::LoadText(const std::string &path) {
    auto v = LoadRaw(path);
    return std::string(v.begin(), v.end());
}

std::u32string FileSystem::LoadTextUTF8(const std::string &path) {
    // MSVC bug
    // https://stackoverflow.com/questions/32055357/visual-studio-c-2015-stdcodecvt-with-char16-t-or-char32-t
#if _MSC_VER >= 1900
    std::wstring_convert<std::codecvt_utf8<uint32_t>, uint32_t> converter;
    auto v = LoadRaw(path);
    auto s = converter.from_bytes(std::string(v.begin(), v.end()));
    return std::u32string(s.begin(), s.end());
#else
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    auto v = LoadRaw(path);
    return converter.from_bytes(std::string(v.begin(), v.end()));
#endif
}

} //namespace a2d