#include "drawer.h"
#include "drawing_solid_color_brush.h"

winpp::drawing::drawer::drawer() = default;

winpp::drawing::drawer::~drawer() = default;

winpp::drawing::drawer::operator pointer_type() const{
	return value();
}

winpp::drawing::drawer::pointer_type winpp::drawing::drawer::operator->() const{
	return value();
}

winpp::drawing::drawer::brush_type &winpp::drawing::drawer::default_brush(){
	if (brush_ == nullptr)//Create a black brush
		brush_ = std::make_shared<solid_color_brush>(*this, ::D2D1::ColorF(0));
	return *brush_;
}

winpp::drawing::drawer::brush_type &winpp::drawing::drawer::default_brush(const colorf_type &color){
	auto &value = default_brush();
	value.color(color);
	return value;
}

winpp::drawing::drawer::brush_type &winpp::drawing::drawer::default_brush(const color_type &color){
	auto &value = default_brush();
	value.color(color);
	return value;
}
