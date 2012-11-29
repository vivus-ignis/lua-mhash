/* MHASH binding */

#include <string.h>
#include <stdlib.h>
#include <mhash.h>

#include <lua.h>
#include <lauxlib.h>

#include "luamhash.h"


#define MHASHHANDLE	"MHASH*"


/*
 * Returns: mhash_udata
 */
static int
lmhash_new (lua_State *L)
{
    lua_boxpointer (L, (void *) MHASH_FAILED);
    luaL_getmetatable (L, MHASHHANDLE);
    lua_setmetatable (L, -2);
    return 1;
}

/*
 * Returns: hashid (number)
 */
static int
lmhash_count (lua_State *L)
{
    lua_pushnumber (L, mhash_count ());
    return 1;
}

/*
 * Arguments: hashid (number)
 * Returns: [name (string)]
 */
static int
lmhash_name (lua_State *L)
{
    hashid id = (hashid) lua_tonumber (L, 1) - 1;
    lua_pushstring (L, mhash_get_hash_name_static (id));
    return 1;
}

/*
 * Arguments: name (string)
 * Returns: [hashid (number)]
 */
static int
lmhash_id (lua_State *L)
{
    const char *name = lua_tostring (L, 1);
    hashid id = mhash_count ();

    while (id--) {
	const char *static_name = mhash_get_hash_name_static (id);
	if (static_name && !strcmp (static_name, name)) {
	    lua_pushnumber (L, id + 1);
	    return 1;
	}
    }
    return 0;
}

/*
 * Arguments: mhash_udata, hashid (number)
 * Returns: [mhash_udata]
 */
static int
lmhash_init (lua_State *L)
{
    MHASH td, *tdp = lua_touserdata (L, 1);
    hashid id = (hashid) lua_tonumber (L, 2) - 1;

    if ((td = mhash_init (id)) != MHASH_FAILED) {
	*tdp = td;
	lua_settop (L, 1);
	return 1;
    }
    return 0;
}

/*
 * Arguments: mhash_udata, [get/discard the digest (boolean)]
 * Returns: [string]
 */
static int
lmhash_done (lua_State *L)
{
    MHASH *tdp = lua_touserdata (L, 1);

    if (*tdp != MHASH_FAILED) {
	void *digest = NULL;
	size_t len = 0;

	if (lua_toboolean (L, 2)) {
	    len = mhash_get_block_size(mhash_get_mhash_algo(*tdp));
	    if (len != 0)
		digest = malloc (len);
	}
	mhash_deinit (*tdp, digest);
	*tdp = MHASH_FAILED;
	if (digest) {
	    lua_pushlstring (L, digest, len);
	    free (digest);
	    return 1;
	}
    }
    return 0;
}

/*
 * Arguments: mhash_udata, string
 * Returns: [mhash_udata]
 */
static int
lmhash_add (lua_State *L)
{
    MHASH td = (MHASH) lua_unboxpointer (L, 1);
    const char *s = lua_tostring (L, 2);
    size_t len = lua_strlen (L, 2);

    lua_settop (L, 1);
    return !mhash (td, s, len) ? 1 : 0;
}

/*
 * Arguments: mhash_udata
 * Returns: hashid (number)
 */
static int
lmhash_algo (lua_State *L)
{
    MHASH td = (MHASH) lua_unboxpointer (L, 1);
    lua_pushnumber (L, mhash_get_mhash_algo (td) + 1);
    return 1;
}


static luaL_reg mhashmeth[] = {
    {"init",	lmhash_init},
    {"done",	lmhash_done},
    {"add",	lmhash_add},
    {"algo",	lmhash_algo},
    {"__gc",	lmhash_done},
    {NULL, NULL}
};

static luaL_reg mhashlib[] = {
    {"instance",	lmhash_new},
    {"count",		lmhash_count},
    {"name",		lmhash_name},
    {"id",		lmhash_id},
    {NULL, NULL}
};

static void
createmeta (lua_State *L)
{
    luaL_newmetatable (L, MHASHHANDLE);
    lua_pushliteral (L, "__index");
    lua_pushvalue (L, -2);  /* push metatable */
    lua_rawset (L, -3);  /* metatable.__index = metatable */
    luaL_openlib (L, NULL, mhashmeth, 0);
}

/*
 * Open mhash library
 */
LUALIB_API int
luaopen_mhash (lua_State *L)
{
    luaL_openlib (L, LUA_MHASHLIBNAME, mhashlib, 0);
    createmeta (L);
    return 0;
}
