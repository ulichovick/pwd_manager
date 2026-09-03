#pragma once

#include <optional>

namespace SqlitoSeguro
{
    struct session{
        std::optional<int> userId;
        int accId;

        bool isLogged() const
        {
            return  userId.has_value();
        }

        void clear()
        {
            userId.reset();
            accId = 0;
        }
    };
}
