#include "dialog_window.h"

winpp::window::dialog::dialog()
	: base_type(::DefDlgProcW){}

winpp::window::dialog::~dialog(){
	destroy(true);
}

bool winpp::window::dialog::is_dialog() const{
	return true;
}
