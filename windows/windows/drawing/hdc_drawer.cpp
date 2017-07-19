#include "hdc_drawer.h"

winpp::drawing::hdc_drawer::hdc_drawer(factory_type &factory)
	: value_(nullptr){
	auto props = ::D2D1::RenderTargetProperties(
		::D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_DEFAULT,
		::D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_IGNORE
		)
	);

	if (FAILED(factory->CreateDCRenderTarget(&props, &value_)))
		value_ = nullptr;//Failed to create render target
}

winpp::drawing::hdc_drawer::~hdc_drawer(){
	if (value_ != nullptr){
		value_->Release();
		value_ = nullptr;
	}
}

winpp::drawing::hdc_drawer::hdc_pointer_type winpp::drawing::hdc_drawer::operator->() const{
	return value_;
}

winpp::drawing::hdc_drawer::hdc_pointer_type winpp::drawing::hdc_drawer::value() const{
	return value_;
}

bool winpp::drawing::hdc_drawer::bind(hdc_type value, const rect_type &rect){
	return (value_ != nullptr && !FAILED(value_->BindDC(value, rect)));
}
