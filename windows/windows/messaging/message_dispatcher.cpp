#include "message_dispatcher.h"
#include "../application/object_manager.h"

winpp::messaging::object_manager_handling_dispatcher::object_manager_handling_dispatcher(handler_type handler)
	: handler_(handler){}

winpp::messaging::dispatcher::lresult_type winpp::messaging::object_manager_handling_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	return (application::object::get().object_manager().*handler_)(*dynamic_cast<window_type *>(&target), info);
}

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

winpp::messaging::dispatcher::lresult_type winpp::messaging::erase_background_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	message_object_type ev(info, is_sent, target.procedure(), reinterpret_cast<gui::object *>(&target));
	ev.skip();

	events::erase_background e(*reinterpret_cast<gui::object *>(&target), ev.dc());
	target.events().erase_background(e);
	if (e.is_prevented())//Handled
		return TRUE;

	events::object bbe(e.target());
	auto brush = target.events().background_brush(bbe);
	if (e.is_prevented())//Deferred
		return FALSE;

	if (brush != nullptr){//Use brush
		return TRUE;
	}

	return call_(target, ev) ? TRUE : FALSE;
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::paint_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	message_object_type ev(info, is_sent, target.procedure(), reinterpret_cast<gui::object *>(&target));
	ev.skip();

	target.drawer()->BeginDraw();
	try{
		events::paint e(*reinterpret_cast<gui::object *>(&target), ev.begin_info());
		target.events().paint(e);
		if (e.is_prevented())//Handled
			return 0;

		call_(target, ev);
	}
	catch (...){
		target.drawing_result(target.drawer()->EndDraw());
		throw;//Forward exception
	}

	target.drawing_result(target.drawer()->EndDraw());
	return 0;
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::mouse_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	info_type retrieved_info{};
	retrieve_info(info, target, retrieved_info);

	auto gui_target = reinterpret_cast<gui::object *>(&target);
	events::mouse e(*gui_target, info.code(), info.wparam(), gui_target);
	if (retrieved_info.event_object != nullptr)//Fire event
		(*retrieved_info.event_object)(e);

	auto handler = retrieved_info.handler;
	auto resolved_info = info;

	mouse ev(gui_target, resolved_info.code(retrieved_info.code), is_sent, target.procedure(), gui_target);
	(target.*handler)(ev);//Call handler

	return ev.handle(false).result();
}

winpp::messaging::mouse_dispatcher::uint_type winpp::messaging::mouse_dispatcher::non_client_to_client(uint_type code){
	switch (code){
	case WM_NCMOUSEMOVE:
		return WM_MOUSEMOVE;
	case WM_NCMOUSEHOVER:
		return WM_MOUSEHOVER;
	case WM_NCLBUTTONDOWN:
		return WM_LBUTTONDOWN;
	case WM_NCMBUTTONDOWN:
		return WM_MBUTTONDOWN;
	case WM_NCRBUTTONDOWN:
		return WM_RBUTTONDOWN;
	case WM_NCLBUTTONUP:
		return WM_LBUTTONUP;
	case WM_NCMBUTTONUP:
		return WM_MBUTTONUP;
	case WM_NCRBUTTONUP:
		return WM_RBUTTONUP;
	case WM_NCLBUTTONDBLCLK:
		return WM_LBUTTONDBLCLK;
	case WM_NCMBUTTONDBLCLK:
		return WM_MBUTTONDBLCLK;
	case WM_NCRBUTTONDBLCLK:
		return WM_RBUTTONDBLCLK;
	default:
		break;
	}

	return code;
}

winpp::messaging::mouse_dispatcher::uint_type winpp::messaging::mouse_dispatcher::client_to_non_client(uint_type code){
	switch (code){
	case WM_MOUSEMOVE:
		return WM_NCMOUSEMOVE;
	case WM_MOUSEHOVER:
		return WM_NCMOUSEHOVER;
	case WM_LBUTTONDOWN:
		return WM_NCLBUTTONDOWN;
	case WM_MBUTTONDOWN:
		return WM_NCMBUTTONDOWN;
	case WM_RBUTTONDOWN:
		return WM_NCRBUTTONDOWN;
	case WM_LBUTTONUP:
		return WM_NCLBUTTONUP;
	case WM_MBUTTONUP:
		return WM_NCMBUTTONUP;
	case WM_RBUTTONUP:
		return WM_NCRBUTTONUP;
	case WM_LBUTTONDBLCLK:
		return WM_NCLBUTTONDBLCLK;
	case WM_MBUTTONDBLCLK:
		return WM_NCMBUTTONDBLCLK;
	case WM_RBUTTONDBLCLK:
		return WM_NCRBUTTONDBLCLK;
	default:
		break;
	}

	return code;
}

void winpp::messaging::mouse_dispatcher::retrieve_info(const msg_type &info, target_type &target, info_type &out){
	out.code = info.lparam<uint_type>();
	if (info.code() == WINPP_WM_RAWMOUSE && dynamic_cast<gui::object *>(&target)->hit_test(threading::message_queue::last_mouse_position()) != hit_target_type::client)
		out.code = client_to_non_client(out.code);//Convert code to non-client version
	retrieve_event_and_handler(target, out);
}

void winpp::messaging::mouse_dispatcher::retrieve_event_and_handler(target_type &target, info_type &in_out){
	switch (in_out.code){
	case WM_NCMOUSEMOVE:
	case WM_MOUSEMOVE:
		in_out.event_object = &target.events().mouse_move;
		in_out.handler = &target_type::on_mouse_move;
		break;
	case WM_NCMOUSEHOVER:
	case WM_MOUSEHOVER:
		in_out.event_object = &target.events().mouse_hover;
		in_out.handler = &target_type::on_mouse_hover;
		break;
	case WINPP_WM_MOUSEENTER:
		in_out.event_object = &target.events().mouse_enter;
		in_out.handler = &target_type::on_mouse_enter;
		break;
	case WINPP_WM_MOUSELEAVE:
		in_out.event_object = &target.events().mouse_leave;
		in_out.handler = &target_type::on_mouse_leave;
		break;
	case WM_NCLBUTTONDOWN:
	case WM_LBUTTONDOWN:
	case WM_NCMBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_NCRBUTTONDOWN:
	case WM_RBUTTONDOWN:
		in_out.event_object = &target.events().mouse_down;
		in_out.handler = &target_type::on_mouse_down;
		break;
	case WM_NCLBUTTONUP:
	case WM_LBUTTONUP:
	case WM_NCMBUTTONUP:
	case WM_MBUTTONUP:
	case WM_NCRBUTTONUP:
	case WM_RBUTTONUP:
		in_out.event_object = &target.events().mouse_up;
		in_out.handler = &target_type::on_mouse_up;
		break;
	case WM_NCLBUTTONDBLCLK:
	case WM_LBUTTONDBLCLK:
	case WM_NCMBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_NCRBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
		in_out.event_object = &target.events().mouse_dbl_click;
		in_out.handler = &target_type::on_mouse_dbl_click;
		break;
	case WINPP_WM_MOUSEDRAG:
		in_out.event_object = &target.events().mouse_drag;
		in_out.handler = &target_type::on_mouse_drag;
		break;
	case WINPP_WM_MOUSEDRAGEND:
		in_out.event_object = &target.events().mouse_drag_end;
		in_out.handler = &target_type::on_mouse_drag_end;
		break;
	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL:
		in_out.event_object = &target.events().mouse_wheel;
		in_out.handler = &target_type::on_mouse_wheel;
		break;
	default:
		break;
	}
}

winpp::messaging::dispatcher::lresult_type winpp::messaging::key_dispatcher::dispatch(const msg_type &info, bool is_sent, target_type &target){
	auto focused = dynamic_cast<target_type *>(application::object::get().object_manager().object_state().focused);
	if (focused == nullptr)//No focused object
		focused = &target;

	info_type retrieved_info{ info.code() };
	retrieve_event_and_handler(target, retrieved_info);

	auto gui_target = reinterpret_cast<gui::object *>(focused);
	events::key e(*gui_target, info.code(), info.wparam(), info.lparam(), gui_target);
	if (retrieved_info.event_object != nullptr)//Fire event
		(*retrieved_info.event_object)(e);

	auto handler = retrieved_info.handler;
	key ev(gui_target, info, is_sent, focused->procedure(), gui_target);

	(focused->*handler)(ev);//Call handler
	return ev.handle(false).result();
}

void winpp::messaging::key_dispatcher::retrieve_event_and_handler(target_type &target, info_type &in_out){
	switch (in_out.code){
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		in_out.event_object = &target.events().key_down;
		in_out.handler = &target_type::on_key_down;
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		in_out.event_object = &target.events().key_up;
		in_out.handler = &target_type::on_key_up;
		break;
	case WM_CHAR:
	case WM_SYSCHAR:
		in_out.event_object = &target.events().key_pressed;
		in_out.handler = &target_type::on_key_pressed;
		break;
	case WM_DEADCHAR:
	case WM_SYSDEADCHAR:
		in_out.event_object = &target.events().dead_key;
		in_out.handler = &target_type::on_dead_key;
		break;
	default:
		break;
	}
}
