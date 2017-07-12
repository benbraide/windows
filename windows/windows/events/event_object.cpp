#include "event_object.h"

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
