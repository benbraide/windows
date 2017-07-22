#pragma once

#ifndef WINPP_MENU_NEWLINE_H
#define WINPP_MENU_NEWLINE_H

#include "menu_group.h"

namespace winpp{
	namespace menu{
		class newline : public group{
		public:
			newline();

			newline(gui_object_type &parent);

			virtual ~newline();

			virtual newline &update_state() override;

			virtual bool is_group() const override;

		protected:
			virtual index_and_size_type post_insert_(gui_object_type &object, index_and_size_type index);

			virtual void post_remove_(gui_object_type &object);
		};

		class bordered_newline : public newline{
		public:
			bordered_newline();

			bordered_newline(gui_object_type &parent);

			virtual ~bordered_newline();
		};
	}
}

#endif /* !WINPP_MENU_NEWLINE_H */
