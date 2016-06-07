#ifndef leveldb_hpp
#define leveldb_hpp

#include <stdio.h>
#include <lua/lua.hpp>
#include <leveldb/db.h>

int leveldb_close(lua_State *L){
    leveldb::DB *db= *(leveldb::DB**)luaL_checkudata(L,1,"leveldb_db");
    if(db)delete db;
    return 0;
}

int leveldb_get(lua_State *L){
    leveldb::DB *db= *(leveldb::DB**)luaL_checkudata(L,1,"leveldb_db");
    lua_remove(L,1);
    size_t len;
    const char *skey = luaL_checklstring(L,1,&len);
    leveldb::Slice key(skey,len);
    std::string value;
    leveldb::Status status = db->Get(leveldb::ReadOptions(),key,&value);
    if(status.ok()) {
        lua_pushlstring(L,value.c_str(),value.size());
        lua_pushnil(L);
    }
    else{
        lua_pushnil(L);
        lua_pushstring(L,status.ToString().c_str());
    }
    return 2;
}

int leveldb_del(lua_State *L){
    leveldb::DB *db= *(leveldb::DB**)luaL_checkudata(L,1,"leveldb_db");
    lua_remove(L,1);
    size_t len;
    const char *skey = luaL_checklstring(L,1,&len);
    leveldb::Slice key(skey,len);
    leveldb::Status status = db->Delete(leveldb::WriteOptions(),key);
    lua_pushboolean(L,status.ok());
    if(!status.ok())lua_pushstring(L,status.ToString().c_str());
    else lua_pushnil(L);
    return 2;
}

int leveldb_put(lua_State *L){
    if(lua_gettop(L)==2 || lua_isnil(L,3)) return leveldb_del(L);
    leveldb::DB *db= *(leveldb::DB**)luaL_checkudata(L,1,"leveldb_db");
    lua_remove(L,1);
    size_t len;
    const char *skey = luaL_checklstring(L,1,&len);
    leveldb::Slice key(skey,len);

    const char *sval = luaL_checklstring(L,2,&len);
    leveldb::Slice value(sval,len);

    leveldb::Status status = db->Put(leveldb::WriteOptions(),key,value);
    lua_pushboolean(L,status.ok());
    if(!status.ok())
        lua_pushstring(L,status.ToString().c_str());
    else
        lua_pushnil(L);
    return 2;
}

int leveldb_iter_gc(lua_State *L){
    leveldb::Iterator *it = *(leveldb::Iterator**)luaL_checkudata(L,1,"leveldb_iter");
    if(it)delete it;
    return 0;
}

int leveldb_iter(lua_State *L){
    leveldb::Iterator *it = *(leveldb::Iterator**)lua_touserdata(L,lua_upvalueindex(1));
    if(it->Valid())
    {
        lua_pushlstring(L,it->key().data(),it->key().size());
        lua_pushlstring(L,it->value().data(),it->value().size());
        it->Next();
        return 2;
    }
    return 0;
}

int leveldb_getiter(lua_State *L){
    leveldb::DB *db= *(leveldb::DB**)luaL_checkudata(L,1,"leveldb_db");
    leveldb::Iterator **it = (leveldb::Iterator**)lua_newuserdata(L,sizeof(leveldb::Iterator*));
    luaL_newmetatable(L,"leveldb_iter");
    lua_pushcfunction(L,leveldb_iter_gc);
    lua_setfield(L,-2,"__gc");
    lua_setmetatable(L,-2);
    *it = db->NewIterator(leveldb::ReadOptions());
    (*it)->SeekToFirst();
    lua_pushcclosure(L,leveldb_iter,1);
    return 1;
}

int leveldb_open(lua_State *L){
    lua_remove(L,1);
    leveldb::DB **db = ((leveldb::DB**)lua_newuserdata(L,sizeof(leveldb::DB*)));
    int userdata = lua_gettop(L);
    leveldb::Options options;
    options.create_if_missing=true;
    std::string dbpath = luaL_checkstring(L,1);
    leveldb::Status status = leveldb::DB::Open(options,dbpath,&*db);
    if(!status.ok())lua_pushstring(L,status.ToString().c_str());
    else lua_pushnil(L);

    lua_newtable(L);
    int methods = lua_gettop(L);

    lua_pushcfunction(L,leveldb_get);
    lua_setfield(L,methods,"get");

    lua_pushcfunction(L,leveldb_put);
    lua_setfield(L,methods,"put");

    lua_pushcfunction(L,leveldb_del);
    lua_setfield(L,methods,"del");

    lua_pushcfunction(L,leveldb_getiter);
    lua_setfield(L,methods,"iterator");

    luaL_newmetatable(L,"leveldb_db");
    int metatable = lua_gettop(L);

    lua_pushcfunction(L,leveldb_close);
    lua_setfield(L,metatable,"__gc");

    lua_pushvalue(L,methods);
    lua_setfield(L,metatable,"__index");

    lua_pushvalue(L,methods);
    lua_setfield(L, metatable, "__metatable");

    lua_setmetatable(L,userdata);
    lua_remove(L,methods);
    return 2;
}

extern "C" {
    LUALIB_API int luaopen_leveldb(lua_State *L)
    {
        lua_newtable(L);

        luaL_newmetatable(L,"leveldb_meta");
        lua_pushcfunction(L,leveldb_open);
        lua_setfield(L,-2,"__call");

        lua_setmetatable(L,-2);

        //lua_setfield(L,LUA_GLOBALSINDEX,"leveldb");
        return 1;
    }
}
#endif //leveldb_hpp
