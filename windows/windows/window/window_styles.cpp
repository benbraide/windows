#include "window_styles.h"
#include "window_object.h"

winpp::window::styles::styles(object &object, info_type &info)
	: object_(&object), info_(&info){
	update_info_ = update_info{ false, thread_id_type(), info_type{}, info_type{} };//Reset
}

winpp::window::styles::~styles() = default;

winpp::window::styles &winpp::window::styles::begin(){
	if (update_info_.active)
		return *this;

	auto app = object_->app();
	if (app != nullptr){
		auto thread = thread_id_type::current();
		app->execute_task([this, thread]{
			update_info_.active = true;
			update_info_.locking_thread = thread;
		});
	}
	else//No synchronization
		update_info_.active = true;

	return *this;
}

winpp::window::styles &winpp::window::styles::end(){
	if (!update_info_.active)
		return *this;

	auto app = object_->app();
	if (app != nullptr){
		auto thread = thread_id_type::current();
		app->execute_task([this, thread]{
			if (update_info_.locking_thread == thread){
				write_changes_();
				update_info_ = update_info{ false, thread_id_type(), info_type{}, info_type{} };//Reset
			}
		});
	}
	else{//No synchronization
		write_changes_();
		update_info_ = update_info{ false, thread_id_type(), info_type{}, info_type{} };//Reset
	}

	return *this;
}

winpp::window::styles &winpp::window::styles::cancel(){
	if (!update_info_.active)
		return *this;

	auto app = object_->app();
	if (app != nullptr){
		auto thread = thread_id_type::current();
		app->execute_task([this, thread]{
			if (update_info_.locking_thread == thread)
				update_info_ = update_info{ false, thread_id_type(), info_type{}, info_type{} };//Reset
		});
	}
	else//No synchronization
		update_info_ = update_info{ false, thread_id_type(), info_type{}, info_type{} };//Reset

	return *this;
}

bool winpp::window::styles::has_begun() const{
	return update_info_.active;
}

winpp::window::styles &winpp::window::styles::apply(dword_type value, bool enabled, bool extended){
	auto app = object_->app();
	if (app != nullptr){
		app->execute_task([this, value, extended, enabled]{
			update_(value, enabled, extended);
		});

		return *this;
	}

	return update_(value, enabled, extended);
}

winpp::window::styles &winpp::window::styles::apply_modal(){
	begin();
	apply(WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME | WS_OVERLAPPEDWINDOW, false, false);
	apply(WS_DLGFRAME | WS_POPUP | WS_SYSMENU | WS_CAPTION, true, false);
	apply(WS_EX_DLGMODALFRAME | WS_EX_STATICEDGE, true, true);
	end();

	return *this;
}

bool winpp::window::styles::has(dword_type value, bool extended) const{
	auto app = object_->app();
	if (app != nullptr){
		return app->execute_task<bool>([this, value, extended]{
			return has_(value, extended);
		});
	}

	return has_(value, extended);
}

winpp::window::styles &winpp::window::styles::dbl_click(bool enabled){
	return apply(CS_DBLCLKS, enabled, false);
}

bool winpp::window::styles::dbl_click() const{
	return has(CS_DBLCLKS, false);
}

winpp::window::styles &winpp::window::styles::drop_shadow(bool enabled){
	return apply(CS_DROPSHADOW, enabled, false);
}

bool winpp::window::styles::drop_shadow() const{
	return has(CS_DROPSHADOW, false);
}

winpp::window::styles &winpp::window::styles::horizontal_redraw(bool enabled){
	return apply(CS_HREDRAW, enabled, false);
}

bool winpp::window::styles::horizontal_redraw() const{
	return has(CS_HREDRAW, false);
}

winpp::window::styles &winpp::window::styles::vertical_redraw(bool enabled){
	return apply(CS_VREDRAW, enabled, false);
}

bool winpp::window::styles::vertical_redraw() const{
	return has(CS_VREDRAW, false);
}

winpp::window::styles &winpp::window::styles::close(bool enabled){
	return apply(CS_NOCLOSE, enabled, false);
}

bool winpp::window::styles::close() const{
	return !has(CS_NOCLOSE, false);
}

winpp::window::styles &winpp::window::styles::maximize(bool enabled){
	return apply(WS_MAXIMIZEBOX, enabled, false);
}

bool winpp::window::styles::maximize() const{
	return has(WS_MAXIMIZEBOX, false);
}

winpp::window::styles &winpp::window::styles::minimize(bool enabled){
	return apply(WS_MINIMIZEBOX, enabled, false);
}

bool winpp::window::styles::minimize() const{
	return has(WS_MINIMIZEBOX, false);
}

winpp::window::styles &winpp::window::styles::border(bool enabled){
	return apply(WS_BORDER, enabled, false);
}

bool winpp::window::styles::border() const{
	return has(WS_BORDER, false);
}

winpp::window::styles &winpp::window::styles::dialog_frame(bool enabled){
	return apply(WS_DLGFRAME, enabled, false);
}

bool winpp::window::styles::dialog_frame() const{
	return has(WS_DLGFRAME, false);
}

winpp::window::styles &winpp::window::styles::caption(bool enabled){
	return apply(WS_CAPTION, enabled, false);
}

bool winpp::window::styles::caption() const{
	return has(WS_CAPTION, false);
}

winpp::window::styles &winpp::window::styles::system_menu(bool enabled){
	return apply(WS_SYSMENU, enabled, false);
}

bool winpp::window::styles::system_menu() const{
	return has(WS_SYSMENU, false);
}

winpp::window::styles &winpp::window::styles::thick_frame(bool enabled){
	return apply(WS_THICKFRAME, enabled, false);
}

bool winpp::window::styles::thick_frame() const{
	return has(WS_THICKFRAME, false);
}

winpp::window::styles &winpp::window::styles::clip_children(bool enabled){
	return apply(WS_CLIPCHILDREN, enabled, false);
}

bool winpp::window::styles::clip_children() const{
	return has(WS_CLIPCHILDREN, false);
}

winpp::window::styles &winpp::window::styles::clip_siblings(bool enabled){
	return apply(WS_CLIPSIBLINGS, enabled, false);
}

bool winpp::window::styles::clip_siblings() const{
	return has(WS_CLIPSIBLINGS, false);
}

winpp::window::styles &winpp::window::styles::horizontal_scroll_bar(bool enabled){
	return apply(WS_HSCROLL, enabled, false);
}

bool winpp::window::styles::horizontal_scroll_bar() const{
	return has(WS_HSCROLL, false);
}

winpp::window::styles &winpp::window::styles::vertical_scroll_bar(bool enabled){
	return apply(WS_VSCROLL, enabled, false);
}

bool winpp::window::styles::vertical_scroll_bar() const{
	return has(WS_VSCROLL, false);
}

winpp::window::styles &winpp::window::styles::tab_stop(bool enabled){
	return apply(WS_TABSTOP, enabled, false);
}

bool winpp::window::styles::tab_stop() const{
	return has(WS_TABSTOP, false);
}

winpp::window::styles &winpp::window::styles::enabled(bool enabled){
	return apply(WS_DISABLED, !enabled, false);
}

bool winpp::window::styles::enabled() const{
	return !has(WS_DISABLED, false);
}

winpp::window::styles &winpp::window::styles::visible(bool enabled){
	return apply(WS_VISIBLE, enabled, false);
}

bool winpp::window::styles::visible() const{
	return has(WS_VISIBLE, false);
}

winpp::window::styles &winpp::window::styles::file_drop(bool enabled){
	return apply(WS_EX_ACCEPTFILES, enabled, true);
}

bool winpp::window::styles::file_drop() const{
	return has(WS_EX_ACCEPTFILES, true);
}

winpp::window::styles &winpp::window::styles::taskbar(bool enabled){
	return apply(WS_EX_APPWINDOW, enabled, true);
}

bool winpp::window::styles::taskbar() const{
	return has(WS_EX_APPWINDOW, true);
}

winpp::window::styles &winpp::window::styles::sunken_edge(bool enabled){
	return apply(WS_EX_CLIENTEDGE, enabled, true);
}

bool winpp::window::styles::sunken_edge() const{
	return has(WS_EX_CLIENTEDGE, true);
}

winpp::window::styles &winpp::window::styles::context_help(bool enabled){
	return apply(WS_EX_CONTEXTHELP, enabled, true);
}

bool winpp::window::styles::context_help() const{
	return has(WS_EX_CONTEXTHELP, true);
}

winpp::window::styles &winpp::window::styles::control_parent(bool enabled){
	return apply(WS_EX_CONTROLPARENT, enabled, true);
}

bool winpp::window::styles::control_parent() const{
	return has(WS_EX_CONTROLPARENT, true);
}

winpp::window::styles &winpp::window::styles::dialog_modal_frame(bool enabled){
	return apply(WS_EX_DLGMODALFRAME, enabled, true);
}

bool winpp::window::styles::dialog_modal_frame() const{
	return has(WS_EX_DLGMODALFRAME, true);
}

winpp::window::styles &winpp::window::styles::layered(bool enabled){
	return apply(WS_EX_LAYERED, enabled, true);
}

bool winpp::window::styles::layered() const{
	return has(WS_EX_LAYERED, true);
}

winpp::window::styles &winpp::window::styles::right_alignment(bool enabled){
	return apply(WS_EX_RIGHT, enabled, true);
}

bool winpp::window::styles::right_alignment() const{
	return has(WS_EX_RIGHT, true);
}

winpp::window::styles &winpp::window::styles::rtl_reading(bool enabled){
	return apply(WS_EX_RTLREADING, enabled, true);
}

bool winpp::window::styles::rtl_reading() const{
	return has(WS_EX_RTLREADING, true);
}

winpp::window::styles &winpp::window::styles::rtl_layout(bool enabled){
	return apply(WS_EX_LAYOUTRTL, enabled, true);
}

bool winpp::window::styles::rtl_layout() const{
	return has(WS_EX_LAYOUTRTL, true);
}

winpp::window::styles &winpp::window::styles::left_scroll_bar(bool enabled){
	return apply(WS_EX_LEFTSCROLLBAR, enabled, true);
}

bool winpp::window::styles::left_scroll_bar() const{
	return has(WS_EX_LEFTSCROLLBAR, true);
}

winpp::window::styles &winpp::window::styles::mouse_activation(bool enabled){
	return apply(WS_EX_NOACTIVATE, !enabled, true);
}

bool winpp::window::styles::mouse_activation() const{
	return !has(WS_EX_NOACTIVATE, true);
}

winpp::window::styles &winpp::window::styles::inheritable_layout(bool enabled){
	return apply(WS_EX_NOINHERITLAYOUT, !enabled, true);
}

bool winpp::window::styles::inheritable_layout() const{
	return !has(WS_EX_NOINHERITLAYOUT, true);
}

winpp::window::styles &winpp::window::styles::parent_notify(bool enabled){
	return apply(WS_EX_NOPARENTNOTIFY, !enabled, true);
}

bool winpp::window::styles::parent_notify() const{
	return !has(WS_EX_NOPARENTNOTIFY, true);
}

winpp::window::styles &winpp::window::styles::static_edge(bool enabled){
	return apply(WS_EX_STATICEDGE, enabled, true);
}

bool winpp::window::styles::static_edge() const{
	return has(WS_EX_STATICEDGE, true);
}

winpp::window::styles &winpp::window::styles::window_edge(bool enabled){
	return apply(WS_EX_WINDOWEDGE, enabled, true);
}

bool winpp::window::styles::window_edge() const{
	return has(WS_EX_WINDOWEDGE, true);
}

winpp::window::styles &winpp::window::styles::top_most(bool enabled){
	return apply(WS_EX_TOPMOST, enabled, true);
}

bool winpp::window::styles::top_most() const{
	return has(WS_EX_TOPMOST, true);
}

winpp::window::styles &winpp::window::styles::transparent(bool enabled){
	return apply(WS_EX_TRANSPARENT, enabled, true);
}

bool winpp::window::styles::transparent() const{
	return has_(WS_EX_TRANSPARENT, true);
}

winpp::window::styles::dword_type winpp::window::styles::retrieve(bool extended) const{
	return retrieve_(extended);
}

const winpp::window::styles::info_type &winpp::window::styles::enabled_changes() const{
	return update_info_.enabled;
}

const winpp::window::styles::info_type &winpp::window::styles::disabled_changes() const{
	return update_info_.disabled;
}

winpp::window::styles::hwnd_type winpp::window::styles::object_value_() const{
	return static_cast<hwnd_type::value_type>(object_->handle());
}

winpp::window::styles::dword_type winpp::window::styles::merge_changes_(bool extended) const{
	if (extended)
		return WINPP_REMOVE_V(WINPP_SET_V(retrieve_(true), update_info_.enabled.extended), update_info_.disabled.extended);
	return WINPP_REMOVE_V(WINPP_SET_V(retrieve_(false), update_info_.enabled.basic), update_info_.disabled.basic);
}

void winpp::window::styles::write_changes_(){
	final_write_(merge_changes_(false), false);
	final_write_(merge_changes_(true), true);
}

void winpp::window::styles::write_(dword_type value, bool extended, bool add){
	final_write_(add ? WINPP_SET_V(retrieve_(extended), value) : WINPP_REMOVE_V(retrieve_(extended), value), extended);
}

void winpp::window::styles::final_write_(dword_type value, bool extended){
	auto object_value = object_value_();
	if (extended){
		if (object_value == nullptr)
			info_->extended = object_->filter_styles(value, true);
		else//Valid handle
			object_value.data(value, data_index_type::extended_styles);
	}
	else if (object_value == nullptr)//Basic
		info_->basic = object_->filter_styles(value, false);
	else//Valid handle
		object_value.data(value, data_index_type::styles);
}

winpp::window::styles &winpp::window::styles::update_(dword_type value, bool enabled, bool extended){
	return enabled ? enable_(value, extended) : disable_(value, extended);
}

winpp::window::styles &winpp::window::styles::enable_(dword_type value, bool extended){
	if (update_info_.active){
		if (extended){
			WINPP_SET(update_info_.enabled.extended, value);
			WINPP_REMOVE(update_info_.disabled.extended, value);
		}
		else{//Basic
			WINPP_SET(update_info_.enabled.basic, value);
			WINPP_REMOVE(update_info_.disabled.basic, value);
		}
	}
	else//Write
		write_(value, extended, true);

	return *this;
}

winpp::window::styles &winpp::window::styles::disable_(dword_type value, bool extended){
	if (update_info_.active){
		if (extended){
			WINPP_SET(update_info_.disabled.extended, value);
			WINPP_REMOVE(update_info_.enabled.extended, value);
		}
		else{//Basic
			WINPP_SET(update_info_.disabled.basic, value);
			WINPP_REMOVE(update_info_.enabled.basic, value);
		}
	}
	else//Write
		write_(value, extended, false);

	return *this;
}

winpp::window::styles::dword_type winpp::window::styles::retrieve_(bool extended) const{
	auto object_value = object_value_();
	if (extended){
		if (object_value == nullptr)
			return info_->extended;

		return object_value.data<dword_type>(data_index_type::extended_styles);
	}

	if (object_value == nullptr)
		return info_->basic;

	return object_value.data<dword_type>(data_index_type::styles);
}

bool winpp::window::styles::has_(dword_type value, bool extended) const{
	if (update_info_.active)//Merge changes
		return WINPP_IS(merge_changes_(extended), value);
	return WINPP_IS(retrieve_(extended), value);
}
