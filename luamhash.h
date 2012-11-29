#ifndef LUAMHASH_H
#define LUAMHASH_H

#define LUA_MHASHLIBNAME	"mhash"

LUALIB_API int
luaopen_mhash (lua_State *L);

#define lua_boxpointer(L,u) (*(void **)(lua_newuserdata(L, sizeof(void *))) = (u))
#define lua_unboxpointer(L,i) (*(void **)(lua_touserdata(L, i)))

#endif
