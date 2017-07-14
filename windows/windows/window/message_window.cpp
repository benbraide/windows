#include "message_window.h"

winpp::window::message::message(){
	reset_persistent_styles_();
}

winpp::window::message::message(const std::wstring &caption){
	reset_persistent_styles_();
	create(caption);
}

winpp::window::message::message(app_type &app, const std::wstring &caption){
	reset_persistent_styles_();
	create(app, caption);
}

winpp::window::message::~message(){
	destroy();
}

winpp::window::object::dword_type winpp::window::message::black_listed_styles(bool is_extended) const{
	return ~0ul;//Blacklist all styles
}

void winpp::window::message::create(const std::wstring &caption){
	create_(caption, nullptr);
}

void winpp::window::message::create(app_type &app, const std::wstring &caption){
	create_(caption, &app);
}

void winpp::window::message::create_(const std::wstring &caption, app_type *app){
	object::create_(create_info_type{
		static_cast<gui_object_type *>(this),		//Params
		nullptr,									//Instance
		nullptr,									//Menu
		HWND_MESSAGE,								//Parent
		0,											//Height
		0,											//Width
		0,											//y-offset
		0,											//x-offset
		0,											//Styles
		caption.c_str(),							//Window name
		nullptr,									//Class name
		0											//Extended styles
	}, app);
}
