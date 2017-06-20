#pragma once

#ifndef WINPP_GUI_OBJECT_ATTRIBUTES_H
#define WINPP_GUI_OBJECT_ATTRIBUTES_H

#include <mutex>
#include <functional>
#include <unordered_map>
#include <list>

#include "../structures/rect_structure.h"

namespace winpp{
	namespace gui{
		class object;

		class object_attributes{
		public:
			typedef structures::enumerations::placement_type placement_type;

			typedef structures::point point_type;
			typedef structures::size size_type;
			typedef structures::sizef sizef_type;
			typedef structures::rect rect_type;

			typedef std::function<void(unsigned __int64)> callback_type;
			typedef std::function<void(const object &)> sibling_callback_type;

			typedef std::unordered_map<unsigned __int64, callback_type> callback_list_type;
			typedef std::unordered_map<const object *, sibling_callback_type> sibling_callback_list_type;

			typedef std::list<const object *> object_list_type;

			typedef std::mutex lock_type;
			typedef std::lock_guard<lock_type> guard_type;

			explicit object_attributes(object &object);

			virtual ~object_attributes();

			object_attributes &trigger(unsigned __int64 types);

			object_attributes &trigger(const object &sibling);

			object_attributes &monitor_parent_size(callback_type callback);

			object_attributes &monitor_siblings(callback_type callback);

			object_attributes &monitor_sibling(const object &sibling, sibling_callback_type callback);

			object_attributes &stop_monitoring(unsigned __int64 type);

			object_attributes &stop_monitoring(const object &sibling);

			object_attributes &stop_monitoring();

			object_attributes &fill_parent_size(const rect_type &padding = rect_type());

			object_attributes &fill_parent_size(placement_type placement, const rect_type &padding = rect_type());

			object_attributes &fill_parent_proportional_size(const sizef_type &proportion, const rect_type &padding = rect_type());

			object_attributes &fill_parent_proportional_size(const sizef_type &proportion, placement_type placement, const rect_type &padding = rect_type());

			object_attributes &fill_parent_width(const size_type &padding = size_type());

			object_attributes &fill_parent_proportional_width(float proportion, const size_type &padding = size_type());

			object_attributes &fill_parent_height(const size_type &padding = size_type());

			object_attributes &fill_parent_proportional_height(float proportion, const size_type &padding = size_type());

			object_attributes &place_in_parent(placement_type placement, const size_type &offset = size_type());

			object_attributes &place_beside_sibling(const object &sibling, placement_type placement, const size_type &offset = size_type());

			bool monitoring_parent_size() const;

			bool monitoring_siblings() const;

			bool monitoring_sibling(const object &sibling) const;

			const object_list_type &dependent_children() const;

			const object_list_type &dependent_siblings() const;

			static const unsigned __int64 type_nil				= (0ull << 0x0000);
			static const unsigned __int64 type_parent_size		= (1ull << 0x0000);
			static const unsigned __int64 type_siblings_size	= (1ull << 0x0001);
			static const unsigned __int64 last_pow				= 0x0001;

		protected:
			virtual object_attributes &trigger_(unsigned __int64 types);

			virtual object_attributes &trigger_(const object &sibling);

			virtual unsigned __int64 last_pow_() const;

			void add_dependent_child_(const object &object);

			void remove_dependent_child_(const object &object);

			void add_dependent_sibling_(const object &object);

			void remove_dependent_sibling_(const object &object);

			object *object_;
			unsigned __int64 active_;
			callback_list_type callback_list_;
			sibling_callback_list_type sibling_callback_list_;
			object_list_type dependent_children_;
			object_list_type dependent_siblings_;
			mutable lock_type lock_;
		};
	}
}

#endif /* !WINPP_GUI_OBJECT_ATTRIBUTES_H */
