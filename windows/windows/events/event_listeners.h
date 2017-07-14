#pragma once

#ifndef WINPP_EVENT_LISTENERS_H
#define WINPP_EVENT_LISTENERS_H

#include <functional>

#include "event_listeners_manager.h"

namespace winpp{
	namespace events{
		class listeners_base{
		public:
			virtual ~listeners_base() = default;

			virtual bool remove(unsigned __int64 id) = 0;

			static unsigned __int32 extract_group_id(unsigned __int64 id){
				return common::methods::low(id);
			}

			static unsigned __int32 extract_item_id(unsigned __int64 id){
				return common::methods::high(id);
			}

			static unsigned __int64 combine_ids(unsigned __int32 group_id, unsigned __int32 item_id){
				return common::methods::combine(group_id, item_id);
			}

			static bool is_valid_id(unsigned __int64 id){
				return (extract_item_id(id) != 0u);
			}
		};

		template <class return_type, class... value_types>
		class listeners : public listeners_base{
		public:
			typedef std::function<return_type(value_types...)> callback_type;
			typedef std::function<return_type()> no_args_callback_type;
			
			typedef std::recursive_mutex lock_type;
			typedef std::lock_guard<lock_type> guard_type;

			typedef std::unordered_map<unsigned __int32, callback_type> callback_list_type;
			typedef common::random_uint32 random_number_type;

			listeners(){
				id_ = listeners_manager::add(*this);
			}

			virtual ~listeners(){
				listeners_manager::remove(id_);
			}

			unsigned __int64 add(callback_type callback){
				return add_(callback);
			}

			template <typename unused_type = return_type>
			std::enable_if_t<!std::is_same_v<std::function<unused_type()>, callback_type>, unsigned __int64> add(no_args_callback_type callback){
				return add_no_args_(callback);
			}

			virtual bool remove(unsigned __int64 id) override{
				if (extract_group_id(id) != id_)
					return false;

				guard_type guard(lock_);
				auto item = callback_list_.find(extract_item_id(id));
				if (item == callback_list_.end())
					return false;

				callback_list_.erase(item);
				return true;
			}

			unsigned __int32 id() const{
				return id_;
			}

			const callback_list_type &callback_list() const{
				return callback_list_;
			}

			lock_type &lock(){
				return lock_;
			}

		protected:
			template <typename callback_type>
			unsigned __int64 add_(callback_type callback){
				guard_type guard(lock_);

				auto id = generator_(1, std::numeric_limits<unsigned __int32>::max());
				if (callback_list_.find(id) != callback_list_.end())
					return 0u;

				callback_list_[id] = callback;
				return combine_ids(id_, id);
			}

			template <typename unused_type = return_type>
			std::enable_if_t<std::is_same_v<unused_type, void>, unsigned __int64> add_no_args_(no_args_callback_type callback){
				return add_([callback](value_types...){
					callback();
				});
			}

			template <typename unused_type = return_type>
			std::enable_if_t<!std::is_same_v<unused_type, void>, unsigned __int64> add_no_args_(no_args_callback_type callback){
				return add_([callback](value_types...) -> return_type{
					return callback();
				});
			}

			unsigned __int32 id_;
			callback_list_type callback_list_;
			random_number_type generator_;
			lock_type lock_;
		};
	}
}

#endif /* !WINPP_EVENT_LISTENERS_H */
