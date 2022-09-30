#include "ft2build.h"
#include FT_FREETYPE_H

int main(void)
{
    FT_Library library;
    FT_Face face;
    FT_Error error;

    error = FT_Init_FreeType(&library);
    if (error) {
        return 1;
    }
    return 0;
}
