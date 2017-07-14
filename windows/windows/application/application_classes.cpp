#include "application_classes.h"
#include "object_manager.h"

winpp::application::classes::classes(){
	::GetClassInfoExW(nullptr, WC_DIALOG, dialog_);

	dialog_.styles(dialog_.styles() | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS);
	dialog_.procedure(object_manager::entry);
	dialog_.name(L"WINPP_DLG_CLS_" WINPP_UUID);
	dialog_.instance(::GetModuleHandleW(nullptr));
	dialog_.create();

	general_.styles(dialog_.styles());
	general_.procedure(dialog_.procedure());
	general_.name(L"WINPP_CLS_" WINPP_UUID);
	general_.cursor(dialog_.cursor());
	general_.instance(dialog_.instance());
	general_.create();
}

winpp::application::classes::wnd_class_type &winpp::application::classes::general(){
	return general_;
}

winpp::application::classes::wnd_class_type &winpp::application::classes::dialog(){
	return dialog_;
}
