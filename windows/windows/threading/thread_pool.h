#pragma once

#ifndef WINPP_THREAD_POOL_H
#define WINPP_THREAD_POOL_H

#include <list>
#include <mutex>
#include <thread>
#include <chrono>
#include <functional>
#include <unordered_map>
#include <condition_variable>

namespace winpp{
	namespace threading{
		template <std::size_t min, std::size_t max>
		class pool{
		public:
			typedef std::size_t size_type;

			typedef std::function<void()> task_type;
			typedef std::thread thread_type;

			typedef std::list<task_type> task_list_type;
			typedef std::unordered_map<thread_type::id, thread_type> thread_list_type;

			typedef std::mutex lock_type;
			typedef std::lock_guard<lock_type> guard_type;
			typedef std::unique_lock<lock_type> unique_guard_type;

			typedef std::condition_variable condition_type;
			typedef void(pool::*run_method_type)(task_type);

			enum class option_type{
				nil,
				dedicated,
				parallel,
			};

			pool()
				: free_count_(0), thread_count_(0), destruct_(false){}

			pool(pool &&target)
				: free_count_(target.free_count_), thread_count_(target.thread_count_), task_list_(std::move(target.task_list_)), thread_list_(std::move(target.thread_list_)),
				destruct_(target.destruct_), condition_(std::move(target.condition_)), lock_(std::move(target.lock_)){
				target.free_count_ = target.thread_count_ = 0u;
				target.destruct_ = true;
			}

			~pool(){
				if (!destruct_){
					guard_type guard(lock_);
					destruct_ = true;
					condition_.notify_all();
				}
			}

			bool add(task_type task, option_type option = option_type::nil){
				if (destruct_)
					return false;

				guard_type guard(lock_);
				if (option == option_type::dedicated){//No queue
					do_run_(task, &pool::run_dedicated_);
					return true;
				}

				if (max_size == 0u)
					return false;

				if (free_count_ > 0u || thread_count_ >= max_size){//Wake one or more threads
					task_list_.push_back(task);
					condition_.notify_all();
				}
				else{//Add new thread
					do_run_(task, &pool::run_);
					++thread_count_;
				}

				return true;
			}

			size_type free_count() const{
				return free_count_;
			}

			size_type thread_count() const{
				return thread_count_;
			}

			size_type task_count() const{
				return task_list_.size();
			}

			bool empty() const{
				return thread_list_.empty();
			}

			static const std::size_t min_size = min;
			static const std::size_t max_size = max;

		private:
			void do_run_(task_type task, run_method_type method){
				thread_type thread([this, task, method](){
					(this->*method)(task);
				});

				thread.detach();
				thread_list_.emplace(thread.get_id(), std::move(thread));
			}

			void run_(task_type task){
				task();//Execute task

				unique_guard_type guard(lock_);
				while (true){
					while (!destruct_ && !task_list_.empty()){
						task = *task_list_.begin();
						task_list_.pop_front();//Remove task from list

						condition_.notify_all();
						guard.unlock();

						task();//Execute task
						guard.lock();
					}

					if (!destruct_ && thread_count_ <= min_size){//Wait for next task
						++free_count_;
						condition_.wait(guard, [this]{
							return (destruct_ || !task_list_.empty());
						});

						--free_count_;
						if (thread_count_ > min_size)
							break;//End thread
					}
					else//End thread
						break;
				}

				if (!destruct_){//Remove thread from queue
					thread_list_.erase(std::this_thread::get_id());
					--thread_count_;
				}
			}

			void run_dedicated_(task_type task){
				task();//Execute task

				lock_.lock();
				if (!destruct_)//Remove thread from queue
					thread_list_.erase(std::this_thread::get_id());
				lock_.unlock();
			}

			size_type free_count_;
			size_type thread_count_;

			task_list_type task_list_;
			thread_list_type thread_list_;

			bool destruct_;
			condition_type condition_;
			lock_type lock_;
		};
	}
}

#endif /* !WINPP_THREAD_POOL_H */
