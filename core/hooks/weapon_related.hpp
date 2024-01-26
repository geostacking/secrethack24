#pragma once
#include "../includes/hinclude.hpp"
#include "../features/options.hpp"

namespace basemelee
{
	inline uintptr_t process_attack = NULL;
	void process_attack_hook( item* base_item, hit_test* hittest );
}