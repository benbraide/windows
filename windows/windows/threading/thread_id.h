#pragma once

#ifndef WINPP_THREAD_ID_H
#define WINPP_THREAD_ID_H

#include "../common/common_headers.h"
#include "../wrappers/value_wrapper.h"

namespace winpp{
	namespace threading{
		template <class value_type>
		class basic_id : public wrappers::value<basic_id<value_type>, value_type>{
		public:
			typedef wrappers::value<basic_id<value_type>, value_type> base_type;

			basic_id()
				: base_type(static_cast<value_type>(0)){}

			explicit basic_id(value_type value)
				: base_type(value){}

			int compare(const_qualified_type value) const{
				if (base_type::value_ != value)
					return (base_type::value_ < value) ? -1 : 1;
				return 0;
			}

			static basic_id current(){
				return basic_id(::GetCurrentThreadId());
			}
		};

		typedef basic_id<::DWORD> id;
	}
}

#endif /* !WINPP_THREAD_ID_H */
