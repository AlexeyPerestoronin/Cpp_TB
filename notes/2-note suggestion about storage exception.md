# Suggestion about storage exceptions
1. All existent exceptions must to be store inside the one linked-list:
```cpp
std::list<TB_NS::Error_NS::Exception> ExceptionsStorage{};
```
2. If an exception have the links to another exceptions, this links must be presented as pointers to the target exceptions from `ExceptionsStorage`-list:
```cpp
class Exception {
    ExceptionPtr d_next; // the pointer to the exceptions are the same level
    ExceptionPtr d_above; // the pointer to the exception are the above level (sub-exceptions of this exception)
};
```
**NOTE:** but for to do the code more simply it is possible to use of std-search algorithm fot search of needed exceptions from `ExceptionsStorage`-list
