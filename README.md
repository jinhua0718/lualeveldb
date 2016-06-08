# lualeveldb

## lua api:

1. **require module**<br/>
>>leveldb = require("leveldb")

2. **open database**<br/>
>>db = leveldb(".db") -- databases path:./.db<br/>

3. **get/put/delete**<br/>
>>db:put(key,value) --e.g: db:put("abc",123)<br/>
>>value = db:get(key) --e.g: db:get("abc")<br/>
>>db:delete(key) --e.g: db:delete("abc")<br/>
>>special: db:put(key,value) == db:delete(key) only if value is nil.<br/>

4. **iterator**<br/>
    for k,v in db:iterator() print(k,v) end<br/>



