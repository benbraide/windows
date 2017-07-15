#pragma once

#ifndef WINPP_OBJECT_MANAGER_H
#define WINPP_OBJECT_MANAGER_H

#include <list>

#include "../wrappers/hwnd_wrapper.h"
#include "../wrappers/msg_wrapper.h"

#include "../gui/gui_object.h"
#include "../messaging/message_map.h"
#include "../window/window_object.h"

#include "application_classes.h"

#define WINPP_WM_PROXY_MSG			(WM_APP + 0x00)
#define WINPP_WM_MARSHALLED_MSG		(WM_APP + 0x01)
#define WINPP_WM_APP_RESERVED_LAST	(WM_APP + 0x09)

#define WINPP_WM_MOUSEENTER			(WINPP_WM_APP_RESERVED_LAST + 0x01)
#define WINPP_WM_MOUSELEAVE			(WINPP_WM_APP_RESERVED_LAST + 0x02)

#define WINPP_WM_MOUSEDRAG			(WINPP_WM_APP_RESERVED_LAST + 0x03)
#define WINPP_WM_MOUSEDRAGEND		(WINPP_WM_APP_RESERVED_LAST + 0x04)
#define WINPP_WM_MOUSEDRAGCANCEL	(WINPP_WM_APP_RESERVED_LAST + 0x05)

#define WINPP_WM_MOUSE				(WINPP_WM_APP_RESERVED_LAST + 0x06)
#define WINPP_WM_RAWMOUSE			(WINPP_WM_APP_RESERVED_LAST + 0x07)

namespace winpp{
	namespace application{
		class object_manager{
		public:
			typedef ::WNDPROC procedure_type;

			typedef ::HHOOK hook_type;
			typedef ::FLASHWINFO flash_info_type;

			typedef ::CWPRETSTRUCT call_return_info_type;
			typedef ::MOUSEHOOKSTRUCT mouse_info_type;

			typedef ::CBT_CREATEWND create_hook_info_type;
			typedef ::CREATESTRUCTW create_info_type;
			typedef ::TRACKMOUSEEVENT track_info_type;

			typedef ::LRESULT lresult_type;
			typedef ::WPARAM wparam_type;
			typedef ::LPARAM lparam_type;

			typedef ::UINT uint_type;
			typedef ::WORD word_type;
			typedef ::DWORD dword_type;

			typedef ::INT_PTR int_ptr_type;
			typedef ::UINT_PTR uint_ptr_type;

			typedef structures::point point_type;
			typedef structures::size size_type;
			typedef structures::rect rect_type;

			typedef structures::enumerations::hit_target_type hit_target_type;
			typedef structures::enumerations::mouse_key_state_type mouse_key_state_type;

			typedef wrappers::msg msg_type;
			typedef wrappers::wnd_class wnd_class_type;

			typedef wrappers::hwnd hwnd_type;
			typedef hwnd_type::value_type hwnd_value_type;
			typedef hwnd_type::create_info_type create_info_type;

			typedef gui::object gui_object_type;
			typedef window::object window_type;
			typedef messaging::map messaging_map_type;

			typedef std::list<gui_object_type *> object_list_type;
			typedef std::unordered_map<hwnd_value_type, window_type *> window_list_type;

			typedef std::shared_ptr<window_type> window_ptr_type;

			enum class object_mouse_state : unsigned int{
				nil				= (0 << 0x0000),
				down			= (1 << 0x0000),
				drag			= (1 << 0x0001),
				track			= (1 << 0x0002),
			};

			struct object_state{
				gui_object_type *moused;
				gui_object_type *focused;
				gui_object_type *captured;
				object_mouse_state mouse_state;
				mouse_key_state_type button;
				point_type down_position;
				point_type last_position;
			};

			struct message_proxy_info{
				gui_object_type *target;
				uint_type code;
				wparam_type wparam;
				lparam_type lparam;
			};

			explicit object_manager(object &app);

			~object_manager();

			object &app();

			void create_proxy();

			void create(const create_info_type &info, hwnd_type &out);

			bool has_top_level() const;

			void update(uint_type code, void *args);

			window_type *find_window(hwnd_value_type handle);

			lresult_type handle_mouse_move(window_type &target, const msg_type &msg);

			lresult_type handle_mouse_leave(window_type &target, const msg_type &msg);

			lresult_type handle_mouse_down(window_type &target, const msg_type &msg);

			lresult_type handle_mouse_up(window_type &target, const msg_type &msg);

			lresult_type handle_mouse_dbl_click(window_type &target, const msg_type &msg);

			template <typename return_type = lresult_type, typename arg_wparam_type = wparam_type, typename arg_lparam_type = lparam_type>
			static return_type send_message(gui_object_type &target, uint_type message, arg_wparam_type wparam, arg_lparam_type lparam){
				if (target.group() == gui_object_type::window_group)//Forward message to window
					return target.query<window_type>().send_message<return_type>(message, wparam, lparam);

				auto target_app = target.app();
				if (target_app == nullptr)
					throw common::no_app_exception();

				message_proxy_info info{
					&target,
					message,
					(wparam_type)wparam,
					(lparam_type)lparam
				};

				return target_app->object_manager().proxy_window_->send_message<return_type>(WINPP_WM_PROXY_MSG, 0, &info);
			}

			static gui_object_type *owner(hwnd_type value);

			static uint_type register_message(const std::wstring &name);

			static lresult_type CALLBACK entry(hwnd_value_type window_handle, uint_type msg, wparam_type wparam, lparam_type lparam);

			static messaging_map_type messaging_map;
			static size_type drag_threshold;

			static const uint_type update_object_created		= 0x00000001u;
			static const uint_type update_object_destroyed		= 0x00000002u;

		private:
			window_type *find_window_(hwnd_value_type handle);

			void update_(uint_type code, void *args);

			void update_object_created_(gui_object_type *object);

			void update_object_destroyed_(gui_object_type *object);

			lresult_type nc_mouse_up_(window_type &target, const msg_type &msg, mouse_key_state_type button);

			gui_object_type *find_window_in_chain_(gui_object_type *target);

			static lresult_type CALLBACK hook_(int code, wparam_type wparam, lparam_type lparam);

			object *app_;
			window_ptr_type proxy_window_;

			void *recent_params_;
			bool replace_procedure_;
			hwnd_type *out_;

			object_list_type list_;
			object_list_type top_levels_;
			window_list_type windows_;
			object_state object_state_{};

			static classes classes_;
		};

		WINPP_MAKE_OPERATORS(object_manager::object_mouse_state);
	}
}

#endif /* !WINPP_OBJECT_MANAGER_H */
