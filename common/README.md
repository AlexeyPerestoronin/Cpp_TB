# **common**
It is the module are encapsulated common program logic for TB.
***
## Table of contents
* [Main hierarchy](#Main-hierarchy)
* [Exceptions](#Exceptions)
    - [Idea description](##Idea-description)
    - [Usage explanation in examples](##Usage-explanation-in-examples)

# Main hierarchy
* namespace TB_NS  
    * class Str - the class extending base functional of standard strings
    * template<class FromType> Str ToStr(FromType&& i_value) - converts the data of any type to string (Str)
    * template<,,,> struct AliasFor - creates the usage-friendly alias for the target type
    * using Int = AliasForBuitInType<int>
    * using Double = AliasForBuitInType<double>
    * using Path = AliasFor<fs::path>
    * using VStrs = AliasFor<std::vector<Str>>
    * using LStrs = AliasFor<std::list<Str>>
    * using StrToStr = AliasFor<std::map<Str, Str>>
    * using StrToInt = AliasFor<std::map<Str, int>>
    * using IntToStr = AliasFor<std::map<int, Str>>
    * class Json - this class is the small overbuild for nlohmann::json  
    * template<,> class RAI - template-class for constructing a RAI-wrapper around a some data of the target type
    * template<ContainerType Container> cppcoro::generator<index_value<Container>> enumerator(Container& i_container) - enumerates the all elements of the target container in like-python style
    * template<ContainerType Container> cppcoro::generator<cindex_value<Container>> cenumerator(const Container& i_container) - enumerates the all elements of the target container in like-python style (const version)
    * namespace Error_NS
        * struct Error - class presents one error that is a base for runtime-exception creation process
        * class Exception - class presents one runtime-exception
        * class Exceptions - class manages the all runtime-exceptions
# Exceptions
Is the part of the common-module is containing the classes for using exceptions in the TB.
***
## Idea-description
The general idea are basing on using a some separate files with the exceptions presented in the json-format.
The main targets of its approach is:
1. No exceptions presented in the C++-code
2. Be possible to make a hierarchy of exceptions
3. Give convenient way to change, modify and restructuring exceptions
4. Make all the exceptions as flexible to specific adjusting as it is needed

## Usage explanation in examples
1. At first there is need to have the file is describing each exceptions in one module, `common-exceptions.json`:
     - quantity of that settings-files is unlimited, and they could be placed in each module where need;
     - the structure of the settings-file with the errors is unlimited in nesting;
    ```json
    {
        "exceptions": [
            {
                "id": "0",
                "key": "unregister exception",
                "values": {
                    "description": "this exception usages when the attempt of using unregister exception happened",
                    "what could be doing": "please register the exception `{}` before using it."
                }
            },
            {
                "id": "1",
                "key": "unexpected exception",
                "values": {
                    "description": "this exception usages when an exception happened but unexpected",
                    "what could be doing": "please review the program code for errors"
                }
            },
            {
                "id": "2",
                "key": "common",
                "values": {
                    "description": "It is the common exception that will happen everywhere in the code"
                },
                "exceptions": [
                    {
                        "id": "2.1",
                        "key": "zero division",
                        "values": {
                            "description": "The division by zero was happen",
                            "what could be doing": "please eliminate zero division"
                        }
                    },
                    {
                        "description": {
                            "id": "2.2",
                            "key": "NULL pointer",
                            "values": {
                                "description": "Attempt usage of NULL pointer to the object `{}`",
                                "what could be doing": "please don't attempt usage of NULL pointer to object"
                            }
                        }
                    }
                ]
            },
            {
                "id": "3",
                "key": "DB",
                "values": {
                    "description": "It is the common exception happened in Data Base"
                },
                "exceptions": [
                    {
                        "id": "3.1",
                        "key": "connection",
                        "values": {
                            "description": "the connection with the DB cannot be establish"
                        },
                        "exceptions": [
                            {
                                "id": "3.1.1",
                                "key": "incorrect name",
                                "values": {
                                    "description": "the name '{}` fo the BD isn't correct",
                                    "what could be doing": "please review the target BD's name"
                                }
                            },
                            {
                                "id": "3.1.2",
                                "key": "lost",
                                "values": {
                                    "description": "the connection with the DB '{}` have lost",
                                    "what could be doing": "please retry to reconnect"
                                }
                            }
                        ]
                    }
                ]
            }
        ]
    }
    ```
2. At second, after we have the target settings-file with the errors it is possible to use their in code:
    - usage of zero-division exception:
    ```cpp
    void DevideTwoDigits(double i_one, double i_two) {
        if (i_two == 0.f)
            throw TB_NS::Error_NS::Error["common"]["zero division"] << Location(BOOST_CURRENT_LOCATION);
        return i_one / i_two;
    }
    ```
    - usage of DB exception that is related with incorrect DB name:
    ```cpp
    Connection EstablishConnection(const std::string& i_dbName) {
        if (!CheckIsNameCorrect(i_dbName))
            throw TB_NS::Error_NS::Error["DB"]["connection"]["incorrect name"] << Location(BOOST_CURRENT_LOCATION);
        return MakeConnectionByName(i_dbName);
    }

    void SomeBusinessLogic() {
        try {
            // ...
            auto connection = EstablishConnection(GetDbConnectionName());
            // ...
        } catch (TB_NS::Error_NS::Error& error) {
            throw TB_NS::Error_NS::Error["unexpected exception"] << Location(BOOST_CURRENT_LOCATION) << Suberror(error);;
        }
    }
    ```
