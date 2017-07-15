#include "message_map.h"
#include "../application/object_manager.h"

winpp::messaging::map::map()
	: unrecognized_dispatcher_(std::make_shared<messaging::unrecognized_dispatcher>(&target::unrecognized_message)){
	list_[WM_NCCREATE] = std::make_shared<messaging::nccreate_dispatcher>(&target::on_nccreate);
	list_[WM_CREATE] = std::make_shared<messaging::create_dispatcher>(&target::on_create);

	list_[WM_DESTROY] = std::make_shared<messaging::destroy_dispatcher>(&target::on_destroy);
	list_[WM_NCDESTROY] = std::make_shared<messaging::ncdestroy_dispatcher>(&target::on_ncdestroy);

	list_[WM_CLOSE] = std::make_shared<messaging::close_dispatcher>(&target::on_close);

	list_[WM_MOUSEACTIVATE] = std::make_shared<messaging::mouse_activate_dispatcher>(&target::on_mouse_activate);
	list_[WM_NCACTIVATE] = std::make_shared<messaging::ncactivate_dispatcher>(&target::on_ncactivate);
	list_[WM_ACTIVATE] = std::make_shared<messaging::activate_dispatcher>(&target::on_activate);

	list_[WM_CHILDACTIVATE] = std::make_shared<messaging::child_activate_dispatcher>(&target::on_child_activate);
	list_[WM_ACTIVATEAPP] = std::make_shared<messaging::activate_app_dispatcher>(&target::on_activate_app);

	list_[WM_CANCELMODE] = std::make_shared<messaging::cancel_mode_dispatcher>(&target::on_cancel_mode);
	list_[WM_KILLFOCUS] = list_[WM_SETFOCUS] = std::make_shared<messaging::focus_change_dispatcher>(&target::on_focus_change);
	list_[WM_ENABLE] = std::make_shared<messaging::enable_dispatcher>(&target::on_enable);

	list_[WM_SETCURSOR] = std::make_shared<messaging::set_cursor_dispatcher>(&target::on_set_cursor);
	list_[WM_NCHITTEST] = std::make_shared<messaging::hit_test_dispatcher>(&target::on_hit_test);

	list_[WM_WINDOWPOSCHANGED] = list_[WM_WINDOWPOSCHANGING] = std::make_shared<messaging::position_dispatcher>(&target::on_position);
	list_[WM_SIZE] = list_[WM_SIZING] = std::make_shared<messaging::size_dispatcher>(&target::on_size);
	list_[WM_MOVE] = list_[WM_MOVING] = std::make_shared<messaging::move_dispatcher>(&target::on_move);

	list_[WM_NCMOUSEMOVE] = list_[WM_MOUSEMOVE] = std::make_shared<messaging::object_manager_handling_dispatcher>(&application::object_manager::handle_mouse_move);
	list_[WM_NCMOUSELEAVE] = list_[WM_MOUSELEAVE] = std::make_shared<messaging::object_manager_handling_dispatcher>(&application::object_manager::handle_mouse_leave);

	list_[WM_NCLBUTTONDOWN] = list_[WM_NCMBUTTONDOWN] = list_[WM_NCRBUTTONDOWN] = list_[WM_LBUTTONDOWN] = list_[WM_MBUTTONDOWN] = list_[WM_RBUTTONDOWN] =
		std::make_shared<messaging::object_manager_handling_dispatcher>(&application::object_manager::handle_mouse_down);

	list_[WM_NCLBUTTONUP] = list_[WM_NCMBUTTONUP] = list_[WM_NCRBUTTONUP] = list_[WM_LBUTTONUP] = list_[WM_MBUTTONUP] = list_[WM_RBUTTONUP] =
		std::make_shared<messaging::object_manager_handling_dispatcher>(&application::object_manager::handle_mouse_up);

	list_[WM_NCLBUTTONDBLCLK] = list_[WM_NCMBUTTONDBLCLK] = list_[WM_NCRBUTTONDBLCLK] = list_[WM_LBUTTONDBLCLK] = list_[WM_MBUTTONDBLCLK] = list_[WM_RBUTTONDBLCLK] =
		std::make_shared<messaging::object_manager_handling_dispatcher>(&application::object_manager::handle_mouse_dbl_click);

	list_[WINPP_WM_MOUSE] = list_[WINPP_WM_RAWMOUSE] = std::make_shared<messaging::mouse_dispatcher>();
}

winpp::messaging::map &winpp::messaging::map::add_entry(uint_type code, dispatcher_ptr_type dispatcher){
	list_[code] = dispatcher;
	return *this;
}

winpp::messaging::dispatcher *winpp::messaging::map::find_dispatcher(uint_type code){
	auto entry = list_.find(code);
	return (entry == list_.end()) ? unrecognized_dispatcher_.get() : entry->second.get();
}
