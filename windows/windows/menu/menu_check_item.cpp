#include "menu_check_item.h"

winpp::menu::check_item::check_item(){}

winpp::menu::check_item::check_item(gui_object_type &parent, const std::wstring &label, menu_state_type states){
	create(parent, label, states);
}

winpp::menu::check_item::~check_item(){
	destroy(force_type::force);
}

winpp::gui::object::index_and_size_type winpp::menu::check_item::internal_insert_child(gui_object_type &child, index_and_size_type before){
	throw common::unsupported_exception();
}

winpp::menu::item &winpp::menu::check_item::internal_remove_child(gui_object_type &child, force_type force){
	throw common::unsupported_exception();
}
