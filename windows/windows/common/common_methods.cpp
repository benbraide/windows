#include "common_methods.h"
#include "../application/application_object.h"

winpp::common::methods::application_type *winpp::common::methods::get_app(){
	return (application_type::current_app == nullptr) ? application_type::main_app : application_type::current_app;
}

bool winpp::common::methods::has_left(placement_type placement){
	return (placement == placement_type::left || placement == placement_type::top_left || placement == placement_type::bottom_left || placement == placement_type::center_left);
}

bool winpp::common::methods::has_top(placement_type placement){
	return (placement == placement_type::top || placement == placement_type::top_left || placement == placement_type::top_right || placement == placement_type::top_center);
}

bool winpp::common::methods::has_right(placement_type placement){
	return (placement == placement_type::right || placement == placement_type::top_right || placement == placement_type::bottom_right || placement == placement_type::center_right);
}

bool winpp::common::methods::has_bottom(placement_type placement){
	return (placement == placement_type::bottom || placement == placement_type::bottom_left || placement == placement_type::bottom_right || placement == placement_type::bottom_center);
}

bool winpp::common::methods::has_horizontal_center(placement_type placement){
	return (placement == placement_type::center || placement == placement_type::top_center || placement == placement_type::bottom_center);
}

bool winpp::common::methods::has_vertical_center(placement_type placement){
	return (placement == placement_type::center || placement == placement_type::center_left || placement == placement_type::center_right);
}

void winpp::common::methods::pixel_scale(d2d_pointf_type &dpi){
	dpi.x /= 96.0f;
	dpi.y /= 96.0f;
}
