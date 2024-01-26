#pragma once
#include "../includes/hinclude.hpp"
#include "../features/options.hpp"

namespace baseplayer {
	inline uintptr_t client_input = NULL;
	inline uintptr_t fix_culling = NULL;
	inline uintptr_t on_attacked = NULL;

	void auto_fire( item* item, base_player* local_player );
	void client_input_hook( base_player* this_player, input_state* state );
	void fix_culling_hook( base_player* baseplayer, float dist, bool visibility = false );
	void on_attacked_hook( void* instance, hit_info* this_hit_info );
}

namespace item_icon_hk {
	inline uintptr_t try_to_move = NULL;

	void try_to_move_hook( item_icon* this_item );
}