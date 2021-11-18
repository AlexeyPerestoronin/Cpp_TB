
# Propositions
There is the some propositions about how to develop the Exchange interaction interfaces(es) as convenient as it is need!  
## The 1: looking from the inside
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

## The 2: looking from the outside
Long form  
```cpp
ExchangerI::SP exchanger = ExmoExchanger::makeShared();
OrdersBook ordersBook{ exchanger };
OrdersBook::Response BTC_USD = ordersBook.request("BTC_USD", 100);
OrdersBook::Response ETH_USD = ordersBook.request("ETH_USD", 100);
```