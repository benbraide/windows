#include "generic_gui_object.h"
#include "gui_object_sibling.h"

#include "../common/common_methods.h"

winpp::gui::generic_object::event_tunnel::event_tunnel()
	: create(id_()), destroy(id_()){}

winpp::gui::generic_object::event_tunnel::~event_tunnel() = default;

bool winpp::gui::generic_object::event_tunnel::unbind(unsigned __int64 id){
	return (create.unbind(id) || destroy.unbind(id));
}

winpp::gui::generic_object::~generic_object() = default;

winpp::gui::object::app_type *winpp::gui::generic_object::app() const{
	return app_;
}

void *winpp::gui::generic_object::handle() const{
	throw common::unsupported_exception();
}

winpp::gui::object *winpp::gui::generic_object::non_sibling(){
	return this;
}

const winpp::gui::object *winpp::gui::generic_object::non_sibling() const{
	return this;
}

winpp::gui::object *winpp::gui::generic_object::parent() const{
	return parent_;
}

winpp::gui::object *winpp::gui::generic_object::child_at_index(index_and_size_type index) const{
	throw common::unsupported_exception();
}

winpp::gui::object *winpp::gui::generic_object::child_at_absolute_index(index_and_size_type index) const{
	throw common::unsupported_exception();
}

winpp::gui::object &winpp::gui::generic_object::traverse_children(object_traverser_type traverser){
	throw common::unsupported_exception();
}

const winpp::gui::object &winpp::gui::generic_object::traverse_children(object_traverser_type traverser) const{
	throw common::unsupported_exception();
}

winpp::gui::object &winpp::gui::generic_object::traverse_siblings(sibling_traverser_type traverser){
	const_cast<const generic_object *>(this)->traverse_siblings(traverser);
	return *this;
}

const winpp::gui::object &winpp::gui::generic_object::traverse_siblings(sibling_traverser_type traverser) const{
	if (parent_ == nullptr)
		throw common::unsupported_exception();

	auto type = sibling_type::previous;
	parent_->traverse_children([&](gui_object_type &child){
		if (child.non_sibling() == this){
			type = sibling_type::next;
			return true;
		}

		object_sibling sibling(child, type);
		return traverser(sibling);
	});

	return *this;
}

winpp::gui::object &winpp::gui::generic_object::outer_rect(const rect_type &value){
	throw common::unsupported_exception();
}

winpp::gui::object::rect_type winpp::gui::generic_object::outer_rect() const{
	throw common::unsupported_exception();
}

winpp::gui::object::rect_type winpp::gui::generic_object::relative_outer_rect() const{
	throw common::unsupported_exception();
}

winpp::gui::object &winpp::gui::generic_object::inner_rect(const rect_type &value){
	throw common::unsupported_exception();
}

winpp::gui::object::rect_type winpp::gui::generic_object::inner_rect() const{
	throw common::unsupported_exception();
}

winpp::gui::object &winpp::gui::generic_object::padding(const rect_type &value){
	return outer_rect(inner_rect().inflate(value));
}

winpp::gui::object::rect_type winpp::gui::generic_object::padding() const{
	auto outer_rect = this->outer_rect(), inner_rect = this->inner_rect();
	return rect_type{ inner_rect.left() - outer_rect.left(), inner_rect.top() - outer_rect.top(), outer_rect.right() - inner_rect.right(), outer_rect.bottom() - inner_rect.bottom() };
}

winpp::gui::object &winpp::gui::generic_object::outer_size(const size_type &value){
	return outer_rect(rect_type{ offset(), value });
}

winpp::gui::object::size_type winpp::gui::generic_object::outer_size() const{
	return outer_rect().size();
}

winpp::gui::object &winpp::gui::generic_object::inner_size(const size_type &value){
	return inner_rect(rect_type{ point_type{}, value });
}

winpp::gui::object::size_type winpp::gui::generic_object::inner_size() const{
	return inner_rect().size();
}

winpp::gui::object &winpp::gui::generic_object::offset(const size_type &value){
	return outer_rect(rect_type{ offset() + value, outer_size() });
}

winpp::gui::object::point_type winpp::gui::generic_object::offset() const{
	return outer_rect().top_left();
}

winpp::gui::object::point_type winpp::gui::generic_object::relative_offset() const{
	return offset();
}

winpp::gui::object &winpp::gui::generic_object::move(const point_type &value){
	return outer_rect(rect_type{ value, outer_size() });
}

winpp::gui::object &winpp::gui::generic_object::absolute_move(const point_type &value){
	return move(value);
}

winpp::gui::object &winpp::gui::generic_object::place(placement_type value, const size_type &delta){
	return move(compute_placement(value, delta));
}

winpp::gui::object::point_type winpp::gui::generic_object::compute_placement(placement_type value, const size_type &delta) const{
	if (parent_ == nullptr)
		throw common::unsupported_exception();
	return (parent_->inner_rect().top_left() + (common::methods::compute_placement_delta(parent_->inner_size(), outer_size(), value) + delta));
}

winpp::structures::enumerations::hit_target_type winpp::gui::generic_object::hit_test(const point_type &value) const{
	if (convert_to_screen(inner_rect()).contains(value))
		return hit_target_type::client;
	return outer_rect().contains(value) ? hit_target_type::border : hit_target_type::nil;
}

winpp::gui::object::gui_object_type *winpp::gui::generic_object::hit_target(const point_type &value) const{
	return (hit_test(value) == hit_target_type::nil) ? nullptr : const_cast<generic_object *>(this);
}

winpp::gui::object::gui_object_type *winpp::gui::generic_object::deepest_hit_target(const point_type &value) const{
	return hit_target(value);
}

winpp::gui::object::point_type winpp::gui::generic_object::convert_to_screen(const point_type &value) const{
	return value;
}

winpp::gui::object::rect_type winpp::gui::generic_object::convert_to_screen(const rect_type &value) const{
	return value;
}

winpp::gui::object::point_type winpp::gui::generic_object::convert_from_screen(const point_type &value) const{
	return value;
}

winpp::gui::object &winpp::gui::generic_object::destory(bool no_throw){
	throw common::unsupported_exception();
}

winpp::gui::object::rect_type winpp::gui::generic_object::convert_from_screen(const rect_type &value) const{
	return value;
}

winpp::gui::object::gui_attributes_type &winpp::gui::generic_object::attributes(){
	return *get_attributes_();
}

winpp::gui::generic_object::event_tunnel &winpp::gui::generic_object::events(){
	return *get_events_();
}

winpp::gui::object::index_and_size_type winpp::gui::generic_object::proposed_index() const{
	throw common::unsupported_exception();
}

winpp::gui::object::index_and_size_type winpp::gui::generic_object::index_in_parent() const{
	if (parent_ == nullptr)
		throw common::unsupported_exception();
	return parent_->child_index(*this);
}

winpp::gui::object::index_and_size_type winpp::gui::generic_object::absolute_index_in_parent() const{
	if (parent_ == nullptr)
		throw common::unsupported_exception();
	return parent_->child_absolute_index(*this);
}

winpp::gui::object::index_and_size_type winpp::gui::generic_object::child_index(const gui_object_type &object) const{
	throw common::unsupported_exception();
}

winpp::gui::object::index_and_size_type winpp::gui::generic_object::child_absolute_index(const gui_object_type &object) const{
	throw common::unsupported_exception();
}

winpp::gui::object::index_and_size_type winpp::gui::generic_object::absolute_index(index_and_size_type index) const{
	throw common::unsupported_exception();
}

winpp::gui::object::index_and_size_type winpp::gui::generic_object::children_count() const{
	throw common::unsupported_exception();
}

winpp::gui::object::index_and_size_type winpp::gui::generic_object::sibling_count() const{
	if (parent_ == nullptr)
		throw common::unsupported_exception();
	return (parent_->children_count() - 1u);
}

bool winpp::gui::generic_object::is_group() const{
	return false;
}

bool winpp::gui::generic_object::is_tree() const{
	return false;
}

bool winpp::gui::generic_object::is_inside_group() const{
	return (parent_ != nullptr && parent_->is_group());
}

bool winpp::gui::generic_object::has_parent() const{
	return (parent_ != nullptr);
}

bool winpp::gui::generic_object::is_sibling(const gui_object_type &object) const{
	return (object.parent() == parent_);
}

bool winpp::gui::generic_object::is_created() const{
	throw common::unsupported_exception();
}

winpp::gui::generic_object::attributes_type winpp::gui::generic_object::get_attributes_(){
	return create_attributes_<gui_attributes_type>();
}

winpp::gui::generic_object::events_type winpp::gui::generic_object::get_events_(){
	return create_events_<event_tunnel>();
}
