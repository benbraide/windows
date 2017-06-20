#include "gui_object_attributes.h"
#include "gui_object.h"

winpp::gui::object_attributes::object_attributes(object &object)
	: object_(&object), active_(type_nil){}

winpp::gui::object_attributes::~object_attributes(){
	if (/*#TODO: Check if application is torn down*/true)
		stop_monitoring();
}

winpp::gui::object_attributes &winpp::gui::object_attributes::trigger(unsigned __int64 types){
	guard_type guard(lock_);
	trigger_(types);
	return *this;
}

winpp::gui::object_attributes &winpp::gui::object_attributes::trigger(const object &sibling){
	guard_type guard(lock_);
	trigger_(sibling);
	return *this;
}

winpp::gui::object_attributes &winpp::gui::object_attributes::monitor_parent_size(callback_type callback){
	if (callback == nullptr)
		throw common::invalid_arg_exception();

	if (!object_->has_parent())
		throw common::unsupported_exception();

	guard_type guard(lock_);
	auto entry = callback_list_.find(type_parent_size);
	if (entry != callback_list_.end()){//Merge with existing
		auto previous = entry->second;
		entry->second = [previous, callback](unsigned __int64 type){
			previous(type);
			callback(type);
		};
	}
	else{//Add new
		callback_list_[type_parent_size] = callback;
		WINPP_SET(active_, type_parent_size);
		object_->parent()->attributes().add_dependent_child_(*object_);
	}

	return *this;
}

winpp::gui::object_attributes &winpp::gui::object_attributes::monitor_siblings(callback_type callback){
	if (callback == nullptr)
		throw common::invalid_arg_exception();

	guard_type guard(lock_);
	auto entry = callback_list_.find(type_siblings_size);
	if (entry != callback_list_.end()){//Merge with existing
		auto previous = entry->second;
		entry->second = [previous, callback](unsigned __int64 type){
			previous(type);
			callback(type);
		};
	}
	else{//Add new
		callback_list_[type_siblings_size] = callback;
		WINPP_SET(active_, type_siblings_size);
	}

	return *this;
}

winpp::gui::object_attributes &winpp::gui::object_attributes::monitor_sibling(const object &sibling, sibling_callback_type callback){
	if (callback == nullptr)
		throw common::invalid_arg_exception();

	guard_type guard(lock_);
	auto entry = sibling_callback_list_.find(&sibling);
	if (entry != sibling_callback_list_.end()){//Merge with existing
		auto previous = entry->second;
		entry->second = [previous, callback](const object &sibling){
			previous(sibling);
			callback(sibling);
		};
	}
	else{//Add new
		sibling_callback_list_[&sibling] = callback;
		const_cast<object &>(sibling).attributes().add_dependent_sibling_(*object_);
	}

	return *this;
}

winpp::gui::object_attributes &winpp::gui::object_attributes::stop_monitoring(unsigned __int64 type){
	guard_type guard(lock_);

	callback_list_.erase(type);
	WINPP_REMOVE(active_, type);
	if (type == type_parent_size)
		object_->parent()->attributes().remove_dependent_child_(*object_);

	return *this;
}

winpp::gui::object_attributes &winpp::gui::object_attributes::stop_monitoring(const object &sibling){
	guard_type guard(lock_);

	sibling_callback_list_.erase(&sibling);
	const_cast<object &>(sibling).attributes().remove_dependent_sibling_(*object_);

	return *this;
}

winpp::gui::object_attributes &winpp::gui::object_attributes::stop_monitoring(){
	guard_type guard(lock_);
	if (WINPP_IS(active_, type_parent_size))
		object_->parent()->attributes().remove_dependent_child_(*object_);

	for (auto &item : sibling_callback_list_)
		const_cast<object *>(item.first)->attributes().remove_dependent_sibling_(*object_);

	sibling_callback_list_.clear();
	callback_list_.clear();
	active_ = type_nil;

	return *this;
}

winpp::gui::object_attributes &winpp::gui::object_attributes::fill_parent_size(const rect_type &padding){
	return fill_parent_size(placement_type::unspecified, padding);
}

winpp::gui::object_attributes &winpp::gui::object_attributes::fill_parent_size(placement_type placement, const rect_type &padding){
	monitor_parent_size([this, placement, padding](unsigned __int64 type){
		if (placement != placement_type::unspecified){
			auto parent_rect = object_->parent()->inner_rect();
			auto computed_rect = parent_rect.deflated(padding);

			computed_rect.offset(common::methods::compute_placement_delta(parent_rect.size(), computed_rect.size(), placement));
			object_->outer_rect(computed_rect);
		}
		else//No placement
			object_->outer_rect(object_->parent()->inner_rect().deflated(padding));
	});

	return trigger(type_parent_size);
}

winpp::gui::object_attributes &winpp::gui::object_attributes::fill_parent_proportional_size(const sizef_type &proportion, const rect_type &padding){
	return fill_parent_proportional_size(proportion, placement_type::unspecified, padding);
}

winpp::gui::object_attributes &winpp::gui::object_attributes::fill_parent_proportional_size(const sizef_type &proportion, placement_type placement, const rect_type &padding){
	monitor_parent_size([this, proportion, placement, padding](unsigned __int64 type){
		auto parent_rect = object_->parent()->inner_rect();
		auto parent_size = parent_rect.size(), object_size = object_->outer_size();

		size_type computed_size{
			(proportion.width() < 0.0f) ? object_size.width() : static_cast<size_type::field_type>(parent_size.width() * proportion.width()),
			(proportion.height() < 0.0f) ? object_size.height() : static_cast<size_type::field_type>(parent_size.height() * proportion.height())
		};

		auto computed_rect = rect_type(parent_rect.top_left(), computed_size).deflated(padding);
		if (placement != placement_type::unspecified)//Apply placement
			computed_rect.offset(common::methods::compute_placement_delta(parent_rect.size(), computed_rect.size(), placement));

		object_->outer_rect(computed_rect);
	});

	return trigger(type_parent_size);
}

winpp::gui::object_attributes &winpp::gui::object_attributes::fill_parent_width(const size_type &padding){
	monitor_parent_size([this, padding](unsigned __int64 type){
		auto parent_rect = object_->parent()->inner_rect();
		size_type computed_size{
			parent_rect.size().width(),
			object_->outer_size().height()
		};

		object_->outer_rect(rect_type(parent_rect.top_left(), computed_size).deflated(rect_type{ padding.width(), 0, padding.height(), 0 }));
	});

	return trigger(type_parent_size);
}

winpp::gui::object_attributes &winpp::gui::object_attributes::fill_parent_proportional_width(float proportion, const size_type &padding){
	monitor_parent_size([this, proportion, padding](unsigned __int64 type){
		auto parent_rect = object_->parent()->inner_rect();
		size_type computed_size{
			static_cast<size_type::field_type>(parent_rect.size().width() * proportion),
			object_->outer_size().height()
		};

		object_->outer_rect(rect_type(parent_rect.top_left(), computed_size).deflated(rect_type{ padding.width(), 0, padding.height(), 0 }));
	});

	return trigger(type_parent_size);
}

winpp::gui::object_attributes &winpp::gui::object_attributes::fill_parent_height(const size_type &padding){
	monitor_parent_size([this, padding](unsigned __int64 type){
		auto parent_rect = object_->parent()->inner_rect();
		size_type computed_size{
			object_->outer_size().width(),
			parent_rect.size().height()
		};

		object_->outer_rect(rect_type(parent_rect.top_left(), computed_size).deflated(rect_type{ 0, padding.width(), 0, padding.height() }));
	});

	return trigger(type_parent_size);
}

winpp::gui::object_attributes &winpp::gui::object_attributes::fill_parent_proportional_height(float proportion, const size_type &padding){
	monitor_parent_size([this, proportion, padding](unsigned __int64 type){
		auto parent_rect = object_->parent()->inner_rect();
		size_type computed_size{
			object_->outer_size().width(),
			static_cast<size_type::field_type>(parent_rect.size().height() * proportion),
		};

		object_->outer_rect(rect_type(parent_rect.top_left(), computed_size).deflated(rect_type{ 0, padding.width(), 0, padding.height() }));
	});

	return trigger(type_parent_size);
}

winpp::gui::object_attributes &winpp::gui::object_attributes::place_in_parent(placement_type placement, const size_type &offset){
	if (placement == placement_type::unspecified)
		return *this;

	monitor_parent_size([this, placement, offset](unsigned __int64 type){
		object_->place(placement, offset);
	});

	return trigger(type_parent_size);
}

winpp::gui::object_attributes &winpp::gui::object_attributes::place_beside_sibling(const object &sibling, placement_type placement, const size_type &offset){
	if (placement == placement_type::unspecified)
		return *this;

	monitor_sibling(sibling, [this, placement, offset](const object &sibling){
		auto computed_rect = sibling.outer_rect();
		auto object_size = object_->outer_size();
		auto sibling_size = computed_rect.size();

		if (common::methods::has_right(placement)){
			computed_rect.right(computed_rect.left());
			computed_rect.left(computed_rect.left() - object_size.width());
		}
		else if (common::methods::has_horizontal_center(placement)){
			computed_rect.left(computed_rect.left() + ((sibling_size.width() - object_size.width()) / 2));
			computed_rect.right(computed_rect.right() - ((sibling_size.width() - object_size.width()) / 2));
		}

		if (common::methods::has_bottom(placement)){
			computed_rect.bottom(computed_rect.top());
			computed_rect.top(computed_rect.top() - object_size.height());
		}
		else if (common::methods::has_vertical_center(placement)){
			computed_rect.top(computed_rect.top() + ((sibling_size.height() - object_size.height()) / 2));
			computed_rect.bottom(computed_rect.bottom() - ((sibling_size.height() - object_size.height()) / 2));
		}

		object_->outer_rect(computed_rect.offset(offset));
	});

	return trigger(sibling);
}

bool winpp::gui::object_attributes::monitoring_parent_size() const{
	return WINPP_IS(active_, type_parent_size);
}

bool winpp::gui::object_attributes::monitoring_siblings() const{
	return WINPP_IS(active_, type_siblings_size);
}

bool winpp::gui::object_attributes::monitoring_sibling(const object &sibling) const{
	return (!sibling_callback_list_.empty() && sibling_callback_list_.find(&sibling) != sibling_callback_list_.end());
}

const winpp::gui::object_attributes::object_list_type &winpp::gui::object_attributes::dependent_children() const{
	return dependent_children_;
}

const winpp::gui::object_attributes::object_list_type &winpp::gui::object_attributes::dependent_siblings() const{
	return dependent_siblings_;
}

winpp::gui::object_attributes &winpp::gui::object_attributes::trigger_(unsigned __int64 types){
	auto resolved_last_pow = last_pow_();
	for (auto pow = 0ull; pow <= resolved_last_pow; ++pow){//Traverse flags
		if (WINPP_IS(types, (1ull << pow)) && WINPP_IS(active_, (1ull << pow)))
			(callback_list_[(1ull << pow)])((1ull << pow));
	}

	return *this;
}

winpp::gui::object_attributes &winpp::gui::object_attributes::trigger_(const object &sibling){
	auto entry = sibling_callback_list_.find(&sibling);
	if (entry != sibling_callback_list_.end())
		entry->second(sibling);

	return *this;
}

unsigned __int64 winpp::gui::object_attributes::last_pow_() const{
	return last_pow;
}

void winpp::gui::object_attributes::add_dependent_child_(const object &object){
	guard_type guard(lock_);
	dependent_children_.push_back(&object);
}

void winpp::gui::object_attributes::remove_dependent_child_(const object &object){
	guard_type guard(lock_);
	auto entry = std::find(dependent_children_.begin(), dependent_children_.end(), &object);
	if (entry != dependent_children_.end())
		dependent_children_.erase(entry);
}

void winpp::gui::object_attributes::add_dependent_sibling_(const object &object){
	guard_type guard(lock_);
	dependent_siblings_.push_back(&object);
}

void winpp::gui::object_attributes::remove_dependent_sibling_(const object &object){
	guard_type guard(lock_);
	auto entry = std::find(dependent_siblings_.begin(), dependent_siblings_.end(), &object);
	if (entry != dependent_siblings_.end())
		dependent_siblings_.erase(entry);
}
