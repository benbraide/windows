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

winpp::gui::object::index_and_size_type winpp::menu::newline::post_insert_(gui_object_type &object, index_and_size_type index){
	index = base_type::post_insert_(object, index);
	if (index == 0u){
		if (children_.size() > 1u)//Update previous item at front of list
			(*std::next(children_.begin()))->update_state();
		(*children_.begin())->update_state();//Update item at front of list
	}

	return index;
}

void winpp::menu::newline::post_remove_(gui_object_type &object){
	base_type::pre_remove_(object);
	if (!children_.empty())//Update item at front of list
		(*children_.begin())->update_state();
}

winpp::menu::bordered_newline::bordered_newline(){}

winpp::menu::bordered_newline::bordered_newline(gui_object_type &parent){
	create(parent);
}

winpp::menu::bordered_newline::~bordered_newline(){
	destroy(force_type::force);
}
