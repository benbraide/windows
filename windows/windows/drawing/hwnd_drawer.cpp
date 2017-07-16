#include "hwnd_drawer.h"

winpp::drawing::hwnd_drawer::hwnd_drawer(factory_type &factory, hwnd_type value)
	: value_(nullptr){
	rect_type client_rect;
	::GetClientRect(value, client_rect);

	auto client_size = client_rect.size();
	auto result = factory->CreateHwndRenderTarget(
		::D2D1::RenderTargetProperties(),
		::D2D1::HwndRenderTargetProperties(value, ::D2D1::SizeU(client_size.width(), client_size.height())),
		&value_
	);

	if (FAILED(result))//Failed to create render target
		value_ = nullptr;
}

winpp::drawing::hwnd_drawer::~hwnd_drawer(){
	if (value_ != nullptr){
		value_->Release();
		value_ = nullptr;
	}
}

winpp::drawing::drawer::pointer_type winpp::drawing::hwnd_drawer::value() const{
	return value_;
}

bool winpp::drawing::hwnd_drawer::is_occluded() const{
	return (value_ != nullptr && value_->CheckWindowState() == D2D1_WINDOW_STATE::D2D1_WINDOW_STATE_OCCLUDED);
}
