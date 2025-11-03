#include "ttfs/text.hpp"
void PrintBitmapAsASCII(const unsigned char* bitmap, int width, int height) {
    if (!bitmap) {
        printf("Error: bitmap is null.\n");
        return;
    }

    // You can tweak this string to change the "contrast"
    const char* shades = " .:-=+*#%@";
    int numShades = 10; // number of characters in the above string

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char pixel = bitmap[y * width + x];
            int shadeIndex = (pixel * (numShades - 1)) / 255; // map 0–255 to 0–9
            putchar(shades[shadeIndex]);
        }
        putchar('\n');
    }
}

int main() {
    std::string text ("hello world");
    CPPN::FileSystem::Init("example", "example");
    std::cout << CPPN::FileSystem::AbsoluteResourcePath("Roboto-Regular.ttf") << std::endl;
    CPPN::FontSystem::Font font(CPPN::FileSystem::AbsoluteResourcePath("Roboto-Regular.ttf"), 32.0f);
    std::cout << "font inited" << std::endl;
    
    unsigned char* bitmap = font.GetTextAsBitmap(text);
    PrintBitmapAsASCII(bitmap, font.textWidth, font.textHeight);
    
    // Free the bitmap memory
    if (bitmap) {
        free(bitmap);
    }
    
    return 0;
}