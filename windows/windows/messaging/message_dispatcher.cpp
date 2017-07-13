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
	target.events().close(e);
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

winpp::messaging::dispatcher::lresult_type winpp::messaging::set_cursor_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	events::set_cursor e(*reinterpret_cast<gui::object *>(&target), info.lparam());
	auto value = target.events().set_cursor(e);
	if (value != nullptr){//Use cursor
		::SetCursor(value);
		return TRUE;
	}

	message_object_type ev(info, is_sent, target.procedure(), reinterpret_cast<gui::object *>(&target));
	ev.skip();

	if ((value = call_(target, ev)) != nullptr){//Use cursor
		::SetCursor(value);
		return TRUE;
	}

	if ((value = retrieve_cursor_(info)) != nullptr)
		::SetCursor(value);//Use retrieved value

	return TRUE;
}

::HCURSOR winpp::messaging::set_cursor_dispatcher::retrieve_cursor_(const msg_type &info){
	//Retrieve cursor -- Ref: Wine
	switch (static_cast<hit_target_type>(info.low_lparam())){
	case hit_target_type::error://Play beep if applicable
		switch (info.high_lparam()){
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_XBUTTONDOWN:
			::MessageBeep(0);
			break;
		default:
			break;
		}

		return nullptr;
	case hit_target_type::client://Use class cursor
		return reinterpret_cast<::HCURSOR>(::GetClassLongPtrW(info.owner(), GCLP_HCURSOR));
	case hit_target_type::left:
	case hit_target_type::right:
		return ::LoadCursorW(nullptr, IDC_SIZEWE);
	case hit_target_type::top:
	case hit_target_type::bottom:
		return ::LoadCursorW(nullptr, IDC_SIZENS);
	case hit_target_type::top_left:
	case hit_target_type::bottom_right:
		return ::LoadCursorW(nullptr, IDC_SIZENWSE);
	case hit_target_type::top_right:
	case hit_target_type::bottom_left:
		return ::LoadCursorW(nullptr, IDC_SIZENESW);
	default:
		break;
	}

	return ::LoadCursorW(nullptr, IDC_ARROW);
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::hit_test_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	events::hit_test e(*reinterpret_cast<gui::object *>(&target), WINPP_MAKE_MOUSE_POSITION(info.lparam()));
	auto value = target.events().hit_test(e);
	if (value != hit_target_type::nil)
		return static_cast<lresult_type>(value);

	message_object_type ev(info, is_sent, target.procedure(), reinterpret_cast<gui::object *>(&target));
	if ((value = call_(target, ev)) != hit_target_type::nil)
		return static_cast<lresult_type>(value);

	return ev.handle(false).result();
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::position_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	events::position e(*reinterpret_cast<gui::object *>(&target), (info.code() == WM_WINDOWPOSCHANGING), *info.lparam<position::window_pos_type *>());
	target.events().position(e);
	call_(info, is_sent, target);
	return 0;
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::size_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	events::size e(*reinterpret_cast<gui::object *>(&target), (info.code() == WM_SIZING), info.wparam<int>());
	target.events().size(e);
	if (e.is_prevented()){//Action prevented
		if (info.code() == WM_SIZING)//Prevent size
			*info.lparam<rect_value_type *>() = rect_value_type{};
		return TRUE;
	}

	message_object_type ev(info, is_sent, target.procedure(), reinterpret_cast<gui::object *>(&target));
	if (!call_(target, ev) && info.code() == WM_SIZING){//Prevent size
		ev.skip();
		*info.lparam<rect_value_type *>() = rect_value_type{};
		return TRUE;
	}

	return ev.handle(false).result();
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::move_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	events::move e(*reinterpret_cast<gui::object *>(&target), (info.code() == WM_MOVING));
	target.events().move(e);
	if (e.is_prevented()){//Action prevented
		if (info.code() == WM_MOVING)//Prevent move
			*info.lparam<rect_value_type *>() = reinterpret_cast<gui::object *>(&target)->outer_rect();
		return TRUE;
	}

	message_object_type ev(info, is_sent, target.procedure(), reinterpret_cast<gui::object *>(&target));
	if (!call_(target, ev) && info.code() == WM_MOVING){//Prevent move
		ev.skip();
		*info.lparam<rect_value_type *>() = reinterpret_cast<gui::object *>(&target)->outer_rect();
		return TRUE;
	}

	return ev.handle(false).result();
}
