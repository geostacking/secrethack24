#pragma once

namespace client {
	inline uintptr_t on_network_message = NULL;

	void on_network_message_hook( void*, void* );
}