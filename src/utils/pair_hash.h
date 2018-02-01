#ifndef YSF_PAIR_HASH_H
#define YSF_PAIR_HASH_H

#include <cstddef>
#include <functional>

namespace aux
{
	struct pair_hash
	{
		template <typename T, typename U>
		std::size_t operator()(const std::pair<T, U> &x) const
		{
			std::size_t hash = 0;
			hash ^= std::hash<T>()(x.first) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			hash ^= std::hash<U>()(x.second) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			return hash;
		}
	};
}

#endif
