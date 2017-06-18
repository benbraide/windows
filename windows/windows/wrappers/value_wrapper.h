#pragma once

#ifndef WINPP_VALUE_WRAPPER_H
#define WINPP_VALUE_WRAPPER_H

#include <type_traits>

namespace winpp{
	namespace wrappers{
		template <class object_type, class value_type>
		class value{
		public:
			typedef object_type object_type;
			typedef value_type value_type;

			typedef std::conditional_t<std::is_object_v<value_type>, value_type &, value_type> non_const_qualified_type;
			typedef std::conditional_t<std::is_object_v<value_type>, const value_type &, value_type> const_qualified_type;

			explicit value(const_qualified_type value)
				: value_(value){}

			operator non_const_qualified_type(){
				return value_;
			}

			template <typename unused_type = value_type>
			operator std::enable_if_t<std::is_object_v<unused_type>, const_qualified_type>() const{
				return value_;
			}

			template <typename unused_type = value_type>
			operator std::enable_if_t<std::is_object_v<unused_type>, value_type *>(){
				return &value_;
			}

			template <typename unused_type = value_type>
			operator std::enable_if_t<std::is_object_v<unused_type>, const value_type *>() const{
				return &value_;
			}

			value &operator =(const_qualified_type value){
				value_ = value;
				return *this;
			}

			bool operator <(const_qualified_type value) const{
				return (static_cast<const object_type *>(this)->compare(value) < 0);
			}

			bool operator <=(const_qualified_type value) const{
				return (*this < value || *this == value);
			}

			bool operator ==(const_qualified_type value) const{
				return (static_cast<const object_type *>(this)->compare(value) == 0);
			}

			bool operator !=(const_qualified_type value) const{
				return !(*this == value);
			}

			bool operator >=(const_qualified_type value) const{
				return !(*this < value);
			}

			bool operator >(const_qualified_type value) const{
				return !(*this < value || *this == value);
			}

			bool operator <(const object_type &value) const{
				return (static_cast<const object_type *>(this)->compare(value) < 0);
			}

			bool operator <=(const object_type &value) const{
				return (*this < value || *this == value);
			}

			bool operator ==(const object_type &value) const{
				return (static_cast<const object_type *>(this)->compare(value) == 0);
			}

			bool operator !=(const object_type &value) const{
				return !(*this == value);
			}

			bool operator >=(const object_type &value) const{
				return !(*this < value);
			}

			bool operator >(const object_type &value) const{
				return !(*this < value || *this == value);
			}

			template <typename unused_type = value_type>
			std::enable_if_t<std::is_pointer_v<unused_type>, bool> operator ==(std::nullptr_t) const{
				return (value_ == nullptr);
			}

			template <typename unused_type = value_type>
			std::enable_if_t<std::is_pointer_v<unused_type>, bool> operator !=(std::nullptr_t) const{
				return (value_ != nullptr);
			}

			friend bool operator <(const_qualified_type lhs, const object_type &rhs){
				return !(rhs <= lhs);
			}

			friend bool operator <=(const_qualified_type lhs, const object_type &rhs){
				return (lhs < rhs || lhs == rhs);
			}

			friend bool operator ==(const_qualified_type lhs, const object_type &rhs){
				return (rhs == lhs);
			}

			friend bool operator !=(const_qualified_type lhs, const object_type &rhs){
				return !(lhs == rhs);
			}

			friend bool operator >=(const_qualified_type lhs, const object_type &rhs){
				return !(lhs < rhs);
			}

			friend bool operator >(const_qualified_type lhs, const object_type &rhs){
				return !(lhs < rhs || lhs == rhs);
			}

			template <typename unused_type = value_type>
			friend std::enable_if_t<std::is_pointer_v<unused_type>, bool> operator ==(std::nullptr_t, const object_type &rhs){
				return (rhs == nullptr);
			}

			template <typename unused_type = value_type>
			friend std::enable_if_t<std::is_pointer_v<unused_type>, bool> operator !=(std::nullptr_t, const object_type &rhs){
				return (rhs != nullptr);
			}

			const_qualified_type native_value() const{
				return value_;
			}

		protected:
			value_type value_;
		};
	}
}

#endif /* !WINPP_VALUE_WRAPPER_H */
