#include "png_loading.h"
// standard
#include <iostream>
// others
#include <glad/glad.h>
#include <png.h>
// engine
#include "../graphics/Texture.h"

int _png_load(const char* file, int* width, int* height)
{
    FILE* f;
    int bit_depth, color_type;
    png_infop info_ptr, end_info;
    png_uint_32 t_width, t_height;
    png_byte header[8];
    png_structp png_ptr;
    GLuint texture;
    int alpha;

    fopen_s(&f, file, "rb");
    if (!f)
    {
        std::cerr << "open failed" << std::endl;
        return 0;
    }
    fread_s(header, 8, 1, 8, f);
    const int is_png = !png_sig_cmp(header, 0, 8);
    if (!is_png)
    {
        std::cerr << "is_png failed" << std::endl;
        fclose(f);
        free(f);
        return 0;
    }
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr)
    {
        std::cerr << "png_ptr failed" << std::endl;
        fclose(f);
        free(f);
        return 0;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        std::cerr << "info_ptr failed" << std::endl;
        fclose(f);
        free(f);
        return 0;
    }
    end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        std::cerr << "end_info failed" << std::endl;
        fclose(f);
        free(f);
        return 0;
    }
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        std::cerr << "setjmp failed" << std::endl;
        fclose(f);
        free(f);
        return 0;
    }
    png_init_io(png_ptr, f);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, &t_width, &t_height, &bit_depth,
                 &color_type, nullptr, nullptr, nullptr);
    *width = t_width;
    *height = t_height;
    png_read_update_info(png_ptr, info_ptr);
    const int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    const auto image_data = static_cast<png_bytep>(malloc(row_bytes * t_height * sizeof(png_byte)));
    if (!image_data)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        std::cerr << "image_data failed" << std::endl;
        fclose(f);
        free(f);
        return 0;
    }
    const auto row_pointers = static_cast<png_bytepp>(malloc(t_height * sizeof(png_bytep)));
    if (!row_pointers)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(f);
        return 0;
    }
    for (unsigned int i = 0; i < t_height; ++i)
    {
        row_pointers[t_height - 1 - i] = image_data + i * row_bytes;
    }
    png_read_image(png_ptr, row_pointers);
    switch (png_get_color_type(png_ptr, info_ptr))
    {
    case PNG_COLOR_TYPE_RGBA:
        alpha = GL_RGBA;
        break;
    case PNG_COLOR_TYPE_RGB:
        alpha = GL_RGB;
        break;
    default:
        printf("Color type %d not supported!\n",
               png_get_color_type(png_ptr, info_ptr));
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(f);
        free(f);
        return 0;
    }
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0,
                 alpha, GL_UNSIGNED_BYTE, static_cast<GLvoid*>(image_data));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    fclose(f);
    return texture;
}

Texture* load_texture(const std::string& filename)
{
    int width, height;
    const GLuint texture = _png_load(filename.c_str(), &width, &height);
    if (texture == 0)
    {
        std::cerr << "Could not load texture with name: " << filename << std::endl;
        return nullptr;
    }
    return new Texture(texture, width, height);
}
