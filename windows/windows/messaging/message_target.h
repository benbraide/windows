#pragma once

#ifndef WINPP_MESSAGE_TARGET_H
#define WINPP_MESSAGE_TARGET_H

#include "../wrappers/msg_wrapper.h"
#include "../wrappers/hwnd_wrapper.h"

#include "../events/event_tunnel.h"

#include "message_handler.h"

namespace winpp{
	namespace messaging{
		class target : protected handler{
		public:
			typedef ::WNDPROC procedure_type;
			typedef ::LRESULT lresult_type;

			typedef wrappers::msg msg_type;
			typedef wrappers::hwnd hwnd_type;

			typedef events::object event_object_type;
			typedef events::tunnel<void> event_tunnel_type;

			virtual ~target();

			virtual bool pre_translate(msg_type &msg);

			virtual lresult_type procedure(const msg_type &msg);

			virtual procedure_type procedure() const = 0;

		protected:
			virtual target *target_parent_() const = 0;

			virtual void *find_event_() = 0;

			virtual lresult_type dispatch_nccreate_();

			virtual lresult_type dispatch_create_();

			virtual lresult_type dispatch_destroy_();

			virtual lresult_type dispatch_ncdestroy_();

			virtual lresult_type dispatch_close_();

			virtual lresult_type dispatch_unrecognized_();

			virtual lresult_type call_default_();

			template <typename return_type>
			return_type default_(){
				return (return_type)call_default_();
			}

			template <typename event_type, typename... args_types>
			bool action_prevented_(args_types &&... args){
				auto event_tunnel = find_event_();
				if (event_tunnel == nullptr)
					return false;

				typename event_type::object_type event_object(*reinterpret_cast<gui::object *>(this), std::forward<args_types>(args)...);
				static_cast<event_type *>(event_tunnel)->fire(event_object);

				return event_object.is_prevented();
			}

			template <typename event_type, typename... args_types>
			void fire_event_(args_types &&... args){
				action_prevented_<event_type>(std::forward<args_types>(args)...);
			}

			msg_type msg_;
		};
	}
}

#endif /* !WINPP_MESSAGE_TARGET_H */
