#pragma once

#ifndef WINPP_GUI_OBJECT_GROUP_H
#define WINPP_GUI_OBJECT_GROUP_H

#include "generic_gui_object.h"

namespace winpp{
	namespace gui{
		class object_group : public generic_object{
		public:
			typedef std::list<gui_object_type *> list_type;
			typedef std::unordered_map<unsigned int, list_type> group_list_type;

			class object_attributes : public gui::object_attributes{
			public:
				typedef gui::object_attributes base_type;

				explicit object_attributes(object &object);

				virtual ~object_attributes();

				object_attributes &monitor_content(callback_type callback);

				object_attributes &fill_content(const rect_type &padding = rect_type());

				bool monitoring_content() const;
			};

			virtual object *child_at_index(index_and_size_type index) const override;

			virtual object *child_at_absolute_index(index_and_size_type index) const override;

			virtual object &traverse_children(object_traverser_type traverser) override;

			virtual const object &traverse_children(object_traverser_type traverser) const override;

			virtual rect_type content_rect() const override;

			virtual index_and_size_type internal_insert_child(gui_object_type &child, index_and_size_type before = invalid_index) override;

			virtual object &internal_remove_child(gui_object_type &child, force_type force = force_type::dont_force) override;

			virtual gui_object_type *hit_target(const point_type &value) const override;

			virtual gui_object_type *deepest_hit_target(const point_type &value) const override;

			virtual object_attributes &attributes() override;

			virtual index_and_size_type child_index(const gui_object_type &object) const override;

			virtual index_and_size_type child_absolute_index(const gui_object_type &object) const override;

			virtual index_and_size_type absolute_index(index_and_size_type index) const override;

			virtual index_and_size_type children_count() const override;

			virtual bool is_group() const override;

			virtual bool has_children() const override;

			virtual bool is_offspring(const gui_object_type &object) const override;

			virtual bool is_child(const gui_object_type &object) const override;

		protected:
			virtual void destroyed_() override;

			virtual attributes_type get_attributes_() override;

			virtual bool pre_insert_(gui_object_type &object, index_and_size_type &index);

			virtual void insert_(gui_object_type &child, index_and_size_type &before);

			virtual index_and_size_type post_insert_(gui_object_type &object, index_and_size_type index);

			virtual bool pre_remove_(gui_object_type &object);

			virtual void remove_(gui_object_type &object);

			virtual void post_remove_(gui_object_type &object);

			virtual bool cache_group_(unsigned int value) const;

			list_type children_;
			group_list_type group_list_;
		};
	}
}

#endif /* !WINPP_GUI_OBJECT_GROUP_H */
