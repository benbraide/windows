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
			typedef hwnd_type::value_type hwnd_value_type;

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

			lresult_type result() const;

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

		class mouse_activate : public object{
		public:
			typedef ::UINT uint_type;
			typedef structures::enumerations::hit_target_type hit_target_type;

			template <typename... args_type>
			explicit mouse_activate(args_type &&... args)
				: object(std::forward<args_type>(args)...){}

			virtual ~mouse_activate();

			virtual hit_target_type hit_target() const;

			virtual uint_type mouse_message() const;
		};

		class ncactivate : public object{
		public:
			template <typename... args_type>
			explicit ncactivate(args_type &&... args)
				: object(std::forward<args_type>(args)...){}

			virtual ~ncactivate();

			virtual bool is_activating() const;
		};

		class activate : public object{
		public:
			template <typename... args_type>
			explicit activate(args_type &&... args)
				: object(std::forward<args_type>(args)...){}

			virtual ~activate();

			virtual bool is_activated() const;

			virtual bool click_activated() const;

			virtual hwnd_type other_window() const;
		};

		class focus_change : public object{
		public:
			template <typename... args_type>
			explicit focus_change(args_type &&... args)
				: object(std::forward<args_type>(args)...){}

			virtual ~focus_change();

			virtual bool is_focused() const;
		};

		class enable : public object{
		public:
			template <typename... args_type>
			explicit enable(args_type &&... args)
				: object(std::forward<args_type>(args)...){}

			virtual ~enable();

			virtual bool is_enabled() const;
		};

		class position : public object{
		public:
			typedef ::WINDOWPOS window_pos_type;

			typedef structures::point point_type;
			typedef structures::size size_type;

			typedef structures::enumerations::position_type position_type;

			template <typename... args_type>
			explicit position(args_type &&... args)
				: object(std::forward<args_type>(args)...){}

			virtual ~position();

			virtual bool is_changing() const;

			virtual point_type offset() const;

			virtual size_type size() const;

			virtual position_type flags() const;
		};

		WINPP_MAKE_OPERATORS(object::state_type);
	}
}

#endif /* !WINPP_MESSAGE_OBJECT_H */
