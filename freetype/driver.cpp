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

#include "freetype/ftstroke.h"

int test_stroker(void)
{
	FT_Library library = nullptr;
	FT_Face face = nullptr;
	FT_Stroker stroker = nullptr;
	FT_Glyph glyph = nullptr;
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

	error = FT_Load_Char(face, 'z', FT_LOAD_DEFAULT);
	if (error) {
		goto free;
	}

	// Render normal glyph with stroke.
	// https://stackoverflow.com/a/28078293
	{
		error = FT_Stroker_New(library, &stroker);
		if (error) {
			goto free;
		}

		//  2 * 64 result in 2px outline
		FT_Stroker_Set(stroker, 2 * 64, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);

		error = FT_Get_Glyph(face->glyph, &glyph);
		if (error) {
			goto free;
		}

		error = FT_Glyph_StrokeBorder(&glyph, stroker, false, true);
		if (error) {
			goto free;
		}

		error = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true);
		if (error) {
			goto free;
		}

		FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);

		// Dump bitmap
		for (int row = 0; row < bitmapGlyph->bitmap.rows; row++) {
			for (int col = 0; col < bitmapGlyph->bitmap.pitch; col++) {
				char c = bitmapGlyph->bitmap.buffer[bitmapGlyph->bitmap.pitch * row + col];
				if (c == 0)
					printf(" ");
				else
					printf("*");
			}
			printf("\n");
		}
		printf("\n");
	}

	// Render normal glyph.
	{
		error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);
		if (error) {
			goto free;
		}

		FT_Bitmap bitmap = face->glyph->bitmap;

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
	}

free:
	if (glyph) {
		FT_Done_Glyph(glyph);
	}
	if (stroker) {
		FT_Stroker_Done(stroker);
	}
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
	// return dump_outline();
	return test_stroker();
}