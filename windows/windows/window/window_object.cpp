#include "window_object.h"

winpp::window::object::event_tunnel::event_tunnel(gui_object_type &object)
	: close(id_()), maximize(id_()), minimize(id_()), restore(id_()), show(id_()), hide(id_()), erase_background(id_()), paint(id_()), timer(id_(), object), interval(id_(), object){}

winpp::window::object::event_tunnel::~event_tunnel() = default;

unsigned __int64 winpp::window::object::event_tunnel::bind(event_type e, const std::any &callback){
	switch (e){
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

bool winpp::window::object::event_tunnel::unbind(unsigned __int64 id){
	return (base_type::unbind(id) || close.unbind(id) || maximize.unbind(id) || minimize.unbind(id) || restore.unbind(id) || show.unbind(id) || hide.unbind(id) ||
		erase_background.unbind(id) || paint.unbind(id) || timer.unbind(id) || interval.unbind(id));
}

unsigned __int64 winpp::window::object::event_tunnel::bind_(const std::wstring &e, const std::any &callback){
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

winpp::window::object::~object(){
	destroy(true);
}

void *winpp::window::object::handle() const{
	return value_.native_value();
}

winpp::window::object &winpp::window::object::outer_rect(const rect_type &value){
	value_.position(value.top_left(), value.size(), position_type::no_z_order | position_type::no_activate, nullptr);
	return *this;
}

winpp::window::object::rect_type winpp::window::object::outer_rect() const{
	return value_.outer_rect();
}

winpp::window::object::rect_type winpp::window::object::relative_outer_rect() const{
	return (parent_ == nullptr) ? outer_rect() : parent_->convert_from_screen(outer_rect());
}

winpp::window::object &winpp::window::object::inner_rect(const rect_type &value){
	return outer_size(value_.adjust_rect(value).size());
}

winpp::window::object::rect_type winpp::window::object::inner_rect() const{
	return value_.inner_rect();
}

winpp::window::object &winpp::window::object::outer_size(const size_type &value){
	value_.position(point_type{}, value, position_type::no_move | position_type::no_z_order | position_type::no_activate, nullptr);
	return *this;
}

winpp::window::object &winpp::window::object::inner_size(const size_type &value){
	return outer_size(value_.adjust_rect(rect_type{ point_type{}, value }).size());
}

winpp::window::object &winpp::window::object::offset(const size_type &value){
	value_.position(relative_offset() + value, size_type{}, position_type::no_size | position_type::no_z_order | position_type::no_activate, nullptr);
	return *this;
}

winpp::window::object::point_type winpp::window::object::relative_offset() const{
	return (parent_ == nullptr) ? offset() : parent_->convert_from_screen(offset());
}

winpp::window::object &winpp::window::object::move(const point_type &value){
	if (parent_ != nullptr){
		auto absolute_value = dynamic_cast<const structures::absolute_point *>(&value);
		if (absolute_value != nullptr)
			value_.position(parent_->convert_from_screen(value), size_type{}, position_type::no_size | position_type::no_z_order | position_type::no_activate, nullptr);
		else//Not absolute
			value_.position(value, size_type{}, position_type::no_size | position_type::no_z_order | position_type::no_activate, nullptr);
	}
	else//No parent
		value_.position(value, size_type{}, position_type::no_size | position_type::no_z_order | position_type::no_activate, nullptr);

	return *this;
}

winpp::window::object &winpp::window::object::absolute_move(const point_type &value){
	return move(structures::absolute_point(value));
}

winpp::window::object::point_type winpp::window::object::convert_to_screen(const point_type &value) const{
	return value_.client_to_screen(value);
}

winpp::window::object::rect_type winpp::window::object::convert_to_screen(const rect_type &value) const{
	return value_.client_to_screen(value);
}

winpp::window::object::point_type winpp::window::object::convert_from_screen(const point_type &value) const{
	return value_.screen_to_client(value);
}

winpp::window::object::rect_type winpp::window::object::convert_from_screen(const rect_type &value) const{
	return value_.screen_to_client(value);
}

winpp::window::object &winpp::window::object::destroy(bool force){
	if (value_ == nullptr)
		return *this;

	value_.menu(nullptr);//Prevent destruction of attached menu
	value_.destroy();

	return *this;
}

winpp::window::object::event_tunnel &winpp::window::object::events(){
	return *dynamic_cast<event_tunnel *>(get_events_().get());
}

unsigned int winpp::window::object::group() const{
	return window_group;
}

bool winpp::window::object::is_created() const{
	return (value_ != nullptr);
}

winpp::window::object::procedure_type winpp::window::object::previous_procedure() const{
	return previous_procedure_;
}

winpp::gui::generic_object::events_type winpp::window::object::get_events_(){
	return create_events_<event_tunnel>(*this);
}

bool winpp::window::object::cache_group_(unsigned int value) const{
	return (value == non_window_group);
}
