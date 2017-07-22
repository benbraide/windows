#include "gui_object_group.h"

winpp::gui::object_group::object_attributes::object_attributes(object &object)
	: base_type(object){}

winpp::gui::object_group::object_attributes::~object_attributes(){
	if (!object_->app()->is_exiting())
		stop_monitoring();
}

winpp::gui::object_group::object_attributes &winpp::gui::object_group::object_attributes::monitor_content(callback_type callback){
	if (callback == nullptr)
		throw common::invalid_arg_exception();

	guard_type guard(lock_);
	auto entry = callback_list_.find(type_content);
	if (entry != callback_list_.end()){//Merge with existing
		auto previous = entry->second;
		entry->second = [previous, callback](unsigned __int64 type){
			previous(type);
			callback(type);
		};
	}
	else{//Add new
		callback_list_[type_content] = callback;
		WINPP_SET(active_, type_content);
	}

	return *this;
}

winpp::gui::object_group::object_attributes &winpp::gui::object_group::object_attributes::fill_content(const rect_type &padding){
	monitor_parent_size([this, padding](unsigned __int64 type){
		auto content_rect = object_->content_rect();
		object_->inner_rect(content_rect.inflate(padding));
	});

	trigger(type_content);
	return *this;
}

bool winpp::gui::object_group::object_attributes::monitoring_content() const{
	return WINPP_IS(active_, type_content);
}

winpp::gui::object *winpp::gui::object_group::child_at_index(index_and_size_type index) const{
	require_app_();
	return app_->execute_task<object *>([&]() -> object *{
		return (children_.size() < index) ? *std::next(children_.begin(), index) : nullptr;
	});
}

winpp::gui::object *winpp::gui::object_group::child_at_absolute_index(index_and_size_type index) const{
	if (parent_ != nullptr){
		auto this_absolute_index = absolute_index_in_parent();
		if (this_absolute_index < index)
			throw common::invalid_arg_exception();
		index = (this_absolute_index - index);
	}

	return child_at_index(index);
}

winpp::gui::object &winpp::gui::object_group::traverse_children(object_traverser_type traverser){
	const_cast<const object_group *>(this)->traverse_children(traverser);
	return *this;
}

const winpp::gui::object &winpp::gui::object_group::traverse_children(object_traverser_type traverser) const{
	if (traverser == nullptr)
		throw common::invalid_arg_exception();

	require_app_();
	app_->execute_task([&]{
		for (auto child : children_){
			if (!traverser(*child))
				break;
		}
	});
	
	return *this;
}

winpp::gui::object::rect_type winpp::gui::object_group::content_rect() const{
	if (children_.empty())
		return rect_type{};

	require_app_();
	return app_->execute_task<rect_type>([&]() -> rect_type{
		rect_type computed_rect{}, child_rect;
		for (auto child : children_){
			if (!(child_rect = child->outer_rect()).is_empty()){
				if (computed_rect.is_empty())
					computed_rect = child_rect;
				else//Combine
					computed_rect.combine(child_rect);
			}
		}

		return computed_rect.move(point_type{});
	});
}

winpp::gui::object::index_and_size_type winpp::gui::object_group::internal_insert_child(gui_object_type &child, index_and_size_type before){
	if (child.has_parent())
		throw common::unsupported_exception();

	require_app_();
	return app_->execute_task<index_and_size_type>([&]() -> index_and_size_type{
		if (!pre_insert_(child, before))
			return invalid_index;

		insert_(child, before);
		return post_insert_(child, before);
	});
}

winpp::gui::object &winpp::gui::object_group::internal_remove_child(gui_object_type &child, force_type force){
	if (child.is_parent(*this))
		throw common::invalid_object_exception();

	require_app_();
	app_->execute_task([&]{
		if (pre_remove_(child) || force != force_type::dont_force)
			remove_(child);
	});
	
	return *this;
}

winpp::gui::object::gui_object_type *winpp::gui::object_group::hit_target(const point_type &value) const{
	if (children_.empty())
		return nullptr;

	require_app_();
	return app_->execute_task<gui_object_type *>([&]() -> gui_object_type *{
		for (auto child : children_){
			if (child->hit_test(value) != hit_target_type::nil)
				return child;
		}

		return nullptr;
	});
}

winpp::gui::object::gui_object_type *winpp::gui::object_group::deepest_hit_target(const point_type &value) const{
	auto child_target = hit_target(value);
	if (child_target == nullptr)
		return nullptr;

	auto offspring_target = child_target->deepest_hit_target(value);
	return (offspring_target == nullptr) ? child_target : offspring_target;
}

winpp::gui::object_group::object_attributes &winpp::gui::object_group::attributes(){
	return *dynamic_cast<object_attributes *>(get_attributes_().get());
}

winpp::gui::object::index_and_size_type winpp::gui::object_group::child_index(const gui_object_type &object) const{
	auto non_sibling = object.non_sibling();
	if (non_sibling->parent() != this)
		throw common::invalid_object_exception();

	require_app_();
	return app_->execute_task<index_and_size_type>([&]{
		index_and_size_type index = 0u;
		for (auto child : children_){
			if (child == non_sibling)
				return index;
			++index;
		}

		return invalid_index;
	});
}

winpp::gui::object::index_and_size_type winpp::gui::object_group::child_absolute_index(const gui_object_type &object) const{
	return absolute_index(child_index(object));
}

winpp::gui::object::index_and_size_type winpp::gui::object_group::absolute_index(index_and_size_type index) const{
	return (parent_ == nullptr) ? index : (parent_->child_absolute_index(*this) + index);
}

winpp::gui::object::index_and_size_type winpp::gui::object_group::children_count() const{
	return children_.size();
}

bool winpp::gui::object_group::is_group() const{
	return true;
}

bool winpp::gui::object_group::has_children() const{
	return (children_count() > 0u);
}

bool winpp::gui::object_group::is_offspring(const gui_object_type &object) const{
	if (object.parent() == nullptr)
		return false;

	if (object.parent() == this)
		return true;

	require_app_();
	return app_->execute_task<bool>([&]{
		for (auto child : children_){
			if (child->is_offspring(object))
				return true;
		}

		return false;
	});
}

bool winpp::gui::object_group::is_child(const gui_object_type &object) const{
	return (object.parent() == this);
}

void winpp::gui::object_group::destroyed_(){
	generic_object::destroyed_();

	auto count = children_.size();
	while (count > 0u){//Destroy children
		if ((*children_.begin())->parent() == this){
			(*children_.begin())->destroy(force_type::force);
			if (children_.size() >= count)//Child was not removed from list
				children_.pop_front();
		}
		else//Child presumably already destroyed
			children_.pop_front();

		count = children_.size();//Update count
	}
}

winpp::gui::generic_object::attributes_type winpp::gui::object_group::get_attributes_(){
	return create_attributes_<object_attributes>();
}

bool winpp::gui::object_group::pre_insert_(gui_object_type &object, index_and_size_type &index){
	return true;
}

void winpp::gui::object_group::insert_(gui_object_type &child, index_and_size_type &before){
	if (before >= children_.size()){//Append
		children_.push_back(child.non_sibling());
		before = (children_.size() - 1u);
	}
	else//Insert
		children_.insert(std::next(children_.begin(), before), child.non_sibling());
}

winpp::gui::object::index_and_size_type winpp::gui::object_group::post_insert_(gui_object_type &object, index_and_size_type index){
	if (cache_group_(object.object_group()))
		group_list_[object.object_group()].push_back(&object);
	return index;
}

bool winpp::gui::object_group::pre_remove_(gui_object_type &object){
	return true;
}

void winpp::gui::object_group::remove_(gui_object_type &object){
	auto entry = std::find(children_.begin(), children_.end(), &object);
	if (entry != children_.end())
		children_.erase(entry);
}

void winpp::gui::object_group::post_remove_(gui_object_type &object){
	if (group_list_.empty())
		return;

	auto entry = group_list_.find(object.object_group());
	if (entry != group_list_.end()){
		auto item = std::find(entry->second.begin(), entry->second.end(), &object);
		if (item != entry->second.end()){
			entry->second.erase(item);
			if (entry->second.empty())
				group_list_.erase(entry);
		}
	}
}

bool winpp::gui::object_group::cache_group_(unsigned int value) const{
	return false;
}
