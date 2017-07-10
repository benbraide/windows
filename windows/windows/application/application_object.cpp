#include "application_object.h"
#include "object_manager.h"

#include "../window/window_object.h"

winpp::application::object::object()
	: states_(state_type::nil), object_manager_(std::make_shared<object_manager_type>(*this)){
	guard_type guard(lock_);
	if (current_app != nullptr)
		throw common::multiple_apps_exception();

	current_app = this;
	if (threading::id::current() == main_thread)
		main_app = this;
}

winpp::application::object::~object(){
	guard_type guard(lock_);
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

bool winpp::application::object::is_exiting() const{
	return WINPP_IS(states_, state_type::exiting);
}

bool winpp::application::object::main_is_exiting(){
	return (main_app == nullptr) ? main_app->is_exiting() : false;
}

winpp::application::object &winpp::application::object::get(){
	if (current_app != nullptr)
		return *current_app;

	guard_type guard(lock_);
	return *(list_[threading::id::current()] = std::make_shared<object>());
}

winpp::application::object &winpp::application::object::get(dword_type id){
	guard_type guard(lock_);
	auto entry = list_.find(id);
	return (entry == list_.end()) ? *(list_[id] = std::make_shared<object>()) : *entry->second;
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
	return false;
}

bool winpp::application::object::is_pre_processed_(){
	auto target = hwnd_type(cache_.owner()).data<gui_object_type *>();
	return (target == nullptr) ? false : target->query<messaging::target>().pre_translate(cache_);
}

winpp::application::object::list_type winpp::application::object::list_;

winpp::application::object::lock_type winpp::application::object::lock_;
