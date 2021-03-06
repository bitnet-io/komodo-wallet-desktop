/******************************************************************************
 * Copyright © 2013-2021 The Komodo Platform Developers.                      *
 *                                                                            *
 * See the AUTHORS, DEVELOPER-AGREEMENT and LICENSE files at                  *
 * the top-level directory of this distribution for the individual copyright  *
 * holder information and the developer policies on copyright and licensing.  *
 *                                                                            *
 * Unless otherwise agreed in a custom licensing agreement, no part of the    *
 * Komodo Platform software, including this file may be copied, modified,     *
 * propagated or distributed except according to the terms contained in the   *
 * LICENSE file                                                               *
 *                                                                            *
 * Removal or modification of this copyright notice is prohibited.            *
 *                                                                            *
 ******************************************************************************/

#pragma once

//! Deps
#include <nlohmann/json_fwd.hpp>

namespace mm2::api
{
    struct order_contents
    {
        std::string coin;
        std::string address;
        std::string price;
        std::string price_fraction_numer;
        std::string price_fraction_denom;
        std::string max_volume_fraction_numer;
        std::string max_volume_fraction_denom;
        std::string maxvolume;
        std::string pubkey;
        std::size_t age;
        std::size_t zcredits;
        std::string total;
        std::string uuid;
        std::string depth_percent;
        bool        is_mine;
        std::string min_volume{"0"};
    };

    void from_json(const nlohmann::json& j, order_contents& contents);
} // namespace mm2::api