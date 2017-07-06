#include "application_object.h"
#include "object_manager.h"

winpp::application::object::object()
	: states_(state_type::nil), object_manager_(std::make_shared<object_manager_type>(*this)){
	guard_type guard(lock_);
	if (current_app != nullptr)
		throw common::multiple_apps_exception();

	current_app = this;
	if (main_app == nullptr){
		main_app = this;
		process_id_ = ::GetCurrentProcessId();
		message_id_ = ::RegisterWindowMessageW(L"WINPP_WM_" WINPP_WUUID);
	}

	list_[threading::id::current()] = this;
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

winpp::application::object *winpp::application::object::find(dword_type id){
	guard_type guard(lock_);
	auto entry = list_.find(id);
	return (entry == list_.end()) ? nullptr : entry->second;
}

winpp::application::object *winpp::application::object::main_app = nullptr;

thread_local winpp::application::object *winpp::application::object::current_app = nullptr;

bool winpp::application::object::is_filtered_message_() const{
	return false;
}

void winpp::application::object::dispatch_(){
	if (!is_dialog_message_())
		base_type::dispatch_();
}

bool winpp::application::object::is_stopped_() const{
	return false;
}

bool winpp::application::object::is_dialog_message_(){
	return false;
}

winpp::application::object::dword_type winpp::application::object::process_id_;

winpp::threading::message_loop::uint_type winpp::application::object::message_id_;

winpp::application::object::list_type winpp::application::object::list_;

winpp::application::object::lock_type winpp::application::object::lock_;
