#include "../exmo_api/exmo_api.hpp"

#include <iostream>

using namespace TB_NS;

int main()
{
	ExmoApi api("your_key", "your_secret");

	http::JsonData response = api.call("user_info", "");
	std::clog << ">> user_info: " << response << "\n\n";
	
	response = api.call("order_book", api.build({ "pair=BTC_USD" }));
	std::clog << ">> order_book: " << response << "\n\n";
	
	return 0;
}