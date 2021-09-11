#include "exmo_api.hpp"
#include <iostream>

int main()
{
	exmo_api api("your_key", "your_secret");

	json_data response = api.call("user_info", "");
	std::clog << ">> user_info: " << response << "\n\n";
	
	response = api.call("order_book", api.build({ "pair=BTC_USD" }));
	std::clog << ">> order_book: " << response << "\n\n";
	
	return 0;
}