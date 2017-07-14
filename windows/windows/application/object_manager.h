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

#define WINPP_WM_PROXY_MSG (WM_APP + 0)
#define WINPP_WM_MARSHALLED_MSG (WM_APP + 1)

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

			struct window_state{
				hwnd_value_type moused;
				hwnd_value_type focused;
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

			void create(const create_info_type &info, hwnd_type &out);

			bool has_top_level() const;

			void update(uint_type code, void *args);

			window_type *find_window(hwnd_value_type handle);

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

			static const uint_type update_object_created		= 0x00000001u;
			static const uint_type update_object_destroyed		= 0x00000002u;

		private:
			window_type *find_window_(hwnd_value_type handle);

			void update_(uint_type code, void *args);

			void update_object_created_(gui_object_type *object);

			void update_object_destroyed_(gui_object_type *object);

			static lresult_type CALLBACK hook_(int code, wparam_type wparam, lparam_type lparam);

			object *app_;
			window_ptr_type proxy_window_;

			void *recent_params_;
			bool replace_procedure_;
			hwnd_type *out_;

			object_list_type list_;
			object_list_type top_levels_;
			window_list_type windows_;
			window_state window_state_{};

			static classes classes_;
			static uint_type non_window_message_id_;
		};
	}
}

#endif /* !WINPP_OBJECT_MANAGER_H */
