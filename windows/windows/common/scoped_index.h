#pragma once

#ifndef WINPP_SCOPED_INDEX_H
#define WINPP_SCOPED_INDEX_H

#include <unordered_map>
#include <memory>
#include <any>

#include "random_number.h"

namespace winpp{
	namespace common{
		class scoped_index_base{
		public:
			virtual ~scoped_index_base() = default;
		};

		template <class scope_type, class key_type, class value_type>
		class scoped_index : public scoped_index_base{
		public:
			typedef scope_type scope_type;
			typedef key_type key_type;
			typedef value_type value_type;

			typedef basic_random_number<key_type> random_number_type;
			typedef std::shared_ptr<random_number_type> random_number_ptr_type;

			typedef std::unordered_map<key_type, value_type> index_type;
			struct scope_info{
				index_type index;
				random_number_ptr_type random_number_;
			};

			typedef std::unordered_map<scope_type, scope_info> scoped_index_type;

			typedef typename index_type::iterator index_iterator_type;
			typedef typename scoped_index_type::iterator scoped_index_iterator_type;

			key_type add(const scope_type &scope, value_type value){
				auto &scoped = scoped_index_[scope];
				if (scoped.random_number_ == nullptr)//Create random generator
					scoped.random_number_ = std::make_shared<random_number_type>();

				auto key = scoped.random_number_->generate(static_cast<key_type>(1), std::numeric_limits<key_type>::max());
				if (scoped.index.find(key) != scoped.index.end())
					return key_type(0);//Failed to generate a unique key

				scoped.index[key] = value;
				return key;
			}

			void remove(const scope_type &scope, key_type key){
				scoped_index_iterator_type scoped;
				index_iterator_type index;

				if (find_(scope, key, &scoped, index)){//Found -- delete
					scoped->second.index.erase(index);
					if (scoped->second.index.empty())//Empty scope -- delete
						scoped_index_.erase(scoped);
				}
			}

			void remove_value(const scope_type &scope, value_type value){
				if (scoped_index_.empty())
					return;

				auto scoped = scoped_index_.find(scope);
				if (scoped == scoped_index_.end())
					return;

				index_iterator_type index = scoped->second.index.begin();
				for (; index != scoped->second.index.end(); ++index){
					if (index->second == value)
						break;
				}

				if (index != scoped->second.index.end())//Found -- delete
					scoped->second.index.erase(index);
			}

			void remove_value(value_type value){
				for (auto &entry : scoped_index_)
					remove_value(entry.first, value);
			}

			value_type find(const scope_type &scope, key_type key) const{
				index_iterator_type index;
				return find_(scope, key, nullptr, index) ? index->second : value_type();
			}

		private:
			bool find_(const scope_type &scope, const key_type &key, scoped_index_iterator_type *scoped, index_iterator_type &index) const{
				if (scoped_index_.empty())
					return false;

				auto scoped_entry = scoped_index_.find(scope);
				if (scoped_entry == scoped_index_.end())
					return false;

				auto index_entry = scoped_entry->second.index.find(key);
				if (index_entry == scoped_entry->second.index.end())
					return false;

				index = index_entry;
				if (scoped != nullptr)
					*scoped = scoped_entry;

				return true;
			}

			scoped_index_type scoped_index_;
		};
	}
}

#endif /* !WINPP_SCOPED_INDEX_H */
