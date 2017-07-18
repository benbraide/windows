#include "application_object.h"
#include "object_manager.h"

#include "../window/window_object.h"

winpp::application::object::object()
	: states_(state_type::nil), object_manager_(std::make_shared<object_manager_type>(*this)){
	if (current_app != nullptr)
		throw common::multiple_apps_exception();

	current_app = this;
	if (threading::id::current() == main_thread)
		main_app = this;
}

winpp::application::object::~object(){
	guard_type guard(lock_);
	if (!list_.empty())
		list_.erase(id_);
}

int winpp::application::object::run(){
	if (threading::id::current() != id_)
		throw common::cross_thread_exception();
	return base_type::run();
}

void winpp::application::object::exit(){
	WINPP_SET(states_, state_type::exiting);
}

winpp::application::object::object_manager_type &winpp::application::object::object_manager(){
	return *object_manager_;
}

winpp::application::object::factory_type &winpp::application::object::drawing_factory(){
	return factory_;
}

bool winpp::application::object::is_exiting() const{
	return WINPP_IS(states_, state_type::exiting);
}

bool winpp::application::object::main_is_exiting(){
	return (main_app == nullptr) ? main_app->is_exiting() : false;
}

winpp::application::object &winpp::application::object::get(){
	if (current_app != nullptr)
		return *current_app;

	{//Limit guard scope
		guard_type guard(lock_);
		*(list_[threading::id::current()] = std::make_shared<object>());
	}

	current_app->object_manager_->create_proxy();
	return *current_app;
}

winpp::application::object &winpp::application::object::get(dword_type id){
	object_ptr_type new_object;
	{//Limit guard scope
		guard_type guard(lock_);

		auto entry = list_.find(id);
		if (entry != list_.end())//Entry found
			return *entry->second;

		new_object = (list_[id] = std::make_shared<object>());
	}
	
	new_object->object_manager_->create_proxy();
	return *new_object;
}

winpp::application::object *winpp::application::object::find(dword_type id){
	guard_type guard(lock_);
	auto entry = list_.find(id);
	return (entry == list_.end()) ? nullptr : entry->second.get();
}

int winpp::application::object::run_app(){
	return get().run();
}

winpp::application::object::dword_type winpp::application::object::main_thread = threading::id::current();

winpp::application::object *winpp::application::object::main_app = nullptr;

thread_local winpp::application::object *winpp::application::object::current_app = nullptr;

bool winpp::application::object::is_filtered_message_() const{
	return false;
}

void winpp::application::object::dispatch_(){
	if (!is_pre_processed_())
		base_type::dispatch_();
}

bool winpp::application::object::is_stopped_() const{
	return !object_manager_->has_top_level();
}

bool winpp::application::object::is_pre_processed_(){
	auto target = object_manager_->find_window(cache_.owner());
	return (target == nullptr) ? false : target->pre_translate(cache_);
}

winpp::application::object::list_type winpp::application::object::list_;

winpp::application::object::lock_type winpp::application::object::lock_;
