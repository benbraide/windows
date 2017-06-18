#include "application_object.h"

winpp::application::object::object(){
	if (current_app != nullptr)
		throw common::multiple_apps_exception();

	current_app = this;
	if (main_app == nullptr)
		main_app = this;
}

winpp::application::object::~object() = default;

winpp::application::object *winpp::application::object::main_app = nullptr;

thread_local winpp::application::object *winpp::application::object::current_app = nullptr;

bool winpp::application::object::is_filtered_message_() const{
	return false;
}

void winpp::application::object::dispatch_(){
	if (!is_dialog_message_())
		base_type::dispatch_();
}

bool winpp::application::object::is_stopped_() const{
	return false;
}

bool winpp::application::object::is_dialog_message_(){
	return false;
}
