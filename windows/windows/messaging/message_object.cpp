#include "message_object.h"
#include "../application/object_manager.h"

winpp::messaging::object::object(const msg_type &info, bool is_sent, procedure_type default_callback, gui_object_type *target)
	: info_(info), default_callback_(default_callback), target_(target), states_(is_sent ? state_type::sent : state_type::nil){
	if (threading::message_queue::is_sent())
		WINPP_SET(states_, state_type::sent | state_type::cross);

	if (default_callback_ == nullptr)
		WINPP_SET(states_, state_type::skipped);
}

winpp::messaging::object::~object(){
	handle(false);
}

winpp::messaging::object &winpp::messaging::object::modify(const msg_type &value){
	info_ = value;
	WINPP_SET(states_, state_type::modified);
	return *this;
}

winpp::messaging::object &winpp::messaging::object::handle(bool overwrite){
	if (!WINPP_IS_ANY(states_, state_type::done | state_type::skipped)){
		WINPP_SET(states_, state_type::done);
		if (default_callback_ != nullptr){//Call procedure
			if (overwrite || !WINPP_IS(states_, state_type::has_value)){
				WINPP_SET(states_, state_type::has_value);
				value_ = ::CallWindowProcW(default_callback_, info_.owner(), info_.code(), info_.wparam(), info_.lparam());
			}
			else//Ignore return
				::CallWindowProcW(default_callback_, info_.owner(), info_.code(), info_.wparam(), info_.lparam());
		}
	}

	return *this;
}

winpp::messaging::object &winpp::messaging::object::skip(){
	if (!WINPP_IS_ANY(states_, state_type::done | state_type::skipped))
		WINPP_SET(states_, state_type::skipped);
	return *this;
}

winpp::messaging::object &winpp::messaging::object::reply(){
	return WINPP_IS(states_, state_type::has_value) ? reply(value_) : *this;
}

winpp::messaging::object &winpp::messaging::object::reply(bool value){
	return reply(value ? TRUE : FALSE);
}

winpp::messaging::object &winpp::messaging::object::value(bool value, bool overwrite){
	return this->value(value ? TRUE : FALSE, overwrite);
}

winpp::messaging::object::lresult_type winpp::messaging::object::result() const{
	return value_;
}

bool winpp::messaging::object::is_sent() const{
	return WINPP_IS(states_, state_type::sent);
}

bool winpp::messaging::object::is_cross_thread() const{
	return WINPP_IS(states_, state_type::cross);
}

bool winpp::messaging::object::is_replied() const{
	return WINPP_IS(states_, state_type::replied);
}

bool winpp::messaging::object::has_value() const{
	return WINPP_IS(states_, state_type::has_value);
}

bool winpp::messaging::object::is_default() const{
	return WINPP_IS(states_, state_type::done);
}

bool winpp::messaging::object::is_skipped() const{
	return WINPP_IS(states_, state_type::skipped);
}

bool winpp::messaging::object::is_handled() const{
	return WINPP_IS_ANY(states_, state_type::skipped | state_type::done | state_type::replied | state_type::has_value);
}

bool winpp::messaging::object::is_modified() const{
	return WINPP_IS(states_, state_type::modified);
}

const winpp::messaging::object::msg_type &winpp::messaging::object::info() const{
	return info_;
}

winpp::messaging::object::gui_object_type *winpp::messaging::object::target() const{
	return application::object::current_app->object_manager().find_window(info_.owner());
}

winpp::messaging::mouse_activate::~mouse_activate() = default;

winpp::messaging::mouse_activate::hit_target_type winpp::messaging::mouse_activate::hit_target() const{
	return static_cast<hit_target_type>(info_.low_lparam<int>());
}

winpp::messaging::mouse_activate::uint_type winpp::messaging::mouse_activate::mouse_message() const{
	return info_.high_lparam<uint_type>();
}

winpp::messaging::ncactivate::~ncactivate() = default;

bool winpp::messaging::ncactivate::is_activating() const{
	return (info_.wparam<::BOOL>() != FALSE);
}

winpp::messaging::activate::~activate() = default;

bool winpp::messaging::activate::is_activated() const{
	return (info_.wparam() != WA_INACTIVE);
}

bool winpp::messaging::activate::click_activated() const{
	return (info_.wparam() == WA_CLICKACTIVE);
}

winpp::messaging::object::hwnd_type winpp::messaging::activate::other_window() const{
	return info_.lparam<hwnd_value_type>();
}

winpp::messaging::focus_change::~focus_change() = default;

bool winpp::messaging::focus_change::is_focused() const{
	return (info_.code() == WM_SETFOCUS);
}

winpp::messaging::enable::~enable() = default;

bool winpp::messaging::enable::is_enabled() const{
	return (info_.wparam<::BOOL>() != FALSE);
}

winpp::messaging::hit_test::~hit_test() = default;

winpp::messaging::hit_test::point_type winpp::messaging::hit_test::mouse_position() const{
	return WINPP_MAKE_MOUSE_POSITION(info_.lparam());
}

winpp::messaging::position::~position() = default;

bool winpp::messaging::position::is_changing() const{
	return (info_.code() == WM_WINDOWPOSCHANGING);
}

winpp::messaging::position::point_type winpp::messaging::position::offset() const{
	auto info = info_.lparam<window_pos_type *>();
	return point_type{ info->x, info->y };
}

winpp::messaging::position::size_type winpp::messaging::position::size() const{
	auto info = info_.lparam<window_pos_type *>();
	return size_type{ info->cx, info->cy };
}

winpp::messaging::position::position_type winpp::messaging::position::flags() const{
	return static_cast<position_type>(info_.lparam<window_pos_type *>()->flags);
}

winpp::messaging::size::~size() = default;

bool winpp::messaging::size::is_changing() const{
	return (info_.code() == WM_SIZING);
}

winpp::messaging::size::window_edge_type winpp::messaging::size::edge() const{
	return (is_changing() ? static_cast<window_edge_type>(info_.wparam<int>()) : window_edge_type::nil);
}

winpp::messaging::size::window_size_type winpp::messaging::size::reason() const{
	return (is_changing() ? window_size_type::restored : static_cast<window_size_type>(info_.wparam<int>()));
}

winpp::messaging::move::~move() = default;

bool winpp::messaging::move::is_changing() const{
	return (info_.code() == WM_MOVING);
}

winpp::messaging::get_min_max_info::~get_min_max_info() = default;

const winpp::messaging::get_min_max_info::info_type &winpp::messaging::get_min_max_info::info() const{
	return *info_.lparam<info_type *>();
}

winpp::messaging::style::~style(){
	info_.lparam<info_type *>()->styleNew = filtered();//Use filtered
}

bool winpp::messaging::style::is_changing() const{
	return (info_.code() == WM_STYLECHANGING);
}

bool winpp::messaging::style::is_extended() const{
	return (static_cast<data_index_type>(info_.wparam<int>()) == data_index_type::extended_styles);
}

winpp::messaging::style::dword_type winpp::messaging::style::previous() const{
	return info_.lparam<info_type *>()->styleOld;
}

winpp::messaging::style::dword_type winpp::messaging::style::current() const{
	return info_.lparam<info_type *>()->styleNew;
}

winpp::messaging::style::dword_type winpp::messaging::style::filtered() const{
	auto info = info_.lparam<info_type *>();
	if (info->styleNew == info->styleOld)//No filtered
		return info->styleNew;

	auto &target = this->target()->query<window::object>();
	auto is_extended = this->is_extended();

	auto styles_removed = target.filter_styles(info->styleOld & ~info->styleNew, is_extended);
	auto styles_added = target.filter_styles(~info->styleOld & info->styleNew, is_extended);

	return WINPP_REMOVE_V(WINPP_SET_V(info->styleOld, styles_added), styles_removed);
}

winpp::messaging::show::~show() = default;

bool winpp::messaging::show::is_hidden() const{
	return (info_.wparam<bool_type>() == FALSE);
}

winpp::messaging::show::show_reason_type winpp::messaging::show::reason() const{
	return static_cast<show_reason_type>(info_.lparam<int>());
}

winpp::messaging::draw::~draw(){
	if (drawer_ != nullptr){
		application::object::current_app->drawing_result((*drawer_)->EndDraw());
		drawer_ = nullptr;
		switch (info_.code()){
		case WM_NCPAINT:
			::ReleaseDC(info_.owner(), dc_);
			break;
		case WM_PAINT:
			::EndPaint(info_.owner(), &paint_struct_);
			break;
		default:
			break;
		}
	}
}

winpp::messaging::draw::drawer_type &winpp::messaging::draw::drawer(){
	if (drawer_ != nullptr)
		return *drawer_;//Use already existing drawer

	switch (info_.code()){
	case WM_NCPAINT:
		::GetClipBox(dc_ = GetDCEx(info_.owner(), info_.wparam<hrgn_type>(), DCX_WINDOW | DCX_INTERSECTRGN), clip_);
		break;
	case WM_PAINT:
		::BeginPaint(info_.owner(), &paint_struct_);
		dc_ = paint_struct_.hdc;
		clip_ = paint_struct_.rcPaint;
		break;
	default:
		break;
	}

	drawer_ = &application::object::current_app->drawer();
	(*drawer_)->BindDC(dc_, clip_);
	(*drawer_)->BeginDraw();
	(*drawer_)->SetTransform(D2D1::Matrix3x2F::Identity());

	return *drawer_;
}

winpp::messaging::draw::hdc_type winpp::messaging::draw::dc() const{
	return dc_;
}

winpp::messaging::draw::rect_type winpp::messaging::draw::clip() const{
	return clip_;
}

bool winpp::messaging::draw::erase_background() const{
	return (drawer_ != nullptr && info_.code() == WM_PAINT) ? (paint_struct_.fErase != FALSE) : false;
}

winpp::messaging::draw::print_option winpp::messaging::draw::print_options() const{
	switch (info_.code()){
	case WM_PRINT:
	case WM_PRINTCLIENT:
		return static_cast<print_option>(info_.lparam<unsigned int>());
	default:
		break;
	}

	return print_option::nil;
}

winpp::messaging::mouse::~mouse() = default;

bool winpp::messaging::mouse::is_client() const{
	switch (info_.code()){
	case WM_NCMOUSEMOVE:
	case WM_NCMOUSEHOVER:
	case WM_NCMOUSELEAVE:
	case WM_NCLBUTTONDOWN:
	case WM_NCLBUTTONUP:
	case WM_NCLBUTTONDBLCLK:
	case WM_NCMBUTTONDOWN:
	case WM_NCMBUTTONUP:
	case WM_NCMBUTTONDBLCLK:
	case WM_NCRBUTTONDOWN:
	case WM_NCRBUTTONUP:
	case WM_NCRBUTTONDBLCLK:
	case WM_NCXBUTTONDOWN:
	case WM_NCXBUTTONUP:
	case WM_NCXBUTTONDBLCLK:
		return false;
	default:
		break;
	}

	return true;
}

bool winpp::messaging::mouse::is_vertical_wheel() const{
	return (info_.code() == WM_MOUSEWHEEL);
}

winpp::messaging::mouse::mouse_key_state_type winpp::messaging::mouse::button() const{
	switch (info_.code()){
	case WM_NCLBUTTONDOWN:
	case WM_LBUTTONDOWN:
	case WM_NCLBUTTONUP:
	case WM_LBUTTONUP:
	case WM_NCLBUTTONDBLCLK:
	case WM_LBUTTONDBLCLK:
		return mouse_key_state_type::left_button;
	case WM_NCMBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_NCMBUTTONUP:
	case WM_MBUTTONUP:
	case WM_NCMBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
		return mouse_key_state_type::middle_button;
	case WM_NCRBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_NCRBUTTONUP:
	case WM_RBUTTONUP:
	case WM_NCRBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
		return mouse_key_state_type::right_button;
	default:
		break;
	}

	return mouse_key_state_type::nil;
}

winpp::messaging::mouse::hit_target_type winpp::messaging::mouse::hit_target() const{
	return (is_client() ? hit_target_type::client : static_cast<hit_target_type>(info_.wparam<int>()));
}

winpp::messaging::mouse::point_type winpp::messaging::mouse::position() const{
	return threading::message_queue::last_mouse_position();
}

short winpp::messaging::mouse::wheel_delta() const{
	switch (info_.code()){
	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL:
		return static_cast<short>(info_.high_wparam<int>() / WHEEL_DELTA);
	default:
		break;
	}

	return 0;
}

winpp::messaging::mouse::size_type winpp::messaging::mouse::drag_delta() const{
	return *info_.wparam<size_type::value_type *>();
}

winpp::messaging::object::gui_object_type *winpp::messaging::mouse::original_target() const{
	return original_target_;
}

winpp::messaging::key::~key() = default;

bool winpp::messaging::key::is_system() const{
	return WINPP_IS(states_, state_type::is_system);
}

bool winpp::messaging::key::is_extended() const{
	return WINPP_IS(states_, state_type::is_extended);
}

bool winpp::messaging::key::was_down() const{
	return WINPP_IS(states_, state_type::was_down);
}

bool winpp::messaging::key::is_being_released() const{
	return WINPP_IS(states_, state_type::being_released);
}

bool winpp::messaging::key::alt_key_down() const{
	return WINPP_IS(states_, state_type::alt_down);
}

bool winpp::messaging::key::is_down() const{
	return WINPP_IS(states_, state_type::down);
}

bool winpp::messaging::key::is_up() const{
	return WINPP_IS(states_, state_type::up);
}

bool winpp::messaging::key::is_char() const{
	return WINPP_IS(states_, state_type::pressed);
}

bool winpp::messaging::key::is_dead() const{
	return WINPP_IS(states_, state_type::dead);
}

unsigned short winpp::messaging::key::code() const{
	return code_;
}

short winpp::messaging::key::scan_code() const{
	return scan_code_;
}

short winpp::messaging::key::repeat_count() const{
	return repeat_count_;
}

winpp::messaging::key::key_state_type winpp::messaging::key::key_states() const{
	return retrieve_key_states();
}

winpp::messaging::key::state_type winpp::messaging::key::states() const{
	return states_;
}

winpp::messaging::object::gui_object_type *winpp::messaging::key::original_target() const{
	return original_target_;
}

bool winpp::messaging::key::cache_key_states(){
	return (::GetKeyboardState(keyboard_state) != FALSE);
}

winpp::messaging::key::key_state_type winpp::messaging::key::retrieve_key_states(){
	static const key_map_info_list_type key_map_info_list{
		key_map_info{ VK_LSHIFT, key_state_type::left_shift, false },
		key_map_info{ VK_RSHIFT, key_state_type::right_shift, false },
		key_map_info{ VK_LCONTROL, key_state_type::left_ctrl, false },
		key_map_info{ VK_RCONTROL, key_state_type::right_ctrl, false },
		key_map_info{ VK_LMENU, key_state_type::left_alt, false },
		key_map_info{ VK_RMENU, key_state_type::right_alt, false },
		key_map_info{ VK_LWIN, key_state_type::left_win, false },
		key_map_info{ VK_RWIN, key_state_type::right_win, false },
		key_map_info{ VK_CAPITAL, key_state_type::caps, true },
		key_map_info{ VK_NUMLOCK, key_state_type::numpad, true },
		key_map_info{ VK_SCROLL, key_state_type::scroll, true },
		key_map_info{ VK_INSERT, key_state_type::insert, true },
	};

	if (!cache_key_states())
		return key_state_type::nil;

	auto states = key_state_type::nil;
	for (auto &info : key_map_info_list){
		if (info.toggle){
			if ((keyboard_state[info.key] & 1) != 0)
				WINPP_SET(states, info.value);
		}
		else if (keyboard_state[info.key] < 0)
			WINPP_SET(states, info.value);
	}

	return states;
}

void winpp::messaging::key::resolve_(){
	switch (info_.code()){
	case WM_KEYDOWN:
		states_ = state_type::down;
		break;
	case WM_SYSKEYDOWN:
		states_ = (state_type::is_system | state_type::down);
		break;
	case WM_KEYUP:
		states_ = state_type::up;
		break;
	case WM_SYSKEYUP:
		states_ = (state_type::is_system | state_type::up);
		break;
	case WM_CHAR:
		states_ = state_type::pressed;
		break;
	case WM_SYSCHAR:
		states_ = (state_type::is_system | state_type::pressed);
		break;
	case WM_DEADCHAR:
		states_ = (state_type::dead | state_type::pressed);
		break;
	case WM_SYSDEADCHAR:
		states_ = (state_type::is_system | state_type::dead | state_type::pressed);
		break;
	default: 
		states_ = state_type::nil;
		break;
	}

	auto lp_value = info_.lparam();
	std::bitset<sizeof(lresult_type) * 8> lparam_bits(lp_value);

	code_ = info_.wparam<unsigned short>();
	repeat_count_ = *reinterpret_cast<short *>(&lp_value);
	scan_code_ = static_cast<short>((reinterpret_cast<byte_type *>(&lp_value))[2]);

	if (lparam_bits.test(24))//Extended
		WINPP_SET(states_, state_type::is_extended);

	if (lparam_bits.test(29))//Alt
		WINPP_SET(states_, state_type::alt_down);

	if (lparam_bits.test(30))//Was down
		WINPP_SET(states_, state_type::was_down);

	if (lparam_bits.test(31))//Being released
		WINPP_SET(states_, state_type::being_released);
}

winpp::messaging::key::byte_type winpp::messaging::key::keyboard_state[0x100];
