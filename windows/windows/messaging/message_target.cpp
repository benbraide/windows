#include "message_target.h"
#include "../application/object_manager.h"

winpp::messaging::target::event_tunnel::~event_tunnel() = default;

unsigned __int64 winpp::messaging::target::event_tunnel::bind(event_type e, const std::any &callback){
	switch (e){
	case event_type::pre_create:
		return pre_create.bind_generic(callback);
	case event_type::post_destroy:
		return post_destroy.bind_generic(callback);
	case event_type::pre_activate:
		return pre_activate.bind_generic(callback);
	case event_type::activate:
		return activate.bind_generic(callback);
	case event_type::close:
		return close.bind_generic(callback);
	case event_type::show:
		return show.bind_generic(callback);
	case event_type::hide:
		return hide.bind_generic(callback);
	case event_type::erase_background:
		return erase_background.bind_generic(callback);
	case event_type::paint:
		return paint.bind_generic(callback);
	default:
		break;
	}

	return base_type::bind(e, callback);
}

unsigned __int64 winpp::messaging::target::event_tunnel::bind_(const std::wstring &e, const std::any &callback){
	if (e == L"pre_create")
		return pre_create.bind_generic(callback);

	if (e == L"post_destroy")
		return post_destroy.bind_generic(callback);

	if (e == L"pre_activate")
		return pre_activate.bind_generic(callback);

	if (e == L"activate")
		return activate.bind_generic(callback);

	if (e == L"close")
		return close.bind_generic(callback);

	if (e == L"show")
		return show.bind_generic(callback);

	if (e == L"hide")
		return hide.bind_generic(callback);

	if (e == L"erase_background")
		return erase_background.bind_generic(callback);

	if (e == L"paint")
		return paint.bind_generic(callback);

	return base_type::bind_(e, callback);
}

winpp::messaging::target::~target() = default;

bool winpp::messaging::target::pre_translate(msg_type &msg){
	auto parent = target_parent_();
	return (parent == nullptr) ? false : parent->pre_translate(msg);
}

winpp::messaging::target::lresult_type winpp::messaging::target::dispatch(const msg_type &msg, bool is_sent){
	events::dispatch_message e(*reinterpret_cast<gui::object *>(this), msg, is_sent);
	auto result = events().dispatch_message(e);
	if (e.is_prevented())//Message handled
		return result;

	auto dispatcher = application::object::get().object_manager().messaging_map.find_dispatcher(msg.code());
	return (dispatcher == nullptr) ? call_default_(msg) : dispatcher->dispatch(msg, is_sent, *this);
}

winpp::messaging::target::lresult_type winpp::messaging::target::unrecognized_message(message_object_type &e){
	events::unrecognized_message ev(*reinterpret_cast<gui::object *>(this), e.info(), e.is_sent());
	auto result = events().unrecognized_message(ev);
	if (ev.is_prevented())//Message handled
		return result;

	return call_default_(e.info());
}

void winpp::messaging::target::drawing_result(hresult_type result){}

winpp::messaging::target::lresult_type winpp::messaging::target::call_default_(const msg_type &info){
	auto default_procedure = procedure();
	if (default_procedure == nullptr)//Use default procedure
		default_procedure = ::DefWindowProcW;

	return ::CallWindowProcW(default_procedure, info.owner(), info.code(), info.wparam(), info.lparam());
}
