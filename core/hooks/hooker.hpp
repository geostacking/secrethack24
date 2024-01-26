#pragma once
#include "../includes/hinclude.hpp"
#include "../features/menu/gui_hook.hpp"
#include "player_related.hpp"
#include "weapon_related.hpp"
#include "projectile_related.hpp"
#include "movement_related.hpp"
#include "client_related.hpp"

class c_hooks
{
public:
	void init_hooks( );
};

extern c_hooks g_hooks;