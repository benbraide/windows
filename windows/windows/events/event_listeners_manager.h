#pragma once

#ifndef WINPP_EVENT_LISTENERS_MAAGER_H
#define WINPP_EVENT_LISTENERS_MAAGER_H

#include <mutex>
#include <unordered_map>

#include "../common/common_methods.h"
#include "../common/random_number.h"

namespace winpp{
	namespace events{
		class listeners_base;

		class listeners_manager{
		public:
		public:
			typedef std::mutex lock_type;
			typedef std::lock_guard<lock_type> guard_type;

			typedef std::unordered_map<unsigned __int32, listeners_base *> list_type;
			typedef common::random_uint32 random_number_type;

			static unsigned __int32 add(listeners_base &item);

			static void remove(unsigned __int32 id);

			static listeners_base *find(unsigned __int32 id);

			static bool unbind(unsigned __int64 id);

		protected:
			static list_type list_;
			static random_number_type generator_;
			static lock_type lock_;
		};
	}
}

#endif /* !WINPP_EVENT_LISTENERS_MAAGER_H */
