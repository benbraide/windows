#include "drawing_solid_color_brush.h"

winpp::drawing::solid_color_brush::solid_color_brush(drawer_type &drawer, const colorf_type &value)
	: value_(nullptr){
	if (FAILED(drawer->CreateSolidColorBrush(value, ::D2D1::BrushProperties(value.a), &value_)))
		value_ = nullptr;
}

winpp::drawing::solid_color_brush::solid_color_brush(drawer_type &drawer, const color_type &value)
	: solid_color_brush(drawer, colorf_type{ value.relative_red(), value.relative_green(), value.relative_blue(), value.relative_alpha() }){}

winpp::drawing::solid_color_brush::~solid_color_brush(){
	if (value_ != nullptr){
		value_->Release();
		value_ = nullptr;
	}
}

winpp::drawing::brush::pointer_type winpp::drawing::solid_color_brush::value() const{
	return value_;
}

void winpp::drawing::solid_color_brush::color(const colorf_type &value){
	if (value_ != nullptr)
		value_->SetColor(value);
}

winpp::drawing::brush::colorf_type winpp::drawing::solid_color_brush::color() const{
	return (value_ == nullptr) ? colorf_type{} : value_->GetColor();
}
