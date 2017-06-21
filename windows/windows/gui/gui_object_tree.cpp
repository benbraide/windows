#include "gui_object_tree.h"

winpp::gui::object_tree::~object_tree() = default;

winpp::gui::object *winpp::gui::object_tree::child_at_absolute_index(index_and_size_type index) const{
	return child_at_index(index);
}

winpp::gui::object::index_and_size_type winpp::gui::object_tree::child_absolute_index(const gui_object_type &object) const{
	return child_absolute_index(object);
}

winpp::gui::object::index_and_size_type winpp::gui::object_tree::absolute_index(index_and_size_type index) const{
	return index;
}

bool winpp::gui::object_tree::is_group() const{
	return false;
}

bool winpp::gui::object_tree::is_tree() const{
	return true;
}
