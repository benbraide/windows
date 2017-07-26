#include "menu_newline.h"

winpp::menu::newline::newline(){}

winpp::menu::newline::newline(gui_object_type &parent){
	create(parent);
}

winpp::menu::newline::~newline(){
	destroy(force_type::force);
}

winpp::menu::newline &winpp::menu::newline::update_state(){
	base_type::update_state();
	if (!children_.empty())//Update item at front of list
		(*children_.begin())->update_state();

	return *this;
}

bool winpp::menu::newline::is_group() const{
	return false;
}

bool winpp::menu::newline::pre_insert_(gui_object_type &object, index_and_size_type &index){
	return gui_group_type::pre_insert_(object, index);
}

winpp::menu::bordered_newline::bordered_newline(){}

winpp::menu::bordered_newline::bordered_newline(gui_object_type &parent){
	create(parent);
}

winpp::menu::bordered_newline::~bordered_newline(){
	destroy(force_type::force);
}
