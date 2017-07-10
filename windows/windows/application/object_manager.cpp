#include "object_manager.h"
#include "../window/window_object.h"

winpp::application::object_manager::object_manager(object &app)
	: app_(&app), recent_params_(nullptr), replace_procedure_(false){
	message_handle_ = ::CreateWindowExW(
		0,
		class_.name().c_str(),
		L"WINPP_MSG_WND_" WINPP_WUUID,
		0,
		0,
		0,
		0,
		0,
		HWND_MESSAGE,
		nullptr,
		nullptr,
		nullptr
	);
}

winpp::application::object_manager::~object_manager() = default;

winpp::application::object &winpp::application::object_manager::app(){
	return *app_;
}

winpp::application::object_manager::hwnd_type winpp::application::object_manager::create(const create_info_type &info){
	if (object::current_app == nullptr)
		return nullptr;

	recent_params_ = info.lpCreateParams;
	auto hook = ::SetWindowsHookExW(WH_CBT, hook_, nullptr, app_->id());//Install hook to intercept window creation
	if (hook == nullptr)
		return nullptr;

	const wchar_t *class_name = nullptr;
	if (info.lpszClass == nullptr){//Use default class
		class_name = static_cast<gui_object_type *>(recent_params_)->is_dialog() ? dialog_class_.name().c_str() : class_.name().c_str();
		replace_procedure_ = false;
	}
	else{//Use class
		class_name = info.lpszClass;
		replace_procedure_ = true;
	}

	auto value = ::CreateWindowExW(
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
		(info.hInstance == nullptr) ? class_.instance() : info.hInstance,
		info.lpCreateParams
	);

	::UnhookWindowsHookEx(hook);//Uninstall hook
	return value;
}

bool winpp::application::object_manager::has_top_level() const{
	return !top_levels_.empty();
}

void winpp::application::object_manager::update(uint_type code, void *args){
	app_->execute_task([=]{
		update_(code, args);
	});
}

winpp::application::object_manager::gui_object_type *winpp::application::object_manager::owner(hwnd_type value){
	return value.data<gui_object_type *>();
}

winpp::application::object_manager::uint_type winpp::application::object_manager::register_message(const std::wstring &name){
	return ::RegisterWindowMessageW(name.c_str());
}

winpp::application::object_manager::lresult_type CALLBACK winpp::application::object_manager::entry(hwnd_value_type window_handle, uint_type msg, wparam_type wparam, lparam_type lparam){
	if (window_handle == nullptr){//Thread message
		if (msg == non_window_message_id_){//Forward to non-window
			auto info = reinterpret_cast<non_window_message_info *>(wparam);
		}
	}

	return 0;
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
	auto entry = std::find(list_.begin(), list_.end(), object);
	if (entry != list_.end())//Remove from list
		list_.erase(entry);

	entry = std::find(top_levels_.begin(), top_levels_.end(), object);
	if (entry != top_levels_.end())//Remove from list
		top_levels_.erase(entry);
}

bool winpp::application::object_manager::create_classes_(){
	auto uuid = common::methods::wuuid();
	::GetClassInfoExW(nullptr, WC_DIALOG, dialog_class_);

	dialog_class_.styles(dialog_class_.styles() | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS);
	dialog_class_.procedure(object_manager::entry);
	dialog_class_.name(L"WINPP_DLG_CLS_" + uuid);
	dialog_class_.instance(::GetModuleHandleW(nullptr));
	dialog_class_.create();

	class_.styles(dialog_class_.styles());
	class_.procedure(dialog_class_.procedure());
	class_.name(L"WINPP_CLS_" + uuid);
	class_.cursor(dialog_class_.cursor());
	class_.instance(dialog_class_.instance());
	class_.create();

	return true;
}

winpp::application::object_manager::lresult_type CALLBACK winpp::application::object_manager::hook_(int code, wparam_type wparam, lparam_type lparam){
	if (code == HCBT_CREATEWND){//Respond to window creation
		auto &manager = object::current_app->object_manager();
		if (manager.recent_params_ != nullptr && reinterpret_cast<create_hook_info_type *>(lparam)->lpcs->lpCreateParams == manager.recent_params_){//Ensure target is valid
			hwnd_type target_hwnd(reinterpret_cast<hwnd_value_type>(wparam));

			target_hwnd.data(static_cast<gui_object_type *>(manager.recent_params_));//Store window object in handle
			if (manager.replace_procedure_)//Replace window procedure
				target_hwnd.data<procedure_type>(&object_manager::entry, hwnd_type::data_index_type::procedure);

			manager.recent_params_ = nullptr;//Reset
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

winpp::application::object_manager::wnd_class_type winpp::application::object_manager::class_;

winpp::application::object_manager::wnd_class_type winpp::application::object_manager::dialog_class_;

bool winpp::application::object_manager::classes_created_ = create_classes_();

winpp::application::object_manager::uint_type winpp::application::object_manager::non_window_message_id_ = ::RegisterWindowMessageW(L"WINPP_NW_WM_" WINPP_WUUID);
