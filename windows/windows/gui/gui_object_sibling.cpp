#include "gui_object_sibling.h"

winpp::gui::object_sibling::object_sibling()
	: object_(nullptr), type_(sibling_type::previous){}

winpp::gui::object_sibling::object_sibling(const gui_object_type &object, sibling_type type)
	: object_(&const_cast<gui_object_type &>(object)), type_(type){}

winpp::gui::object_sibling::object_sibling(gui_object_type &object, sibling_type type)
	: object_(&object), type_(type){}

winpp::gui::object::app_type *winpp::gui::object_sibling::app() const{
	return object_->app();
}

void *winpp::gui::object_sibling::handle() const{
	return object_->handle();
}

winpp::gui::object *winpp::gui::object_sibling::non_sibling(){
	return object_->non_sibling();
}

const winpp::gui::object *winpp::gui::object_sibling::non_sibling() const{
	return object_->non_sibling();
}

winpp::gui::object *winpp::gui::object_sibling::parent() const{
	return object_->parent();
}

winpp::gui::object *winpp::gui::object_sibling::child_at_index(index_and_size_type index) const{
	return object_->child_at_index(index);
}

winpp::gui::object *winpp::gui::object_sibling::child_at_absolute_index(index_and_size_type index) const{
	return object_->child_at_absolute_index(index);
}

winpp::gui::object &winpp::gui::object_sibling::traverse_children(object_traverser_type traverser){
	object_->traverse_children(traverser);
	return *this;
}

const winpp::gui::object &winpp::gui::object_sibling::traverse_children(object_traverser_type traverser) const{
	object_->traverse_children(traverser);
	return *this;
}

winpp::gui::object &winpp::gui::object_sibling::traverse_siblings(sibling_traverser_type traverser){
	object_->traverse_siblings(traverser);
	return *this;
}

const winpp::gui::object &winpp::gui::object_sibling::traverse_siblings(sibling_traverser_type traverser) const{
	object_->traverse_siblings(traverser);
	return *this;
}

winpp::gui::object &winpp::gui::object_sibling::outer_rect(const rect_type &value){
	object_->outer_rect(value);
	return *this;
}

winpp::gui::object::rect_type winpp::gui::object_sibling::outer_rect() const{
	return object_->outer_rect();
}

winpp::gui::object::rect_type winpp::gui::object_sibling::relative_outer_rect() const{
	return object_->relative_outer_rect();
}

winpp::gui::object &winpp::gui::object_sibling::inner_rect(const rect_type &value){
	object_->inner_rect(value);
	return *this;
}

winpp::gui::object::rect_type winpp::gui::object_sibling::inner_rect() const{
	return object_->inner_rect();
}

winpp::gui::object &winpp::gui::object_sibling::padding(const rect_type &value){
	object_->padding(value);
	return *this;
}

winpp::gui::object::rect_type winpp::gui::object_sibling::padding() const{
	return object_->padding();
}

winpp::gui::object &winpp::gui::object_sibling::outer_size(const size_type &value){
	object_->outer_size(value);
	return *this;
}

winpp::gui::object::size_type winpp::gui::object_sibling::outer_size() const{
	return object_->outer_size();
}

winpp::gui::object &winpp::gui::object_sibling::inner_size(const size_type &value){
	object_->inner_size(value);
	return *this;
}

winpp::gui::object::size_type winpp::gui::object_sibling::inner_size() const{
	return object_->inner_size();
}

winpp::gui::object &winpp::gui::object_sibling::offset(const size_type &value){
	object_->offset(value);
	return *this;
}

winpp::gui::object::point_type winpp::gui::object_sibling::offset() const{
	return object_->offset();
}

winpp::gui::object::point_type winpp::gui::object_sibling::relative_offset() const{
	return object_->relative_offset();
}

winpp::gui::object &winpp::gui::object_sibling::move(const point_type &value){
	object_->move(value);
	return *this;
}

winpp::gui::object &winpp::gui::object_sibling::absolute_move(const point_type &value){
	object_->absolute_move(value);
	return *this;
}

winpp::gui::object &winpp::gui::object_sibling::place(placement_type value, const size_type &delta){
	object_->place(value, delta);
	return *this;
}

winpp::gui::object::point_type winpp::gui::object_sibling::compute_placement(placement_type value, const size_type &delta) const{
	return object_->compute_placement(value, delta);
}

winpp::gui::object::hit_target_type winpp::gui::object_sibling::hit_test(const point_type &value) const{
	return object_->hit_test(value);
}

winpp::gui::object::gui_object_type *winpp::gui::object_sibling::hit_target(const point_type &value) const{
	return object_->hit_target(value);
}

winpp::gui::object::gui_object_type *winpp::gui::object_sibling::deepest_hit_target(const point_type &value) const{
	return object_->deepest_hit_target(value);
}

winpp::gui::object::point_type winpp::gui::object_sibling::convert_to_screen(const point_type &value) const{
	return object_->convert_to_screen(value);
}

winpp::gui::object::rect_type winpp::gui::object_sibling::convert_to_screen(const rect_type &value) const{
	return object_->convert_to_screen(value);
}

winpp::gui::object::point_type winpp::gui::object_sibling::convert_from_screen(const point_type &value) const{
	return object_->convert_from_screen(value);
}

winpp::gui::object::rect_type winpp::gui::object_sibling::convert_from_screen(const rect_type &value) const{
	return object_->convert_from_screen(value);
}

winpp::gui::object::gui_attributes_type &winpp::gui::object_sibling::attributes(){
	return object_->attributes();
}

winpp::gui::object::event_tunnel &winpp::gui::object_sibling::events(){
	return object_->events();
}

winpp::gui::object::index_and_size_type winpp::gui::object_sibling::proposed_index() const{
	return (object_->index_in_parent() + ((type_ == sibling_type::previous) ? 1u : 0u));
}

winpp::gui::object::index_and_size_type winpp::gui::object_sibling::index_in_parent() const{
	return object_->index_in_parent();
}

winpp::gui::object::index_and_size_type winpp::gui::object_sibling::absolute_index_in_parent() const{
	return object_->absolute_index_in_parent();
}

winpp::gui::object::index_and_size_type winpp::gui::object_sibling::child_index(const gui_object_type &object) const{
	return object_->child_index(object);
}

winpp::gui::object::index_and_size_type winpp::gui::object_sibling::child_absolute_index(const gui_object_type &object) const{
	return object_->child_absolute_index(object);
}

winpp::gui::object::index_and_size_type winpp::gui::object_sibling::absolute_index(index_and_size_type index) const{
	return object_->absolute_index(index);
}

winpp::gui::object::index_and_size_type winpp::gui::object_sibling::children_count() const{
	return object_->children_count();
}

winpp::gui::object::index_and_size_type winpp::gui::object_sibling::sibling_count() const{
	return object_->sibling_count();
}

bool winpp::gui::object_sibling::is_group() const{
	return object_->is_group();
}

bool winpp::gui::object_sibling::is_tree() const{
	return object_->is_tree();
}

bool winpp::gui::object_sibling::is_inside_group() const{
	return object_->is_inside_group();
}

bool winpp::gui::object_sibling::has_parent() const{
	return object_->has_parent();
}

bool winpp::gui::object_sibling::is_sibling(const gui_object_type &object) const{
	return object_->is_sibling(object);
}

bool winpp::gui::object_sibling::is_previous() const{
	return (type_ == sibling_type::previous);
}
