# TB
Trade Bot

## Table of contents
* [Code Style](##Code-Style)  
    - [Modules](###Modules)

## Code Style
### Modules
Each module is a separate compilation unit - *.lib  
1. All modules are located in the root directory.  
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
