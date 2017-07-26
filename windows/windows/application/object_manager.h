#pragma once

#ifndef WINPP_OBJECT_MANAGER_H
#define WINPP_OBJECT_MANAGER_H

#include <list>

#include "../common/scoped_index.h"
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

#define WINPP_MENU_ATOM				0x8000
#define WINPP_MENU_CLS				L"#32768"
#define WINPP_DLG_CLS				L"#32770"

namespace winpp{
	namespace application{
		class object_manager{
		public:
			typedef ::WNDPROC procedure_type;
			typedef ::HMENU hmenu_type;

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
			typedef std::unordered_map<void *, gui_object_type *> objects_with_handle_list_type;

			typedef std::shared_ptr<window_type> window_ptr_type;
			typedef common::scoped_index_base scoped_index_base_type;

			typedef std::shared_ptr<scoped_index_base_type> scoped_index_base_ptr_type;
			typedef std::list<scoped_index_base_ptr_type> scoped_index_base_ptr_list_type;

			template <typename key_type>
			using scoped_index_index_type = common::scoped_index<gui_object_type *, key_type, gui_object_type *>;

			enum class menu_type{
				bar,
				popup
			};

			enum class object_mouse_state : unsigned int{
				nil				= (0 << 0x0000),
				down			= (1 << 0x0000),
				drag			= (1 << 0x0001),
				track			= (1 << 0x0002),
			};

			struct object_state_type{
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

			struct object_map_type{
				void *handle;
				gui_object_type *owner;
			};

			explicit object_manager(object &app);

			~object_manager();

			object &app();

			void create_proxy();

			void create(const create_info_type &info, hwnd_type &out);

			hmenu_type create_menu(gui_object_type &owner, menu_type type);

			bool has_top_level() const;

			void object_created(gui_object_type &value);

			void object_destroyed(gui_object_type &value);

			template <typename key_type>
			key_type add_index(gui_object_type &value, gui_object_type *scope = nullptr){
				if (object::current_app == nullptr || object::current_app != app_)
					throw common::no_app_exception();
				return get_scoped_index_<key_type>()->add(scope, &value);
			}

			template <typename key_type>
			void remove_index(key_type key, gui_object_type *scope = nullptr){
				if (object::current_app == nullptr || object::current_app != app_)
					throw common::no_app_exception();

				auto scoped_index = get_scoped_index_<key_type>(false);
				if (scoped_index != nullptr)
					scoped_index->remove(scope, key);
			}

			template <typename key_type>
			gui_object_type *find_index(key_type key, gui_object_type *scope = nullptr){
				if (object::current_app == nullptr || object::current_app != app_)
					throw common::no_app_exception();

				auto scoped_index = get_scoped_index_<key_type>(false);
				return (scoped_index == nullptr) ? nullptr : scoped_index->find(scope, key);
			}

			template <typename key_type>
			scoped_index_index_type<key_type> *find_scoped_index(){
				if (object::current_app == nullptr || object::current_app != app_)
					throw common::no_app_exception();
				return get_scoped_index_<key_type>(false);
			}

			gui_object_type *find_object(void *handle);

			lresult_type handle_mouse_move(window_type &target, const msg_type &msg);

			lresult_type handle_mouse_hover(window_type &target, const msg_type &msg);

			lresult_type handle_mouse_leave(window_type &target, const msg_type &msg);

			lresult_type handle_mouse_down(window_type &target, const msg_type &msg);

			lresult_type handle_mouse_up(window_type &target, const msg_type &msg);

			lresult_type handle_mouse_dbl_click(window_type &target, const msg_type &msg);

			lresult_type handle_mouse_wheel(window_type &target, const msg_type &msg);

			object_state_type &object_state();

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

		private:
			gui_object_type *find_object_(void *handle);

			lresult_type nc_mouse_up_(window_type &target, const msg_type &msg, mouse_key_state_type button);

			gui_object_type *find_window_in_chain_(gui_object_type *target);

			template <typename key_type>
			scoped_index_index_type<key_type> *get_scoped_index_(bool create = true){
				scoped_index_index_type<key_type> *value;
				for (auto entry : scoped_index_list_){
					if ((value = dynamic_cast<scoped_index_index_type<key_type> *>(entry.get())) != nullptr)
						return value;
				}

				if (!create)
					return nullptr;

				auto ptr_value = std::make_shared<scoped_index_index_type<key_type>>();
				scoped_index_list_.push_back(ptr_value);

				return ptr_value.get();
			}

			static lresult_type CALLBACK hook_(int code, wparam_type wparam, lparam_type lparam);

			object *app_;
			window_ptr_type proxy_window_;

			void *recent_params_;
			bool replace_procedure_;
			hwnd_type *out_;

			object_list_type list_;
			object_list_type top_levels_;

			objects_with_handle_list_type objects_with_handle_;
			object_map_type last_search_{};

			object_state_type object_state_{};
			scoped_index_base_ptr_list_type scoped_index_list_;

			static classes classes_;
		};

		WINPP_MAKE_OPERATORS(object_manager::object_mouse_state);
	}
}

#endif /* !WINPP_OBJECT_MANAGER_H */
