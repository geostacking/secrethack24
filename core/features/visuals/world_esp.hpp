#pragma once
#include "../../includes/includes.hpp"
#include "../../game/sdk.hpp"

class c_world_esp 
{
public:
	void render( vec4_t, std::wstring,
		std::wstring, int, base_combat_entity*,
		event_type, base_entity* );
};
extern c_world_esp g_world;