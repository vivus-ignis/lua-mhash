LUA_INC      := /opt/openresty/luajit/include/luajit-2.0
LUA_LIB      := /opt/openresty/luajit/lib
INSTALL_PATH := $(LUA_LIB)/lua/5.1
CC           := gcc

mhash.so:
	$(CC) -O2 -fPIC -I$(LUA_INC) -L$(LUA_LIB) -c -o mhash.o luamhash.c
	$(CC) -O2 -fPIC -I$(LUA_INC) -L$(LUA_LIB) -o mhash.so -shared mhash.o

install:
	cp mhash.so $(INSTALL_PATH)/
