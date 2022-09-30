#include "ft2build.h"
#include FT_FREETYPE_H

int main(void)
{
    FT_Library library = nullptr;
    FT_Face face = nullptr;
    FT_Error error;

    error = FT_Init_FreeType(&library);
    if (error) {
        return error;
    }

	const char* filename = "C:\\Windows\\Fonts\\arial.ttf";

	error = FT_New_Face(library, filename, 0, &face);
	if (error) {
		goto free;
	}

	error = FT_Set_Char_Size(face, 40 * 64, 0, 100, 0);
	if (error) {
		goto free;
	}

	error = FT_Load_Char(face, 'q', FT_LOAD_DEFAULT);
	if (error) {
		goto free;
	}
	FT_GlyphSlot glyph = face->glyph;

	error = FT_Render_Glyph(glyph, FT_RENDER_MODE_MONO);
	if (error) {
		goto free;
	}

	FT_Bitmap bitmap = glyph->bitmap;

	// Dump bitmap
	for (int row = 0; row < bitmap.rows; row++) {
		for (int col = 0; col < bitmap.pitch; col++) {
			char c = bitmap.buffer[bitmap.pitch * row + col];
			for (int bit = 7; bit >= 0; bit--) {
				if (((c >> bit) & 1) == 0)
					printf(" ");
				else
					printf("*");
			}
		}
		printf("\n");
	}

free:
	if (face) {
		FT_Done_Face(face);
	}
	if (library) {
		FT_Done_FreeType(library);
	}

    return error;
}
