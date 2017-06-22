#pragma once

#ifndef WINPP_RECT_STRUCTURE_H
#define WINPP_RECT_STRUCTURE_H

#include "../common/common_methods.h"

#include "enumerations.h"
#include "size_structure.h"
#include "point_structure.h"

namespace winpp{
	namespace structures{
		template <class value_type>
		struct default_rect_native_value{
			value_type left;
			value_type top;
			value_type right;
			value_type bottom;
		};

		template <class point_type, class size_type, class value_type, class field_type = decltype(value_type::left)>
		class basic_rect : public wrappers::value<basic_rect<point_type, size_type, value_type, field_type>, value_type>{
		public:
			typedef wrappers::value<basic_rect<point_type, size_type, value_type, field_type>, value_type> base_type;
			typedef enumerations::placement_type placement_type;

			typedef value_type value_type;
			typedef field_type field_type;

			typedef point_type point_type;
			typedef size_type size_type;

			basic_rect()
				: base_type(value_type{}){}

			basic_rect(field_type left, field_type top, field_type right, field_type bottom)
				: base_type(value_type{ left, top, right, bottom }){}

			basic_rect(field_type left, field_type top_bottom, field_type right)
				: base_type(value_type{ left, top_bottom, right, top_bottom }){}

			basic_rect(field_type left_right, field_type top_bottom)
				: base_type(value_type{ left_right, top_bottom, left_right, top_bottom }){}

			basic_rect(field_type value)
				: base_type(value_type{ value, value, value, value }){}

			basic_rect(const point_type &top_left, const point_type &bottom_right)
				: base_type(value_type{ top_left.x(), top_left.y(), bottom_right.x(), bottom_right.y() }){}

			basic_rect(const point_type &top_left, const size_type &size, placement_type origin = placement_type::unspecified)
				: base_type(value_type{ top_left.x(), top_left.y(), top_left.x() + size.width(), top_left.y() + size.height() }){
				if (origin != placement_type::unspecified)//Apply placement
					common::methods::apply_placement(origin, base_type::value_, size);
			}

			basic_rect(const value_type &value)
				: base_type(value){}

			basic_rect(const basic_rect &value)
				: base_type(value.value_){}

			basic_rect(basic_rect &&value)
				: base_type(value.value_){
				value.value_ = {};
			}

			basic_rect &operator =(const value_type &value){
				base_type::value_ = value;
				return *this;
			}

			basic_rect &operator =(const basic_rect &value){
				base_type::value_ = value.value_;
				return *this;
			}

			basic_rect &operator =(basic_rect &&value){
				base_type::value_ = value.value_;
				value.value_ = {};
				return *this;
			}

			basic_rect operator +(const size_type &rhs) const{
				basic_rect copy(*this);
				return (copy += rhs);
			}

			basic_rect operator -(const size_type &rhs) const{
				basic_rect copy(*this);
				return (copy -= rhs);
			}

			basic_rect &operator +=(const size_type &rhs){
				return offset(rhs);
			}

			basic_rect &operator -=(const size_type &rhs){
				return offset(-rhs);
			}

			basic_rect &operator()(field_type left, field_type top, field_type right, field_type bottom){
				base_type::value_ = value_type{ left, top, right, bottom };
				return *this;
			}

			basic_rect &operator()(field_type left, field_type top_bottom, field_type right){
				base_type::value_ = value_type{ left, top_bottom, right, top_bottom };
				return *this;
			}

			basic_rect &operator()(field_type left_right, field_type top_bottom){
				base_type::value_ = value_type{ left_right, top_bottom, left_right, top_bottom };
				return *this;
			}

			basic_rect &operator()(field_type value){
				base_type::value_ = value_type{ value, value, value, value };
				return *this;
			}

			basic_rect &operator()(const point_type &top_left, const point_type &bottom_right){
				base_type::value_ = value_type{ top_left.x(), top_left.y(), bottom_right.x(), bottom_right.y() };
				return *this;
			}

			basic_rect &operator()(const point_type &top_left, const size_type &size, placement_type origin = placement_type::unspecified){
				base_type::value_ = value_type{ top_left.x(), top_left.y(), top_left.x() + size.width(), top_left.y() + size.height() };
				if (origin != placement_type::unspecified)//Apply placement
					common::methods::apply_placement(origin, base_type::value_, size);
				return *this;
			}

			basic_rect &operator ()(const basic_rect &value){
				return (*this = value);
			}

			basic_rect &operator ()(basic_rect &&value){
				return (*this = std::move(value));
			}

			basic_rect &normalize(){
				if (base_type::value_.right < base_type::value_.left)
					std::swap(base_type::value_.left, base_type::value_.right);

				if (base_type::value_.bottom < base_type::value_.top)
					std::swap(base_type::value_.top, base_type::value_.bottom);

				return *this;
			}

			basic_rect &left(field_type value){
				base_type::value_.left = value;
				return *this;
			}

			field_type left() const{
				return base_type::value_.left;
			}

			basic_rect &top(field_type value){
				base_type::value_.top = value;
				return *this;
			}

			field_type top() const{
				return base_type::value_.top;
			}

			basic_rect &right(field_type value){
				base_type::value_.right = value;
				return *this;
			}

			field_type right() const{
				return base_type::value_.right;
			}

			basic_rect &bottom(field_type value){
				base_type::value_.bottom = value;
				return *this;
			}

			field_type bottom() const{
				return base_type::value_.bottom;
			}

			basic_rect &top_left(const point_type &value){
				base_type::value_.left = value.x();
				base_type::value_.top = value.y();
				return *this;
			}

			point_type top_left() const{
				return point_type{ base_type::value_.left, base_type::value_.top };
			}

			basic_rect &top_right(const point_type &value){
				base_type::value_.right = value.x();
				base_type::value_.top = value.y();
				return *this;
			}

			point_type top_right() const{
				return point_type{ base_type::value_.right, base_type::value_.top };
			}

			basic_rect &bottom_right(const point_type &value){
				base_type::value_.right = value.x();
				base_type::value_.bottom = value.y();
				return *this;
			}

			point_type bottom_right() const{
				return point_type{ base_type::value_.right, base_type::value_.bottom };
			}

			basic_rect &bottom_left(const point_type &value){
				base_type::value_.left = value.x();
				base_type::value_.bottom = value.y();
				return *this;
			}

			point_type bottom_left() const{
				return point_type{ base_type::value_.left, base_type::value_.bottom };
			}

			basic_rect &size(const size_type &value){
				base_type::value_.right = (base_type::value_.left + value.width());
				base_type::value_.bottom = (base_type::value_.top + value.height());
				return *this;
			}

			size_type size() const{
				return size_type{ base_type::value_.right - base_type::value_.left, base_type::value_.bottom - base_type::value_.top };
			}

			size_type sum() const{
				return size_type{ base_type::value_.right + base_type::value_.left, base_type::value_.bottom + base_type::value_.top };
			}

			basic_rect &inflate(const size_type &value){
				base_type::value_.left -= value.width();
				base_type::value_.top -= value.height();
				base_type::value_.right += value.width();
				base_type::value_.bottom += value.height();
				return *this;
			}

			basic_rect inflated(const size_type &value) const{
				basic_rect copy(*this);
				return copy.inflate(value);
			}

			basic_rect &inflate(const basic_rect &value){
				base_type::value_.left -= value.value_.left;
				base_type::value_.top -= value.value_.top;
				base_type::value_.right += value.value_.right;
				base_type::value_.bottom += value.value_.bottom;
				return *this;
			}

			basic_rect inflated(const basic_rect &value) const{
				basic_rect copy(*this);
				return copy.inflate(value);
			}

			basic_rect &deflate(const size_type &value){
				base_type::value_.left += value.width();
				base_type::value_.top += value.height();
				base_type::value_.right -= value.width();
				base_type::value_.bottom -= value.height();
				return *this;
			}

			basic_rect deflated(const size_type &value) const{
				basic_rect copy(*this);
				return copy.deflate(value);
			}

			basic_rect &deflate(const basic_rect &value){
				base_type::value_.left += value.value_.left;
				base_type::value_.top += value.value_.top;
				base_type::value_.right -= value.value_.right;
				base_type::value_.bottom -= value.value_.bottom;
				return *this;
			}

			basic_rect deflated(const basic_rect &value) const{
				basic_rect copy(*this);
				return copy.deflate(value);
			}

			basic_rect &offset(const size_type &value){
				base_type::value_.left += value.width();
				base_type::value_.top += value.height();
				base_type::value_.right += value.width();
				base_type::value_.bottom += value.height();
				return *this;
			}

			basic_rect after_offset(const size_type &value) const{
				basic_rect copy(*this);
				return copy.offset(value);
			}

			basic_rect &move(const point_type &value){
				return offset(value - top_left());
			}

			basic_rect moved(const point_type &value) const{
				basic_rect copy(*this);
				return copy.move(value);
			}

			basic_rect &intersect(const basic_rect &value){
				return (*this = intersection(value));
			}

			basic_rect intersection(const basic_rect &value) const{
				return basic_rect{
					(base_type::value_.left > value.value_.left) ? base_type::value_.left : value.value_.left,
					(base_type::value_.top > value.value_.top) ? base_type::value_.top : value.value_.top,
					(base_type::value_.right < value.value_.right) ? base_type::value_.right : value.value_.right,
					(base_type::value_.bottom < value.value_.bottom) ? base_type::value_.bottom : value.value_.bottom
				};
			}

			basic_rect &combine(const basic_rect &value){
				return (*this = combination(value));
			}

			basic_rect combination(const basic_rect &value) const{
				return basic_rect{
					(base_type::value_.left < value.value_.left) ? base_type::value_.left : value.value_.left,
					(base_type::value_.top < value.value_.top) ? base_type::value_.top : value.value_.top,
					(base_type::value_.right > value.value_.right) ? base_type::value_.right : value.value_.right,
					(base_type::value_.bottom > value.value_.bottom) ? base_type::value_.bottom : value.value_.bottom
				};
			}

			bool is_empty() const{
				return (base_type::value_.left >= base_type::value_.right || base_type::value_.top >= base_type::value_.bottom);
			}

			bool is_normalized() const{
				return (base_type::value_.left <= base_type::value_.right && base_type::value_.top <= base_type::value_.bottom);
			}

			bool intersects(const basic_rect &value) const{
				return ((is_empty() || value.is_empty()) ? false : !intersection(value).is_empty());
			}

			bool contains(const basic_rect &value) const{
				if (is_empty() || value.is_empty())
					return false;

				return (value.value_.left >= base_type::value_.left && value.value_.top >= base_type::value_.top &&
					value.value_.right <= base_type::value_.right && value.value_.bottom <= base_type::value_.bottom);
			}

			bool contains(const point_type &value) const{
				return (value.x() >= base_type::value_.left && value.y() >= base_type::value_.top &&
					value.x() < base_type::value_.right && value.y() < base_type::value_.bottom);
			}

			int compare(const_qualified_type value) const{
				if (base_type::value_.top != value.top)
					return (base_type::value_.top < value.top) ? -1 : 1;

				if (base_type::value_.bottom != value.bottom)
					return (base_type::value_.bottom < value.bottom) ? -1 : 1;

				if (base_type::value_.left != value.left)
					return (base_type::value_.left < value.left) ? -1 : 1;

				if (base_type::value_.right != value.right)
					return (base_type::value_.right < value.right) ? -1 : 1;

				return 0;
			}
		};

		template <class point_type, class size_type, class value_type, class field_type = decltype(value_type::left)>
		class outer_basic_rect : public basic_rect<point_type, size_type, value_type, field_type>{
		public:
			typedef basic_rect<point_type, size_type, value_type, field_type> base_type;

			template <typename... args_types>
			outer_basic_rect(args_types &&... args)
				: base_type(std::forward<args_types>(args)...){}

			virtual ~outer_basic_rect() = default;
		};

		template <class point_type, class size_type, class value_type, class field_type = decltype(value_type::left)>
		class inner_basic_rect : public basic_rect<point_type, size_type, value_type, field_type>{
		public:
			typedef basic_rect<point_type, size_type, value_type, field_type> base_type;

			template <typename... args_types>
			inner_basic_rect(args_types &&... args)
				: base_type(std::forward<args_types>(args)...){}

			virtual ~inner_basic_rect() = default;
		};

		typedef basic_rect<point, size, ::RECT> rect;
		typedef basic_rect<pointf, sizef, default_rect_native_value<float>, float> rectf;

		typedef outer_basic_rect<point, size, ::RECT> outer_rect;
		typedef outer_basic_rect<pointf, sizef, default_rect_native_value<float>, float> outer_rectf;

		typedef inner_basic_rect<point, size, ::RECT> inner_rect;
		typedef inner_basic_rect<pointf, sizef, default_rect_native_value<float>, float> inner_rectf;
	}
}

#endif /* !WINPP_RECT_STRUCTURE_H */
