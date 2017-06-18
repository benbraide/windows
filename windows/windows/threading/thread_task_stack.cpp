#include "thread_task_stack.h"

winpp::threading::task_stack::task_stack(id_type id)
	: id_(id){}

void winpp::threading::task_stack::add(callback_type task){
	if (id_type::current() == id_)//Same thread
		task();//Execute task
	else//Add to list
		callback_list_.push_back(task);
}

void winpp::threading::task_stack::execute(){
	if (callback_list_.empty())
		return;//Empty list

	callback_list_type list(std::move(callback_list_));
	for (auto task : list)
		task();
}

winpp::threading::task_stack::id_type winpp::threading::task_stack::id() const{
	return id_;
}

bool winpp::threading::task_stack::empty() const{
	return callback_list_.empty();
}
