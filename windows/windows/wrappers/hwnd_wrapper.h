#pragma once

#ifndef WINPP_HWND_WRAPPER_H
#define WINPP_HWND_WRAPPER_H

#include <string>

#include "../structures/enumerations.h"
#include "../structures/rect_structure.h"
#include "../structures/color_structure.h"

namespace winpp{
	namespace wrappers{
		template <class value_type>
		class basic_hwnd : public value<basic_hwnd<value_type>, value_type>{
		public:
			typedef ::LRESULT result_type;

			typedef ::WPARAM wparam_type;
			typedef ::LPARAM lparam_type;

			typedef ::LPVOID vptr_type;
			typedef ::LONG_PTR lptr_type;

			typedef ::UINT uint_type;
			typedef ::WORD word_type;
			typedef ::DWORD dword_type;

			typedef ::CREATESTRUCTW create_info_type;
			typedef ::WINDOWPLACEMENT placement_info_type;

			typedef ::HMENU menu_type;

			typedef structures::enumerations::animation_type animation_type;
			typedef structures::enumerations::show_mode show_mode;
			typedef structures::enumerations::position_type position_type;
			typedef structures::enumerations::data_index_type data_index_type;
			typedef structures::enumerations::layered_attributes_option layered_attributes_option;

			typedef structures::point point_type;
			typedef structures::size size_type;
			typedef structures::rect rect_type;
			typedef structures::color color_type;

			typedef value<basic_hwnd<value_type>, value_type> base_type;

			basic_hwnd(value_type value = nullptr)
				: basic_hwnd(value){}

			basic_hwnd(const basic_hwnd &) = default;

			basic_hwnd &operator =(const basic_hwnd &) = default;

			bool create(const create_info_type &info, basic_hwnd parent){
				return ((base_type::value_ = ::CreateWindowExW(
					info.dwExStyle,
					info.lpszClass,
					info.lpszName,
					info.style,
					info.x,
					info.y,
					info.cx,
					info.cy,
					parent,
					info.hMenu,
					info.hInstance,
					info.lpCreateParams
				)) != nullptr);
			}

			template <typename params_type>
			bool create(const create_info_type &info, params_type params, value_type parent){
				return ((base_type::value_ = ::CreateWindowExW(
					info.dwExStyle,
					info.lpszClass,
					info.lpszName,
					info.style,
					info.x,
					info.y,
					info.cx,
					info.cy,
					parent,
					info.hMenu,
					info.hInstance,
					(vptr_type)params
				)) != nullptr);
			}

			bool destroy(){
				return (::DestroyWindow(base_type::value_) != FALSE);
			}

			bool menu(menu_type value){
				return (::SetMenu(base_type::value_, value) != FALSE);
			}

			menu_type menu() const{
				return ::GetMenu(base_type::value_);
			}

			bool invalidate(const rect_type &rect, bool erase = true){
				return (::InvalidateRect(base_type::value_, rect, erase ? TRUE : FALSE) != FALSE);
			}

			bool invalidate(bool erase = true){
				return (::InvalidateRect(base_type::value_, nullptr, erase ? TRUE : FALSE) != FALSE);
			}

			bool validate(const rect_type &rect){
				return (::ValidateRect(base_type::value_, rect) != FALSE);
			}

			bool validate(){
				return (::ValidateRect(base_type::value_, nullptr) != FALSE);
			}

			bool enable(bool enable = true){
				return (::EnableWindow(base_type::value_, enable ? TRUE : FALSE) != FALSE);
			}

			bool minimize(){
				return (::CloseWindow(base_type::value_) != FALSE);
			}

			bool maximize(){
				return show(show_mode::maximize);
			}

			bool restore_minimized(){
				return (::OpenIcon(base_type::value_) != FALSE);
			}

			bool show(show_mode mode){
				return (::ShowWindow(base_type::value_, static_cast<int>(mode)) != FALSE);
			}

			bool show_asyn(show_mode mode){
				return (::ShowWindowAsync(base_type::value_, static_cast<int>(mode)) != FALSE);
			}

			bool animate(animation_type type, dword_type delay = 200){
				return (::AnimateWindow(base_type::value_, delay, static_cast<dword_type>(type)) != FALSE);
			}

			bool move(const point_type &offset, const size_type &size, bool repaint){
				return (::MoveWindow(base_type::value_, offset.x(), offset.y(), size.width(), size.height(), repaint ? TRUE : FALSE) != FALSE);
			}

			bool position(const point_type &offset, const size_type &size, position_type type, value_type after){
				return (::SetWindowPos(base_type::value_, after, offset.x(), offset.y(), size.width(), size.height(), static_cast<uint_type>(type)) != FALSE);
			}

			bool place(const placement_info_type &info){
				return (::SetWindowPlacement(base_type::value_, &info) != FALSE);
			}

			bool bring_to_top(){
				return (::BringWindowToTop(base_type::value_) != FALSE);
			}

			bool update(){
				return (::UpdateWindow(base_type::value_) != FALSE);
			}

			void switch_to_this(bool cause_is_alt_tab = false){
				::SwitchToThisWindow(base_type::value_, cause_is_alt_tab ? TRUE : FALSE);
			}

			bool make_foreground(){
				return (::SetForegroundWindow(base_type::value_) != FALSE);
			}

			value_type make_active(){
				return ::SetActiveWindow(base_type::value_);
			}

			bool draw_menu_bar(){
				return (::DrawMenuBar(base_type::value_) != FALSE);
			}

			bool set_layered_attributes(const color_type &color, layered_attributes_option options = layered_attributes_option::all){
				return (::SetLayeredWindowAttributes(base_type::value_, color.rgb(), color.alpha(), static_cast<dword_type>(options)) != FALSE);
			}

			value_type parent(value_type parent){
				return ::SetParent(base_type::value_, parent);
			}

			value_type parent() const{
				return ::GetParent(base_type::value_);
			}

			bool text(const std::wstring &value){
				return (::SetWindowTextW(base_type::value_, value.c_str()) != FALSE);
			}

			std::wstring text() const{
				auto size = text_length();
				if (size <= 0)
					return L"";

				std::wstring value;
				value.resize(size);

				if (::GetWindowTextW(base_type::value_, const_cast<wchar_t *>(value.c_str()), static_cast<int>(value.size())) == FALSE)
					return L"";

				return value;
			}

			int text_length() const{
				return ::GetWindowTextLengthW(base_type::value_);
			}

			template <typename value_type>
			value_type data(const value_type &data, data_index index = data_index::user_data){
				if (base_type::value_ == nullptr)
					return value_type();
				return (value_type)::SetWindowLongPtrW(base_type::value_, static_cast<int>(index), (lptr_type)data);
			}

			template <typename value_type>
			value_type data(data_index index = data_index::user_data) const{
				if (base_type::value_ == nullptr)
					return value_type();
				return (value_type)::GetWindowLongPtrW(base_type::value_, static_cast<int>(index));
			}

			rect_type update_rect() const{
				rect_type value;
				::GetUpdateRect(base_type::value_, value, false);
				return value;
			}

			rect_type outer_rect() const{
				rect_type value;
				::GetWindowRect(base_type::value_, value);
				return value;
			}

			rect_type inner_rect() const{
				rect_type value;
				::GetClientRect(base_type::value_, value);
				return value;
			}

			point_type client_to_screen(point_type value) const{
				::ClientToScreen(base_type::value_, value);
				return value;
			}

			rect_type client_to_screen(rect_type value) const{
				auto top_left = value.top_left();
				auto computed_top_left = client_to_screen(top_left);
				return rect_type{ computed_top_left, value.bottom_right() + (computed_top_left - top_left) };
			}

			point_type screen_to_client(point_type value) const{
				::ScreenToClient(base_type::value_, value);
				return value;
			}

			rect_type screen_to_client(rect_type value) const{
				auto top_left = value.top_left();
				auto computed_top_left = screen_to_client(top_left);
				return rect_type{ computed_top_left, value.bottom_right() + (computed_top_left - top_left) };
			}

			rect_type adjust_rect(const rect_type &value) const{
				return adjust_rect(value, get_data<dword_type>(data_index::styles), get_data<dword_type>(data_index::extended_styles), menu() != nullptr);
			}

			bool is_visible() const{
				return (::IsWindowVisible(base_type::value_) != FALSE);
			}

			bool is_enabled() const{
				return (::IsWindowEnabled(base_type::value_) != FALSE);
			}

			bool is_maximized() const{
				return (::IsZoomed(base_type::value_) != FALSE);
			}

			bool is_minimized() const{
				return (::IsIconic(base_type::value_) != FALSE);
			}

			bool is_frozen() const{
				return (::IsHungAppWindow(base_type::value_) != FALSE);
			}

			bool is_foreground() const{
				return (::GetForegroundWindow() == base_type::value_);
			}

			template <typename return_type = result_type, typename arg_wparam_type = wparam_type, typename arg_lparam_type = lparam_type>
			return_type send_message(uint_type msg, const arg_wparam_type &wparam, const arg_lparam_type &lparam) const{
				if (base_type::value_ == nullptr)
					return return_type();
				return (return_type)::SendMessageW(base_type::value_, msg, (wparam_type)wparam, (lparam_type)lparam);
			}

			template <typename arg_wparam_type = wparam_type, typename arg_lparam_type = lparam_type>
			bool post_message(uint_type msg, const arg_wparam_type &wparam, const arg_lparam_type &lparam) const{
				return (base_type::value_ != nullptr && ::PostMessageW(base_type::value_, msg, (wparam_type)wparam, (lparam_type)lparam) != FALSE);
			}

			static value_type active(){
				return ::GetActiveWindow();
			}

			static rect_type adjust_rect(rect_type value, dword_type styles, dword_type extended_styles, bool has_menu){
				::AdjustWindowRectEx(value, styles, has_menu ? TRUE : FALSE, extended_styles);
				return value;
			}
		};

		typedef basic_hwnd<::HWND> hwnd;
	}
}

#endif /* !WINPP_HWND_WRAPPER_H */
