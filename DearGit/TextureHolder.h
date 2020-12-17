#ifndef __TEXTURE_HOLDER_H__
#define __TEXTURE_HOLDER_H__

#include <map>
#include <iostream>

enum TextureName
{
    TEXTURE_PUSH,
    TEXTURE_PULL
};

class TextureHolder
{
public:
    static const unsigned int INVALID_IMAGE_ID = 0;

    static TextureHolder& Instance()
    {
        static TextureHolder instance;
        return instance;
    }

    bool Init();
    unsigned int GetTextureId(TextureName textureName);

protected:
    TextureHolder(); // Prevent construction
    TextureHolder(const TextureHolder&); // Prevent construction by copying
    TextureHolder& operator=(const TextureHolder&); // Prevent assignment
    ~TextureHolder(); // Prevent unwanted destruction
private:
    std::map<TextureName, unsigned int> textures;

    unsigned int LoadTexture(const char* fileName);

};
#endif
