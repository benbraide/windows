#include "event_object.h"

winpp::events::object::object(gui_object_type &target)
	: target_(&target), states_(state_type::nil){}

winpp::events::object &winpp::events::object::prevent(){
	WINPP_SET(states_, state_type::prevented);
	return *this;
}

winpp::events::object &winpp::events::object::stop_propagation(){
	WINPP_SET(states_, state_type::stopped);
	return *this;
}

bool winpp::events::object::is_prevented() const{
	return WINPP_IS(states_, state_type::prevented);
}

bool winpp::events::object::is_propagating() const{
	return WINPP_IS(states_, state_type::stopped);
}

winpp::events::object::gui_object_type &winpp::events::object::target() const{
	return *target_;
}