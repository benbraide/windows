#include "message_target.h"
#include "../application/object_manager.h"

winpp::messaging::target::event_tunnel::~event_tunnel() = default;

unsigned __int64 winpp::messaging::target::event_tunnel::bind(event_type e, const std::any &callback){
	switch (e){
	case event_type::pre_create:
		return pre_create(callback);
	case event_type::post_destroy:
		return post_destroy(callback);
	case event_type::pre_activate:
		return pre_activate(callback);
	case event_type::activate:
		return activate(callback);
	case event_type::close:
		return close(callback);
	case event_type::maximize:
		return maximize(callback);
	case event_type::minimize:
		return minimize(callback);
	case event_type::restore:
		return restore(callback);
	case event_type::show:
		return show(callback);
	case event_type::hide:
		return hide(callback);
	case event_type::erase_background:
		return erase_background(callback);
	case event_type::paint:
		return paint(callback);
	default:
		break;
	}

	return base_type::bind(e, callback);
}

unsigned __int64 winpp::messaging::target::event_tunnel::bind_(const std::wstring &e, const std::any &callback){
	if (e == L"pre_create")
		return pre_create(callback);

	if (e == L"post_destroy")
		return post_destroy(callback);

	if (e == L"pre_activate")
		return pre_activate(callback);

	if (e == L"activate")
		return activate(callback);

	if (e == L"close")
		return close(callback);

	if (e == L"maximize")
		return maximize(callback);

	if (e == L"minimize")
		return minimize(callback);

	if (e == L"restore")
		return restore(callback);

	if (e == L"show")
		return show(callback);

	if (e == L"hide")
		return hide(callback);

	if (e == L"erase_background")
		return erase_background(callback);

	if (e == L"paint")
		return paint(callback);

	return base_type::bind_(e, callback);
}

winpp::messaging::target::~target() = default;

bool winpp::messaging::target::pre_translate(msg_type &msg){
	auto parent = target_parent_();
	return (parent == nullptr) ? false : parent->pre_translate(msg);
}

winpp::messaging::target::lresult_type winpp::messaging::target::unrecognized_message(message_object_type &e){
	return call_default_(e.info());
}

winpp::messaging::target::lresult_type winpp::messaging::target::procedure(const msg_type &msg, bool is_sent){
	auto dispatcher = application::object::get().object_manager().messaging_map.find_dispatcher(msg.code());
	return (dispatcher == nullptr) ? call_default_(msg) : dispatcher->dispatch(msg, is_sent, *this);
}

/*
winpp::messaging::target::lresult_type winpp::messaging::target::dispatch_ncactivate_(){
	if (action_prevented_<events::tunnel<void>>() && msg_.wparam<::BOOL>() == FALSE)
		return FALSE;//Prevent

	ncactivate_message_type e(msg_, false, procedure());
	if (!on_ncactivate(e) && msg_.wparam<::BOOL>() == FALSE){//Prevent
		e.skip();
		return FALSE;
	}

	return e.handle(false).result();
}

winpp::messaging::target::lresult_type winpp::messaging::target::dispatch_activate_(){
	fire_event_<events::tunnel<void>>();
	on_activate(activate_message_type(msg_, false, procedure()));
	return 0;
}*/

winpp::messaging::target::lresult_type winpp::messaging::target::call_default_(const msg_type &info){
	auto default_procedure = procedure();
	if (default_procedure == nullptr)//Use default procedure
		default_procedure = ::DefWindowProcW;

	return ::CallWindowProcW(default_procedure, info.owner(), info.code(), info.wparam(), info.lparam());
}
