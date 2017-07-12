#include "message_object.h"

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
	return hwnd_type(info_.owner()).data<gui_object_type *>();
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
