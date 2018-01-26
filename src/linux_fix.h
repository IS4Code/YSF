#ifndef YSF_FALLBACK_H
#define YSF_FALLBACK_H

#include <memory>

#ifndef _WIN32

namespace std
{
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}
#endif

#endif
