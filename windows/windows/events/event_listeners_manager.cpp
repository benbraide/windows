#include "event_listeners_manager.h"
#include "event_listeners.h"

unsigned __int32 winpp::events::listeners_manager::add(listeners_base &item){
	guard_type guard(lock_);

	auto id = generator_(1, std::numeric_limits<unsigned __int32>::max());
	list_[id] = &item;

	return id;
}

void winpp::events::listeners_manager::remove(unsigned __int32 id){
	guard_type guard(lock_);
	list_.erase(id);
}

winpp::events::listeners_base *winpp::events::listeners_manager::find(unsigned __int32 id){
	guard_type guard(lock_);
	auto entry = list_.find(id);
	return (entry == list_.end()) ? nullptr : entry->second;
}

bool winpp::events::listeners_manager::unbind(unsigned __int64 id){
	auto target = find(listeners_base::extract_group_id(id));
	return (target == nullptr) ? false : target->remove(id);
}

winpp::events::listeners_manager::list_type winpp::events::listeners_manager::list_;

winpp::events::listeners_manager::random_number_type winpp::events::listeners_manager::generator_;

winpp::events::listeners_manager::lock_type winpp::events::listeners_manager::lock_;
