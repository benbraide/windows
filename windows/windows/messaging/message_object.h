#pragma once

#ifndef WINPP_MESSAGE_OBJECT_H
#define WINPP_MESSAGE_OBJECT_H

#include "../wrappers/msg_wrapper.h"
#include "../wrappers/hwnd_wrapper.h"

#include "../threading/thread_message_queue.h"
#include "../gui/gui_object.h"

namespace winpp{
	namespace messaging{
		class object{
		public:
			typedef ::WNDPROC procedure_type;
			typedef ::LRESULT lresult_type;

			typedef wrappers::msg msg_type;
			typedef wrappers::hwnd hwnd_type;

			typedef gui::object gui_object_type;

			enum class state_type : unsigned int{
				nil				= (0 << 0x0000),
				sent			= (1 << 0x0000),
				cross			= (1 << 0x0001),
				replied			= (1 << 0x0002),
				skipped			= (1 << 0x0003),
				done			= (1 << 0x0004),
				has_value		= (1 << 0x0005),
				modified		= (1 << 0x0006),
			};

			object(const msg_type &info, bool is_sent, procedure_type default_callback, gui_object_type *target = nullptr);

			virtual ~object();

			object &modify(const msg_type &value);

			object &handle(bool overwrite = false);

			object &skip();

			object &reply();

			object &reply(bool value);

			template <typename value_type>
			object &reply(value_type value){
				if (WINPP_IS(states_, state_type::cross) && !WINPP_IS(states_, state_type::replied)){
					threading::message_queue::reply((lresult_type)value);
					WINPP_SET(states_, state_type::replied);
				}

				return *this;
			}

			object &value(bool value, bool overwrite = false);

			template <typename value_type>
			object &value(value_type value, bool overwrite = false){
				if (overwrite || !WINPP_IS(states_, state_type::has_value)){
					value_ = (lresult_type)value;
					WINPP_SET(states_, state_type::has_value);
				}

				return *this;
			}

			template <typename value_type>
			std::enable_if_t<!std::is_same_v<value_type, bool>, value_type> value() const{
				return WINPP_IS(states_, state_type::has_value) ? (value_type)value_ : value_type();
			}

			template <typename value_type>
			std::enable_if_t<std::is_same_v<value_type, bool>, value_type> value() const{
				return WINPP_IS(states_, state_type::has_value) ? (value_ != 0u) : false;
			}

			bool is_sent() const;

			bool is_cross_thread() const;

			bool is_replied() const;

			bool has_value() const;

			bool is_default() const;

			bool is_skipped() const;

			bool is_handled() const;

			bool is_modified() const;

			const msg_type &info() const;

			gui_object_type *target() const;

		protected:
			msg_type info_;
			procedure_type default_callback_;
			gui_object_type *target_;
			state_type states_;
			lresult_type value_;
		};

		WINPP_MAKE_OPERATORS(object::state_type);
	}
}

#endif /* !WINPP_MESSAGE_OBJECT_H */
