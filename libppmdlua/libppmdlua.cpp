#include <sstream>
#include <string>
extern "C" {
#include <lua.h>
#include <lauxlib.h>
}
#include <ppmd_coder.h>

static int l_decompress(lua_State *L) {
    size_t l;
    const char *s = luaL_checklstring(L, 1, &l);
    lua_Number n = luaL_checknumber(L, 2);
    std::string str(s, l);
    std::istringstream iss(str, std::ios::binary);
    std::ostringstream oss(std::ios::binary);
    PPMD_Coder ppmd;
    if (ppmd.Uncompress(iss, oss, (std::size_t)n)) {
        lua_pushlstring(L, oss.str().data(), oss.str().size());
    } else {
        lua_pushnil(L);
    }
    return 1;
}

static const struct luaL_Reg ppmd[] = {
    {"decompress", l_decompress},
    {NULL, NULL}
};

extern "C" int __declspec(dllexport) luaopen_libppmdlua(lua_State *L) {
    lua_newtable(L);
    luaL_setfuncs(L, ppmd, 0);
    return 1;
}
