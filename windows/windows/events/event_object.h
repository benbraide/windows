#pragma once

#ifndef WINPP_EVENT_OBJECT_H
#define WINPP_EVENT_OBJECT_H

#include "../gui/gui_object.h"

namespace winpp{
	namespace events{
		class object{
		public:
			typedef gui::object gui_object_type;

			enum class state_type : unsigned int{
				nil				= (0 << 0x0000),
				prevented		= (1 << 0x0000),
				stopped			= (1 << 0x0001),
			};

			explicit object(gui_object_type &target);

			virtual ~object();

			object &prevent();

			object &stop_propagation();

			bool is_prevented() const;

			bool is_propagating() const;

			gui_object_type &target() const;

		protected:
			gui_object_type *target_;
			state_type states_;
		};

		typedef class dispatch_message : public object{
		public:
			typedef wrappers::msg msg_type;

			dispatch_message(gui_object_type &target, const msg_type &msg, bool is_sent);

			const msg_type &msg() const;

			bool is_sent() const;

		protected:
			msg_type msg_;
			bool is_sent_;
		} unrecognized_message;

		typedef class mouse_activate : public object{
		public:
			typedef ::LPARAM lparam_type;
			typedef ::UINT uint_type;

			typedef structures::enumerations::hit_target_type hit_target_type;

			mouse_activate(gui_object_type &target, lparam_type lparam);

			hit_target_type hit_target() const;

			uint_type mouse_message() const;

		protected:
			lparam_type lparam_;
		} set_cursor;

		class pre_activate : public object{
		public:
			pre_activate(gui_object_type &target, bool activating);

			bool is_activating() const;

		protected:
			bool activating_;
		};

		class activate : public object{
		public:
			typedef wrappers::hwnd hwnd_type;

			enum class state{
				inactive			= WA_INACTIVE,
				active				= WA_ACTIVE,
				click_active		= WA_CLICKACTIVE
			};

			activate(gui_object_type &target, state state, hwnd_type other_window);

			bool is_activated() const;

			bool click_activated() const;

			hwnd_type other_window() const;

		protected:
			state state_;
			hwnd_type other_window_;
		};

		class focus_change : public object{
		public:
			focus_change(gui_object_type &target, bool focused);

			bool is_focused() const;

		protected:
			bool focused_;
		};

		class enable : public object{
		public:
			enable(gui_object_type &target, bool enabled);

			bool is_enabled() const;

		protected:
			bool enabled_;
		};

		class hit_test : public object{
		public:
			typedef structures::point point_type;

			hit_test(gui_object_type &target, const point_type &position);

			virtual ~hit_test();

			virtual const point_type &mouse_position() const;

		protected:
			point_type position_;
		};

		class position : public object{
		public:
			typedef ::WINDOWPOS window_pos_type;

			typedef structures::point point_type;
			typedef structures::size size_type;

			typedef structures::enumerations::position_type position_type;

			position(gui_object_type &target, bool changing, window_pos_type &info);

			virtual ~position();

			virtual bool is_changing() const;

			virtual point_type offset() const;

			virtual size_type size() const;

			virtual position_type flags() const;

		protected:
			bool changing_;
			window_pos_type *info_;
		};

		class size : public object{
		public:
			typedef structures::enumerations::window_edge_type window_edge_type;
			typedef structures::enumerations::window_size_type window_size_type;

			size(gui_object_type &target, bool changing, int value);

			virtual ~size();

			virtual bool is_changing() const;

			virtual window_edge_type edge() const;

			virtual window_size_type reason() const;

		protected:
			bool changing_;
			int value_;
		};

		class move : public object{
		public:
			typedef structures::enumerations::window_edge_type window_edge_type;
			typedef structures::enumerations::window_size_type window_size_type;

			move(gui_object_type &target, bool changing);

			virtual ~move();

			virtual bool is_changing() const;

		protected:
			bool changing_;
		};

		class mouse : public object{
		public:
			typedef structures::point point_type;
			typedef structures::size size_type;

			typedef structures::enumerations::hit_target_type hit_target_type;
			typedef structures::enumerations::mouse_key_state_type mouse_key_state_type;

			typedef ::UINT uint_type;
			typedef ::WPARAM wparam_type;
			typedef ::LPARAM lparam_type;

			mouse(gui_object_type &target, uint_type code, wparam_type wparam, gui_object_type *original_target);

			virtual ~mouse();

			virtual bool is_client() const;

			virtual bool is_vertical_wheel() const;

			virtual mouse_key_state_type button() const;

			virtual hit_target_type hit_target() const;

			virtual point_type position() const;

			virtual short wheel_delta() const;

			virtual size_type drag_delta() const;

			virtual gui_object_type *original_target() const;

		protected:
			uint_type code_;
			wparam_type wparam_;
			gui_object_type *original_target_;
		};

		class key : public object{
		public:
			typedef ::BYTE byte_type;
			typedef ::UINT uint_type;

			typedef ::WPARAM wparam_type;
			typedef ::LPARAM lparam_type;
			typedef ::LRESULT lresult_type;

			typedef structures::enumerations::key_event_state_type state_type;
			typedef structures::enumerations::key_state_type key_state_type;

			explicit key(gui_object_type &target, uint_type code, wparam_type wparam, lparam_type lparam, gui_object_type *original_target);

			virtual ~key();

			virtual bool is_system() const;

			virtual bool is_extended() const;

			virtual bool was_down() const;

			virtual bool is_being_released() const;

			virtual bool alt_key_down() const;

			virtual bool is_down() const;

			virtual bool is_up() const;

			virtual bool is_char() const;

			virtual bool is_dead() const;

			virtual unsigned short code() const;

			virtual short scan_code() const;

			virtual short repeat_count() const;

			virtual key_state_type key_states() const;

			virtual state_type states() const;

			virtual gui_object_type *original_target() const;

		protected:
			virtual void resolve_(uint_type code, wparam_type wparam, lparam_type lparam);

			gui_object_type *original_target_;
			unsigned short code_;
			short scan_code_;
			short repeat_count_;
			state_type states_;
		};

		template <class value_type>
		class object_with_data : public object{
		public:
			typedef value_type value_type;
			typedef std::conditional_t<std::is_object_v<value_type>, const value_type &, value_type> const_qualified_type;

			object_with_data(gui_object_type &target, const_qualified_type data)
				: object(target), data_(data){}

			virtual ~object_with_data() = default;

			const_qualified_type data() const{
				return data_;
			}

		private:
			value_type data_;
		};

		WINPP_MAKE_OPERATORS(object::state_type);
	}
}

#endif /* !WINPP_EVENT_OBJECT_H */
