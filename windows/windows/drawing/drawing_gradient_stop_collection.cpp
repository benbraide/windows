#include "drawing_gradient_stop_collection.h"

winpp::drawing::gradient_stop_collection::gradient_stop_collection(drawer_type &drawer, const gradient_stop_list_type &list, gamma_type gamma, extend_mode_type extend_mode)
	: gradient_stop_collection(drawer, list.data(), static_cast<uint_type>(list.size()), gamma, extend_mode){}

winpp::drawing::gradient_stop_collection::gradient_stop_collection(drawer_type &drawer, gradient_stop_const_ptr_type list, uint_type list_count, gamma_type gamma, extend_mode_type extend_mode)
	: value_(nullptr){
	if (FAILED(drawer->CreateGradientStopCollection(list, list_count, gamma, extend_mode, &value_)))
		value_ = nullptr;
}

winpp::drawing::gradient_stop_collection::~gradient_stop_collection(){
	if (value_ != nullptr){
		value_->Release();
		value_ = nullptr;
	}
}

winpp::drawing::gradient_stop_collection::operator pointer_type() const{
	return value();
}

winpp::drawing::gradient_stop_collection::pointer_type winpp::drawing::gradient_stop_collection::operator->() const{
	return value();
}

winpp::drawing::gradient_stop_collection::pointer_type winpp::drawing::gradient_stop_collection::value() const{
	return value_;
}

void winpp::drawing::gradient_stop_collection::create_uniform_gradient_stop_list(const colorf_list_type &color_list, gradient_stop_list_type &stop_list){
	stop_list.clear();

	auto size = color_list.size();
	switch (size){
	case 0u://Empty list
		return;
	case 1u:
		stop_list.push_back(gradient_stop_type{ 0.0f, color_list[0] });
		return;
	default:
		break;
	}


	stop_list.reserve(size);
	auto increment = (1.0f / static_cast<int>(size)), position = 0.0f;
	for (decltype(size) i = 0; i < (size - 1); ++i){//Fill to 1 less than size
		stop_list.push_back(gradient_stop_type{ position, color_list[i] });
		position += increment;
	}

	stop_list.push_back(gradient_stop_type{ 1.0f, color_list[size - 1] });
}

void winpp::drawing::gradient_stop_collection::create_uniform_gradient_stop_list(const color_list_type &color_list, gradient_stop_list_type &stop_list){
	stop_list.clear();

	auto size = color_list.size();
	switch (size){
	case 0u://Empty list
		return;
	case 1u:
		stop_list.push_back(gradient_stop_type{ 0.0f, to_colorf(color_list[0]) });
		return;
	default:
		break;
	}


	stop_list.reserve(size);
	auto increment = (1.0f / static_cast<int>(size)), position = 0.0f;
	for (decltype(size) i = 0; i < (size - 1); ++i){//Fill to 1 less than size
		stop_list.push_back(gradient_stop_type{ position, to_colorf(color_list[i]) });
		position += increment;
	}

	stop_list.push_back(gradient_stop_type{ 1.0f, to_colorf(color_list[size - 1]) });
}

winpp::drawing::gradient_stop_collection::colorf_type winpp::drawing::gradient_stop_collection::to_colorf(const color_type &value){
	return colorf_type{ value.relative_red(), value.relative_green(), value.relative_blue(), value.relative_alpha() };
}
