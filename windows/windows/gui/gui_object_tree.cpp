#include "gui_object_tree.h"

winpp::gui::object_tree::~object_tree() = default;

bool winpp::gui::object_tree::is_group() const{
	return false;
}

bool winpp::gui::object_tree::is_tree() const{
	return true;
}
