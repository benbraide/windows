#pragma once

#ifndef WINPP_MESSAGE_MAP_H
#define WINPP_MESSAGE_MAP_H

#include <memory>
#include <unordered_map>

#include "message_dispatcher.h"

namespace winpp{
	namespace messaging{
		class map{
		public:
			typedef ::UINT uint_type;
			typedef ::LRESULT lresult_type;

			typedef std::shared_ptr<dispatcher> dispatcher_ptr_type;
			typedef std::unordered_map<uint_type, dispatcher_ptr_type> list_type;

			map();

			map &add_entry(uint_type code, dispatcher_ptr_type dispatcher);

			template <typename dispatcher_type, typename... args_types>
			map &add_type(uint_type code, args_types &&... args){
				return add_entry(code, std::make_shared<dispatcher_type>(std::forward<args_types>(args)...));
			}

			dispatcher *find_dispatcher(uint_type code);

		private:
			list_type list_;
			dispatcher_ptr_type unrecognized_dispatcher_;
		};
	}
}

#endif /* !WINPP_MESSAGE_MAP_H */
