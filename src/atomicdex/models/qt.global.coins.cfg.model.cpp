/******************************************************************************
 * Copyright © 2013-2019 The Komodo Platform Developers.                      *
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

//! Project Headers
#include "atomicdex/models/qt.global.coins.cfg.model.hpp"
#include "atomicdex/utilities/qt.utilities.hpp"

//! Constructor
namespace atomic_dex
{
    global_coins_cfg_model::global_coins_cfg_model(QObject* parent) noexcept :
        QAbstractListModel(parent)
    {
        for (int i = 0; i < CoinType::Size; ++i)
        {
            m_proxies[i] = new global_coins_cfg_proxy_model(this);
            m_proxies[i]->setSourceModel(this);
            m_proxies[i]->setDynamicSortFilter(true);
            m_proxies[i]->setFilterRole(CoinsRoles::TickerAndNameRole);
            m_proxies[i]->setFilterCaseSensitivity(Qt::CaseInsensitive);
            m_proxies[i]->setSortRole(CoinsRoles::NameRole);

            //! Initial State will be enableable
            m_proxies[i]->filter_by_enableable();
            m_proxies[i]->filter_by_type(static_cast<::CoinType>(i));
    
            m_proxies[i]->sort(0);
        }
    }
    
    global_coins_cfg_model::~global_coins_cfg_model() noexcept
    {
        for (int i = 0; i < CoinType::Size; i++)
        {
            delete m_proxies[i];
            m_proxies[i] = nullptr;
        }
    }
} // namespace atomic_dex

//! Override
namespace atomic_dex
{
    QVariant
    global_coins_cfg_model::data(const QModelIndex& index, int role) const
    {
        if (!hasIndex(index.row(), index.column(), index.parent()))
        {
            return {};
        }

        const coin_config& item = m_model_data.at(index.row());
        switch (static_cast<CoinsRoles>(role))
        {
        case TickerRole:
            return QString::fromStdString(item.ticker);
        case GuiTickerRole:
            return QString::fromStdString(item.gui_ticker);
        case NameRole:
            return QString::fromStdString(item.name);
        case IsClaimable:
            return item.is_claimable;
        case CurrentlyEnabled:
            return item.currently_enabled;
        case Active:
            return item.active;
        case IsCustomCoin:
            return item.is_custom_coin;
        case Type:
            return QString::fromStdString(item.type);
        case CoinType:
            return static_cast<int>(item.coin_type);
        case TickerAndNameRole:
            return QString::fromStdString(item.ticker) + QString::fromStdString(item.name); ///! ETHethereum
        case Checked:
            return item.checked;
        }
        return {};
    }

    bool
    global_coins_cfg_model::setData(const QModelIndex& index, const QVariant& value, int role)
    {
        coin_config& item = m_model_data[index.row()];
        switch (static_cast<CoinsRoles>(role))
        {
        case CurrentlyEnabled:
            item.currently_enabled = value.toBool();
            break;
        case Active:
            item.active = value.toBool();
            break;
        case Checked:
        {
            if (item.checked != value.toBool())
            {
                item.checked = value.toBool();
                if (item.checked)
                {
                    m_checked_nb++;
                }
                else
                {
                    m_checked_nb--;
                }
                emit checked_nbChanged();
            }
            break;
        }
        default:
            return false;
        }

        emit dataChanged(index, index, {role});
        emit get_all_proxy()->lengthChanged();
        return true;
    }

    int
    global_coins_cfg_model::rowCount([[maybe_unused]] const QModelIndex& parent) const
    {
        return m_model_data.size();
    }

    QHash<int, QByteArray>
    global_coins_cfg_model::roleNames() const
    {
        return {{TickerRole, "ticker"}, {GuiTickerRole, "gui_ticker"},    {NameRole, "name"}, {IsClaimable, "is_claimable"}, {CurrentlyEnabled, "enabled"},
                {Active, "active"},     {IsCustomCoin, "is_custom_coin"}, {Type, "type"},     {Checked, "checked"}};
    }
} // namespace atomic_dex

//! CPP API
namespace atomic_dex
{
    void
    global_coins_cfg_model::initialize_model(std::vector<coin_config> cfg) noexcept
    {
        SPDLOG_INFO("Initializing global coin cfg model with size {}", cfg.size());
        set_checked_nb(0);
        beginResetModel();
        m_model_data = std::move(cfg);
        endResetModel();
        emit lengthChanged();
        emit get_all_proxy()->lengthChanged();
    }

    template <typename TArray>
    void
    global_coins_cfg_model::update_status(const TArray& tickers, bool status) noexcept
    {
        auto update_functor = [this, status](QModelIndexList res, const QString& ticker) {
            SPDLOG_INFO("Changing Active/CurrentlyEnabled status to {} for ticker {}", status, ticker.toStdString());
            const QModelIndex& idx = res.at(0);
            update_value(Active, status, idx, *this);
            update_value(CurrentlyEnabled, status, idx, *this);
        };

        for (auto&& ticker: tickers)
        {
            QString final_ticker = "";

            if constexpr (std::is_same_v<std::string, std::decay_t<decltype(ticker)>>)
            {
                final_ticker = QString::fromStdString(ticker);
            }
            else if constexpr (std::is_same_v<QString, std::decay_t<decltype(ticker)>>)
            {
                final_ticker = ticker;
            }

            if (const auto res = this->match(this->index(0, 0), TickerRole, final_ticker); not res.isEmpty())
            {
                update_functor(res, final_ticker);
            }
        }
    }

    template void global_coins_cfg_model::update_status(const QStringList&, bool);
    template void global_coins_cfg_model::update_status(const std::vector<std::string>&, bool);
} // namespace atomic_dex

//! QML API
namespace atomic_dex
{
    QStringList global_coins_cfg_model::get_checked_coins() const noexcept
    {
        QStringList result;
        
        for (auto&& coin_cfg : m_model_data)
        {
            if (coin_cfg.checked)
            {
                result.push_back(QString::fromStdString(coin_cfg.ticker));
            }
        }
        return result;
    }
}

//! Properties
namespace atomic_dex
{
    global_coins_cfg_proxy_model* global_coins_cfg_model::get_qrc20_proxy() const noexcept
    {
        return m_proxies[CoinType::QRC20];
    }
    
    global_coins_cfg_proxy_model* global_coins_cfg_model::get_erc20_proxy() const noexcept
    {
        return m_proxies[CoinType::ERC20];
    }
    
    global_coins_cfg_proxy_model* global_coins_cfg_model::get_smartchains_proxy() const noexcept
    {
        return m_proxies[CoinType::SmartChain];
    }
    
    global_coins_cfg_proxy_model* global_coins_cfg_model::get_utxo_proxy() const noexcept
    {
        return m_proxies[CoinType::UTXO];
    }
    
    [[nodiscard]]
    global_coins_cfg_proxy_model* global_coins_cfg_model::get_all_proxy() const noexcept
    {
        return m_proxies[CoinType::All];
    }
    
    int global_coins_cfg_model::get_length() const noexcept
    {
        return rowCount();
    }
    
    int global_coins_cfg_model::get_checked_nb() const noexcept
    {
        return m_checked_nb;
    }
    
    void global_coins_cfg_model::set_checked_nb(int value) noexcept
    {
        if (value == m_checked_nb)
        {
            return;
        }
        m_checked_nb = value;
        emit checked_nbChanged();
    }
}