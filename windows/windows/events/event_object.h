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

		class mouse_activate : public object{
		public:
			typedef ::LPARAM lparam_type;
			typedef ::UINT uint_type;

			typedef structures::enumerations::hit_target_type hit_target_type;

			mouse_activate(gui_object_type &target, lparam_type lparam);

			hit_target_type hit_target() const;

			uint_type mouse_message() const;

		protected:
			lparam_type lparam_;
		};

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
