#pragma once

#ifndef WINPP_OBJECT_MANAGER_H
#define WINPP_OBJECT_MANAGER_H

#include <list>

#include "../wrappers/hwnd_wrapper.h"
#include "../wrappers/msg_wrapper.h"
#include "../wrappers/wnd_class_wrapper.h"

#include "../gui/gui_object.h"

#include "application_object.h"

#define WINPP_WM_MARSHALLED_MSG (WM_APP + 1)

namespace winpp{
	namespace window{
		class object;
	}

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

			typedef std::list<gui_object_type *> object_list_type;

			struct window_state{
				hwnd_value_type moused;
				hwnd_value_type focused;
			};

			struct non_window_message_info{
				gui_object_type *target;
				uint_type code;
				wparam_type wparam;
				lparam_type lparam;
			};

			explicit object_manager(object &app);

			~object_manager();

			object &app();

			hwnd_type create(const create_info_type &info);

			bool has_top_level() const;

			void update(uint_type code, void *args);

			template <typename return_type = lresult_type, typename arg_wparam_type = wparam_type, typename arg_lparam_type = lparam_type>
			static return_type send_message(gui_object_type &target, uint_type message, arg_wparam_type wparam, arg_lparam_type lparam){
				if (target.group() == gui_object_type::window_group)//Forward message to window
					return hwnd_type(static_cast<hwnd_value_type>(target.handle())).send_message<return_type>(message, wparam, lparam);

				auto target_app = target.app();
				if (target_app == nullptr)
					throw common::no_app_exception();

				non_window_message_info info{
					&target,
					message,
					(wparam_type)wparam,
					(lparam_type)lparam
				};

				return (return_type)target_app->execute_task<lresult_type>([&info]{
					return entry(nullptr, non_window_message_id_, reinterpret_cast<wparam_type>(&info), 0u);
				});
			}

			static gui_object_type *owner(hwnd_type value);

			static uint_type register_message(const std::wstring &name);

			static lresult_type CALLBACK entry(hwnd_value_type window_handle, uint_type msg, wparam_type wparam, lparam_type lparam);

			static const uint_type update_object_created		= 0x00000001u;
			static const uint_type update_object_destroyed		= 0x00000002u;

		private:
			void update_(uint_type code, void *args);

			void update_object_created_(gui_object_type *object);

			void update_object_destroyed_(gui_object_type *object);

			static bool create_classes_();

			static lresult_type CALLBACK hook_(int code, wparam_type wparam, lparam_type lparam);

			object *app_;
			hwnd_value_type message_handle_;

			void *recent_params_;
			bool replace_procedure_;

			object_list_type list_;
			object_list_type top_levels_;
			window_state window_state_{};

			static wnd_class_type class_;
			static wnd_class_type dialog_class_;
			static bool classes_created_;

			static uint_type non_window_message_id_;
		};
	}
}

#endif /* !WINPP_OBJECT_MANAGER_H */
