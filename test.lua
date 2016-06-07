local leveldb = require("leveldb")
local db = leveldb(".db")
print("db->put:",db:put("abc",123))
print("db->get:",db:get("abc"))
print("db->get:",db:get("abcd"))
print("db->delete:",db:delete("abc"))
for i=1,10 do
	db:put(string.format("k%03d",i),string.format("v%03d",i))
end

for k,v in db:iterator() do
	print(k,v)
end
