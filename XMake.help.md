## From console
* Generating VS solution of newest VS-version:  
create a latest available version VS solution  
```bash
xmake project -k vsxmake
```
create a VS2019 solution
```bash
xmake project -k vsxmake2019 (see after the `vsxmake2019`-folder)  
```
create a VS2022 solution
```bash
xmake project -k vsxmake2022 (see after the `vsxmake2022`-folder)  
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