#include "menu_group.h"
#include "menu_object.h"

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

winpp::gui::object::index_and_size_type winpp::menu::group::insert_into_parent_(gui_object_type &parent, index_and_size_type index){
	if (!parent.is_type<menu::object>() && !parent.is_type<newline>())
		throw common::invalid_arg_exception();
	return base_type::insert_into_parent_(parent, index);
}

bool winpp::menu::group::pre_insert_(gui_object_type &object, index_and_size_type &index){
	if (!object.is_type<radio_item>())
		throw common::invalid_arg_exception();
	return base_type::pre_insert_(object, index);
}

winpp::gui::object::index_and_size_type winpp::menu::group::post_insert_(gui_object_type &object, index_and_size_type index){
	index = base_type::post_insert_(object, index);
	if (index == 0u){
		if (children_.size() > 1u)//Update previous item at front of list
			(*std::next(children_.begin()))->update_state();
		(*children_.begin())->update_state();//Update item at front of list
	}

	return index;
}

void winpp::menu::group::post_remove_(gui_object_type &object){
	base_type::pre_remove_(object);
	if (!children_.empty())//Update item at front of list
		(*children_.begin())->update_state();
}
