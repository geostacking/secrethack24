#pragma once
#include "../includes/hinclude.hpp"
#include "../features/options.hpp"

namespace _player_walk_movement {
	inline uintptr_t client_input = NULL;
	void client_input_hook( player_walk_movement* self, void* input, model_state* state );
}