#pragma once
#include "cfg.hpp"

namespace perf_ui
{
	inline uintptr_t ongui = NULL;
	inline uintptr_t performance_ui_update = NULL;

	void ongui_hook( );

	inline static bool first_loaded = true;
	void performance_ui_update_hook( void* instance );
}