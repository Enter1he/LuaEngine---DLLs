#include "stdlib.h"
#include "stdio.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "ft2build.h"
#include "freetype/freetype.h"
#include "freetype/ftglyph.h"


typedef unsigned int GLuint;

typedef unsigned char GLubyte;

inline int next_p2 (int a)
{
    int rval=1;
    while(rval<a) 
        rval<<=1;

    return rval;
}

void stackDump(lua_State *L)
{
	int i;
	int top = lua_gettop(L);

	for(i = 1; i <= top; i++) {
		int t = lua_type(L, i);
		switch(t) {
			case LUA_TNIL:
				printf("nil");
				break;
			case LUA_TBOOLEAN:
				printf(lua_toboolean(L, i) ? "true" : "false");
				break;
			case LUA_TNUMBER:
				printf("%g", lua_tonumber(L, i));
				break;
			case LUA_TSTRING:
				printf("%s", lua_tostring(L, i));
				break;

			default:
				printf("%s", lua_typename(L, t));
		}
		printf(" ");
	}
	printf("\n\n");
}


int Font(lua_State *L){// (string folder, int height) --> table of font data
    
    const char *folder = lua_tostring(L, 1); unsigned int h = lua_tointeger(L,2);
    FT_Library Library = 0; FT_Face face = 0;

    FT_Init_FreeType(&Library);
    FT_New_Face(Library, folder, 0, &face);
    FT_Set_Char_Size( face, h << 6, h << 6, 96, 96);


    lua_createtable(L, 128,0);
    
    for (unsigned char ch = 0; ch < 128; ch++){
        FT_Glyph glyph = 0;
        FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT);
        FT_Get_Glyph(face->glyph, &glyph);
        
        FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
        
        FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
        FT_Bitmap bitmap = bitmap_glyph->bitmap;
        
        
        int width = next_p2(bitmap.width);
        int height = next_p2(bitmap.rows);
        int len = 2 * width * height;
        
        GLubyte* data = malloc(sizeof(GLubyte) * len);
        for(int i=0; i <height; i++) {
            for(int j=0; j < width; j++){
                data[2*(j+i*width)+1] = (j>=bitmap.width || i>=bitmap.rows) ? 0 : bitmap.buffer[j + bitmap.width*i];
                data[2*(j+i*width)] = data[2*(j+i*width)+1];
                
            };
        };
        lua_createtable(L, 5, 0);

        lua_pushlightuserdata(L, data);
        lua_seti(L,4, 0);
        
        lua_pushinteger(L, bitmap.width);
        lua_seti(L,4, 1);

        lua_pushinteger(L, bitmap.rows);
        lua_seti(L,4, 2);

        lua_pushinteger(L, width);
        lua_seti(L,4, 3);

        lua_pushinteger(L, height);
        lua_seti(L,4, 4);
        
        lua_pushinteger(L, bitmap_glyph->top);
        lua_seti(L,4, 5);


        lua_seti(L,3, (int)ch);
        
        FT_Done_Glyph(glyph);
    }
    
    
    FT_Done_Face(face);
    FT_Done_FreeType(Library);
    
    return 1;
}


int DestroyFace(lua_State *L){
    for(int i = 0; i < 128; i++){
        lua_geti(L, 1, i);
        lua_geti(L, 2, 0);
        GLubyte *data = (GLubyte*)lua_touserdata(L, 3);
        lua_pop(L, 2);
        free(data);

    };
    
    return 0;
}

int printLetter(lua_State *L){

    lua_geti(L, 1, 1);
    
    GLubyte *data = (GLubyte*)lua_touserdata(L, 2);
    lua_geti(L, 1, 2);
    lua_geti(L, 1, 3);
    int width = lua_tointeger(L, 3), height = lua_tointeger(L, 4);
    int width_p2 = next_p2(width);

    for(int i=0; i <height; i++) {
        for(int j=0; j < width; j++){
            int a = data[2*(j+i*width_p2)];
            if (a < 10)
                printf(" %d  ", a);
            else if (a < 100)
                printf(" %d ", a);
            else if (a > 99)
                printf("%d ", a);
        };
        printf("\n");
    };
    
    
    return 0;
}

static const struct luaL_Reg func[] = {
    {"LoadFont", &Font},
    {"printLetter", &printLetter},
    {"DestroyFace", &DestroyFace},
    {NULL, NULL}
};

int __declspec(dllexport) luaopen_Fonting(lua_State *L){
    luaL_newlibtable(L, func);
    luaL_setfuncs(L, func, 0);
    return 1;
}