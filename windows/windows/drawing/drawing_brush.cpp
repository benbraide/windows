#include "drawing_brush.h"

winpp::drawing::brush::brush() = default;

winpp::drawing::brush::~brush() = default;

winpp::drawing::brush::operator pointer_type() const{
	return value();
}

winpp::drawing::brush::pointer_type winpp::drawing::brush::operator->() const{
	return value();
}

void winpp::drawing::brush::color(const colorf_type &value){
	throw common::unsupported_exception();
}

void winpp::drawing::brush::color(const color_type &value){
	color(colorf_type{ value.relative_red(), value.relative_green(), value.relative_blue(), value.relative_alpha() });
}

winpp::drawing::brush::colorf_type winpp::drawing::brush::color() const{
	throw common::unsupported_exception();
}
