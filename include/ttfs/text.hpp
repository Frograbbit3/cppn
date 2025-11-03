#define STB_TRUETYPE_IMPLEMENTATION
#include "../ttfs/stb_truetype.h"
#include "../filesystem/filesystem_core.hpp"
#include <cstdio>
#include <stdlib.h>
#include <string>



namespace CPPN {
    namespace FontSystem {
        class Font {
            private:
                stbtt_fontinfo font;
                std::string ttf_data; // Store the font data persistently
                float scale=0.0f;
                int ascent, descent, lineGap;
                float baseline;
                int width, height, xoff, yoff;
                unsigned char* GetCharacter(const char* character) {
                    unsigned char* bitmap = stbtt_GetCodepointBitmap(&font, 0, scale, character[0], &width, &height, &xoff, &yoff);
                    return bitmap;
                };
                float GetSpacing(const char* character, const char* character2) {
                    int advance, lsb;
                    stbtt_GetCodepointHMetrics(&font, character[0], &advance, &lsb);

                    float advance_px = advance * scale;
                    float left_side_bearing = lsb * scale;
                    int kern = stbtt_GetCodepointKernAdvance(&font, character[0], character2[0]);
                    float kern_px = kern * scale;
                    return kern_px;
                }
            public:
                float size;
                int textWidth;
                int textHeight;
                Font(std::string path, float sizet): size(sizet) {
                    if (!CPPN::FileSystem::FileExists(path)) {
                        throw std::runtime_error("Font does not exist!");
                    }
                    // Load font data as BINARY (TTF files are binary, not text!)
                    ttf_data = CPPN::FileSystem::OpenFileAsBinary(path);
                    if (ttf_data.empty()) {
                        throw std::runtime_error("Font file is empty!");
                    }
                    // Initialize font with the persistent buffer
                    if (!stbtt_InitFont(&font, (unsigned char*)ttf_data.data(), 
                                       stbtt_GetFontOffsetForIndex((unsigned char*)ttf_data.data(), 0))) {
                        throw std::runtime_error("Could not init font!");
                    }
                    scale = stbtt_ScaleForPixelHeight(&font, (float)size);
                    stbtt_GetFontVMetrics(&font, &ascent, &descent, &lineGap);
                    baseline = ascent * scale;
                }
                unsigned char* GetTextAsBitmap(std::string& txt) {
                    // Create a copy to avoid                     
                    std::string txtCopy = txt + "  ";
                    const char* st = txtCopy.c_str(); 
                    size_t size = strlen(st);
                    int currentx = 0;
                    float currentSpacing = 0.0f;
                    unsigned char* currentBitmap = nullptr;
                    char currentChar;

                    int totalWidth = 0;
                    int maxAboveBaseline = 0;
                    int maxBelowBaseline = 0;
                    int totalHeight = 0;
                    
                    // First pass: calculate dimensions
                    for (int i = 0; i < txtCopy.size() - 2; i++) {
                        int glyphWidth, glyphHeight, xoff, yoff;
                        unsigned char* temp = stbtt_GetCodepointBitmap(&font, 0, scale, txt[i],
                                                                    &glyphWidth, &glyphHeight, &xoff, &yoff);

                        maxAboveBaseline = std::max(maxAboveBaseline, static_cast<int>(baseline + yoff));
                        maxBelowBaseline = std::max(maxBelowBaseline, glyphHeight - (static_cast<int>(baseline) + yoff));
                        
                        // Use advance width instead of glyph width
                        int advance, lsb;
                        stbtt_GetCodepointHMetrics(&font, txt[i], &advance, &lsb);
                        totalWidth += (int)(advance * scale);


                        stbtt_FreeBitmap(temp, NULL);

                        // Add kerning between this glyph and the next
                        if (i + 1 < txt.size())
                            totalWidth += (int)(stbtt_GetCodepointKernAdvance(&font, txt[i], txt[i + 1]) * scale);
                    }
                    
                    // Calculate total height from baseline metrics
                    totalHeight = maxAboveBaseline + maxBelowBaseline;
                    
                    // Allocate bitmap with proper size
                    if (totalWidth <= 0 || totalHeight <= 0) {
                        textWidth = 0;
                        textHeight = 0;
                        return nullptr;
                    }
                    
                    unsigned char* bitmap = (unsigned char*)calloc(totalWidth * totalHeight, sizeof(unsigned char));
                    if (!bitmap) {
                        throw std::runtime_error("Failed to allocate bitmap memory");
                    }

                    // Second pass: render glyphs
                    for (int c = 0; c < size; c++) {
                        currentChar = st[c];
                        
                        // Get the glyph - this updates width, height, xoff, yoff member variables
                        currentBitmap = GetCharacter(&currentChar);
                        if (!currentBitmap) {
                            // If glyph couldn't be rendered, still advance by some amount
                            int advance, lsb;
                            stbtt_GetCodepointHMetrics(&font, currentChar, &advance, &lsb);
                            currentx += (int)(advance * scale);
                            continue;
                        }
                        
                        // Store current glyph's dimensions (GetCharacter updates these)
                        int glyphWidth = width;
                        int glyphHeight = height;
                        int glyphXoff = xoff;
                        int glyphYoff = yoff;
                        
                        // Calculate baseline position for this glyph
                        // The baseline is at maxAboveBaseline, and yoff is the offset from baseline
                        int baselineY = maxAboveBaseline + glyphYoff;
                        
                        // Copy glyph bitmap to main bitmap (with correct positioning)
                        for (int y = 0; y < glyphHeight && (baselineY + y) < totalHeight; y++) {
                            for (int x = 0; x < glyphWidth && (currentx + glyphXoff + x) < totalWidth; x++) {
                                if ((baselineY + y) >= 0 && (currentx + glyphXoff + x) >= 0) {
                                    unsigned char pixel = currentBitmap[y * glyphWidth + x];
                                    bitmap[(baselineY + y) * totalWidth + (currentx + glyphXoff + x)] = pixel;
                                }
                            }
                        }
                        
                        // Calculate advance for next character
                        int advance, lsb;
                        stbtt_GetCodepointHMetrics(&font, currentChar, &advance, &lsb);
                        int kern = 0;
                        if (c + 1 < size) {
                            kern = stbtt_GetCodepointKernAdvance(&font, currentChar, st[c + 1]);
                        }
                        currentx += (int)((advance + kern) * scale);
                        
                        stbtt_FreeBitmap(currentBitmap, nullptr);
                    }
                    textWidth =totalWidth;
                    textHeight=totalHeight;
                    return bitmap;
                };
            
        };

    }
}
