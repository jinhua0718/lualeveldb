# lualeveldb
lua api:
1. require thsi module
leveldb = require("leveldb")

2. open database:
db = leveldb(".db") -- databases path:./.db

3. get/put/delete item
key/value --> string/string
db:put(key,value) --e.g: db:put("abc",123)
value = db:get(key) --e.g: db:get("abc")
db:delete(key) --e.g: db:delete("abc")
special: db:put(key,value) == db:delete(key) only if value is nil.

4. iterator
for k,v in db:iterator()
	print(k,v)
end



