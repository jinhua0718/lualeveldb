env=Environment()
env.SharedObject("leveldb","leveldb.cpp",CPPPATH=["include"],LIBS=["lua","m"],LIBPATH="libs")

env=Environment(SHLIBPREFIX="",CCFLAGS=["-shared"])
env.SharedLibrary("leveldb",["leveldb.os"],LIBS=["leveldb"],LIBPATH="libs")
