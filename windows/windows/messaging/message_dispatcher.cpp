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

winpp::messaging::dispatcher::lresult_type winpp::messaging::mouse_activate_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	events::mouse_activate e(*reinterpret_cast<gui::object *>(&target), info.lparam());
	target.events().mouse_activate(e);
	if (e.is_prevented())//Prevent activation
		return MA_NOACTIVATEANDEAT;

	message_object_type ev(info, is_sent, target.procedure(), reinterpret_cast<gui::object *>(&target));
	auto value = call_(target, ev);
	if (value != mouse_activate_type::nil){//Skip default
		ev.skip();
		return static_cast<lresult_type>(value);
	}

	return ev.handle(false).result();//Use default
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::ncactivate_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	events::pre_activate e(*reinterpret_cast<gui::object *>(&target), (info.wparam<::BOOL>() != FALSE));
	target.events().pre_activate(e);
	if (e.is_prevented())//Prevent activation
		return FALSE;

	message_object_type ev(info, is_sent, target.procedure(), reinterpret_cast<gui::object *>(&target));
	if (!call_(target, ev)){//Prevent activation
		ev.skip();
		return FALSE;
	}

	return ev.handle(false).result();
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::activate_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	events::activate e(*reinterpret_cast<gui::object *>(&target), static_cast<events::activate::state>(info.wparam<int>()), info.lparam<::HWND>());
	target.events().activate(e);
	call_(info, is_sent, target);
	return 0;
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::child_activate_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	events::object e(*reinterpret_cast<gui::object *>(&target));
	target.events().child_activate(e);
	call_(info, is_sent, target);
	return 0;
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::activate_app_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	events::pre_activate e(*reinterpret_cast<gui::object *>(&target), (info.wparam<::BOOL>() != FALSE));
	target.events().activate_app(e);
	call_(info, is_sent, target);
	return 0;
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::cancel_mode_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	events::object e(*reinterpret_cast<gui::object *>(&target));
	target.events().cancel_mode(e);
	call_(info, is_sent, target);
	return 0;
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::focus_change_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	events::focus_change e(*reinterpret_cast<gui::object *>(&target), (info.code() == WM_SETFOCUS));
	target.events().focus_change(e);
	call_(info, is_sent, target);
	return 0;
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::enable_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	events::enable e(*reinterpret_cast<gui::object *>(&target), (info.wparam<::BOOL>() != FALSE));
	target.events().enable(e);
	call_(info, is_sent, target);
	return 0;
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::position_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	events::position e(*reinterpret_cast<gui::object *>(&target), (info.code() == WM_WINDOWPOSCHANGING), *info.lparam<position::window_pos_type *>());
	target.events().position(e);
	call_(info, is_sent, target);
	return 0;
}
