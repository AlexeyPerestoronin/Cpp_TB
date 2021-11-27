# EXMO API
The library contains common logic that is described how to interact with an Exchange platform

# Table of contents
* [Main hierarchy](#Main-hierarchy)
* [Propositions](#Propositions)
    * [1: looking from the inside](##1:-looking-from-the-inside)
    * [2: looking from the outside](##2:-looking-from-the-outside)

# Main hierarchy
[_go back_](#Table-of-contents)  
* namespace TB_NS
    * namespace Trader_NS
        * namespace Interaction_NS
            * `using Cost = Double` - tdata type for the cost of an order
            * `using Price = Double` - data type for the price of an order
            * `using Quantity = Double` - data type for the quantity of a currency of an order
            * `struct Order` - one order presentation object
            * `using RequestLimit = Limit<int>` - data type that is presenting a limit in an Exchange platform request
            * `enum ExchangeID` - exchange platforms' ID(s) collection
            * `using ExchangeCode = AliasFor<ExchangeID,,>` - the alias based extension for exchange platforms' ID(s)
            * `enum CurrencyID` - the collections of ID(s) of currencies
            * `using CurrencyCode = AliasFor<CurrencyID,,>` - the alias based extension for currencies' ID(s)
            * `enum CommandID` - collection of commands' ID(s) to an exchange platform
            * `using CommandCode = AliasFor<CommandID,,>` - the alias based extension for commands' ID(s)
            * `class InteractI` - common interface for interacting with an Exchange platform
            * `struct ExchangeI` - common interface for presenting all Exchange platforms
            * `class CommandI` - common interface for presenting all commands that are requested some data from an Exchange platform
            * namespace Commands_NS
                * `struct OrdersBook` - the command to obtain information about orders-book from an Exchange platform
                    * `struct OrdersBook::Response` - presents received data of a response

# Propositions
[_go back_](#Table-of-contents)  
There is the some propositions about how to develop the Exchange interaction interfaces(es) as convenient as it is need!  
## 1: looking from the inside
[_go back_](#Table-of-contents)  
Long form  
```cpp
ExchangerI::SP exchanger = ExmoExchanger::makeShared();
CommandI::SP command = OrdersBook::makeShared();
RequestI request = command.makeRequest("BTC_USD", 100);
ResponseI response = exchanger->interact(request);
OrdersBook::Response ordersBook(response);
```
Short form  
```cpp
ExchangerI::SP exchanger = ExmoExchanger::makeShared();
auto ordersBook = OrdersBook::Response{ exchanger->interact(OrdersBook::MakeRequest("BTC_USD", 100)) };
```

## 2: looking from the outside
[_go back_](#Table-of-contents)  
Long form  
```cpp
ExchangerI::SP exchanger = ExmoExchanger::makeShared();
OrdersBook ordersBook{ exchanger };
OrdersBook::Response BTC_USD = ordersBook.request("BTC_USD", 100);
```