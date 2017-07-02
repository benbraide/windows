#pragma once

#ifndef WINPP_WINDOW_MANAGER_H
#define WINPP_WINDOW_MANAGER_H

#include <list>

#include "../wrappers/hwnd_wrapper.h"
#include "../wrappers/msg_wrapper.h"
#include "../wrappers/wnd_class_wrapper.h"

#include "../gui/gui_object.h"

#include "application_object.h"

#define WINPP_WM_MARSHALLED_MSG (WM_APP + 1)

namespace winpp{
	namespace application{
		class window_manager{
		public:
			typedef ::WNDPROC procedure_type;

			typedef ::HHOOK hook_type;
			typedef ::FLASHWINFO flash_info_type;

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

			typedef std::list<hwnd_value_type> hwnd_value_list_type;

			window_manager();

			~window_manager();

			void initialize(object &app);

			object &app();

			hwnd_type create(const create_info_type &info, bool is_dialog, bool replace_procedure = false);

			static uint_type register_message(const std::wstring &name);

			static lresult_type CALLBACK entry(hwnd_value_type window_handle, uint_type msg, wparam_type wparam, lparam_type lparam);

		private:
			static lresult_type CALLBACK hook_(int code, wparam_type wparam, lparam_type lparam);

			object *app_;
			hwnd_value_list_type hwnd_value_list_;

			void *recent_params_;
			bool replace_procedure_;

			static wnd_class_type class_;
			static wnd_class_type dialog_class_;
		};
	}
}

#endif /* !WINPP_WINDOW_MANAGER_H */
