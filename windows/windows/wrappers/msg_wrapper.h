#pragma once

#ifndef WINPP_MSG_WRAPPER_H
#define WINPP_MSG_WRAPPER_H

#include "value_wrapper.h"

#include "../structures/enumerations.h"
#include "../structures/point_structure.h"

namespace winpp{
	namespace wrappers{
		class msg : public value<msg, ::MSG>{
		public:
			typedef value<msg, ::MSG> base_type;

			typedef ::LRESULT result_type;
			typedef ::WPARAM wparam_type;
			typedef ::LPARAM lparam_type;

			typedef ::UINT uint_type;
			typedef ::WORD word_type;
			typedef ::DWORD dword_type;

			typedef ::HWND hwnd_type;

			msg();

			msg(const value_type &value);

			bool translate() const;

			result_type dispatch() const;

			result_type send() const;

			msg &code(uint_type value);

			uint_type code() const;

			msg &owner(hwnd_type value);

			hwnd_type owner() const;

			template <typename value_type>
			msg &wparam(value_type value){
				value_.wParam = (wparam_type)value;
				return *this;
			}

			template <typename value_type = wparam_type>
			value_type wparam() const{
				return (value_type)value_.wParam;
			}

			template <typename value_type>
			msg &lparam(value_type value){
				value_.wParam = (lparam_type)value;
				return *this;
			}

			template <typename value_type = lparam_type>
			value_type lparam() const{
				return (value_type)value_.lParam;
			}

			template <typename value_type = word_type>
			value_type low_wparam() const{
				return (value_type)LOWORD(value_.wParam);
			}

			template <typename value_type = word_type>
			value_type high_wparam() const{
				return (value_type)HIWORD(value_.wParam);
			}

			template <typename value_type = word_type>
			value_type low_lparam() const{
				return (value_type)LOWORD(value_.lParam);
			}

			template <typename value_type = word_type>
			value_type high_lparam() const{
				return (value_type)HIWORD(value_.lParam);
			}

			msg &time(dword_type value);

			dword_type time() const;

			msg &mouse_position(const structures::point &value);

			structures::point mouse_position() const;

			bool is_quit() const;

			bool is_dialog(hwnd_type target);
		};
	}
}

#endif /* !WINPP_MSG_WRAPPER_H */
