#include "message_target.h"

winpp::messaging::target::~target() = default;

bool winpp::messaging::target::pre_translate(msg_type &msg){
	auto parent = target_parent_();
	return (parent == nullptr) ? false : parent->pre_translate(msg);
}

winpp::messaging::target::lresult_type winpp::messaging::target::procedure(const msg_type &msg){
	switch ((msg_ = msg).code()){
	case WM_NCCREATE:
		return dispatch_nccreate_();
	case WM_CREATE:
		return dispatch_create_();
	case WM_DESTROY:
		return dispatch_destroy_();
	case WM_NCDESTROY:
		return dispatch_ncdestroy_();
	case WM_CLOSE:
		return dispatch_close_();
	default:
		break;
	}

	return dispatch_unrecognized_();
}

winpp::messaging::target::lresult_type winpp::messaging::target::dispatch_nccreate_(){
	if (action_prevented_<events::tunnel<void>>())
		return FALSE;

	return on_nccreate(message_object_type(msg_, false, procedure())) ? TRUE : FALSE;
}

winpp::messaging::target::lresult_type winpp::messaging::target::dispatch_create_(){
	if (action_prevented_<events::tunnel<void>>())
		return -1;

	return on_create(message_object_type(msg_, false, procedure())) ? 0 : -1;
}

winpp::messaging::target::lresult_type winpp::messaging::target::dispatch_destroy_(){
	fire_event_<events::tunnel<void>>();
	on_destroy(message_object_type(msg_, false, procedure()));
	return 0;
}

winpp::messaging::target::lresult_type winpp::messaging::target::dispatch_ncdestroy_(){
	fire_event_<events::tunnel<void>>();
	on_ncdestroy(message_object_type(msg_, false, procedure()));
	return 0;
}

winpp::messaging::target::lresult_type winpp::messaging::target::dispatch_close_(){
	if (action_prevented_<events::tunnel<void>>())
		return 0;

	message_object_type e(msg_, false, procedure());
	if (on_close(e))
		dynamic_cast<gui::object *>(this)->destroy(gui::object::force_type::dont_force);
	else//Prevent default handling
		e.skip();

	return 0;
}

winpp::messaging::target::lresult_type winpp::messaging::target::dispatch_unrecognized_(){
	return call_default_();
}

winpp::messaging::target::lresult_type winpp::messaging::target::call_default_(){
	auto default_procedure = procedure();
	if (default_procedure == nullptr)//Use default procedure
		default_procedure = ::DefWindowProcW;

	return ::CallWindowProcW(default_procedure, msg_.owner(), msg_.code(), msg_.wparam(), msg_.lparam());
}
