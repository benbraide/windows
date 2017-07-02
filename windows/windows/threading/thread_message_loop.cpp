#include "thread_message_loop.h"

winpp::threading::message_loop::message_loop()
	: id_(id_type::current()), status_(status_info{ false }){}

winpp::threading::message_loop::~message_loop() = default;

int winpp::threading::message_loop::run(){
	auto idle_index = 0;
	while (!is_stopped_()){
		task_stack_.execute();//Execute pending tasks
		if (queue_.peek(cache_, message_queue::peek_type::remove)){//Not idle
			if (!cache_.is_quit()){//Forward message
				if (cache_.owner() == nullptr)//Thread message
					dispatch_thread_message_();
				else if (!is_filtered_message_())
					dispatch_();

				idle_index = 0;//Reset idle index
			}
			else{//Quit received
				status_.return_value = cache_.wparam<int>();
				break;
			}
		}
		else if (!on_idle_(idle_index++))//No more idle tasks -- wait for event
			queue_.wait();
	}

	return status_.return_value;
}

int winpp::threading::message_loop::result() const{
	return status_.return_value;
}

const winpp::threading::message_loop::id_type &winpp::threading::message_loop::id() const{
	return id_;
}

void winpp::threading::message_loop::add_task(task_type task){
	task_stack_.add(task);
}

void winpp::threading::message_loop::execute_task(std::function<void()> task){
	execute_task_<void>(std::true_type(), task);
}

bool winpp::threading::message_loop::is_filtered_message_() const{
	return false;
}

void winpp::threading::message_loop::translate_() const{
	cache_.translate();
}

void winpp::threading::message_loop::dispatch_(){
	translate_();
	cache_.dispatch();
}

void winpp::threading::message_loop::dispatch_thread_message_(){
	translate_();
	cache_.dispatch();
}

bool winpp::threading::message_loop::on_idle_(int index){
	return false;
}

bool winpp::threading::message_loop::is_stopped_() const{
	return true;
}
