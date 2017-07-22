#include "menu_group.h"

winpp::menu::group::group(){}

winpp::menu::group::group(gui_object_type &parent){
	create(parent);
}

winpp::menu::group::~group(){
	destroy(force_type::force);
}

void *winpp::menu::group::handle() const{
	if (parent_ == nullptr)
		throw common::unsupported_exception();
	return parent_->handle();
}

winpp::menu::group &winpp::menu::group::destroy(force_type force){
	if (is_created()){
		parent_->internal_remove_child(*this);
		parent_ = nullptr;
	}

	return *this;
}

unsigned int winpp::menu::group::object_group() const{
	return menu_group;
}

bool winpp::menu::group::is_created() const{
	return (parent_ != nullptr);
}

bool winpp::menu::group::is_menu_group() const{
	return true;
}

void winpp::menu::group::create(gui_object_type &parent){
	if (is_created())
		throw common::unsupported_exception();
	internal_insert_into_parent(parent);
}
