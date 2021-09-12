## From console
* Generating VS solution of newest VS-version:  
```bash
xmake project -k vsxmake
```

## From **xmale.lua**
* Add `*.hpp` and `*.md` files into target VS project:
```lua
target(...)
    --
    add_headerfiles("*.hpp", "*.md")
```