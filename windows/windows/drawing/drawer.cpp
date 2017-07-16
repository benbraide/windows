#include "drawer.h"

winpp::drawing::drawer::drawer() = default;

winpp::drawing::drawer::~drawer() = default;

winpp::drawing::drawer::operator pointer_type() const{
	return value();
}

winpp::drawing::drawer::pointer_type winpp::drawing::drawer::operator->() const{
	return value();
}
