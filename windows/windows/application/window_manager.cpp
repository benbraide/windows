#include "window_manager.h"

winpp::application::window_manager::window_manager()
	: app_(nullptr), recent_params_(nullptr), replace_procedure_(false){}

winpp::application::window_manager::~window_manager() = default;

void winpp::application::window_manager::initialize(object &app){
	if (app_ != nullptr)
		return;

	if ((app_ = &app) == object::main_app){
		auto uuid = common::methods::wuuid();
		::GetClassInfoExW(nullptr, WC_DIALOG, dialog_class_);

		dialog_class_.styles(dialog_class_.styles() | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS);
		dialog_class_.procedure(window_manager::entry);
		dialog_class_.name(L"WINPP_DLG_CLS_" + uuid);
		dialog_class_.instance(::GetModuleHandleW(nullptr));
		dialog_class_.create();

		class_.styles(dialog_class_.styles());
		class_.procedure(dialog_class_.procedure());
		class_.name(L"WINPP_CLS_" + uuid);
		class_.cursor(dialog_class_.cursor());
		class_.instance(dialog_class_.instance());
		class_.create();
	}
}

winpp::application::object &winpp::application::window_manager::app(){
	return *app_;
}

winpp::application::window_manager::hwnd_type winpp::application::window_manager::create(const create_info_type &info, bool replace_procedure){
	if (object::current_app == nullptr)
		return nullptr;

	recent_params_ = info.lpCreateParams;
	replace_procedure_ = replace_procedure;

	auto hook = ::SetWindowsHookExW(WH_CBT, hook_, nullptr, app_->id());//Install hook to intercept window creation
	if (hook == nullptr)
		return nullptr;

	auto value = ::CreateWindowExW(
		info.dwExStyle,
		class_,
		info.lpszName,
		static_cast<dword_type>(info.style),
		info.x,
		info.y,
		info.cx,
		info.cy,
		info.hwndParent,
		info.hMenu,
		class_.instance(),
		info.lpCreateParams
	);

	::UnhookWindowsHookEx(hook);//Uninstall hook
	return value;
}

winpp::application::window_manager::uint_type winpp::application::window_manager::register_message(const std::wstring &name){
	return ::RegisterWindowMessageW(name.c_str());
}

winpp::application::window_manager::lresult_type CALLBACK winpp::application::window_manager::entry(hwnd_value_type window_handle, uint_type msg, wparam_type wparam, lparam_type lparam){
	return 0;
}

winpp::application::window_manager::lresult_type CALLBACK winpp::application::window_manager::hook_(int code, wparam_type wparam, lparam_type lparam){
	if (code == HCBT_CREATEWND){//Respond to window creation
		auto &manager = object::current_app->window_manager();
		if (manager.recent_params_ != nullptr && reinterpret_cast<create_hook_info_type *>(lparam)->lpcs->lpCreateParams == manager.recent_params_){//Ensure target is valid
			hwnd_type target_hwnd(reinterpret_cast<hwnd_value_type>(wparam));

			target_hwnd.data(reinterpret_cast<gui::object *>(manager.recent_params_));//Store window object in handle
			if (manager.replace_procedure_)//Replace window procedure
				target_hwnd.data<procedure_type>(&window_manager::entry, hwnd_type::data_index_type::procedure);

			manager.recent_params_ = nullptr;//Reset
			manager.replace_procedure_ = false;
		}
	}

	return ::CallNextHookEx(nullptr, code, wparam, lparam);
}

winpp::application::window_manager::wnd_class_type winpp::application::window_manager::class_;

winpp::application::window_manager::wnd_class_type winpp::application::window_manager::dialog_class_;
