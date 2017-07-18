#include "message_handler.h"
#include "message_dispatcher.h"

winpp::messaging::handler::~handler() = default;

winpp::messaging::handler *winpp::messaging::handler::parent_handler() const{
	return dynamic_cast<handler *>(dynamic_cast<const gui::object *>(this)->parent());
}

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

void winpp::messaging::handler::on_activate_app(ncactivate_message_type &e){}

void winpp::messaging::handler::on_cancel_mode(message_object_type &e){}

void winpp::messaging::handler::on_focus_change(focus_change_message_type &e){}

void winpp::messaging::handler::on_enable(enable_message_type &e){}

winpp::messaging::handler::hcursor_type winpp::messaging::handler::on_set_cursor(set_cursor_message_type &e){
	return nullptr;
}

winpp::messaging::handler::hit_target_type winpp::messaging::handler::on_hit_test(hit_test_message_type &e){
	return hit_target_type::nil;
}

void winpp::messaging::handler::on_position(position_message_type &e){}

bool winpp::messaging::handler::on_size(size_message_type &e){
	return true;
}

bool winpp::messaging::handler::on_move(move_message_type &e){
	return true;
}

bool winpp::messaging::handler::on_erase_background(erase_background_message_type &e){
	return false;
}

void winpp::messaging::handler::on_mouse_move(mouse_message_type &e){
	bubble_mouse(e);
}

void winpp::messaging::handler::on_mouse_hover(mouse_message_type &e){
	bubble_mouse(e);
}

void winpp::messaging::handler::on_mouse_enter(mouse_message_type &e){}

void winpp::messaging::handler::on_mouse_leave(mouse_message_type &e){}

void winpp::messaging::handler::on_mouse_down(mouse_message_type &e){
	bubble_mouse(e);
}

void winpp::messaging::handler::on_mouse_up(mouse_message_type &e){
	bubble_mouse(e);
}

void winpp::messaging::handler::on_mouse_dbl_click(mouse_message_type &e){
	bubble_mouse(e);
}

void winpp::messaging::handler::on_mouse_drag(mouse_message_type &e){
	bubble_mouse(e);
}

void winpp::messaging::handler::on_mouse_drag_end(mouse_message_type &e){
	bubble_mouse(e);
}

void winpp::messaging::handler::on_mouse_wheel(mouse_message_type &e){
	bubble_mouse(e);
}

void winpp::messaging::handler::on_key_down(key_message_type &e){
	bubble_key(e);
}

void winpp::messaging::handler::on_key_up(key_message_type &e){
	bubble_key(e);
}

void winpp::messaging::handler::on_key_pressed(key_message_type &e){
	bubble_key(e);
}

void winpp::messaging::handler::on_dead_key(key_message_type &e){
	bubble_key(e);
}

void winpp::messaging::handler::bubble_mouse(mouse_message_type &e){
	auto parent = parent_handler();
	if (parent == nullptr)//No bubbling
		return;

	mouse_dispatcher::info_type info{ e.info().lparam<uint_type>() };
	mouse_dispatcher::retrieve_event_and_handler(*dynamic_cast<target *>(parent), info);

	auto gui_target = reinterpret_cast<gui::object *>(parent);
	events::mouse eo(*gui_target, info.code, e.info().wparam(), e.original_target());
	if (info.event_object != nullptr)//Fire event
		(*info.event_object)(eo);

	auto handler = info.handler;
	auto resolved_info = e.info();

	mouse ev(gui_target, resolved_info.code(info.code), true, nullptr, gui_target);
	(dynamic_cast<target *>(parent)->*handler)(ev);//Call handler
}

void winpp::messaging::handler::bubble_key(key_message_type &e){
	auto parent = parent_handler();
	if (parent == nullptr)//No bubbling
		return;

	key_dispatcher::info_type info{ e.info().lparam<uint_type>() };
	key_dispatcher::retrieve_event_and_handler(*dynamic_cast<target *>(parent), info);

	auto gui_target = reinterpret_cast<gui::object *>(parent);
	events::key eo(*gui_target, info.code, e.info().wparam(), e.info().lparam(), e.original_target());
	if (info.event_object != nullptr)//Fire event
		(*info.event_object)(eo);

	auto handler = info.handler;
	key ev(gui_target, e.info(), true, nullptr, gui_target);

	(dynamic_cast<target *>(parent)->*handler)(ev);//Call handler
}
