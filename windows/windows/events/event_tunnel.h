#pragma once

#ifndef WINPP_EVENT_TUNNEL_H
#define WINPP_EVENT_TUNNEL_H

#include <any>

#include "event_listeners.h"
#include "event_object.h"

namespace winpp{
	namespace events{
		template <class return_type, class object_type = object, class... value_types>
		class tunnel{
		public:
			typedef listeners<return_type, object_type &, value_types...> listeners_type;

			typedef typename listeners_type::no_args_callback_type no_args_callback_type;
			typedef typename listeners_type::native_callback_type native_callback_type;
			typedef typename listeners_type::callback_type callback_type;
			typedef typename listeners_type::guard_type guard_type;

			typedef std::function<return_type(object &)> base_arg_callback_type;

			explicit tunnel(unsigned __int32 id)
				: listeners_(id){}

			template <typename callback_type>
			unsigned __int64 operator ()(callback_type callback){
				return listeners_.add(callback);
			}

			template <typename unused_type = return_type>
			std::enable_if_t<!std::is_same_v<std::function<unused_type(object &)>, callback_type>, unsigned __int64> operator ()(base_arg_callback_type callback){
				return add_base_arg_(callback);
			}

			template <typename unused_type = return_type, typename... args_types>
			std::enable_if_t<std::is_same_v<unused_type, void>, unused_type> operator ()(object_type &e, args_types &&... args){
				fire(e, std::forward<args_types>(args)...);
			}

			template <typename unused_type = return_type, typename... args_types>
			std::enable_if_t<!std::is_same_v<unused_type, void>, unused_type> operator ()(object_type &e, unused_type default_value = unused_type(), args_types &&... args){
				return fire(e, default_value, std::forward<args_types>(args)...);
			}

			unsigned __int64 operator ()(const std::any &callback){
				try{
					return operator ()(std::any_cast<callback_type>(callback));
				}
				catch (const std::bad_any_cast &){
					try{
						return operator ()(std::any_cast<native_callback_type>(callback));
					}
					catch (const std::bad_any_cast &){
						return operator ()(std::any_cast<no_args_callback_type>(callback));
					}
				}

				return 0u;
			}

			bool unbind(unsigned __int64 id){
				return listeners_.remove(id);
			}

			bool unbind(native_callback_type callback){
				return listeners_.remove(callback);
			}

			bool unbind(const std::any &value){
				try{
					return unbind(std::any_cast<unsigned __int64>(value));
				}
				catch (const std::bad_any_cast &){
					return unbind(std::any_cast<native_callback_type>(value));
				}

				return false;
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
			template <typename unused_type = return_type>
			std::enable_if_t<std::is_same_v<unused_type, void>, unsigned __int64> add_base_arg_(base_arg_callback_type callback){
				return operator ()([callback](object_type &e, value_types...){
					callback(e);
				});
			}

			template <typename unused_type = return_type>
			std::enable_if_t<!std::is_same_v<unused_type, void>, unsigned __int64> add_base_arg_(base_arg_callback_type callback){
				return operator ()([callback](object_type &e, value_types...) -> return_type{
					return callback(e);
				});
			}

			listeners_type listeners_;
		};
	}
}

#endif /* !WINPP_EVENT_TUNNEL_H */
