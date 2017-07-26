#include "menu_radio_item.h"
#include "menu_object.h"

winpp::menu::radio_item::radio_item(){}

winpp::menu::radio_item::radio_item(gui_object_type &parent, const std::wstring &label, menu_state_type states){
	create(parent, label, states);
}

winpp::menu::radio_item::~radio_item(){
	destroy(force_type::force);
}

winpp::menu::radio_item &winpp::menu::radio_item::check(){
	if (parent_ != nullptr)//Parent required
		throw common::unsupported_exception();

	require_app_();
	app_->execute_task([&]{
		auto offset = parent_->absolute_index_in_parent();
		::CheckMenuRadioItem(
			parent_->get_type<menu_object_type>(),						//Handle
			static_cast<uint_type>(offset),								//Start
			static_cast<uint_type>(offset + parent_->children_count()),	//End
			static_cast<uint_type>(offset + index_in_parent()),			//Target
			MF_BYPOSITION
		);
	});

	return *this;
}

winpp::menu::radio_item &winpp::menu::radio_item::uncheck(){
	throw common::unsupported_exception();
}
