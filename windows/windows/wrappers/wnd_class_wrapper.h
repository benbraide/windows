#pragma once

#ifndef WINPP_WND_CLASS_WRAPPER_H
#define WINPP_WND_CLASS_WRAPPER_H

#include <string>

#include "../common/common_headers.h"
#include "value_wrapper.h"

namespace winpp{
	namespace wrappers{
		template <class value_type>
		class basic_wnd_class : public value<basic_wnd_class<value_type>, value_type>{
		public:
			typedef value<basic_wnd_class<value_type>, value_type> base_type;

			typedef ::WNDPROC procedure_type;
			typedef ::HINSTANCE instance_type;

			typedef ::UINT uint_type;
			typedef ::ATOM atom_type;

			typedef ::HICON icon_type;
			typedef ::HCURSOR cursor_type;
			typedef ::HBRUSH brush_type;

			basic_wnd_class()
				: base_type(value_type{}){
				base_type::value_.style = (CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS);
			}

			~basic_wnd_class(){
				destroy();
			}

			operator const wchar_t *() const{
				return base_type::value_.lpszClassName;
			}

			operator atom_type() const{
				return id_;
			}

			basic_wnd_class &name(const std::wstring &value){
				base_type::value_.lpszClassName = (name_ = value).c_str();
				return *this;
			}

			const std::wstring &name() const{
				return name_;
			}

			basic_wnd_class &styles(uint_type value){
				base_type::value_.style = value;
				return *this;
			}

			uint_type styles() const{
				return base_type::value_.style;
			}

			basic_wnd_class &procedure(procedure_type value){
				base_type::value_.lpfnWndProc = value;
				return *this;
			}

			procedure_type procedure() const{
				return base_type::value_.lpfnWndProc;
			}

			basic_wnd_class &instance(instance_type value){
				base_type::value_.hInstance = value;
				return *this;
			}

			instance_type instance() const{
				return base_type::value_.hInstance;
			}

			basic_wnd_class &background_brush(brush_type value){
				base_type::value_.hbrBackground = value;
				return *this;
			}

			brush_type background_brush() const{
				return base_type::value_.hbrBackground;
			}

			basic_wnd_class &cursor(cursor_type value){
				base_type::value_.hCursor = value;
				return *this;
			}

			cursor_type cursor() const{
				return base_type::value_.hCursor;
			}

			basic_wnd_class &icon(icon_type value){
				base_type::value_.hIcon = value;
				return *this;
			}

			icon_type icon() const{
				return base_type::value_.hIcon;
			}

			basic_wnd_class &small_icon(icon_type value){
				base_type::value_.hIconSm = value;
				return *this;
			}

			icon_type small_icon() const{
				return base_type::value_.hIconSm;
			}

			basic_wnd_class &menu(const std::wstring &value){
				base_type::value_.lpszClassName = (menu_ = value).c_str();
				return *this;
			}

			const std::wstring &menu() const{
				return menu_;
			}

			virtual bool create(){
				return (id_ == static_cast<atom_type>(0) && (id_ = ::RegisterClassExW(&base_type::value_)) != static_cast<atom_type>(0));
			}

			virtual bool destroy(){
				if (id_ != static_cast<atom_type>(0) && ::UnregisterClassW(base_type::value_.lpszClassName, base_type::value_.hInstance) != FALSE){
					id_ = 0u;
					base_type::value_ = value_type{ sizeof(value_type) };
				}

				return (id_ == static_cast<atom_type>(0));
			}

			virtual bool is_created() const{
				return (id_ != static_cast<atom_type>(0));
			}

		private:
			atom_type id_;
			std::wstring name_;
			std::wstring menu_;
		};
	}
}

#endif /* !WINPP_WND_CLASS_WRAPPER_H */
