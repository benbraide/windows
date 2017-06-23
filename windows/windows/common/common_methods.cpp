#include "common_methods.h"
#include "../application/application_object.h"

winpp::common::methods::application_type *winpp::common::methods::get_app(){
	return (application_type::current_app == nullptr) ? application_type::main_app : application_type::current_app;
}

winpp::common::methods::hwnd_type winpp::common::methods::create_window(const create_info_type &info, window_type &owner, application_type *&app){
	if (app == nullptr && (app = get_app()) == nullptr)
		throw common::no_app_exception();

	hwnd_type value = nullptr;
	if (app == application_type::current_app){
		value = ::CreateWindowExW(
			info.dwExStyle,
			info.lpszClass,
			info.lpszName,
			static_cast<dword_type>(info.style),
			info.x,
			info.y,
			info.cx,
			info.cy,
			info.hwndParent,
			info.hMenu,
			info.hInstance,
			info.lpCreateParams
		);
	}
	else{//Add to thread queue
		value = app->execute_task<hwnd_type>([&info]{
			return ::CreateWindowExW(
				info.dwExStyle,
				info.lpszClass,
				info.lpszName,
				static_cast<dword_type>(info.style),
				info.x,
				info.y,
				info.cx,
				info.cy,
				info.hwndParent,
				info.hMenu,
				info.hInstance,
				info.lpCreateParams
			);
		});
	}

	return value;
}

std::wstring winpp::common::methods::convert_string(const std::string &value){
	if (value.empty())
		return L"";

	std::wstring converted_value(value.size(), L' ');
	for (std::size_t i = 0; i < value.size(); ++i)
		converted_value[i] = static_cast<wchar_t>(value[i]);

	return converted_value;
}

void winpp::common::methods::to_upper(std::wstring &value){
	format_string(value, [](wchar_t &c) -> bool{
		c = ::towupper(c);
		return true;
	});
}

void winpp::common::methods::to_upper(std::string &value){
	format_string(value, [](char &c) -> bool{
		c = ::toupper(c);
		return true;
	});
}

void winpp::common::methods::to_lower(std::wstring &value){
	format_string(value, [](wchar_t &c) -> bool{
		c = ::towlower(c);
		return true;
	});
}

void winpp::common::methods::to_lower(std::string &value){
	format_string(value, [](char &c) -> bool{
		c = ::tolower(c);
		return true;
	});
}

bool winpp::common::methods::has_left(placement_type placement){
	return (placement == placement_type::left || placement == placement_type::top_left || placement == placement_type::bottom_left || placement == placement_type::center_left);
}

bool winpp::common::methods::has_top(placement_type placement){
	return (placement == placement_type::top || placement == placement_type::top_left || placement == placement_type::top_right || placement == placement_type::top_center);
}

bool winpp::common::methods::has_right(placement_type placement){
	return (placement == placement_type::right || placement == placement_type::top_right || placement == placement_type::bottom_right || placement == placement_type::center_right);
}

bool winpp::common::methods::has_bottom(placement_type placement){
	return (placement == placement_type::bottom || placement == placement_type::bottom_left || placement == placement_type::bottom_right || placement == placement_type::bottom_center);
}

bool winpp::common::methods::has_horizontal_center(placement_type placement){
	return (placement == placement_type::center || placement == placement_type::top_center || placement == placement_type::bottom_center);
}

bool winpp::common::methods::has_vertical_center(placement_type placement){
	return (placement == placement_type::center || placement == placement_type::center_left || placement == placement_type::center_right);
}

void winpp::common::methods::pixel_scale(d2d_pointf_type &dpi){
	dpi.x /= 96.0f;
	dpi.y /= 96.0f;
}
