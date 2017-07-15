#include "event_object.h"
#include "../application/object_manager.h"

winpp::events::object::object(gui_object_type &target)
	: target_(&target), states_(state_type::nil){}

winpp::events::object::~object() = default;

winpp::events::object &winpp::events::object::prevent(){
	WINPP_SET(states_, state_type::prevented);
	return *this;
}

winpp::events::object &winpp::events::object::stop_propagation(){
	WINPP_SET(states_, state_type::stopped);
	return *this;
}

bool winpp::events::object::is_prevented() const{
	return WINPP_IS(states_, state_type::prevented);
}

bool winpp::events::object::is_propagating() const{
	return WINPP_IS(states_, state_type::stopped);
}

winpp::events::object::gui_object_type &winpp::events::object::target() const{
	return *target_;
}

winpp::events::dispatch_message::dispatch_message(gui_object_type &target, const msg_type &msg, bool is_sent)
	: object(target), msg_(msg), is_sent_(is_sent){}

const winpp::events::dispatch_message::msg_type &winpp::events::dispatch_message::msg() const{
	return msg_;
}

bool winpp::events::dispatch_message::is_sent() const{
	return is_sent_;
}

winpp::events::mouse_activate::mouse_activate(gui_object_type &target, lparam_type lparam)
	: object(target), lparam_(lparam){}

winpp::events::mouse_activate::hit_target_type winpp::events::mouse_activate::hit_target() const{
	return static_cast<hit_target_type>(LOWORD(lparam_));
}

winpp::events::mouse_activate::uint_type winpp::events::mouse_activate::mouse_message() const{
	return HIWORD(lparam_);
}

winpp::events::pre_activate::pre_activate(gui_object_type &target, bool activating)
	: object(target), activating_(activating){}

bool winpp::events::pre_activate::is_activating() const{
	return activating_;
}

winpp::events::activate::activate(gui_object_type &target, state state, hwnd_type other_window)
	: object(target), state_(state), other_window_(other_window){}

bool winpp::events::activate::is_activated() const{
	return (state_ != state::inactive);
}

bool winpp::events::activate::click_activated() const{
	return (state_ == state::click_active);
}

winpp::events::activate::hwnd_type winpp::events::activate::other_window() const{
	return other_window_;
}

winpp::events::focus_change::focus_change(gui_object_type &target, bool focused)
	: object(target), focused_(focused){}

bool winpp::events::focus_change::is_focused() const{
	return focused_;
}

winpp::events::enable::enable(gui_object_type &target, bool enabled)
	: object(target), enabled_(enabled){}

bool winpp::events::enable::is_enabled() const{
	return enabled_;
}

winpp::events::position::position(gui_object_type &target, bool changing, window_pos_type &info)
	: object(target), changing_(changing), info_(&info){}

winpp::events::position::~position() = default;

bool winpp::events::position::is_changing() const{
	return changing_;
}

winpp::events::position::point_type winpp::events::position::offset() const{
	return point_type{ info_->x, info_->y };
}

winpp::events::position::size_type winpp::events::position::size() const{
	return size_type{ info_->cx, info_->cy };
}

winpp::events::position::position_type winpp::events::position::flags() const{
	return static_cast<position_type>(info_->flags);
}

winpp::events::size::size(gui_object_type &target, bool changing, int value)
	: object(target), changing_(changing), value_(value){}

winpp::events::size::~size() = default;

bool winpp::events::size::is_changing() const{
	return changing_;
}

winpp::events::size::window_edge_type winpp::events::size::edge() const{
	return (is_changing() ? static_cast<window_edge_type>(value_) : window_edge_type::nil);
}

winpp::events::size::window_size_type winpp::events::size::reason() const{
	return (is_changing() ? window_size_type::restored : static_cast<window_size_type>(value_));
}

winpp::events::move::move(gui_object_type &target, bool changing)
	: object(target), changing_(changing){}

winpp::events::move::~move() = default;

bool winpp::events::move::is_changing() const{
	return changing_;
}

winpp::events::hit_test::hit_test(gui_object_type &target, const point_type &position)
	: object(target), position_(position){}

winpp::events::hit_test::~hit_test() = default;

const winpp::events::hit_test::point_type &winpp::events::hit_test::mouse_position() const{
	return position_;
}

winpp::events::mouse::mouse(gui_object_type &target, uint_type code, wparam_type wparam, gui_object_type *original_target)
	: object(target), code_(code), wparam_(wparam), original_target_(original_target){}

winpp::events::mouse::~mouse() = default;

bool winpp::events::mouse::is_client() const{
	switch (code_){
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

bool winpp::events::mouse::is_vertical_wheel() const{
	return (code_ == WM_MOUSEWHEEL);
}

winpp::events::mouse::mouse_key_state_type winpp::events::mouse::button() const{
	switch (code_){
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

winpp::events::mouse::hit_target_type winpp::events::mouse::hit_target() const{
	return (is_client() ? hit_target_type::client : static_cast<hit_target_type>(wparam_));
}

winpp::events::mouse::point_type winpp::events::mouse::position() const{
	return threading::message_queue::last_mouse_position();
}

short winpp::events::mouse::wheel_delta() const{
	switch (code_){
	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL:
		return static_cast<short>(HIWORD(wparam_) / WHEEL_DELTA);
	default:
		break;
	}

	return 0;
}

winpp::events::mouse::size_type winpp::events::mouse::drag_delta() const{
	return *reinterpret_cast<size_type::value_type *>(wparam_);
}

winpp::events::object::gui_object_type *winpp::events::mouse::original_target() const{
	return original_target_;
}
