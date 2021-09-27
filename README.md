# TB
Trade Bot

## Table of contents
* [Code Style](##Code-Style)  
    - [Modules](###Modules)
    - [Functions](###Functions)
    - [Namespaces](###Namespaces)
    - [Classes](###Classes)
* [How to build](##How-to-build)  

## Code Style
### Modules
Each module is a separate compilation unit - *.lib  
1. All modules are located in the root directory and must be named by lower register symbols with sharps, for example: `exmo_api, common`.  
2. Each module must contain a readme.md file.  
3. The source code of the module is placed in the subfolder of the SRC module.  
4. Unit tests (if necessary) are placed in the subfolder of the Unit-Tests module.  
5. Suite tests (if necessary) are placed in the subfolder of the Suit-Tests module.  
6. All module header files are placed in the module folder.  
7. All private header files (if necessary) start from the bottom undergoing.  

Example of **module tree directory**:
```txt
./Cpp_TP\
|  README.md
|  module_1\
|  |  README.md
|  |  src\
|  |  |  source_file_1.cpp
|  |  |  source_file_2.cpp
...
|  |  |  source_file_N.cpp
|  |  unit-tests\
|  |  |  unit-test-module_1-test_name_1.cpp
|  |  |  unit-test-module_1-test_name_2.cpp
...
|  |  |  unit-test-module_1-test_name_N.cpp
|  |  suit-tests\
|  |  |  suit-test-module_1-test_name_1.cpp
|  |  |  suit-test-module_1-test_name_2.cpp
...
|  |  |  suit-test-module_1-test_name_N.cpp
|  |  _private_header_file_1.hpp
|  |  _private_header_file_2.hpp
...
|  |  _private_header_file_N.hpp
|  |  public_header_file_1.hpp
|  |  public_header_file_2.hpp
...
|  |  public_header_file_N.hpp
|  module_2\
...
|  module_N\
```
### Functions
Soft recommendations
```cpp
ReturnType SomeActions(const Type1& i_onlyInputVar, Type2& io_inputOutputVar, Type& o_onlyOutputVar)
{
    ReturnType r_result;
    // ...
    return r_result;
}
```
### Namespaces
Strictly recommendations  
```cpp
namespace TB_NS {
    namespace InnerSpace_NS {
        // ...

    } // namespace InnerSpace_NS
} // namespace TB_NS
```
### Classes
Soft recommendations  
```cpp
class Creator
    : public Interface1
    , public Interface2
    // ...
    , public InterfaceN {
    
#pragma region members
    int m_privateMember;
    // ...

    protected:
    int m_protectedMember;
    // ...
    
    public:
    int m_publicMember1;
    // ...
#pragma endregion members

#pragma region methods
    private:
    void privateMethods(...);
    // ...

    protected:
    void protectedMethods(...);
    // ...

    public:
    void publicMethods(...);
    // ...
#pragma endregion methods

#pragma region static methods
    private:
    void privateStaticMethods(...);
    // ...

    protected:
    void protectedStaticMethods(...);
    // ...

    public:
    void publicStaticMethods(...);
    // ...
#pragma endregion static methods

#pragma region operators
    public:
    bool publicMethods(...);
    // ...
#pragma endregion methods
};
```
## How to build
TB building by [XMake](https://xmake.io/#/) and so most 3th-party-libraries are downloaded and compiled automatically, but next listed libraries must be using by another way:
1. [**cppcoro**](https://github.com/lewissbaker/cppcoro#generatort)  
This library usages for using corutines that are possible with 20th C++ standard.  
This library implemented in TB as git-submodule, automatically compiling from source files and no needed any additional actions for its using.
