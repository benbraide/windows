#include "window/frame_window.h"

int WINAPI wWinMain(::HINSTANCE app_instance, ::HINSTANCE, ::LPWSTR cmd_line, int show_type){
	typedef winpp::structures::point point_type;
	typedef winpp::structures::size size_type;
	typedef winpp::structures::rect rect_type;

	winpp::window::framed_top_level ftl(L"Framed Top-Level Window", point_type{ 10 }, size_type{ 600, 400 });

	winpp::application::object::get().run();
	winpp::application::object::get().exit();

	return 0;
}
