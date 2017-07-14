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
			typedef object_type object_type;
			typedef listeners<return_type, object_type &, value_types...> listeners_type;

			typedef typename listeners_type::no_args_callback_type no_args_callback_type;
			typedef typename listeners_type::callback_type callback_type;
			typedef typename listeners_type::guard_type guard_type;

			unsigned __int64 operator ()(callback_type callback){
				return listeners_.add(callback);
			}

			unsigned __int64 operator ()(no_args_callback_type callback){
				return listeners_.add(callback);
			}

			template <typename unused_type = return_type, typename... args_types>
			std::enable_if_t<std::is_same_v<unused_type, void>, unused_type> operator ()(object_type &e, args_types &&... args){
				fire(e, std::forward<args_types>(args)...);
			}

			template <typename unused_type = return_type, typename... args_types>
			std::enable_if_t<!std::is_same_v<unused_type, void>, unused_type> operator ()(object_type &e, unused_type default_value = unused_type(), args_types &&... args){
				return fire(e, default_value, std::forward<args_types>(args)...);
			}

			unsigned __int64 bind(callback_type callback){
				return listeners_.add(callback);
			}

			unsigned __int64 bind(no_args_callback_type callback){
				return listeners_.add(callback);
			}

			unsigned __int64 bind_generic(const std::any &callback){
				try{
					return operator ()(std::any_cast<callback_type>(callback));
				}
				catch (const std::bad_any_cast &){
					return operator ()(std::any_cast<no_args_callback_type>(callback));
				}

				return 0u;
			}

			bool unbind(unsigned __int64 id){
				return listeners_.remove(id);
			}

			template <typename unused_type = return_type, typename... args_types>
			std::enable_if_t<std::is_same_v<unused_type, void>, unused_type> fire(object_type &e, args_types &&... args){
				guard_type guard(listeners_.lock());
				for (auto &item : listeners_.callback_list()){
					item.second(e, std::forward<args_types>(args)...);
					if (!e.is_propagating())
						break;
				}
			}

			template <typename unused_type = return_type, typename... args_types>
			std::enable_if_t<!std::is_same_v<unused_type, void>, unused_type> fire(object_type &e, unused_type default_value = unused_type(), args_types &&... args){
				guard_type guard(listeners_.lock());
				for (auto &item : listeners_.callback_list()){
					default_value = item.second(e, std::forward<args_types>(args)...);
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

			static bool is_valid_id(unsigned __int64 id){
				return listeners_type::is_valid_id(id);
			}

		protected:
			listeners_type listeners_;
		};

		template <bool periodic, class... value_types>
		class timer_tunnel : public tunnel<void, object_with_data<::DWORD>, value_types...>{
		public:
			typedef tunnel<void, object_with_data<::DWORD>, value_types...> base_type;

			typedef ::DWORD dword_type;
			typedef ::HANDLE handle_type;

			typedef ::LPVOID lpvoid_type;
			typedef ::BOOLEAN boolean_type;

			typedef std::recursive_mutex lock_type;
			typedef std::lock_guard<lock_type> guard_type;

			struct callback_param{
				unsigned __int64 id;
				timer_tunnel *object;
				dword_type duration;
			};

			struct handler_info{
				handle_type handle;
				callback_param param;
			};

			typedef gui::object gui_object_type;
			typedef std::unordered_map<unsigned __int64, handler_info> handle_list_type;

			explicit timer_tunnel(gui_object_type &object)
				: object_(&object){}

			virtual ~timer_tunnel(){
				if (!application::object::main_is_exiting()){
					guard_type guard(base_type::listeners_.lock());
					if (!handle_list_.empty()){
						for (auto &item : handle_list_)
							::DeleteTimerQueueEx(item.second.handle, nullptr);
					}
				}
			}

			unsigned __int64 operator ()(dword_type duration, callback_type callback){
				return bind_(duration, callback);
			}

			unsigned __int64 operator ()(dword_type duration, no_args_callback_type callback){
				return bind_(duration, callback);
			}

			unsigned __int64 bind(dword_type duration, callback_type callback){
				return bind_(duration, callback);
			}

			unsigned __int64 bind(dword_type duration, no_args_callback_type callback){
				return bind_(duration, callback);
			}

			bool unbind(unsigned __int64 id){
				guard_type guard(base_type::listeners_.lock());
				if (!base_type::unbind(id))
					return false;

				auto entry = handle_list_.find(id);
				if (entry != handle_list_.end()){
					::DeleteTimerQueueEx(entry->second.handle, nullptr);
					handle_list_.erase(entry);
				}

				return true;
			}

			static const auto is_periodic = periodic;

		protected:
			template <typename callback_type>
			unsigned __int64 bind_(dword_type duration, callback_type callback){
				guard_type guard(base_type::listeners_.lock());

				auto id = base_type::operator ()(callback);
				if (!is_valid_id(id))
					return id;

				handle_type handle = nullptr;
				auto param = new callback_param{ id, this, duration };
				if (param == nullptr || ::CreateTimerQueueTimer(&handle, nullptr, &callback_, param, duration, is_periodic ? duration : 0u, WT_EXECUTEDEFAULT) == FALSE)
					return cancel_(id);

				handle_list_[id] = handler_info{ handle, param };
				return id;
			}

			unsigned __int64 cancel_(unsigned __int64 id){
				unbind(id);
				return 0u;
			}

			static VOID CALLBACK callback_(lpvoid_type params, boolean_type){
				auto info = reinterpret_cast<handler_info *>(params);
				guard_type guard(info->param.object->listeners_.lock());

				auto entry = info->param.object->handle_list_.find(id);
				if (entry = info->param.object->handle_list_.end())
					return;

				auto &list = info->param.object->listeners_.callback_list();
				auto callback_entry = list.find(info->param.id);
				if (callback_entry == list.end())
					return;

				object_type e(*object_, info->param.duration, false);
				if (std::holds_alternative<native_callback_type>(callback_entry->second))
					std::get<native_callback_type>(callback_entry->second)(e);
				else
					std::get<callback_type>(callback_entry->second)(e);

				if (!is_periodic)
					info->param.object->unbind(info->param.id);
			}

			gui_object_type *object_;
			handle_list_type handle_list_;
			lock_type lock_;
		};
	}
}

#endif /* !WINPP_EVENT_TUNNEL_H */
