// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include "../interaction.hpp"

namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS {
    // brief: the command to obtain information about pair-settings from an Exchange platform
    struct PairSettigs final : CommandI {
        TB_PRS(PairSettigs);

        using CommandI::CommandI;

#pragma region CommandI
        CommandCode code() const noexcept override;
#pragma endregion

        // brief: presents received data of pair-settings response
        struct Response {
            friend struct PairSettigs;

            struct RequestedData {
                TradePair pair{};
            } const request{};

            struct ReceivedData {
                struct {
                    Quantity min{};
                    Quantity max{};
                } quantity{};

                struct {
                    Price min{};
                    Price max{};
                } price{};

                struct {
                    Cost min{};
                    Cost max{};
                } cost{};
            } const response{};

            private:
            Response(TradePair i_pair, ExchangeCode i_code, Json::CR i_json);
        };

        using ECodeAndTPair = std::pair<ExchangeID, TradePair>;
        // brief: the data type that is presenting a key-pair map-container data-collection for identifying a previously obtained requested data
        using CachedRequests = AliasFor<std::vector<std::pair<ECodeAndTPair, Response>>>;

        // brief: collects the data that were obtained by request-operation
        static CachedRequests Cache;

        TB_NODISCARD Response request(TradePair i_pair) const;
    };
} // namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS
