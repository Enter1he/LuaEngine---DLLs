#include "luadef.h"
#include "stdio.h"
#include <windows.h>
#include "GL/gl.h"


int DrawSprite(lua_args){
    lua_cleanargs(1);
    lua_getfield(L, 1, "pos");
    lua_getfield(L, 1, "origin");


    lua_geti(L, 2, 1);       //Sprite pos x
    lua_geti(L, 2, 2);       //Sprite pos y
    lua_getfield(L, 1, "w"); //Sprite width
    lua_getfield(L, 1, "h"); //Sprite height
    lua_geti(L, 3, 1);       //Sprite origin pos x
    lua_geti(L, 3, 2);       //Sprite origin pos y
    

    lua_Number w = lua_tonumber(L,6), h = lua_tonumber(L,7);
    lua_Number  x = lua_tonumber(L,4) - w*lua_tonumber(L,8), 
                y = lua_tonumber(L,5) + h*lua_tonumber(L,9);
    
    lua_pop(L, 8);


    lua_getfield(L, 1, "color");
    lua_geti(L,2,1); lua_geti(L,2,2); lua_geti(L,2,3);
    
    lua_getfield(L, 1, "tex");
    lua_geti(L, 6, 1);


    glPushMatrix();
        glColor3d(lua_tonumber(L,3), lua_tonumber(L,4), lua_tonumber(L,5));
        glTranslated(x,y,0);
        glScaled(w,h,0);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glBindTexture(GL_TEXTURE_2D, lua_tointeger(L, 7));   
            glBegin(GL_QUADS);
                glTexCoord2d(0, 1); glVertex2d(0, 0);
                glTexCoord2d(1, 1); glVertex2d(1, 0);
                glTexCoord2d(1, 0); glVertex2d(1, -1);
                glTexCoord2d(0, 0); glVertex2d(0, -1);
            glEnd();
        
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
    
    glPopMatrix();

    return 0;
}




int LUA_DLL luaopen_DrawSprite(lua_args){
    lua_pushcfunction(L, &DrawSprite);
    return 1;
}