#include "raii.h"

winpp::common::raii::raii(callback_type init, callback_type uninit)
	: uninit_(uninit){
	init();
}

winpp::common::raii::~raii(){
	uninit_();
}
