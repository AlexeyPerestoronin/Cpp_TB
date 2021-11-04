## From console
* Generating VS solution of newest VS-version:  
```bash
xmake project -k vsxmake
```
* Observe compiling options related with target library (`boost`, for example)
```bash
xmake require --info boost
```
* Build only target ('cppcoro', for example)
```bash
xmake build targetname
```

## From **xmale.lua**
* Add `*.hpp` and `*.md` files into target VS project:
```lua
target(...)
    --
    add_headerfiles("*.hpp", "*.md")
```