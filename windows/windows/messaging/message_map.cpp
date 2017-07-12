#include "message_map.h"

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
}

winpp::messaging::map &winpp::messaging::map::add_entry(uint_type code, dispatcher_ptr_type dispatcher){
	list_[code] = dispatcher;
	return *this;
}

winpp::messaging::dispatcher *winpp::messaging::map::find_dispatcher(uint_type code){
	auto entry = list_.find(code);
	return (entry == list_.end()) ? unrecognized_dispatcher_.get() : entry->second.get();
}
