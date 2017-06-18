#include "msg_wrapper.h"

winpp::wrappers::msg::msg()
	: base_type(value_type{}){}

winpp::wrappers::msg::msg(const value_type &value)
	: base_type(value){}

bool winpp::wrappers::msg::translate() const{
	return (::TranslateMessage(&value_) != FALSE);
}

winpp::wrappers::msg::result_type winpp::wrappers::msg::dispatch() const{
	return (::DispatchMessageW(&value_) != FALSE);
}

winpp::wrappers::msg::result_type winpp::wrappers::msg::send() const{
	return ::SendMessageW(value_.hwnd, value_.message, value_.wParam, value_.lParam);
}

winpp::wrappers::msg &winpp::wrappers::msg::code(uint_type value){
	value_.message = value;
	return *this;
}

winpp::wrappers::msg::uint_type winpp::wrappers::msg::code() const{
	return value_.message;
}

winpp::wrappers::msg &winpp::wrappers::msg::owner(hwnd_type value){
	value_.hwnd = value;
	return *this;
}

winpp::wrappers::msg::hwnd_type winpp::wrappers::msg::owner() const{
	return value_.hwnd;
}

winpp::wrappers::msg &winpp::wrappers::msg::time(dword_type value){
	value_.time = value;
	return *this;
}

winpp::wrappers::msg::dword_type winpp::wrappers::msg::time() const{
	return value_.time;
}

winpp::wrappers::msg &winpp::wrappers::msg::mouse_position(const structures::point &value){
	value_.pt = value;
	return *this;
}

winpp::structures::point winpp::wrappers::msg::mouse_position() const{
	return value_.pt;
}

bool winpp::wrappers::msg::is_quit() const{
	return (value_.message == WM_QUIT);
}

bool winpp::wrappers::msg::is_dialog(hwnd_type target){
	return (::IsDialogMessageW(target, &value_) != FALSE);
}
