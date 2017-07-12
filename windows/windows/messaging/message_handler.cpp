#include "message_handler.h"

winpp::messaging::handler::~handler() = default;

bool winpp::messaging::handler::on_nccreate(message_object_type &e){
	return true;
}

bool winpp::messaging::handler::on_create(message_object_type &e){
	return true;
}

void winpp::messaging::handler::on_destroy(message_object_type &e){}

void winpp::messaging::handler::on_ncdestroy(message_object_type &e){}

bool winpp::messaging::handler::on_close(message_object_type &e){
	return true;
}

winpp::messaging::handler::mouse_activate_type winpp::messaging::handler::on_mouse_activate(mouse_activate_message_type &e){
	return mouse_activate_type::nil;
}

bool winpp::messaging::handler::on_ncactivate(ncactivate_message_type &e){
	return true;
}

void winpp::messaging::handler::on_activate(activate_message_type &e){}

void winpp::messaging::handler::on_child_activate(message_object_type &e){}

void winpp::messaging::handler::on_activate_app(message_object_type &e){}
