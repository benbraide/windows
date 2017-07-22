#include "menu_item.h"
#include "menu_object.h"
#include "../application/object_manager.h"

winpp::menu::item::item()
	: id_(invalid_id){}

winpp::menu::item::item(gui_object_type &parent, const std::wstring &label, menu_state_type states)
	: id_(invalid_id){
	create(parent, label, states);
}

winpp::menu::item::~item(){
	destroy(force_type::force);
}

void *winpp::menu::item::handle() const{
	return reinterpret_cast<void *>(static_cast<ulong_ptr_type>(id_));
}

winpp::structures::enumerations::hit_target_type winpp::menu::item::hit_test(const point_type &value) const{
	return (outer_rect().contains(value) ? hit_target_type::client : hit_target_type::nil);
}

winpp::gui::object::rect_type winpp::menu::item::outer_rect() const{
	if (parent_ == nullptr)//Parent required
		throw common::invalid_object_exception();

	auto owner = parent_->owner();
	if (owner == nullptr || !owner->is_window())
		return rect_type{};//Owner is required

	rect_type value;
	if (owner == nullptr || !owner->is_window())
		::GetMenuItemRect(nullptr, static_cast<hmenu_type>(parent_->handle()), static_cast<uint_type>(absolute_index_in_parent()), value);
	else//Has window owner
		::GetMenuItemRect(owner->query<window::object>(), static_cast<hmenu_type>(parent_->handle()), static_cast<uint_type>(absolute_index_in_parent()), value);

	return value;
}

winpp::gui::object::rect_type winpp::menu::item::relative_outer_rect() const{
	return outer_rect();
}

winpp::gui::object::rect_type winpp::menu::item::inner_rect() const{
	return outer_rect();
}

winpp::menu::item &winpp::menu::item::update_state(){
	return *this;
}

winpp::menu::item &winpp::menu::item::destroy(force_type force){
	return *this;
}

unsigned int winpp::menu::item::object_group() const{
	return menu_item_group;
}

bool winpp::menu::item::is_created() const{
	return (parent_ != nullptr && id_ != invalid_id);
}

bool winpp::menu::item::is_menu_item() const{
	return true;
}

void winpp::menu::item::create(gui_object_type &parent, const std::wstring &label, menu_state_type states){
	if (is_created())
		throw common::unsupported_exception();

	auto app = parent.app();
	if (app == nullptr)//App is required
		throw common::no_app_exception();

	if ((id_ = app->object_manager().add_index<word_type>(*this)) == invalid_id)//Failed to generate ID
		throw common::internal_error_exception();

	if (internal_insert_into_parent(parent) == invalid_index)//Internal error
		throw common::internal_error_exception();

	uint_type types = 0;
	if (parent_->try_query<newline>() != nullptr){//New line
		if (parent_->try_query<bordered_newline>() != nullptr)
			WINPP_SET(types, MFT_MENUBARBREAK);
		else//No border
			WINPP_SET(types, MFT_MENUBREAK);
	}

	if (is_owner_drawn())//Owner drawn item
		WINPP_SET(types, MFT_OWNERDRAW);

	uint_type mask = MIIM_STRING | MIIM_ID | MIIM_DATA;
	if (types != 0u)//Set type mask
		WINPP_SET(mask, MIIM_FTYPE);

	info_type info{
		sizeof(info_type),							//Size
		mask,										//Flags
		types,										//Types
		0,											//States
		id_,										//Id
		nullptr,									//Sub-menu
		nullptr,									//Checked bitmap
		nullptr,									//Unchecked bitmap
		reinterpret_cast<ulong_ptr_type>(this),		//Data
		const_cast<wchar_t *>(label.c_str()),		//String
		static_cast<uint_type>(label.size()),		//String size
	};

	if (::InsertMenuItemW(static_cast<hmenu_type>(parent_->handle()), static_cast<uint_type>(absolute_index_in_parent()), TRUE, &info) == FALSE){//Failed to insert item
		parent_->internal_remove_child(*this);
		parent_ = nullptr;
		throw common::windows_error();
	}
}

winpp::menu::item &winpp::menu::item::states(menu_state_type value){
	return *this;
}

winpp::menu::item::menu_state_type winpp::menu::item::states() const{
	return menu_state_type::nil;
}

bool winpp::menu::item::is_owner_drawn() const{
	return false;
}

winpp::gui::generic_object::attributes_type winpp::menu::item::get_attributes_(){
	auto value = base_type::get_attributes_();
	if (value != nullptr){//Set ID callback
		value->internal_set_uint64_callback(gui_attributes_type::key_type::id, [this]{
			return static_cast<unsigned __int64>(id_);
		});
	}

	return value;
}
