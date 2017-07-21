#include "generic_gui_object.h"
#include "gui_object_sibling.h"

#include "../common/common_methods.h"
#include "../application/object_manager.h"

winpp::gui::generic_object::event_tunnel::~event_tunnel() = default;

unsigned __int64 winpp::gui::generic_object::event_tunnel::bind(std::wstring e, const std::any &callback){
	common::methods::to_lower(e);
	return bind_(e, callback);
}

unsigned __int64 winpp::gui::generic_object::event_tunnel::bind(const std::string &e, const std::any &callback){
	auto converted = common::methods::convert_string(e);
	common::methods::to_lower(converted);
	return bind_(converted, callback);
}

unsigned __int64 winpp::gui::generic_object::event_tunnel::bind(event_type e, const std::any &callback){
	switch (e){
	case event_type::create:
		return create.bind_generic(callback);
	case event_type::destroy:
		return destroy.bind_generic(callback);
	case event_type::move:
		return move.bind_generic(callback);
	case event_type::size:
		return size.bind_generic(callback);
	default:
		break;
	}

	return 0u;
}

unsigned __int64 winpp::gui::generic_object::event_tunnel::bind_(const std::wstring &e, const std::any &callback){
	if (e == L"create")
		return create.bind_generic(callback);

	if (e == L"destroy")
		return destroy.bind_generic(callback);

	if (e == L"move")
		return move.bind_generic(callback);

	if (e == L"size")
		return size.bind_generic(callback);

	return 0u;
}

winpp::gui::generic_object::generic_object()
	: app_(nullptr), scope_app_(&app_type::get()), parent_(nullptr){}

winpp::gui::generic_object::~generic_object() = default;

winpp::gui::object::app_type *winpp::gui::generic_object::app() const{
	return app_;
}

winpp::gui::object::app_type *winpp::gui::generic_object::scope_app() const{
	return scope_app_;
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

winpp::gui::object *winpp::gui::generic_object::ancestor(index_and_size_type index) const{
	return (parent_ == nullptr || index == 0u) ? parent_ : parent_->ancestor(index - 1);
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

	if (traverser == nullptr)
		throw common::invalid_arg_exception();

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

winpp::gui::object &winpp::gui::generic_object::internal_set_parent(gui_object_type *parent){
	if (parent != parent_ && parent_ != nullptr && attributes_ != nullptr)//Stop monitoring current parent's size
		attributes_->stop_monitoring(gui_attributes_type::type_parent_size);
	parent_ = parent;
	return *this;
}

winpp::gui::object &winpp::gui::generic_object::traverse_ancestors(object_traverser_type traverser){
	const_cast<const generic_object *>(this)->traverse_ancestors(traverser);
	return *this;
}

const winpp::gui::object &winpp::gui::generic_object::traverse_ancestors(object_traverser_type traverser) const{
	auto ancestor = parent_;
	while (ancestor != nullptr && traverser(*ancestor))
		ancestor = ancestor->parent();
	return *this;
}

winpp::gui::object::index_and_size_type winpp::gui::generic_object::internal_insert_into_parent(gui_object_type &object){
	if (!object.is_sibling())
		return object.internal_insert_child(*this);

	auto parent = object.parent();
	if (parent == nullptr)//Parent required
		throw common::invalid_object_exception();

	return parent->internal_insert_child(*this, object.proposed_index());
}

winpp::gui::object::index_and_size_type winpp::gui::generic_object::internal_insert_child(gui_object_type &child, index_and_size_type before){
	throw common::unsupported_exception();
}

winpp::gui::object &winpp::gui::generic_object::internal_remove_child(gui_object_type &child, force_type force){
	throw common::unsupported_exception();
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

winpp::gui::object::rect_type winpp::gui::generic_object::content_rect() const{
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
	throw common::unsupported_exception();
	return (hit_test(value) == hit_target_type::nil) ? nullptr : const_cast<generic_object *>(this);
}

winpp::gui::object::gui_object_type *winpp::gui::generic_object::deepest_hit_target(const point_type &value) const{
	throw common::unsupported_exception();
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

winpp::gui::object &winpp::gui::generic_object::destroy(force_type force){
	throw common::unsupported_exception();
}

winpp::gui::object &winpp::gui::generic_object::show(show_mode_type mode){
	throw common::unsupported_exception();
}

winpp::gui::object &winpp::gui::generic_object::hide(){
	throw common::unsupported_exception();
}

bool winpp::gui::generic_object::is_hidden() const{
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

unsigned int winpp::gui::generic_object::group() const{
	return default_group;
}

winpp::gui::object::index_and_size_type winpp::gui::generic_object::proposed_index() const{
	return invalid_index;
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

winpp::gui::object::index_and_size_type winpp::gui::generic_object::ancestor_count() const{
	return (parent_ == nullptr) ? 0u : (parent_->ancestor_count() + 1);
}

bool winpp::gui::generic_object::is_sibling() const{
	return false;
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

bool winpp::gui::generic_object::has_children() const{
	throw common::unsupported_exception();
}

bool winpp::gui::generic_object::has_siblings() const{
	return (sibling_count() > 0u);
}

bool winpp::gui::generic_object::has_parent() const{
	return (parent_ != nullptr);
}

bool winpp::gui::generic_object::is_group(unsigned int value) const{
	return (group() == value);
}

bool winpp::gui::generic_object::is_offspring(const gui_object_type &object) const{
	throw common::unsupported_exception();
}

bool winpp::gui::generic_object::is_child(const gui_object_type &object) const{
	throw common::unsupported_exception();
}

bool winpp::gui::generic_object::is_parent(const gui_object_type &object) const{
	return (parent_ == &object);
}

bool winpp::gui::generic_object::is_ancestor(const gui_object_type &object) const{
	if (parent_ == &object)
		return true;
	return (parent_ == nullptr) ? false : parent_->is_ancestor(object);
}

bool winpp::gui::generic_object::is_sibling(const gui_object_type &object) const{
	return (object.parent() == parent_);
}

bool winpp::gui::generic_object::is_created() const{
	throw common::unsupported_exception();
}

bool winpp::gui::generic_object::is_top_level() const{
	return false;
}

bool winpp::gui::generic_object::is_menu() const{
	return false;
}

bool winpp::gui::generic_object::is_menu_item() const{
	return false;
}

bool winpp::gui::generic_object::is_non_window() const{
	return false;
}

bool winpp::gui::generic_object::is_window() const{
	return false;
}

bool winpp::gui::generic_object::is_dialog() const{
	return false;
}

bool winpp::gui::generic_object::is_modal() const{
	return false;
}

bool winpp::gui::generic_object::is_control() const{
	return false;
}

winpp::gui::object::procedure_type winpp::gui::generic_object::procedure() const{
	throw common::unsupported_exception();
}

void winpp::gui::generic_object::created_(){
	app_->object_manager().update(application::object_manager::update_object_created, this);
}

void winpp::gui::generic_object::destroyed_(){
	app_->object_manager().update(application::object_manager::update_object_destroyed, this);
	if (parent_ != nullptr){//Remove from parent
		parent_->internal_remove_child(*this, force_type::force);
		parent_ = nullptr;
	}

	if (attributes_ != nullptr)
		attributes_->stop_monitoring();
}

void winpp::gui::generic_object::sized_(){
	if (parent_ != nullptr)//Alert parent of content change
		parent_->attributes().trigger(object_attributes::type_content);

	if (attributes_ != nullptr){
		for (auto child : attributes_->dependent_children())//Alert children of size change
			const_cast<gui_object_type *>(child)->attributes().trigger(object_attributes::type_parent_size);

		for (auto sibling : attributes_->dependent_siblings())//Alert siblings of size change
			const_cast<gui_object_type *>(sibling)->attributes().trigger(*this);
	}
}

void winpp::gui::generic_object::moved_(){
	if (parent_ != nullptr)//Alert parent of content change
		parent_->attributes().trigger(object_attributes::type_content);

	if (attributes_ != nullptr){
		for (auto sibling : attributes_->dependent_siblings())//Alert siblings of position change
			const_cast<gui_object_type *>(sibling)->attributes().trigger(*this);
	}
}

void winpp::gui::generic_object::require_app_() const{
	if (app_ == nullptr)
		throw common::no_app_exception();
}

winpp::gui::generic_object::attributes_type winpp::gui::generic_object::get_attributes_(){
	return create_attributes_<gui_attributes_type>();
}

winpp::gui::generic_object::events_type winpp::gui::generic_object::get_events_(){
	return create_events_<event_tunnel>();
}
