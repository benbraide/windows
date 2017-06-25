#pragma once

#ifndef WINPP_EVENT_LISTENERS_H
#define WINPP_EVENT_LISTENERS_H

#include <mutex>
#include <variant>
#include <functional>
#include <unordered_map>

#include "../common/common_methods.h"
#include "../common/common_headers.h"
#include "../common/random_number.h"

namespace winpp{
	namespace events{
		template <class return_type, class... value_types>
		class listeners{
		public:
			typedef std::function<return_type(value_types...)> callback_type;
			typedef std::function<return_type()> no_args_callback_type;
			
			typedef return_type(*native_callback_type)(value_types...);
			typedef std::variant<callback_type, native_callback_type> variant_callback_type;

			typedef std::recursive_mutex lock_type;
			typedef std::lock_guard<lock_type> guard_type;

			typedef std::unordered_map<unsigned __int64, variant_callback_type> callback_list_type;
			typedef common::random_uint32 random_number_type;

			explicit listeners(unsigned __int32 id)
				: id_(id){}

			unsigned __int64 add(native_callback_type callback){
				return add_(callback);
			}

			unsigned __int64 add(callback_type callback){
				return add_(callback);
			}

			template <typename unused_type = return_type>
			std::enable_if_t<!std::is_same_v<std::function<unused_type()>, callback_type>, unsigned __int64> add(no_args_callback_type callback){
				return add_no_args_(callback);
			}

			bool remove(unsigned __int64 id){
				if (extract_group_id(id) != id_)
					return false;

				guard_type guard(lock_);
				auto item = callback_list_.find(id);
				if (item == callback_list_.end())
					return false;

				callback_list_.erase(item);
				return true;
			}

			bool remove(native_callback_type callback){
				guard_type guard(lock_);
				for (auto item = callback_list_.begin(); item != callback_list_.end(); ++item){
					if (std::holds_alternative<native_callback_type>(item->second) && std::get<native_callback_type>(item->second) == callback){
						callback_list_.erase(item);
						return true;
					}
				}

				return false;
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

		protected:
			template <typename callback_type>
			unsigned __int64 add_(callback_type callback){
				guard_type guard(lock_);

				auto id = combine_ids(id_, generator_(1, std::numeric_limits<unsigned __int32>::max()));
				if (callback_list_.find(id) != callback_list_.end())
					return 0u;

				callback_list_[id] = callback;
				return id;
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
