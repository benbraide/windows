#include "menu_object.h"
#include "../application/object_manager.h"

winpp::menu::object::object(action_type action)
	: value_(nullptr){
	if (action == action_type::create)
		create();
}

winpp::menu::object::object(app_type &app)
	: value_(nullptr){
	create(app);
}

winpp::menu::object::object(gui_object_type &parent)
	: value_(nullptr){
	create(parent);
}

winpp::menu::object::~object(){
	destroy(force_type::force);
}

winpp::menu::object::operator hmenu_type() const{
	return value_;
}

void *winpp::menu::object::handle() const{
	return value_;
}

winpp::gui::object::hit_target_type winpp::menu::object::hit_test(const point_type &value) const{
	return (outer_rect().contains(value) ? hit_target_type::client : hit_target_type::nil);
}

winpp::gui::object::rect_type winpp::menu::object::outer_rect() const{
	return rect_type{};
}

winpp::gui::object::rect_type winpp::menu::object::relative_outer_rect() const{
	return outer_rect();
}

winpp::gui::object::rect_type winpp::menu::object::inner_rect() const{
	return outer_rect();
}

winpp::menu::object &winpp::menu::object::destroy(force_type force){
	if (!is_created())
		return *this;

	require_app_();
	app_->execute_task([&]{
		if (::DestroyMenu(value_) != FALSE || force == force_type::force){
			value_ = nullptr;
			destroyed_();
		}
	});

	return *this;
}

unsigned int winpp::menu::object::object_group() const{
	return menu_group;
}

bool winpp::menu::object::is_created() const{
	return (value_ != nullptr);
}

bool winpp::menu::object::is_menu() const{
	return true;
}

bool winpp::menu::object::is_popup() const{
	return (is_created() && (parent_ == nullptr || !parent_->is_type<window::object>()));
}

void winpp::menu::object::create(){
	if (app_type::current_app == nullptr)
		create(*scope_app_);
	else//Use current app
		create(*app_type::current_app);
}

void winpp::menu::object::create(app_type &app){
	if (is_created())
		throw common::unsupported_exception();

	(app_ = &app)->execute_task([&]{
		if ((value_ = app_->object_manager().create_menu(*this, application::object_manager::menu_type::popup)) == nullptr){
			app_ = nullptr;
			if (::GetLastError() == ERROR_SUCCESS)
				throw common::internal_error_exception();
			else//Windows error
				throw common::windows_error();
		}
		else//Success
			created_();
	});
}

void winpp::menu::object::create(gui_object_type &parent){
	if (is_created())
		throw common::unsupported_exception();

	if (internal_insert_into_parent(parent) == invalid_index)//Internal error
		throw common::internal_error_exception();

	application::object_manager::menu_type type;
	if (parent_->is_type<item>())
		type = application::object_manager::menu_type::popup;
	else//menu bar
		type = application::object_manager::menu_type::bar;

	if ((app_ = parent_->app()) == nullptr)//App is required
		throw common::no_app_exception();

	app_->execute_task([&]{
		if ((value_ = app_->object_manager().create_menu(*this, type)) == nullptr){
			parent_->internal_remove_child(*this);
			parent_ = nullptr;
			app_ = nullptr;

			if (::GetLastError() == ERROR_SUCCESS)
				throw common::internal_error_exception();
			else//Windows error
				throw common::windows_error();
		}
		
		if (type == application::object_manager::menu_type::bar)
			::SetMenu(parent_->query<window::object>(), value_);
		created_();
	});
}

winpp::gui::object::index_and_size_type winpp::menu::object::insert_into_parent_(gui_object_type &parent, index_and_size_type index){
	if ((parent.is_type<separator>() || parent.is_type<check_item>()) && !parent.is_type<item>() && !parent.is_type<window::object>())
		throw common::invalid_arg_exception();
	return base_type::insert_into_parent_(parent, index);
}
