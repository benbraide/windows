#include "drawing_radial_gradient_brush.h"

winpp::drawing::radial_gradient_brush::radial_gradient_brush(drawer_type &drawer, const properties_type &properties, gradient_stop_collection &stop_collection)
	: value_(nullptr){
	if (FAILED(drawer->CreateRadialGradientBrush(properties, stop_collection, &value_)))
		value_ = nullptr;
}

winpp::drawing::radial_gradient_brush::~radial_gradient_brush(){
	if (value_ != nullptr){
		value_->Release();
		value_ = nullptr;
	}
}

winpp::drawing::brush::pointer_type winpp::drawing::radial_gradient_brush::value() const{
	return value_;
}
