#include "message_handler.h"

winpp::messaging::handler::~handler() = default;

bool winpp::messaging::handler::on_nccreate(const message_object_type &e){
	return true;
}

bool winpp::messaging::handler::on_create(const message_object_type &e){
	return true;
}

void winpp::messaging::handler::on_destroy(const message_object_type &e){}

void winpp::messaging::handler::on_ncdestroy(const message_object_type &e){}

bool winpp::messaging::handler::on_close(const message_object_type &e){
	return true;
}
