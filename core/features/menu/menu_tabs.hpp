#pragma once
#include "../../includes/includes.hpp"

class c_menu {
public:
	int max_distance = 5000;
	bool russian_language = false;

	enum color_tab_t {
		visible = 0,
		invisible = 1,
		npc_visible = 2,
		npc_invisible = 3
	};

	std::vector< std::wstring > bones = { xs( L"head" ), xs( L"neck" ), xs( L"pelvis" ), xs( L"dick" ) };

	std::vector< std::wstring > box_esp_mode = { xs( L"off" ), xs( L"normal" ), xs( L"corner" ) };

	std::vector< std::wstring > aim_mode = { xs( L"off" ), xs( L"silent" ), xs( L"memory" ), xs( L"psilent" ) };
	
	std::vector< std::wstring > sound_list = { xs( L"plop" ), xs( L"plop 2" ), xs( L"bell" ), xs( L"bepis" ), xs( L"fatality" ),
		xs( L"phonk" ), xs( L"sparkle" ), xs( L"cock" ), xs( L"bameware" ), xs( L"dopium" ) };

	std::vector< std::wstring > smooth_type = { xs( L"off" ), xs( L"constant" ), xs( L"regular" ) };

	std::vector< std::wstring > chams_vector = { xs( L"flat" ), xs( L"hologram" ), xs( L"invisible" ), xs( L"galaxy" ), xs( L"transparent" ), xs( L"wireframe" ), xs( L"circuit" ) };

	std::vector< std::wstring > cfg_vector = { xs( L"rage" ), xs( L"blatant" ), xs( L"legit" ), xs( L"secret" ), xs( L"ziebel" ), xs( L"alpha" ) };

	std::vector< std::wstring > color_list = { xs( L"visible" ), xs( L"invisible" ), xs( L"npc visible" ), xs( L"npc invisible" ) };

	std::vector< std::wstring > tab_list = { xs( L"first" ), xs( L"second" ) };

	std::vector< std::wstring > grade_tier_list = { xs( L"none" ), xs( L"wood" ), xs( L"stone" ), xs( L"metal" ), xs( L"hqm" ) };

	std::vector< std::wstring > font_flags = { xs( L"none" ), xs( L"dropshadow" ), xs( L"outline" ) };

	std::vector< std::wstring > tab_choice = { xs( L"choice" ), xs( L"colors" ), xs( L"sliders" ) };

	std::vector< std::wstring > key_vector =
	{
		xs( L"left mouse button" ), xs( L"right mouse button" ),
		xs( L"middle mouse button" ), xs( L"x1 mouse button" ),
		xs( L"x2 mouse button" ), xs( L"control-break processing" ),
		xs( L"backspace" ), xs( L"tab" ), xs( L"clear" ), xs( L"enter" ),
		xs( L"shift" ), xs( L"ctrl" ), xs( L"alt" ), xs( L"caps lock" ),
		xs( L"esc" ), xs( L"space" ), xs( L"a" ), xs( L"b" ), xs( L"c" ), xs( L"d" ), xs( L"e" ),
		xs( L"f" ), xs( L"g" ), xs( L"h" ), xs( L"i" ), xs( L"j" ), xs( L"k" ), xs( L"l" ), xs( L"m" ), xs( L"n" ),
		xs( L"o" ), xs( L"p" ), xs( L"q" ), xs( L"r" ), xs( L"s" ), xs( L"t" ), xs( L"u" ),
		xs( L"v" ), xs( L"w" ), xs( L"x" ), xs( L"y" ), xs( L"z" ), xs( L"f1" ), xs( L"f2" ),
		xs( L"f3" ), xs( L"f4" ), xs( L"f5" ), xs( L"f6" ), xs( L"f7" ), xs( L"f8" ), xs( L"f9" ),
		xs( L"f10" ), xs( L"f11" )
	};

	std::vector< std::wstring > fire_type =
	{
		xs( L"desync" ), xs( L"normal" )
	};

	std::vector< std::wstring > hitboxes_list =
	{
		xs( L"head" ), xs( L"stomach" ), xs( L"chest" )
	};

	std::vector< std::wstring > default_gestures =
	{
		xs( L"clap" ), xs( L"friendly" ), xs( L"thumbsdown" ),
		xs( L"thumbsup" ), xs( L"ok" ), xs( L"point" ), xs( L"shrug" ), xs( L"victory" ),
		xs( L"wave" ), xs( L"cabbage patch" ), xs( L"twist" )
	};

	/* draw menu tab */
	void draw_menu_tab( );

	/* draw aimbot tab */
	void draw_aimbot_tab( );

	/* draw visuals tab */
	void draw_visuals_tab( );

	/* draw world tab */
	void draw_worlds_tab( );
	
	/* draw combat tab */
	void draw_combat_tab( );

	/* draw time tab */
	void draw_time_tab( );

	/* draw movement tab */
	void draw_movement_tab( );

	/* draw misc tab */
	void draw_misc_tab( );

	/* draw modulations tab */
	void draw_modulations_tab( );

	/* draw automations tab */
	void draw_automations_tab( );

	/* draw menu */
	void draw_menu( );
};

extern c_menu g_menu;