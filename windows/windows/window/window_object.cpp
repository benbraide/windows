#include "window_object.h"

winpp::window::object::event_tunnel::event_tunnel(gui_object_type &object)
	: timer(object), interval(object){}

winpp::window::object::event_tunnel::~event_tunnel() = default;

unsigned __int64 winpp::window::object::event_tunnel::bind(event_type e, const std::any &callback){
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

unsigned __int64 winpp::window::object::event_tunnel::bind_(const std::wstring &e, const std::any &callback){
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

winpp::window::object::object(procedure_type procedure)
	: procedure_(procedure){}

winpp::window::object::~object(){
	destroy(force_type::force);
}

winpp::window::object::operator hwnd_value_type() const{
	return value_;
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

winpp::window::object &winpp::window::object::destroy(force_type force){
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

bool winpp::window::object::is_top_level() const{
	return !has_parent();
}

bool winpp::window::object::is_window() const{
	return true;
}

winpp::window::object::procedure_type winpp::window::object::procedure() const{
	return procedure_;
}

bool winpp::window::object::pre_translate(msg_type &msg){
	if (!is_dialog())
		return target::pre_translate(msg);

	auto code = msg.code();
	if (code < WM_KEYFIRST || code > WM_KEYLAST)
		return target::pre_translate(msg);

	return (::IsDialogMessageW(value_, msg) == FALSE) ? target::pre_translate(msg) : true;
}

winpp::window::object::styles_type &winpp::window::object::styles(){
	return *get_styles_();
}

winpp::window::object::dword_type winpp::window::object::filter_styles(dword_type value, bool is_extended) const{
	auto black_listed = WINPP_REMOVE_V(black_listed_styles(is_extended), white_listed_styles(is_extended));
	return WINPP_REMOVE_V(value, black_listed);
}

winpp::window::object::dword_type winpp::window::object::white_listed_styles(bool is_extended) const{
	return (is_extended ? 0 : (CS_DBLCLKS | CS_DROPSHADOW | WS_BORDER | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_TABSTOP | WS_DISABLED | WS_VISIBLE));
}

winpp::window::object::dword_type winpp::window::object::black_listed_styles(bool is_extended) const{
	return (is_extended ? WS_EX_LEFTSCROLLBAR : (WS_HSCROLL | WS_VSCROLL));
}

winpp::gui::generic_object::events_type winpp::window::object::get_events_(){
	return create_events_<event_tunnel>(*this);
}

bool winpp::window::object::cache_group_(unsigned int value) const{
	return (value == non_window_group);
}

winpp::messaging::target *winpp::window::object::target_parent_() const{
	return dynamic_cast<target *>(parent_);
}

void *winpp::window::object::find_event_(){
	switch (msg_.code()){
	case WM_NCCREATE:
		return &events().pre_create;
	case WM_CREATE:
		return &events().create;
	case WM_DESTROY:
		return &events().destroy;
	case WM_NCDESTROY:
		return &events().post_destroy;
	default:
		break;
	}

	return nullptr;
}

winpp::window::object::styles_ptr_type winpp::window::object::get_styles_(){
	return create_styles_<styles_type>(*this, persistent_styles_);
}

void winpp::window::object::reset_persistent_styles_(){
	persistent_styles_ = styles_info_type{ WS_CLIPCHILDREN | WS_CLIPSIBLINGS };
}

void winpp::window::object::create_(const std::wstring &caption, const point_type &offset, const size_type &size, dword_type styles, dword_type extended_styles, const wchar_t *class_name, app_type *app){
	point_type computed_offset;
	hwnd_value_type parent_handle;

	if (parent_ != nullptr){
		auto absolute_offset = dynamic_cast<const absolute_point_type *>(&offset);
		if (absolute_offset == nullptr)//Relative offset
			computed_offset = offset;
		else//Convert absolute to relative
			computed_offset = parent_->convert_from_screen(offset);

		parent_handle = static_cast<hwnd_value_type>(parent_->handle());
		WINPP_SET(styles, WS_CHILD);//Set child flag
	}
	else{//No parent
		computed_offset = offset;
		parent_handle = nullptr;
	}

	WINPP_SET(styles, persistent_styles_.basic);
	WINPP_SET(extended_styles, persistent_styles_.extended);

	create_(create_info_type{
		static_cast<gui_object_type *>(this),		//Params
		nullptr,									//Instance
		nullptr,									//Menu
		parent_handle,								//Parent
		size.height(),								//Height
		size.width(),								//Width
		computed_offset.y(),						//y-offset
		computed_offset.x(),						//x-offset
		static_cast<long>(styles),					//Styles
		caption.c_str(),							//Window name
		class_name,									//Class name
		extended_styles								//Extended styles
	}, app);
}

void winpp::window::object::create_(const create_info_type &info, app_type *app){
	if (is_created())
		throw common::unsupported_exception();

	if ((value_ = (app_ = ((app == nullptr) ? scope_app_ : app))->object_manager().create(info)) == nullptr){
		if (parent_ != nullptr){
			parent_->internal_remove_child(*this, force_type::force);
			parent_ = nullptr;
		}

		if (::GetLastError() != ERROR_SUCCESS)
			throw common::windows_error();
	}
	else
		created_();
}
