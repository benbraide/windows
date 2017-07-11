#include "message_dispatcher.h"

winpp::messaging::dispatcher::lresult_type winpp::messaging::unrecognized_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	return call_(info, is_sent, target);
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::nccreate_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	events::object e(*reinterpret_cast<gui::object *>(&target));
	target.events().pre_create(e);
	return (e.is_prevented() || !call_(info, is_sent, target)) ? FALSE : TRUE;
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::create_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	events::object e(*reinterpret_cast<gui::object *>(&target));
	target.events().create(e);
	return (e.is_prevented() || !call_(info, is_sent, target)) ? -1 : 0;
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::destroy_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	events::object e(*reinterpret_cast<gui::object *>(&target));
	target.events().destroy(e);
	call_(info, is_sent, target);
	return 0;
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::ncdestroy_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	call_(info, is_sent, target);
	events::object e(*reinterpret_cast<gui::object *>(&target));
	target.events().post_destroy(e);
	return 0;
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::close_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	events::object e(*reinterpret_cast<gui::object *>(&target));
	target.events().pre_create(e);
	if (e.is_prevented())
		return 0;

	if (call_(info, is_sent, target, true))//Destroy object
		dynamic_cast<gui::object *>(&target)->destroy(gui::object::force_type::dont_force);

	return 0;
}
