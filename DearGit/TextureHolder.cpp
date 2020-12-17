#include "TextureHolder.h"

#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

TextureHolder::TextureHolder()
{
    textures = std::map<TextureName, unsigned int>();
}

TextureHolder::TextureHolder(const TextureHolder& other)
{
}

TextureHolder& TextureHolder::operator=(const TextureHolder& other)
{
    return *this;
}

TextureHolder::~TextureHolder()
{

}

bool TextureHolder::Init()
{
    std::map<TextureName, const char*> texturesToLoad =
    {
        {TEXTURE_PUSH, "Push.png"},
        {TEXTURE_PULL, "Pull.png"}
    };
    for(auto entry: texturesToLoad)
    {
        unsigned int imageId = LoadTexture(entry.second);
        if(imageId == INVALID_IMAGE_ID)
            return false;
        textures[entry.first] = imageId;
    }

    return true;
}

unsigned int TextureHolder::GetTextureId(TextureName textureName)
{
    auto item = textures.find(textureName);
    if(item != textures.end())
        return item->second;
    return INVALID_IMAGE_ID;
}

unsigned int TextureHolder::LoadTexture(const char* fileName)
{
    unsigned int textureId = INVALID_IMAGE_ID;
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(fileName, &image_width, &image_height, NULL, 4);
    if(image_data == NULL)
    {
        std::cout << "Could not load the file: " << fileName << "\n";
        return INVALID_IMAGE_ID;
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    return textureId;
}
