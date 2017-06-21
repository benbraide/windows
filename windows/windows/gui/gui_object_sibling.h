#pragma once

#ifndef WINPP_GUI_OBJECT_SIBLING_H
#define WINPP_GUI_OBJECT_SIBLING_H

#include "gui_object.h"

namespace winpp{
	namespace gui{
		class object_sibling : public object{
		public:
			object_sibling(const gui_object_type &object, sibling_type type);

			object_sibling(gui_object_type &object, sibling_type type);

			virtual ~object_sibling() = default;

			virtual app_type *app() const override;

			virtual void *handle() const override;

			virtual object *non_sibling() override;

			virtual const object *non_sibling() const override;

			virtual object *parent() const override;

			virtual object *ancestor(index_and_size_type index) const override;

			virtual object *child_at_index(index_and_size_type index) const override;

			virtual object *child_at_absolute_index(index_and_size_type index) const override;

			virtual object &traverse_children(object_traverser_type traverser) override;

			virtual const object &traverse_children(object_traverser_type traverser) const override;

			virtual object &traverse_siblings(sibling_traverser_type traverser) override;

			virtual const object &traverse_siblings(sibling_traverser_type traverser) const override;

			virtual object &traverse_ancestors(object_traverser_type traverser) override;

			virtual const object &traverse_ancestors(object_traverser_type traverser) const override;

			virtual object &internal_set_parent(gui_object_type *parent) override;

			virtual index_and_size_type internal_insert_into_parent(gui_object_type &object) override;

			virtual index_and_size_type internal_insert_child(gui_object_type &child, index_and_size_type before = invalid_index) override;

			virtual object &internal_remove_child(gui_object_type &child, bool force = false) override;

			virtual object &outer_rect(const rect_type &value) override;

			virtual rect_type outer_rect() const override;

			virtual rect_type relative_outer_rect() const override;

			virtual object &inner_rect(const rect_type &value) override;

			virtual rect_type inner_rect() const override;

			virtual rect_type content_rect() const override;

			virtual object &padding(const rect_type &value) override;

			virtual rect_type padding() const override;

			virtual object &outer_size(const size_type &value) override;

			virtual size_type outer_size() const override;

			virtual object &inner_size(const size_type &value) override;

			virtual size_type inner_size() const override;

			virtual object &offset(const size_type &value) override;

			virtual point_type offset() const override;

			virtual point_type relative_offset() const override;

			virtual object &move(const point_type &value) override;

			virtual object &absolute_move(const point_type &value) override;

			virtual object &place(placement_type value, const size_type &delta = size_type()) override;

			virtual point_type compute_placement(placement_type value, const size_type &delta = size_type()) const override;

			virtual hit_target_type hit_test(const point_type &value) const override;

			virtual gui_object_type *hit_target(const point_type &value) const override;

			virtual gui_object_type *deepest_hit_target(const point_type &value) const override;

			virtual point_type convert_to_screen(const point_type &value) const override;

			virtual rect_type convert_to_screen(const rect_type &value) const override;

			virtual point_type convert_from_screen(const point_type &value) const override;

			virtual rect_type convert_from_screen(const rect_type &value) const override;

			virtual object &destroy(bool force = false) override;

			virtual gui_attributes_type &attributes() override;

			virtual event_tunnel &events() override;

			virtual index_and_size_type proposed_index() const override;

			virtual index_and_size_type index_in_parent() const override;

			virtual index_and_size_type absolute_index_in_parent() const override;

			virtual index_and_size_type child_index(const gui_object_type &object) const override;

			virtual index_and_size_type child_absolute_index(const gui_object_type &object) const override;

			virtual index_and_size_type absolute_index(index_and_size_type index) const override;

			virtual index_and_size_type children_count() const override;

			virtual index_and_size_type sibling_count() const override;

			virtual index_and_size_type ancestor_count() const override;

			virtual bool is_sibling() const override;

			virtual bool is_group() const override;

			virtual bool is_tree() const override;

			virtual bool is_inside_group() const override;

			virtual bool has_children() const override;

			virtual bool has_siblings() const override;

			virtual bool has_parent() const override;

			virtual bool is_offspring(const gui_object_type &object) const override;

			virtual bool is_child(const gui_object_type &object) const override;

			virtual bool is_parent(const gui_object_type &object) const override;

			virtual bool is_ancestor(const gui_object_type &object) const override;

			virtual bool is_sibling(const gui_object_type &object) const override;

			virtual bool is_created() const override;

			virtual bool is_previous() const;

		private:
			gui_object_type *object_;
			sibling_type type_;
		};
	}
}

#endif /* !WINPP_GUI_OBJECT_SIBLING_H */
