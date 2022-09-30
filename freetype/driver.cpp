#include "ft2build.h"
#include FT_FREETYPE_H

int dump_bitmap(void)
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

#include "freetype/ftoutln.h"

int move_to(const FT_Vector* to, void* user)
{
	printf("Move to (%ld, %ld)\n", to->x, to->y);
	return 0;
}

int line_to(const FT_Vector* to, void* user)
{
	printf("Line to (%ld, %ld)\n", to->x, to->y);
	return 0;
}

int conic_to(const FT_Vector* control, const FT_Vector* to, void* user)
{
	printf("Conic to (%ld, %ld) with control (%ld, %ld)\n", to->x, to->y, control->x, control->y);
	return 0;
}

int cubic_to(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user)
{
	printf("Cubic to (%ld, %ld) with controls (%ld, %ld) and (%ld, %ld)\n", to->x, to->y, control1->x, control1->y, control2->x, control2->y);
	return 0;
}

int dump_outline(void)
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

	FT_Outline outline = glyph->outline;

	FT_Outline_Funcs funcs;
	funcs.move_to = move_to;
	funcs.line_to = line_to;
	funcs.conic_to = conic_to;
	funcs.cubic_to = cubic_to;

	FT_Outline_Decompose(&outline, &funcs, nullptr);

free:
	if (face) {
		FT_Done_Face(face);
	}
	if (library) {
		FT_Done_FreeType(library);
	}

	return error;
}

int main(void)
{
	// return dump_bitmap();
	return dump_outline();
}