#include "drawing_linear_gradient_brush.h"

winpp::drawing::linear_gradient_brush::linear_gradient_brush(drawer_type &drawer, const properties_type &properties, gradient_stop_collection &stop_collection)
	: value_(nullptr){
	if (FAILED(drawer->CreateLinearGradientBrush(properties, stop_collection, &value_)))
		value_ = nullptr;
}

winpp::drawing::linear_gradient_brush::~linear_gradient_brush(){
	if (value_ != nullptr){
		value_->Release();
		value_ = nullptr;
	}
}

winpp::drawing::brush::pointer_type winpp::drawing::linear_gradient_brush::value() const{
	return value_;
}
