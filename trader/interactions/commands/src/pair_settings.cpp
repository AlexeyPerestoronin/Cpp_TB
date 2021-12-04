// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include "../pair_settings.hpp"

namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS {
    PairSettigs::CachedRequests PairSettigs::Cache{};

    CommandCode PairSettigs::code() const noexcept {
        return CommandID::GET_PAIR_SETTINGS;
    }

    TB_NODISCARD PairSettigs::Response PairSettigs::request(TradePair i_pair) const {
        try {
            // try to find in the Cache
            for (const auto& [requestedData, obtainedData] : Cache)
                if (const auto& [exchangeID, tradePair] = requestedData; exchangeID == m_exchange->code() && tradePair == i_pair)
                    return obtainedData;

            auto interaction = m_exchange->createIteractions(code());
            interaction->prepare(i_pair);
            Response responce(i_pair, m_exchange->code(), interaction->interact());
            return Cache.emplace_back(ECodeAndTPair{ m_exchange->code(), std::move(i_pair) }, std::move(responce)).second;
        } catch (TB_NS::Error_NS::Exception& error) {
            throw TB_EXEPT["CommandI"]["PairSettigs"] << TB_SUBERROR(error) << TB_LOCATION;
        }
    }
} // namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS
