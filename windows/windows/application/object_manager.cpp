#include "object_manager.h"

#include "../menu/menu_object.h"
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
	if (object::current_app == nullptr || object::current_app != app_)
		throw common::no_app_exception();

	out_ = &out;
	recent_params_ = info.lpCreateParams;

	auto hook_handle = ::SetWindowsHookExW(WH_CBT, hook_, nullptr, app_->id());//Install hook to intercept certain events
	if (hook_handle == nullptr){//Failed to install hook
		out = nullptr;
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

	try{//Inside try-block -- Always uninstall hook
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
	}
	catch (...){
		::UnhookWindowsHookEx(hook_handle);//Uninstall hook
		throw;//Forward exception
	}

	::UnhookWindowsHookEx(hook_handle);//Uninstall hook
}

winpp::application::object_manager::hmenu_type winpp::application::object_manager::create_menu(gui_object_type &owner, menu_type type){
	if (object::current_app == nullptr || object::current_app != app_)
		throw common::no_app_exception();
	return ((type == menu_type::bar) ? ::CreateMenu() : ::CreatePopupMenu());
}

bool winpp::application::object_manager::has_top_level() const{
	return !top_levels_.empty();
}

void winpp::application::object_manager::object_created(gui_object_type &value){
	if (object::current_app == nullptr || object::current_app != app_)
		throw common::no_app_exception();

	list_.push_back(&value);
	if (value.is_window() && value.is_top_level())
		top_levels_.push_back(&value);

	if (!value.is_group()){
		auto handle = value.handle();
		if (handle != nullptr)//Set association
			objects_with_handle_[handle] = &value;
	}
}

void winpp::application::object_manager::object_destroyed(gui_object_type &value){
	if (object::current_app == nullptr || object::current_app != app_)
		throw common::no_app_exception();

	if (list_.empty())
		return;

	auto entry = std::find(list_.begin(), list_.end(), &value);
	if (entry != list_.end())//Remove from list
		list_.erase(entry);

	entry = std::find(top_levels_.begin(), top_levels_.end(), &value);
	if (entry != top_levels_.end())//Remove from list
		top_levels_.erase(entry);

	auto handle = value.handle();
	if (handle != nullptr)//Remove association
		objects_with_handle_.erase(handle);

	auto window_object = dynamic_cast<window_type *>(&value);
	if (window_object != nullptr)//Remove association
		objects_with_handle_.erase(static_cast<hwnd_value_type>(window_object->handle()));

	if (&value == last_search_.owner)
		last_search_ = object_map_type{};

	auto menu_item = value.try_query<menu::item>();
	if (menu_item != nullptr && menu_item->id() != menu::item::invalid_id)//Remove ID entry
		remove_index(menu_item->id(), &menu_item->parent()->get_type<menu::object>());
}

winpp::application::object_manager::gui_object_type * winpp::application::object_manager::find_object(void *handle){
	if (object::current_app == nullptr || object::current_app != app_)
		throw common::no_app_exception();
	return find_object_(handle);
}

winpp::application::object_manager::lresult_type winpp::application::object_manager::handle_mouse_move(window_type &target, const msg_type &msg){
	auto mouse_position = threading::message_queue::last_mouse_position();
	auto is_client = (msg.code() == WM_MOUSEMOVE);

	auto old_moused = object_state_.moused;
	if (object_state_.moused == nullptr)//First movement
		object_state_.moused = &target;

	if (is_client){//Inside client - find deepest offspring
		auto moused = object_state_.moused;
		while (moused != nullptr){//Mouse enter
			object_state_.moused = moused;
			object_state_.moused->query<messaging::target>().dispatch(msg_type(msg_type::value_type{
				static_cast<hwnd_value_type>(object_state_.moused->handle()),
				WINPP_WM_MOUSE,
				0,
				WINPP_WM_MOUSEENTER
			}), true);//Send mouse enter message
			moused = moused->hit_target(mouse_position);//Find next target
		}
	}
	else{//Non-client
		object_state_.moused->query<messaging::target>().dispatch(msg_type(msg_type::value_type{
			static_cast<hwnd_value_type>(object_state_.moused->handle()),
			WINPP_WM_MOUSE,
			0,
			WINPP_WM_MOUSEENTER
		}), true);//Send mouse enter message
	}

	if (object_state_.moused != old_moused){//New target
		object_state_.mouse_state = object_mouse_state::nil;
		object_state_.button = mouse_key_state_type::nil;
	}

	if (!WINPP_IS(object_state_.mouse_state, object_mouse_state::track)){//Track mouse
		track_info_type track_info{ sizeof(track_info_type), TME_HOVER | TME_LEAVE | (is_client ? 0u : TME_NONCLIENT), target, HOVER_DEFAULT };
		::TrackMouseEvent(&track_info);//Notify when mouse leaves window or client area
	}

	object_state_.moused->query<messaging::target>().dispatch(msg_type(msg_type::value_type{
		static_cast<hwnd_value_type>(object_state_.moused->handle()),
		static_cast<uint_type>((object_state_.moused == &target) ? WINPP_WM_MOUSE : WINPP_WM_RAWMOUSE),
		msg.wparam(),
		msg.code()
	}), true);//Send message

	if (WINPP_IS(object_state_.mouse_state, object_mouse_state::down) && !WINPP_IS(object_state_.mouse_state, object_mouse_state::drag)){//Check for drag
		auto delta = (mouse_position - object_state_.down_position);
		size_type abs_delta{//Absolute values
			(delta.x() < 0) ? -delta.x() : delta.x(),
			(delta.y() < 0) ? -delta.y() : delta.y()
		};

		if ((abs_delta.width() >= drag_threshold.width() || abs_delta.height() >= drag_threshold.height()))
			WINPP_SET(object_state_.mouse_state, object_mouse_state::drag);//Drag begin
	}

	if (WINPP_IS(object_state_.mouse_state, object_mouse_state::drag)){//Do drag
		size_type::value_type delta{ (mouse_position.x() - object_state_.last_position.x()), (mouse_position.y() - object_state_.last_position.y()) };
		object_state_.moused->query<messaging::target>().dispatch(msg_type(msg_type::value_type{
			static_cast<hwnd_value_type>(object_state_.moused->handle()),
			WINPP_WM_MOUSE,
			*reinterpret_cast<wparam_type *>(&delta),
			WINPP_WM_MOUSEDRAG
		}), true);//Send message
	}

	object_state_.last_position = mouse_position;
	return ::CallWindowProcW(target.procedure(), msg.owner(), msg.code(), msg.wparam(), msg.lparam());
}

winpp::application::object_manager::lresult_type winpp::application::object_manager::handle_mouse_hover(window_type &target, const msg_type &msg){
	switch (msg.code()){
	case WM_NCMOUSEHOVER:
		return target.query<messaging::target>().dispatch(msg_type(msg_type::value_type{
			static_cast<hwnd_value_type>(target.handle()),
			WINPP_WM_MOUSE,
			msg.wparam(),
			msg.code()
		}), true);//Send message
	default:
		break;
	}

	return object_state_.moused->query<messaging::target>().dispatch(msg_type(msg_type::value_type{
		static_cast<hwnd_value_type>(object_state_.moused->handle()),
		static_cast<uint_type>((object_state_.moused == &target) ? WINPP_WM_MOUSE : WINPP_WM_RAWMOUSE),
		msg.wparam(),
		msg.code()
	}), true);//Send message
}

winpp::application::object_manager::lresult_type winpp::application::object_manager::handle_mouse_leave(window_type &target, const msg_type &msg){
	auto mouse_position = threading::message_queue::last_mouse_position();
	auto hit_target = target.hit_test(mouse_position);

	if (hit_target == hit_target_type::nil){//Mouse is outside window
		while (object_state_.moused != nullptr){//Send message to chain
			object_state_.moused->query<messaging::target>().dispatch(msg_type(msg_type::value_type{
				static_cast<hwnd_value_type>(object_state_.moused->handle()),
				WINPP_WM_MOUSE,
				0,
				WINPP_WM_MOUSELEAVE
			}), true);//Send mouse leave message

			if (object_state_.moused == &target){//Target reached
				object_state_.moused = object_state_.moused->parent();//Climb hierarchy
				break;
			}
			else//Continue climbing
				object_state_.moused = object_state_.moused->parent();//Climb hierarchy
		}

		while (object_state_.moused != nullptr && object_state_.moused->hit_test(mouse_position) == hit_target_type::nil){//Notify applicable ancestors
			object_state_.moused->query<messaging::target>().dispatch(msg_type(msg_type::value_type{
				static_cast<hwnd_value_type>(object_state_.moused->handle()),
				WINPP_WM_MOUSE,
				0,
				WINPP_WM_MOUSELEAVE
			}), true);//Send mouse leave message
			object_state_.moused = object_state_.moused->parent();//Climb hierarchy
		}

		WINPP_REMOVE(object_state_.mouse_state, object_mouse_state::track);//Stopped tracking
		return 0;
	}

	auto is_client = (msg.code() == WM_MOUSELEAVE);
	if (!is_client || hit_target != hit_target_type::client){//Mouse moved from non-client to client or vice versa
		track_info_type track_info{ sizeof(track_info_type), TME_HOVER | TME_LEAVE | (is_client ? TME_NONCLIENT : 0u), target, HOVER_DEFAULT };
		::TrackMouseEvent(&track_info);//Notify when mouse leaves window or client area
	}

	return 0;
}

winpp::application::object_manager::lresult_type winpp::application::object_manager::handle_mouse_down(window_type &target, const msg_type &msg){
	switch (msg.code()){
	case WM_NCLBUTTONDOWN:
	case WM_NCMBUTTONDOWN:
	case WM_NCRBUTTONDOWN:
		target.query<messaging::target>().dispatch(msg_type(msg_type::value_type{
			static_cast<hwnd_value_type>(target.handle()),
			WINPP_WM_MOUSE,
			msg.wparam(),
			msg.code()
		}), true);//Send message
		return ::CallWindowProcW(target.procedure(), msg.owner(), msg.code(), msg.wparam(), msg.lparam());
	case WM_LBUTTONDOWN:
		WINPP_SET(object_state_.button, mouse_key_state_type::left_button);
		break;
	case WM_MBUTTONDOWN:
		WINPP_SET(object_state_.button, mouse_key_state_type::middle_button);
		break;
	case WM_RBUTTONDOWN:
		WINPP_SET(object_state_.button, mouse_key_state_type::right_button);
		break;
	default:
		break;
	}

	if (!WINPP_IS(object_state_.mouse_state, object_mouse_state::down)){//No button pressed
		object_state_.down_position = threading::message_queue::last_mouse_position();
		WINPP_SET(object_state_.mouse_state, object_mouse_state::down);
	}

	if (object_state_.moused == nullptr)
		object_state_.moused = &target;

	object_state_.moused->query<messaging::target>().dispatch(msg_type(msg_type::value_type{
		static_cast<hwnd_value_type>(object_state_.moused->handle()),
		static_cast<uint_type>((object_state_.moused == &target) ? WINPP_WM_MOUSE : WINPP_WM_RAWMOUSE),
		msg.wparam(),
		msg.code()
	}), true);//Send message

	return ::CallWindowProcW(target.procedure(), msg.owner(), msg.code(), msg.wparam(), msg.lparam());
}

winpp::application::object_manager::lresult_type winpp::application::object_manager::handle_mouse_up(window_type &target, const msg_type &msg){
	switch (msg.code()){
	case WM_NCLBUTTONUP:
		return nc_mouse_up_(target, msg, mouse_key_state_type::left_button);
	case WM_NCMBUTTONUP:
		return nc_mouse_up_(target, msg, mouse_key_state_type::middle_button);
	case WM_NCRBUTTONUP:
		return nc_mouse_up_(target, msg, mouse_key_state_type::right_button);
	case WM_LBUTTONDOWN:
		WINPP_REMOVE(object_state_.button, mouse_key_state_type::left_button);
		break;
	case WM_MBUTTONDOWN:
		WINPP_REMOVE(object_state_.button, mouse_key_state_type::middle_button);
		break;
	case WM_RBUTTONDOWN:
		WINPP_REMOVE(object_state_.button, mouse_key_state_type::right_button);
		break;
	default:
		break;
	}

	if (object_state_.moused == nullptr)
		object_state_.moused = &target;

	if (object_state_.button == mouse_key_state_type::nil){//All buttons are released
		WINPP_REMOVE(object_state_.mouse_state, object_mouse_state::down);
		if (WINPP_IS(object_state_.mouse_state, object_mouse_state::drag)){//End drag
			WINPP_REMOVE(object_state_.mouse_state, object_mouse_state::drag);
			object_state_.moused->query<messaging::target>().dispatch(msg_type(msg_type::value_type{
				static_cast<hwnd_value_type>(object_state_.moused->handle()),
				WINPP_WM_MOUSE,
				0,
				WINPP_WM_MOUSEDRAGEND
			}), true);//Forward message
		}
	}

	object_state_.moused->query<messaging::target>().dispatch(msg_type(msg_type::value_type{
		static_cast<hwnd_value_type>(object_state_.moused->handle()),
		static_cast<uint_type>((object_state_.moused == &target) ? WINPP_WM_MOUSE : WINPP_WM_RAWMOUSE),
		msg.wparam(),
		msg.code()
	}), true);//Send message

	return ::CallWindowProcW(target.procedure(), msg.owner(), msg.code(), msg.wparam(), msg.lparam());
}

winpp::application::object_manager::lresult_type winpp::application::object_manager::handle_mouse_dbl_click(window_type &target, const msg_type &msg){
	switch (msg.code()){
	case WM_NCLBUTTONDBLCLK:
	case WM_NCMBUTTONDBLCLK:
	case WM_NCRBUTTONDBLCLK:
		target.query<messaging::target>().dispatch(msg_type(msg_type::value_type{
			static_cast<hwnd_value_type>(target.handle()),
			WINPP_WM_MOUSE,
			msg.wparam(),
			msg.code()
		}), true);//Send message
		return ::CallWindowProcW(target.procedure(), msg.owner(), msg.code(), msg.wparam(), msg.lparam());
	default:
		break;
	}

	if (object_state_.moused == nullptr)
		object_state_.moused = &target;

	object_state_.moused->query<messaging::target>().dispatch(msg_type(msg_type::value_type{
		static_cast<hwnd_value_type>(object_state_.moused->handle()),
		static_cast<uint_type>((object_state_.moused == &target) ? WINPP_WM_MOUSE : WINPP_WM_RAWMOUSE),
		msg.wparam(),
		msg.code()
	}), true);//Send message

	return ::CallWindowProcW(target.procedure(), msg.owner(), msg.code(), msg.wparam(), msg.lparam());
}

winpp::application::object_manager::lresult_type winpp::application::object_manager::handle_mouse_wheel(window_type &target, const msg_type &msg){
	return object_state_.moused->query<messaging::target>().dispatch(msg_type(msg_type::value_type{
		static_cast<hwnd_value_type>(object_state_.moused->handle()),
		WINPP_WM_MOUSE,
		msg.wparam(),
		msg.code()
	}), true);//Send message
}

winpp::application::object_manager::object_state_type &winpp::application::object_manager::object_state(){
	return object_state_;
}

winpp::application::object_manager::gui_object_type *winpp::application::object_manager::owner(hwnd_type value){
	return value.data<gui_object_type *>();
}

winpp::application::object_manager::uint_type winpp::application::object_manager::register_message(const std::wstring &name){
	return ::RegisterWindowMessageW(name.c_str());
}

winpp::application::object_manager::lresult_type CALLBACK winpp::application::object_manager::entry(hwnd_value_type window_handle, uint_type msg, wparam_type wparam, lparam_type lparam){
	auto &manager = object::current_app->object_manager();
	auto target = manager.find_object_(window_handle);
	if (target == nullptr)//Unidentified handle
		return ::DefWindowProcW(window_handle, msg, wparam, lparam);

	auto &window_target = target->query<window_type>();
	auto result = window_target.dispatch(msg_type({ window_handle, msg, wparam, lparam }), false);

	switch (msg){
	case WM_NCDESTROY:
		window_target.destroyed_();
		break;
	case WM_SETFOCUS://Focus acquired
		manager.object_state_.focused = target;
		break;
	case WM_KILLFOCUS://Focus lost
		manager.object_state_.focused = nullptr;
		break;
	default:
		break;
	}

	return result;
}

winpp::application::object_manager::messaging_map_type winpp::application::object_manager::messaging_map;

winpp::application::object_manager::size_type winpp::application::object_manager::drag_threshold{ ::GetSystemMetrics(SM_CXDRAG), ::GetSystemMetrics(SM_CYDRAG) };

winpp::application::object_manager::gui_object_type * winpp::application::object_manager::find_object_(void *handle){
	if (objects_with_handle_.empty())
		return nullptr;

	if (handle == last_search_.handle)
		return last_search_.owner;

	auto entry = objects_with_handle_.find(handle);
	if (entry == objects_with_handle_.end())
		return nullptr;

	last_search_ = object_map_type{//Cache search
		handle,
		entry->second
	};

	return last_search_.owner;
}

winpp::application::object_manager::lresult_type winpp::application::object_manager::nc_mouse_up_(window_type &target, const msg_type &msg, mouse_key_state_type button){
	if (WINPP_REMOVE(object_state_.button, button) == mouse_key_state_type::nil){//All buttons are released
		WINPP_REMOVE(object_state_.mouse_state, object_mouse_state::down);
		if (WINPP_IS(object_state_.mouse_state, object_mouse_state::drag)){//End drag
			WINPP_REMOVE(object_state_.mouse_state, object_mouse_state::drag);
			object_state_.moused->query<messaging::target>().dispatch(msg_type(msg_type::value_type{
				static_cast<hwnd_value_type>(object_state_.moused->handle()),
				WINPP_WM_MOUSE,
				0,
				WINPP_WM_MOUSEDRAGEND
			}), true);//Forward message
		}
	}

	target.query<messaging::target>().dispatch(msg_type(msg_type::value_type{
		static_cast<hwnd_value_type>(target.handle()),
		WINPP_WM_MOUSE,
		msg.wparam(),
		msg.code()
	}), true);//Send message

	return ::CallWindowProcW(target.procedure(), msg.owner(), msg.code(), msg.wparam(), msg.lparam());
}

winpp::application::object_manager::gui_object_type *winpp::application::object_manager::find_window_in_chain_(gui_object_type *target){
	while (target != nullptr){
		if (!target->is_window())
			target = target->parent();//Climb
		else//Window found
			break;
	}

	return target;
}

winpp::application::object_manager::lresult_type CALLBACK winpp::application::object_manager::hook_(int code, wparam_type wparam, lparam_type lparam){
	if (code == HCBT_CREATEWND){//Respond to window creation
		if (object::current_app == nullptr)//App not initialized
			return ::CallNextHookEx(nullptr, code, wparam, lparam);

		auto &manager = object::current_app->object_manager();
		if (manager.recent_params_ == nullptr)//Not monitoring event
			return ::CallNextHookEx(nullptr, code, wparam, lparam);

		auto create_info = reinterpret_cast<create_hook_info_type *>(lparam)->lpcs;
		if (create_info->lpCreateParams == manager.recent_params_){//Ensure target is valid
			auto target_hwnd_value = reinterpret_cast<hwnd_value_type>(wparam);
			if (manager.replace_procedure_)//Replace window procedure
				hwnd_type(target_hwnd_value).data<procedure_type>(&object_manager::entry, hwnd_type::data_index_type::procedure);

			*manager.out_ = target_hwnd_value;
			manager.last_search_ = object_map_type{//Cache search
				target_hwnd_value,
				(manager.objects_with_handle_[target_hwnd_value] = static_cast<gui_object_type *>(manager.recent_params_))
			};

			manager.out_ = nullptr;
			manager.recent_params_ = nullptr;
		}
	}

	return ::CallNextHookEx(nullptr, code, wparam, lparam);
}

winpp::application::classes winpp::application::object_manager::classes_;
