#include "object_manager.h"
#include "../window/message_window.h"

winpp::application::object_manager::object_manager(object &app)
	: app_(&app), recent_params_(nullptr), replace_procedure_(false){}

winpp::application::object_manager::~object_manager() = default;

winpp::application::object &winpp::application::object_manager::app(){
	return *app_;
}

void winpp::application::object_manager::create_proxy(){
	if (proxy_window_ != nullptr)
		return;//Proxy already created

	proxy_window_ = std::make_shared<window::message>(*app_, L"WINPP_MSG_WND_" WINPP_WUUID);
	proxy_window_->events().unrecognized_message([](events::unrecognized_message &e) -> lresult_type{
		auto &msg = e.msg();
		if (msg.code() != WINPP_WM_PROXY_MSG)
			return 0;

		e.prevent();//Indicate handled
		return msg.lparam<message_proxy_info *>()->target->query<messaging::target>().dispatch(msg, e.is_sent());
	});
}

void winpp::application::object_manager::create(const create_info_type &info, hwnd_type &out){
	if (object::current_app == nullptr){
		out = nullptr;
		throw common::no_app_exception();
		return;
	}

	out_ = &out;
	recent_params_ = info.lpCreateParams;

	auto hook = ::SetWindowsHookExW(WH_CBT, hook_, nullptr, app_->id());//Install hook to intercept window creation
	if (hook == nullptr){
		out = nullptr;
		::SetLastError(ERROR_INTERNAL_ERROR);
		return;
	}

	const wchar_t *class_name = nullptr;
	if (info.lpszClass == nullptr){//Use default class
		class_name = static_cast<gui_object_type *>(recent_params_)->is_dialog() ? classes_.dialog().name().c_str() : classes_.general().name().c_str();
		replace_procedure_ = false;
	}
	else{//Use class
		class_name = info.lpszClass;
		replace_procedure_ = true;
	}

	out = ::CreateWindowExW(
		info.dwExStyle,
		class_name,
		info.lpszName,
		static_cast<dword_type>(info.style),
		info.x,
		info.y,
		info.cx,
		info.cy,
		info.hwndParent,
		info.hMenu,
		(info.hInstance == nullptr) ? ::GetModuleHandleW(nullptr) : info.hInstance,
		info.lpCreateParams
	);

	if (out == nullptr)//Remove association
		windows_.erase(static_cast<hwnd_value_type>(recent_params_));

	out_ = nullptr;
	recent_params_ = nullptr;

	::UnhookWindowsHookEx(hook);//Uninstall hook
}

bool winpp::application::object_manager::has_top_level() const{
	return !top_levels_.empty();
}

void winpp::application::object_manager::update(uint_type code, void *args){
	app_->execute_task([=]{
		update_(code, args);
	});
}

winpp::application::object_manager::window_type *winpp::application::object_manager::find_window(hwnd_value_type handle){
	return app_->execute_task<window_type *>([=]{
		return find_window_(handle);
	});
}

winpp::application::object_manager::gui_object_type *winpp::application::object_manager::owner(hwnd_type value){
	return value.data<gui_object_type *>();
}

winpp::application::object_manager::uint_type winpp::application::object_manager::register_message(const std::wstring &name){
	return ::RegisterWindowMessageW(name.c_str());
}

winpp::application::object_manager::lresult_type CALLBACK winpp::application::object_manager::entry(hwnd_value_type window_handle, uint_type msg, wparam_type wparam, lparam_type lparam){
	auto target = object::current_app->object_manager().find_window_(window_handle);
	if (target == nullptr)//Unidentified handle
		return ::DefWindowProcW(window_handle, msg, wparam, lparam);

	return target->dispatch(msg_type({ window_handle, msg, wparam, lparam }), false);
}

winpp::application::object_manager::messaging_map_type winpp::application::object_manager::messaging_map;

winpp::application::object_manager::window_type *winpp::application::object_manager::find_window_(hwnd_value_type handle){
	if (windows_.empty())
		return nullptr;

	auto entry = windows_.find(handle);
	return (entry == windows_.end()) ? nullptr : entry->second;
}

void winpp::application::object_manager::update_(uint_type code, void *args){
	switch (code){
	case update_object_created:
		update_object_created_(static_cast<gui_object_type *>(args));
		break;
	case update_object_destroyed:
		update_object_destroyed_(static_cast<gui_object_type *>(args));
		break;
	default:
		break;
	}
}

void winpp::application::object_manager::update_object_created_(gui_object_type *object){
	list_.push_back(object);
	if (object->is_top_level())
		top_levels_.push_back(object);
}

void winpp::application::object_manager::update_object_destroyed_(gui_object_type *object){
	if (object == nullptr || list_.empty())
		return;

	auto entry = std::find(list_.begin(), list_.end(), object);
	if (entry != list_.end())//Remove from list
		list_.erase(entry);

	entry = std::find(top_levels_.begin(), top_levels_.end(), object);
	if (entry != top_levels_.end())//Remove from list
		top_levels_.erase(entry);

	auto window_object = dynamic_cast<window_type *>(object);
	if (window_object != nullptr)//Remove association
		windows_.erase(static_cast<hwnd_value_type>(window_object->handle()));
}

winpp::application::object_manager::lresult_type CALLBACK winpp::application::object_manager::hook_(int code, wparam_type wparam, lparam_type lparam){
	if (code == HCBT_CREATEWND){//Respond to window creation
		auto &manager = object::current_app->object_manager();
		if (manager.recent_params_ != nullptr && reinterpret_cast<create_hook_info_type *>(lparam)->lpcs->lpCreateParams == manager.recent_params_){//Ensure target is valid
			auto target_hwnd_value = reinterpret_cast<hwnd_value_type>(wparam);

			manager.windows_[target_hwnd_value] = &static_cast<gui_object_type *>(manager.recent_params_)->query<window_type>();
			if (manager.replace_procedure_)//Replace window procedure
				hwnd_type(target_hwnd_value).data<procedure_type>(&object_manager::entry, hwnd_type::data_index_type::procedure);

			*manager.out_ = target_hwnd_value;
			manager.recent_params_ = target_hwnd_value;
			manager.replace_procedure_ = false;
		}
	}

	return ::CallNextHookEx(nullptr, code, wparam, lparam);
}

/*
winpp::application::object_manager::lresult_type CALLBACK winpp::application::object_manager::mouse_hook_(int code, wparam_type wparam, lparam_type lparam){
	if (code == HC_ACTION){
		auto &manager = object::current_app->object_manager();
		auto info = reinterpret_cast<mouse_info_type *>(lparam);
		if (wparam == WM_NCMOUSEMOVE || wparam == WM_MOUSEMOVE){
			auto target = owner(info->hwnd);
			auto moused_target = owner(manager.window_state_.moused);

			if (info->hwnd != manager.window_state_.moused){//Update window with mouse
				if (manager.window_state_.moused != nullptr && (moused_target == nullptr || !moused_target->is_ancestor(*target)))
					send_message(*moused_target, WM_NCMOUSELEAVE, 0, 0);
				manager.window_state_.moused = info->hwnd;
			}

			if (target != nullptr && target->has_parent())
				send_message(*target->parent(), WM_MOUSEMOVE, 0, 0);
		}
	}

	return ::CallNextHookEx(nullptr, code, wparam, lparam);
}*/

winpp::application::classes winpp::application::object_manager::classes_;

winpp::application::object_manager::uint_type winpp::application::object_manager::non_window_message_id_ = ::RegisterWindowMessageW(L"WINPP_NW_WM_" WINPP_WUUID);
