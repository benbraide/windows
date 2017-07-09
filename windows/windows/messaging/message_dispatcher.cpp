#include "message_dispatcher.h"

winpp::messaging::dispatcher::lresult_type winpp::messaging::dispatcher::dispatch(const msg_type &info, bool is_sent, const std::any &handler, gui_object_type *target){
	if (target == nullptr && (target = hwnd_type(info.owner()).data<gui_object_type *>()) == nullptr)//Unrecognized handle
		return ::DefWindowProcW(info.owner(), info.code(), info.wparam(), info.lparam());

	switch (info.code()){
	case WM_NCCREATE:
		return dispatch_nc_create_(info, is_sent, handler, target);
	case WM_CREATE:
		return dispatch_create_(info, is_sent, handler, target);
	case WM_DESTROY:
		return dispatch_destroy_(info, is_sent, handler, target);
	case WM_NCDESTROY:
		return dispatch_nc_destroy_(info, is_sent, handler, target);
	default:
		break;
	}

	if (info.owner() == nullptr)
		throw common::invalid_arg_exception();

	return ::DefWindowProcW(info.owner(), info.code(), info.wparam(), info.lparam());
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::dispatcher::dispatch_nc_create_(const msg_type &info, bool is_sent, const std::any &handler, gui_object_type *target){
	return 0;
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::dispatcher::dispatch_create_(const msg_type &info, bool is_sent, const std::any &handler, gui_object_type *target){
	return 0;
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::dispatcher::dispatch_destroy_(const msg_type &info, bool is_sent, const std::any &handler, gui_object_type *target){
	return 0;
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::dispatcher::dispatch_nc_destroy_(const msg_type &info, bool is_sent, const std::any &handler, gui_object_type *target){
	return 0;
}
