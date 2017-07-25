#include "menu_separator.h"

winpp::menu::separator::separator(){}

winpp::menu::separator::separator(gui_object_type &parent){
	create(parent);
}

winpp::menu::separator::~separator(){
	destroy(force_type::force);
}

winpp::gui::object::index_and_size_type winpp::menu::separator::internal_insert_child(gui_object_type &child, index_and_size_type before){
	throw common::unsupported_exception();
}

winpp::menu::item &winpp::menu::separator::internal_remove_child(gui_object_type &child, force_type force){
	throw common::unsupported_exception();
}

void winpp::menu::separator::create(gui_object_type &parent, const std::wstring &label, menu_state_type states){
	create(parent);
}

void winpp::menu::separator::create(gui_object_type &parent){
	create_(parent, L"", menu_state_type::nil);
}

winpp::menu::separator &winpp::menu::separator::states(menu_state_type value){
	throw common::unsupported_exception();
}

bool winpp::menu::separator::is_action() const{
	return false;
}

bool winpp::menu::separator::is_separator() const{
	return true;
}

winpp::menu::item::uint_type winpp::menu::separator::get_types_() const{
	return (item::get_types_() | MFT_SEPARATOR);
}
