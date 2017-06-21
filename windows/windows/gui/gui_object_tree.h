#pragma once

#ifndef WINPP_GUI_OBJECT_TREE_H
#define WINPP_GUI_OBJECT_TREE_H

#include "gui_object_group.h"

namespace winpp{
	namespace gui{
		class object_tree : public object_group{
		public:
			virtual ~object_tree();

			virtual object *child_at_absolute_index(index_and_size_type index) const override;

			virtual index_and_size_type child_absolute_index(const gui_object_type &object) const override;

			virtual index_and_size_type absolute_index(index_and_size_type index) const override;

			virtual bool is_group() const override;

			virtual bool is_tree() const override;
		};
	}
}

#endif /* !WINPP_GUI_OBJECT_TREE_H */
