#pragma once

#include <optional>

namespace SqlitoSeguro
{
    struct session{
        std::optional<int> userId;

        bool isLogged() const
        {
            return  userId.has_value();
        }

        void clear()
        {
            userId.reset();
        }
    };
}
