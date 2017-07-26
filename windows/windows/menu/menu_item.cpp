#include "menu_item.h"
#include "menu_object.h"
#include "../application/object_manager.h"

winpp::menu::item::item()
	: id_(invalid_id), sub_(nullptr){}

winpp::menu::item::item(gui_object_type &parent, const std::wstring &label, menu_state_type states)
	: id_(invalid_id), sub_(nullptr){
	create(parent, label, states);
}

winpp::menu::item::~item(){
	destroy(force_type::force);
}

winpp::gui::object::index_and_size_type winpp::menu::item::internal_insert_child(gui_object_type &child, index_and_size_type before){
	require_app_();
	app_->execute_task([&]{
		if (sub_ == nullptr)
			sub_ = &child.query<menu_object_type>();
		else//Already linked with a sub menu
			throw common::unsupported_exception();
	});
	
	return 0;
}

winpp::menu::item &winpp::menu::item::internal_remove_child(gui_object_type &child, force_type force){
	require_app_();
	app_->execute_task([&]{
		if (&child == sub_)//Unlink sub menu
			sub_ = nullptr;
	});
	
	return *this;
}

winpp::structures::enumerations::hit_target_type winpp::menu::item::hit_test(const point_type &value) const{
	return (outer_rect().contains(value) ? hit_target_type::client : hit_target_type::nil);
}

winpp::gui::object::rect_type winpp::menu::item::outer_rect() const{
	if (parent_ == nullptr)//Parent required
		throw common::invalid_object_exception();

	require_app_();
	return app_->execute_task<rect_type>([&]() -> rect_type{
		auto owner = parent_->owner();
		if (owner == nullptr || !owner->is_window())
			return rect_type{};//Owner is required

		rect_type value;
		if (owner == nullptr || !owner->is_window())
			::GetMenuItemRect(nullptr, parent_->get_type<menu_object_type>(), static_cast<uint_type>(absolute_index_in_parent()), value);
		else//Has window owner
			::GetMenuItemRect(owner->query<window::object>(), parent_->get_type<menu_object_type>(), static_cast<uint_type>(absolute_index_in_parent()), value);

		return value;
	});
}

winpp::gui::object::rect_type winpp::menu::item::relative_outer_rect() const{
	return outer_rect();
}

winpp::gui::object::rect_type winpp::menu::item::inner_rect() const{
	return outer_rect();
}

winpp::menu::item &winpp::menu::item::update_state(){
	if (parent_ == nullptr)//Parent required
		return *this;

	require_app_();
	app_->execute_task([&]{
		info_type info{};
		get_info_(info);

		auto is_newline_parent = ((parent_ == nullptr) ? false : parent_->is_type<newline>());
		if (!is_newline_parent && parent_ != nullptr && parent_->is_type<group>()){//Try grandparent
			auto grand_parent = parent_->parent();
			is_newline_parent = ((grand_parent == nullptr) ? false : grand_parent->is_type<newline>());
		}

		if (is_newline_parent && index_in_parent() == 0u){//New line
			if (parent_->is_type<bordered_newline>())
				WINPP_SET(info.fType, MFT_MENUBARBREAK);
			else//No border
				WINPP_SET(info.fType, MFT_MENUBREAK);
		}
		else//Remove new line types
			WINPP_REMOVE(info.fType, MFT_MENUBARBREAK | MFT_MENUBREAK);

		set_info_(info);
	});

	return *this;
}

winpp::menu::item &winpp::menu::item::destroy(force_type force){
	if (parent_ == nullptr)
		return *this;

	require_app_();
	app_->execute_task([&]{
		auto &menu_parent = parent_->get_type<menu_object_type>();
		if (::RemoveMenu(menu_parent, static_cast<uint_type>(absolute_index_in_parent()), TRUE) != FALSE || force == force_type::force){
			app_->object_manager().remove_index(id_, &menu_parent);
			id_ = invalid_id;
			destroyed_();
		}
	});

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
	create_(parent, label, states);
}

winpp::menu::item::word_type winpp::menu::item::id() const{
	return id_;
}

winpp::menu::item::menu_object_type *winpp::menu::item::sub() const{
	return sub_;
}

winpp::menu::item &winpp::menu::item::states(menu_state_type value){
	if (parent_ != nullptr)//Parent required
		throw common::unsupported_exception();

	require_app_();
	app_->execute_task([&]{
		set_info_(info_type{
			sizeof(info_type),							//Size
			MIIM_STATE,									//Flags
			0,											//Types
			static_cast<uint_type>(value),				//States
		});
	});

	return *this;
}

winpp::menu::item::menu_state_type winpp::menu::item::states() const{
	if (parent_ != nullptr)//Parent required
		throw common::unsupported_exception();

	require_app_();
	return app_->execute_task<menu_state_type>([&]() -> menu_state_type{
		info_type info{
			sizeof(info_type),							//Size
			MIIM_STATE,									//Flags
		};

		get_info_(info);
		return static_cast<menu_state_type>(info.fState);
	});
}

bool winpp::menu::item::is_owner_drawn() const{
	return false;
}

winpp::menu::item &winpp::menu::item::enable(){
	return states(WINPP_REMOVE_V(states(), menu_state_type::disabled));
}

winpp::menu::item &winpp::menu::item::disable(){
	return states(WINPP_SET_V(states(), menu_state_type::disabled));
}

bool winpp::menu::item::is_disabled() const{
	return WINPP_IS(states(), menu_state_type::disabled);
}

winpp::menu::item &winpp::menu::item::set_as_default(bool is_default){
	if (is_default)
		return states(WINPP_SET_V(states(), menu_state_type::is_default));
	return states(WINPP_REMOVE_V(states(), menu_state_type::is_default));
}

bool winpp::menu::item::is_default() const{
	return WINPP_IS(states(), menu_state_type::is_default);
}

winpp::menu::item &winpp::menu::item::check(){
	return states(WINPP_SET_V(states(), menu_state_type::checked));
}

winpp::menu::item &winpp::menu::item::uncheck(){
	return states(WINPP_REMOVE_V(states(), menu_state_type::checked));
}

winpp::menu::item &winpp::menu::item::toggle_check(){
	auto states = this->states();
	if (WINPP_IS(states, menu_state_type::checked))//Remove check
		return this->states(WINPP_REMOVE_V(states, menu_state_type::checked));
	return this->states(WINPP_SET_V(states, menu_state_type::checked));//Check
}

bool winpp::menu::item::is_checked() const{
	return WINPP_IS(states(), menu_state_type::checked);
}

bool winpp::menu::item::is_action() const{
	return (sub_ == nullptr);
}

bool winpp::menu::item::is_separator() const{
	return false;
}

winpp::gui::object::index_and_size_type winpp::menu::item::insert_into_parent_(gui_object_type &parent, index_and_size_type index){
	if (!parent.is_type<menu_object_type>())
		throw common::invalid_arg_exception();
	return base_type::insert_into_parent_(parent, index);
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

void winpp::menu::item::create_(gui_object_type &parent, const std::wstring &label, menu_state_type states){
	if (is_created())
		throw common::unsupported_exception();

	if (internal_insert_into_parent(parent) == invalid_index)//Internal error
		throw common::internal_error_exception();

	if ((app_ = parent_->app()) == nullptr)//App is required
		throw common::no_app_exception();

	app_->execute_task([&]{
		auto &menu_parent = parent_->get_type<menu_object_type>();
		if ((id_ = app_->object_manager().add_index<word_type>(*this, &menu_parent)) == invalid_id){//Failed to generate ID
			parent_->internal_remove_child(*this);
			parent_ = nullptr;
			app_ = nullptr;
			throw common::internal_error_exception();
		}

		auto types = get_types_();
		if (is_owner_drawn())//Owner drawn item
			WINPP_SET(types, MFT_OWNERDRAW);

		uint_type mask = MIIM_STRING | MIIM_ID | MIIM_DATA;
		if (types != 0u)//Set type mask
			WINPP_SET(mask, MIIM_FTYPE);

		if (states != menu_state_type::nil)
			WINPP_SET(mask, MIIM_STATE);

		info_type info{
			sizeof(info_type),							//Size
			mask,										//Flags
			types,										//Types
			static_cast<uint_type>(states),				//States
			id_,										//Id
			nullptr,									//Sub-menu
			nullptr,									//Checked bitmap
			nullptr,									//Unchecked bitmap
			reinterpret_cast<ulong_ptr_type>(this),		//Data
			const_cast<wchar_t *>(label.c_str()),		//String
			static_cast<uint_type>(label.size()),		//String size
		};

		if (::InsertMenuItemW(menu_parent, static_cast<uint_type>(absolute_index_in_parent()), TRUE, &info) == FALSE){//Failed to insert item
			app_->object_manager().remove_index(id_, &menu_parent);

			parent_->internal_remove_child(*this);
			parent_ = nullptr;

			app_ = nullptr;
			id_ = invalid_id;

			if (::GetLastError() == ERROR_SUCCESS)
				throw common::internal_error_exception();
			else//Windows error
				throw common::windows_error();
		}
		
		if (!menu_parent.is_popup())//Update view
			::DrawMenuBar(menu_parent.parent()->query<window::object>());

		created_();
	});
}

winpp::menu::item::uint_type winpp::menu::item::get_types_() const{
	uint_type types = 0;
	if (parent_->is_type<newline>()){//New line
		if (parent_->is_type<bordered_newline>())
			WINPP_SET(types, MFT_MENUBARBREAK);
		else//No border
			WINPP_SET(types, MFT_MENUBREAK);
	}

	return types;
}

void winpp::menu::item::set_info_(const info_type &info){
	auto &menu_parent = parent_->get_type<menu_object_type>();
	::SetMenuItemInfoW(menu_parent, static_cast<uint_type>(absolute_index_in_parent()), TRUE, &info);
	if (!menu_parent.is_popup())//Update view
		::DrawMenuBar(menu_parent.parent()->query<window::object>());
}

void winpp::menu::item::get_info_(info_type &info) const{
	info.cbSize = sizeof(info_type);
	::GetMenuItemInfoW(parent_->get_type<menu_object_type>(), static_cast<uint_type>(absolute_index_in_parent()), TRUE, &info);
}
