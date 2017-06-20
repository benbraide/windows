#pragma once

#ifndef WINPP_EVENT_TUNNEL_H
#define WINPP_EVENT_TUNNEL_H

#include "event_listeners.h"
#include "event_object.h"

namespace winpp{
	namespace events{
		template <class return_type, class object_type = object, class... value_types>
		class tunnel{
		public:
			typedef listeners<return_type, object_type &, value_types...> listeners_type;

			typedef typename listeners_type::native_callback_type native_callback_type;
			typedef typename listeners_type::callback_type callback_type;
			typedef typename listeners_type::guard_type guard_type;

			explicit tunnel(unsigned __int32 id)
				: listeners_(id){}

			template <typename callback_type>
			unsigned __int64 operator ()(callback_type callback){
				return listeners_.add(callback);
			}

			bool unbind(unsigned __int64 id){
				return listeners_.remove(id);
			}

			bool unbind(native_callback_type callback){
				return listeners_.remove(callback);
			}

			template <typename unused_type = return_type, typename... args_types>
			std::enable_if_t<std::is_same_v<unused_type, void>, unused_type> fire(object_type &e, args_types &&... args){
				guard_type guard(listeners_.lock());
				for (auto &item : listeners_.callback_list()){
					if (std::holds_alternative<native_callback_type>(item.second))
						std::get<native_callback_type>(item.second)(e, std::forward<args_types>(args)...);
					else
						std::get<callback_type>(item.second)(e, std::forward<args_types>(args)...);

					if (!e.is_propagating())
						break;
				}
			}

			template <typename unused_type = return_type, typename... args_types>
			std::enable_if_t<!std::is_same_v<unused_type, void>, unused_type> fire(object_type &e, unused_type default_value = unused_type(), args_types &&... args){
				guard_type guard(listeners_.lock());
				for (auto &item : listeners_.callback_list()){
					if (std::holds_alternative<native_callback_type>(item.second))
						default_value = std::get<native_callback_type>(item.second)(e, std::forward<args_types>(args)...);
					else
						default_value = std::get<callback_type>(item.second)(e, std::forward<args_types>(args)...);

					if (!e.is_propagating())
						break;
				}

				return default_value;
			}

			unsigned __int32 id() const{
				return listeners_.id();
			}

			bool empty() const{
				return listeners_.callback_list().empty();
			}

		private:
			listeners_type listeners_;
		};
	}
}

#endif /* !WINPP_EVENT_TUNNEL_H */