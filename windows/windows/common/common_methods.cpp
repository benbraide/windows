#include "common_methods.h"

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
