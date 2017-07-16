#include "drawing_factory.h"

winpp::drawing::factory::factory()
	: value_(nullptr){
	if (FAILED(::D2D1CreateFactory(::D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED, &value_)))
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
