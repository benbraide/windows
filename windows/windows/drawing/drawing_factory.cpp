#include "drawing_factory.h"

winpp::drawing::factory::factory(factory_type type)
	: value_(nullptr){
	if (FAILED(::D2D1CreateFactory(type, &value_)))
		value_ = nullptr;//Failed to create factory
}

winpp::drawing::factory::~factory(){
	if (value_ != nullptr){
		value_->Release();
		value_ = nullptr;
	}
}

winpp::drawing::factory::operator pointer_type() const{
	return value();
}

winpp::drawing::factory::pointer_type winpp::drawing::factory::operator->() const{
	return value();
}

winpp::drawing::factory::pointer_type winpp::drawing::factory::value() const{
	return value_;
}
