add_library(lua52 SHARED
    lua/lapi.c
    lua/lauxlib.c
    lua/lbaselib.c
    lua/lbitlib.c
    lua/lcode.c
    lua/lcorolib.c
    lua/lctype.c
    lua/ldblib.c
    lua/ldebug.c
    lua/ldo.c
    lua/ldump.c
    lua/lfunc.c
    lua/lgc.c
    lua/linit.c
    lua/liolib.c
    lua/llex.c
    lua/lmathlib.c
    lua/lmem.c
    lua/loadlib.c
    lua/lobject.c
    lua/lopcodes.c
    lua/loslib.c
    lua/lparser.c
    lua/lstate.c
    lua/lstring.c
    lua/lstrlib.c
    lua/ltable.c
    lua/ltablib.c
    lua/ltests.c
    lua/ltm.c
    lua/lundump.c
    lua/lvm.c
    lua/lzio.c
)
target_include_directories(lua52 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lua)
target_compile_definitions(lua52 PUBLIC
    LUA_COMPAT_5_2
    $<$<PLATFORM_ID:Linux>:LUA_USE_LINUX>
    $<$<PLATFORM_ID:Darwin>:LUA_USE_MACOSX>
    $<$<PLATFORM_ID:Windows>:LUA_USE_WINDOWS LUA_BUILD_AS_DLL>
)
