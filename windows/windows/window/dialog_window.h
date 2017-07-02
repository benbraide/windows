#pragma once

#ifndef WINPP_DIALOG_WINDOW_H
#define WINPP_DIALOG_WINDOW_H

#include "window_object.h"

namespace winpp{
	namespace window{
		class dialog : public object{
		public:
			typedef object base_type;

			dialog();

			virtual ~dialog();

			virtual bool is_dialog() const override;
		};
	}
}

#endif /* !WINPP_DIALOG_WINDOW_H */
