#include "menu_tabs.hpp"
#include "framework/gui_framework.hpp"
#include "../options.hpp"
#include "../aimbot/aimbot.hpp"

c_menu g_menu;

void c_menu::draw_menu_tab( ) {
	g_framework.create_check_box( russian_language ? xs( L"�����������" ) : xs( L"rusification" ), &russian_language, 1 );
	g_framework.create_check_box( russian_language ? xs( L"�������� �������" ) : xs( L"rainbow accent" ), &options::rainbow_accent, 1 );
	g_framework.color_picker( russian_language ? xs( L"���� �������" ) : xs( L"menu accent color" ), &options::accent_color, 1 );

	g_framework.create_label( russian_language ? xs( L"����� �������" ) :  xs( L"quick config" ), 1 );
	g_framework.create_combo_box( cfg_vector, &options::cfg_index, 1 );
	g_framework.create_check_box( russian_language ? xs( L"���������" ) : xs( L"load" ), &options::load_cfg, 1 );
	g_framework.create_check_box( russian_language ? xs( L"���������" ) : xs( L"save" ), &options::save_cfg, 1 );

	g_framework.create_label( russian_language ? xs( L"��� ����� ������" ) : xs( L"esp font flags" ), 1 );
	g_framework.create_combo_box( font_flags, &options::font_flags, 1 );
				
	g_framework.create_label( russian_language ? xs( L"����� ����������" ) : xs( L"local indicators" ), 2 );
	g_framework.create_check_box( russian_language ? xs( L"����������" ) : xs( L"indicators" ), &options::visuals::indicators, 2 );
	if( options::visuals::indicators )
		g_framework.create_check_box( russian_language ? xs( L"����-��� ���������" ) : xs( L"fly hack indicator" ), &options::visuals::fly_hack_indicator, 2 );

	g_framework.create_check_box( russian_language ? xs( L"��� �����" ) : xs( L"hit sound" ), &options::aimbot::hit_sound, 2 );
	g_framework.create_combo_box( sound_list, &options::aimbot::sound_type, 2 );
	g_framework.slider_float( russian_language ? xs( L"���������" ) :  xs( L"volume" ), &options::aimbot::hit_sound_volume, 0.0f, 100.f, 2 );

	g_framework.create_check_box( russian_language ? xs( L"��� ��������" ) : xs( L"draw hit bullet tracers" ), &options::visuals::draw_tracers, 2 );
	g_framework.color_picker( russian_language ? xs( L"���� ��� ���������" ) : xs( L"hit tracers color" ), &options::visuals::tracers_color, 2 );
	g_framework.create_check_box( russian_language ? xs( L"����� ������" ) : xs( L"draw local trails" ), &options::visuals::local_trails, 2 );
	g_framework.color_picker( russian_language ? xs( L"���� ����� ����� ������" ) : xs( L"trails color" ), &options::visuals::trails_color, 2 );
	g_framework.create_check_box( russian_language ? xs( L"������ ���������������" ) : xs( L"draw desync bar" ), &options::visuals::show_desync_bar, 2 );
	g_framework.color_picker( russian_language ? xs( L"���� ������ ���������������" ) : xs( L"desync bar color" ), &options::visuals::desync_bar_color, 2 );
	g_framework.create_check_box( russian_language ? xs( L"������ �����������" ) : xs( L"draw reload bar" ), &options::visuals::show_reload_bar, 2 );
	g_framework.color_picker( russian_language ? xs( L"���� ������ �����������" ) : xs( L"reload bar color" ), &options::visuals::reload_bar_color, 2 );

	g_framework.create_label( russian_language ? xs( L"�������� �������" ) : xs( L"location manager" ), 2 );
	g_framework.create_hotkey( russian_language ? xs( L"������� �������" ) : xs( L"remove last position" ), &options::visuals::remove_positions_key, 2 );
	g_framework.create_hotkey( russian_language ? xs( L"��������� �������" ) : xs( L"save pos key" ), &options::visuals::save_pos_key, 2 );
	g_framework.create_check_box( russian_language ? xs( L"������� ����" ) : xs( L"is home position" ), &options::visuals::is_home_pos, 2 );
	g_framework.color_picker( russian_language ? xs( L"���� �������" ) : xs( L"position color" ), &options::visuals::pos_color, 2 );
}

void c_menu::draw_aimbot_tab( ) {
	g_framework.create_combo_box( aim_mode, &options::aimbot::aim_type, 1 );

	g_framework.create_check_box( xs( L"auto-fire" ), &options::aimbot::auto_fire, 1 );
	g_framework.create_combo_box( fire_type, &options::aimbot::auto_fire_type, 1 );

	g_framework.create_check_box( xs( L"ignore melee" ), &options::aimbot::ignore_melee, 1 );

	g_framework.create_check_box( xs( L"auto reload" ), &options::aimbot::auto_reload, 1 );
	g_framework.create_check_box( xs( L"simulate player movement" ), &options::aimbot::should_simulate_movement, 1 );

	g_framework.create_check_box( xs( L"aim at nearest bone" ), &options::aimbot::aim_nearest_bone, 1 );
	if( !options::aimbot::aim_nearest_bone )
		g_framework.create_combo_box( bones, &options::aimbot::aim_bone, 1 );

	g_framework.create_hotkey( xs( L"aim key" ), &g_aimbot.aim_key, 1 );
				
	g_framework.create_check_box( xs( L"draw field of view circle" ), &options::aimbot::draw_fov, 1 );
	g_framework.color_picker( xs( L"fov circle color" ), &options::aimbot::fov_circle_color, 1 );

	g_framework.slider_int( xs( L"field of view" ), &options::aimbot::fov_amount, 0, 1000, 1 );

	g_framework.create_label( xs( L"smooth type" ), 1 );
	g_framework.create_combo_box( smooth_type, &options::aimbot::smooth_type, 1 );
	g_framework.slider_float( xs( L"smoothness amount" ), &options::aimbot::smoothness_amount, 0.0f, 10.f, 1 );

	g_framework.create_check_box( xs( L"draw line to target" ), &options::aimbot::draw_target, 1 );
	g_framework.color_picker( xs( L"line color" ), &options::aimbot::target_line_color, 1 );

	g_framework.create_check_box( xs( L"aim at knocked" ), &options::aimbot::aim_knocked, 1 );
	g_framework.create_check_box( xs( L"aim at sleepers" ), &options::aimbot::aim_sleepers, 1 );
	g_framework.create_check_box( xs( L"aim at npc" ), &options::aimbot::aim_npc, 1 );
	g_framework.create_check_box( xs( L"aim at helicopter" ), &options::aimbot::aim_helicopter, 1 );
	g_framework.create_check_box( xs( L"only aim at visible" ), &options::aimbot::aim_check, 1 );

	g_framework.create_check_box( xs( L"draw crosshair" ), &options::aimbot::draw_crosshair, 1 );
	g_framework.create_check_box( xs( L"killaura" ), &options::aimbot::kill_aura, 1 );

	g_framework.slider_int( xs( L"target max distance" ), &options::aimbot::max_target_distance, 0, options::visuals::max_player_distance, 2 );

	g_framework.create_label( xs( L"manipulator" ), 2 );
	g_framework.create_check_box( xs( L"visualize manipulation" ), &options::aimbot::visualize_manipulation, 2 );

	g_framework.create_check_box( xs( L"manipulator" ), &options::aimbot::manipulation, 2 );
	g_framework.create_check_box( xs( L"manipulate visible" ), &options::aimbot::visible_manipulation, 2 );
	g_framework.create_check_box( xs( L"bullet teleport" ), &options::aimbot::bullet_tp, 2 );

	g_framework.create_hotkey( xs( L"manipulator key" ), &options::aimbot::manipulation_key, 2 );
	g_framework.create_check_box( xs( L"generate points" ), &options::aimbot::generate_points, 2 );
	g_framework.slider_int( xs( L"max angles" ), &options::aimbot::manipulation_angles, 0, 1000, 2 );
	if( options::aimbot::generate_points ) {
		g_framework.create_check_box( xs( L"use half points" ), &options::aimbot::use_half_points, 2 );
		g_framework.create_check_box( xs( L"use max eye points" ), &options::aimbot::use_max_eye_points, 2 );
		
	}

	g_framework.create_check_box( xs( L"ricochet" ), &options::aimbot::exploits::combat::ricochet, 2 );

	g_framework.create_check_box( xs( L"override hitbox" ), &options::aimbot::exploits::combat::override_hitbox, 2 );
	g_framework.create_combo_box( hitboxes_list, &options::aimbot::exploits::combat::overriden_hitbox, 2 );
}

void c_menu::draw_visuals_tab( ) {
	g_framework.create_check_box( xs( L"master-switch" ), &options::visuals::draw_visuals, 1 );
	g_framework.create_combo_box( color_list, &options::color_tab, 1 );
	if( options::visuals::draw_visuals ) {
		g_framework.create_check_box( xs( L"name" ), &options::visuals::show_name, 1 );
		switch( options::color_tab ) {
		case invisible:
			g_framework.color_picker( xs( L"name color" ), &options::visuals::name_color, 1 );
			break;
		case visible:
			g_framework.color_picker( xs( L"name color" ), &options::visuals::vis_name_color, 1 );
			break;
		case npc_invisible:
			g_framework.color_picker( xs( L"name color" ), &options::visuals::npc_name_color, 1 );
			break;
		case npc_visible:
			g_framework.color_picker( xs( L"name color" ), &options::visuals::npc_vis_name_color, 1 );
			break;
		}

		g_framework.create_check_box( xs( L"radar" ), &options::visuals::draw_radar, 1 );
		switch( options::color_tab ) {
		case invisible:
			g_framework.color_picker( xs( L"radar color" ), &options::visuals::radar_color, 1 );
			break;
		case visible:
			g_framework.color_picker( xs( L"radar color" ), &options::visuals::vis_radar_color, 1 );
			break;
		case npc_invisible:
			g_framework.color_picker( xs( L"radar color" ), &options::visuals::npc_radar_color, 1 );
			break;
		case npc_visible:
			g_framework.color_picker( xs( L"radar color" ), &options::visuals::npc_vis_radar_color, 1 );
			break;
		}

		g_framework.create_combo_box( box_esp_mode, &options::visuals::box_type, 1 );
		switch( options::color_tab ) {
		case invisible:
			g_framework.color_picker( xs( L"box color" ), &options::visuals::box_color, 1 );
			break;
		case visible:
			g_framework.color_picker( xs( L"box color" ), &options::visuals::vis_box_color, 1 );
			break;
		case npc_invisible:
			g_framework.color_picker( xs( L"box color" ), &options::visuals::npc_box_color, 1 );
			break;
		case npc_visible:
			g_framework.color_picker( xs( L"box color" ), &options::visuals::npc_vis_box_color, 1 );
			break;
		}

		g_framework.create_check_box( xs( L"health" ), &options::visuals::show_health, 1 );
		if( options::visuals::show_health ) {
			g_framework.slider_int( xs( L"health thickness" ), &options::visuals::health_thickness, 1, 3, 1 );
			g_framework.create_check_box( xs( L"custom health color" ), &options::visuals::modify_health_color, 1 );
			if( options::visuals::modify_health_color ) {
				switch( options::color_tab ) {
				case invisible:
					g_framework.color_picker( xs( L"health color" ), &options::visuals::custom_health_color, 1 );
					break;
				case visible:
					g_framework.color_picker( xs( L"health color" ), &options::visuals::vis_custom_health_color, 1 );
					break;
				case npc_invisible:
					g_framework.color_picker( xs( L"health color" ), &options::visuals::npc_custom_health_color, 1 );
					break;
				case npc_visible:
					g_framework.color_picker( xs( L"health color" ), &options::visuals::npc_vis_custom_health_color, 1 );
					break;
				}
			}
		}
		g_framework.create_check_box( xs( L"health text" ), &options::visuals::show_health_text, 1 );
		switch( options::color_tab ) {
		case invisible:
			g_framework.color_picker( xs( L"health text color" ), &options::visuals::health_text_color, 1 );
			break;
		case visible:
			g_framework.color_picker( xs( L"health text color" ), &options::visuals::vis_health_text_color, 1 );
			break;
		case npc_invisible:
			g_framework.color_picker( xs( L"health text color" ), &options::visuals::npc_health_text_color, 1 );
			break;
		case npc_visible:
			g_framework.color_picker( xs( L"health text color" ), &options::visuals::npc_vis_health_text_color, 1 );
			break;
		}

		g_framework.create_check_box( xs( L"skeleton" ), &options::visuals::draw_bones, 1 );
		switch( options::color_tab ) {
		case invisible:
			g_framework.color_picker( xs( L"skeleton color" ), &options::visuals::skeleton_color, 1 );
			break;
		case visible:
			g_framework.color_picker( xs( L"skeleton color" ), &options::visuals::vis_skeleton_color, 1 );
			break;
		case npc_invisible:
			g_framework.color_picker( xs( L"skeleton color" ), &options::visuals::npc_skeleton_color, 1 );
			break;
		case npc_visible:
			g_framework.color_picker( xs( L"skeleton color" ), &options::visuals::npc_vis_skeleton_color, 1 );
			break;
		}

		g_framework.create_check_box( xs( L"weapon" ), &options::visuals::show_weapon, 1 );
		switch( options::color_tab ) {
		case invisible:
			g_framework.color_picker( xs( L"weapon color" ), &options::visuals::weapon_color, 1 );
			break;
		case visible:
			g_framework.color_picker( xs( L"weapon color" ), &options::visuals::vis_weapon_color, 1 );
			break;
		case npc_invisible:
			g_framework.color_picker( xs( L"weapon color" ), &options::visuals::npc_weapon_color, 1 );
			break;
		case npc_visible:
			g_framework.color_picker( xs( L"weapon color" ), &options::visuals::npc_vis_weapon_color, 1 );
			break;
		}

		g_framework.create_check_box( xs( L"distance" ), &options::visuals::show_distance, 1 );
		switch( options::color_tab ) {
		case invisible:
			g_framework.color_picker( xs( L"distance color" ), &options::visuals::distance_color, 1 );
			break;
		case visible:
			g_framework.color_picker( xs( L"distance color" ), &options::visuals::vis_distance_color, 1 );
			break;
		case npc_invisible:
			g_framework.color_picker( xs( L"distance color" ), &options::visuals::npc_distance_color, 1 );
			break;
		case npc_visible:
			g_framework.color_picker( xs( L"distance color" ), &options::visuals::npc_vis_distance_color, 1 );
			break;
		}

		g_framework.create_check_box( xs( L"snap-lines" ), &options::visuals::show_lines, 1 );
		switch( options::color_tab ) {
		case invisible:
			g_framework.color_picker( xs( L"lines color" ), &options::visuals::snap_lines_color, 1 );
			break;
		case visible:
			g_framework.color_picker( xs( L"lines color" ), &options::visuals::vis_snap_lines_color, 1 );
			break;
		case npc_invisible:
			g_framework.color_picker( xs( L"lines color" ), &options::visuals::npc_snap_lines_color, 1 );
			break;
		case npc_visible:
			g_framework.color_picker( xs( L"lines color" ), &options::visuals::npc_vis_snap_lines_color, 1 );
			break;
		}

		g_framework.create_check_box( xs( L"show view direction" ), &options::visuals::show_view_direction, 1 );
		switch( options::color_tab ) {
		case invisible:
			g_framework.color_picker( xs( L"view direction color" ), &options::visuals::view_direction_color, 1 );
			break;
		case visible:
			g_framework.color_picker( xs( L"view direction color" ), &options::visuals::vis_view_direction_color, 1 );
			break;
		case npc_invisible:
			g_framework.color_picker( xs( L"view direction color" ), &options::visuals::npc_view_direction_color, 1 );
			break;
		case npc_visible:
			g_framework.color_picker( xs( L"view direction color" ), &options::visuals::npc_vis_view_direction_color, 1 );
			break;
		}

		g_framework.create_check_box( xs( L"offscreen arrows" ), &options::visuals::offscreen_arrows, 1 );
		switch( options::color_tab ) {
		case invisible:
			g_framework.color_picker( xs( L"oof color" ), &options::visuals::oof_color, 1 );
			break;
		case visible:
			g_framework.color_picker( xs( L"oof color" ), &options::visuals::vis_oof_color, 1 );
			break;
		case npc_invisible:
			g_framework.color_picker( xs( L"oof color" ), &options::visuals::npc_oof_color, 1 );
			break;
		case npc_visible:
			g_framework.color_picker( xs( L"oof color" ), &options::visuals::npc_vis_oof_color, 1 );
			break;
		}

		g_framework.create_check_box( xs( L"chams" ), &options::visuals::chams, 1 );
		if( options::visuals::chams )
			g_framework.create_check_box( xs( L"rgb chams" ), &options::visuals::rgb_chams, 1 );

		g_framework.create_combo_box( chams_vector, &options::visuals::chams_type, 1 );
					
		g_framework.create_label( xs( L"states" ), 2 );
		g_framework.create_check_box( xs( L"show sleepers" ), &options::visuals::can_show_sleepers, 2 );
		g_framework.create_check_box( xs( L"show knocked" ), &options::visuals::can_show_knocked, 2 );
		g_framework.create_check_box( xs( L"show npc" ), &options::visuals::can_show_npc, 2 );
		if( options::visuals::offscreen_arrows ) {
			g_framework.create_check_box( xs( L"show sleepers oof" ), &options::visuals::can_show_sleepers_oof, 2 );
			g_framework.create_check_box( xs( L"show knocked oof" ), &options::visuals::can_show_knocked_oof, 2 );
			g_framework.create_check_box( xs( L"show npc oof" ), &options::visuals::can_show_npc_oof, 2 );
		}

		g_framework.create_label( xs( L"flags" ), 2 );
		g_framework.create_check_box( xs( L"in duck" ), &options::visuals::show_ducking_flag, 2 );
		switch( options::color_tab ) {
		case invisible:
			g_framework.color_picker( xs( L"ducking color" ), &options::visuals::ducking_color, 2 );
			break;
		case visible:
			g_framework.color_picker( xs( L"ducking color" ), &options::visuals::vis_ducking_color, 2 );
			break;
		case npc_invisible:
			g_framework.color_picker( xs( L"ducking color" ), &options::visuals::npc_ducking_color, 2 );
			break;
		case npc_visible:
			g_framework.color_picker( xs( L"ducking color" ), &options::visuals::npc_vis_ducking_color, 2 );
			break;
		}

		g_framework.create_check_box( xs( L"is knocked" ), &options::visuals::show_knocked_flag, 2 );
		switch( options::color_tab ) {
		case invisible:
			g_framework.color_picker( xs( L"knocked color" ), &options::visuals::knocked_color, 2 );
			break;
		case visible:
			g_framework.color_picker( xs( L"knocked color" ), &options::visuals::vis_knocked_color, 2 );
			break;
		case npc_invisible:
			g_framework.color_picker( xs( L"knocked color" ), &options::visuals::npc_knocked_color, 2 );
			break;
		case npc_visible:
			g_framework.color_picker( xs( L"knocked color" ), &options::visuals::npc_vis_knocked_color, 2 );
			break;
		}

		g_framework.create_label( xs( L"information" ), 2 );
		g_framework.create_check_box( xs( L"show hot-bar" ), &options::visuals::show_hotbar, 2 );
		g_framework.create_check_box( xs( L"show clothes" ), &options::visuals::show_clothes, 2 );

		g_framework.slider_int( xs( L"player max distance" ), &options::visuals::max_player_distance, 0, max_distance, 2 );

		if( !options::visuals::rgb_chams ) {
			if( options::visuals::chams_type != 3 ) {
				bool one_color = options::visuals::chams_type == 6 || options::visuals::chams_type == 5;
				if( options::color_tab == invisible
					|| one_color )
					g_framework.color_picker( xs( L"chams color" ), &options::visuals::chams_color, 1 );
				else
					g_framework.color_picker( xs( L"chams color" ), &options::visuals::chams_visible, 1 );
			}
		}
	}
}

void c_menu::draw_worlds_tab( ) {
	g_framework.create_check_box( xs( L"master-switch" ), &options::visuals::world::master_switch, 1 );

	if( options::visuals::world::master_switch ) {
		g_framework.create_check_box( xs( L"show distance" ), &options::visuals::world::show_distance, 1 );
		g_framework.create_check_box( xs( L"show health" ), &options::visuals::world::show_health, 1 );
		g_framework.create_check_box( xs( L"show amount of items" ), &options::visuals::world::show_item_amount, 1 );

		g_framework.create_check_box( xs( L"prefab name debug" ), &options::visuals::prefab_name_debug, 1 );
		g_framework.create_check_box( xs( L"class name debug" ), &options::visuals::class_name_debug, 1 );

		// tab selection.
		g_framework.create_combo_box( tab_choice, &options::tab_selection, 1 );
		if( options::tab_selection == 0 ) {
			// first tab additionals.
			g_framework.create_combo_box( tab_list, &options::first_tab, 1 );

			if( options::first_tab == 1 ) {
				g_framework.create_label( xs( L"additional" ), 1, true );
				g_framework.create_check_box( xs( L"raid" ), &options::visuals::world::show_raid, 1 );
				g_framework.create_check_box( xs( L"show grenades" ), &options::visuals::world::show_grenades, 1 );
				g_framework.create_check_box( xs( L"player corpse" ), &options::visuals::world::show_corpse, 1 );
				g_framework.create_check_box( xs( L"backpack" ), &options::visuals::world::show_backpack, 1 );

				g_framework.create_label( xs( L"loot containers" ), 1, true );
				g_framework.create_check_box( xs( L"mine crate" ), &options::visuals::world::show_mine_crate, 1 );
				g_framework.create_check_box( xs( L"small loot crate" ), &options::visuals::world::show_small_crate, 1 );
				g_framework.create_check_box( xs( L"tool box" ), &options::visuals::world::show_tool_box, 1 );
				g_framework.create_check_box( xs( L"underwater crate" ), &options::visuals::world::show_underwater_crate, 1 );
				g_framework.create_check_box( xs( L"elite crate" ), &options::visuals::world::show_elite_crate, 1 );
				g_framework.create_check_box( xs( L"military crate" ), &options::visuals::world::show_military_crate, 1 );
				g_framework.create_check_box( xs( L"food crate" ), &options::visuals::world::show_food_crate, 1 );
				g_framework.create_check_box( xs( L"medical crate" ), &options::visuals::world::show_medical_crate, 1 );
				g_framework.create_check_box( xs( L"normal crate" ), &options::visuals::world::show_normal_crate, 1 );
				g_framework.create_check_box( xs( L"helicopter crate" ), &options::visuals::world::show_helicopter_crate, 1 );
				g_framework.create_check_box( xs( L"bradley crate" ), &options::visuals::world::show_bradley_crate, 1 );
				g_framework.create_check_box( xs( L"supply drop crate" ), &options::visuals::world::show_supply_drops, 1 );
				g_framework.create_check_box( xs( L"supply signals" ), &options::visuals::world::show_supply_signals, 1 );
				g_framework.create_check_box( xs( L"hackable locked crate" ), &options::visuals::world::show_hackable_locked_crate, 1 );
				g_framework.create_check_box( xs( L"stashes" ), &options::visuals::world::show_stashes, 1 );

				g_framework.create_label( xs( L"traps" ), 1, true );
				g_framework.create_check_box( xs( L"landmine" ), &options::visuals::world::show_landmines, 1 );
				g_framework.create_check_box( xs( L"npc turrets" ), &options::visuals::world::show_npc_turrets, 1 );
				g_framework.create_check_box( xs( L"auto-turrets" ), &options::visuals::world::show_auto_turrets, 1 );
				g_framework.create_check_box( xs( L"shotgun trap" ), &options::visuals::world::show_shotgun_traps, 1 );
				g_framework.create_check_box( xs( L"tesla coil" ), &options::visuals::world::show_tesla_coil, 1 );
				g_framework.create_check_box( xs( L"flame turret" ), &options::visuals::world::show_flame_turrets, 1 );
				g_framework.create_check_box( xs( L"sam-site" ), &options::visuals::world::show_sam_site, 1 );
				g_framework.create_check_box( xs( L"land spikes" ), &options::visuals::world::show_land_spikes, 1 );
				g_framework.create_check_box( xs( L"bear trap" ), &options::visuals::world::show_bear_trap, 1 );
			}
			else {
				g_framework.create_label( xs( L"items" ), 1, true );
				g_framework.create_check_box( xs( L"weapons" ), &options::visuals::world::show_weapons, 1 );
				g_framework.create_check_box( xs( L"melee weapons" ), &options::visuals::world::show_melee_weapons, 1 );
				g_framework.create_check_box( xs( L"heal" ), &options::visuals::world::show_heal, 1 );
				g_framework.create_check_box( xs( L"everything" ), &options::visuals::world::show_everything, 1 );
				g_framework.create_check_box( xs( L"drone" ), &options::visuals::world::show_drone, 1 );
				g_framework.create_check_box( xs( L"recycler" ), &options::visuals::world::show_recycler, 1 );
				g_framework.create_check_box( xs( L"show nails and arrows" ), &options::visuals::world::show_ammo_nails_arrows, 1 );

				g_framework.create_label( xs( L"ore collectibles" ), 1, true );
				g_framework.create_check_box( xs( L"sulfur collectible" ), &options::visuals::world::show_sulfur_collectibles, 1 );
				g_framework.create_check_box( xs( L"metal collectible" ), &options::visuals::world::show_metal_collectibles, 1 );
				g_framework.create_check_box( xs( L"wood collectible" ), &options::visuals::world::show_wood_collectibles, 1 );
				g_framework.create_check_box( xs( L"stone collectible" ), &options::visuals::world::show_stone_collectibles, 1 );

				g_framework.create_label( xs( L"ores" ), 1, true );
				g_framework.create_check_box( xs( L"sulfur ore" ), &options::visuals::world::show_sulfur_ore, 1 );
				g_framework.create_check_box( xs( L"metal ore" ), &options::visuals::world::show_metal_ore, 1 );
				g_framework.create_check_box( xs( L"stone ore" ), &options::visuals::world::show_stone_ore, 1 );

				g_framework.create_label( xs( L"barrels" ), 1, true );
				g_framework.create_check_box( xs( L"oil barrel" ), &options::visuals::world::show_oil_barrel, 1 );
				g_framework.create_check_box( xs( L"regular barrels" ), &options::visuals::world::show_regular_barrels, 1 );
				g_framework.create_check_box( xs( L"diesel barrels" ), &options::visuals::world::show_diesel_fuel, 1 );					

				g_framework.create_label( xs( L"storages" ), 1, true );
				g_framework.create_check_box( xs( L"tool cupboard" ), &options::visuals::world::show_tool_cupboard, 1 );
				g_framework.create_check_box( xs( L"box storages" ), &options::visuals::world::show_box_storages, 1 );
				g_framework.create_check_box( xs( L"vending machines" ), &options::visuals::world::show_vending_machine, 1 );
			}

			g_framework.create_label( xs( L"food" ), 2, true );
			g_framework.create_check_box( xs( L"hemp" ), &options::visuals::world::show_hemp, 2 );
			g_framework.create_check_box( xs( L"mushroom" ), &options::visuals::world::show_mushroom, 2 );
			g_framework.create_check_box( xs( L"pumpkin" ), &options::visuals::world::show_pumpkin, 2 );
			g_framework.create_check_box( xs( L"corn" ), &options::visuals::world::show_corns, 2 );
			g_framework.create_check_box( xs( L"berry" ), &options::visuals::world::show_berrys, 2 );
			g_framework.create_check_box( xs( L"potato" ), &options::visuals::world::show_potatos, 2 );

			g_framework.create_label( xs( L"animals" ), 2, true );
			g_framework.create_check_box( xs( L"chicken" ), &options::visuals::world::show_chickens, 2 );
			g_framework.create_check_box( xs( L"boars" ), &options::visuals::world::show_boars, 2 );
			g_framework.create_check_box( xs( L"wolves" ), &options::visuals::world::show_wolves, 2 );
			g_framework.create_check_box( xs( L"deer" ), &options::visuals::world::show_deers, 2 );
			g_framework.create_check_box( xs( L"polar bear" ), &options::visuals::world::show_polar_bears, 2 );
			g_framework.create_check_box( xs( L"bear" ), &options::visuals::world::show_bears, 2 );
			g_framework.create_check_box( xs( L"sharks" ), &options::visuals::world::show_sharks, 2 );
			g_framework.create_check_box( xs( L"horses" ), &options::visuals::world::show_horses, 2 );

			g_framework.create_label( xs( L"vehicles" ), 2, true );
			g_framework.create_check_box( xs( L"mini-copter" ), &options::visuals::world::show_minicopter, 2 );
			g_framework.create_check_box( xs( L"scrap helicopter" ), &options::visuals::world::show_scrap_helicopter, 2 );
			g_framework.create_check_box( xs( L"rhib" ), &options::visuals::world::show_rhib, 2 );
			g_framework.create_check_box( xs( L"kayak" ), &options::visuals::world::show_kayak, 2 );
			g_framework.create_check_box( xs( L"small motorboat" ), &options::visuals::world::show_small_motorboat, 2 );
			g_framework.create_check_box( xs( L"solo submarine" ), &options::visuals::world::show_solo_submarine, 2 );
			g_framework.create_check_box( xs( L"duo submarine" ), &options::visuals::world::show_duo_submarine, 2 );
			g_framework.create_check_box( xs( L"bradley tank" ), &options::visuals::world::show_bradley_apc, 2 );

			g_framework.create_label( xs( L"helicopters" ), 2, true );
			g_framework.create_check_box( xs( L"attack helicopter" ), &options::visuals::world::show_attack_helicopter, 2 );
			g_framework.create_check_box( xs( L"chinook helicopter" ), &options::visuals::world::show_chinook_helicopter, 2 );
			g_framework.create_check_box( xs( L"patrol helicopter" ), &options::visuals::world::show_patrol_helicopter, 2 );

			g_framework.create_label( xs( L"cars" ), 2, true );
			g_framework.create_check_box( xs( L"modular car" ), &options::visuals::world::show_modular_car, 2 );
			g_framework.create_check_box( xs( L"2 module car" ), &options::visuals::world::show_two_modules_car, 2 );
		}
		else if( options::tab_selection == 1 ) {
			// first tab additionals.
			g_framework.create_combo_box( tab_list, &options::first_tab, 1 );

			if( options::first_tab == 1 ) {
				g_framework.color_picker( xs( L"player corpse" ), &options::visuals::world::color_corpse, 1 );
				g_framework.color_picker( xs( L"backpack" ), &options::visuals::world::color_backpack, 1 );
				
				g_framework.create_label( xs( L"loot containers" ), 1, true );

				g_framework.color_picker( xs( L"mine crate" ), &options::visuals::world::color_mine_crate, 1 );
				g_framework.color_picker( xs( L"small loot crate" ), &options::visuals::world::color_small_crate, 1 );
				g_framework.color_picker( xs( L"tool box" ), &options::visuals::world::color_tool_box, 1 );
				g_framework.color_picker( xs( L"underwater crate" ), &options::visuals::world::color_underwater_crate, 1 );
				g_framework.color_picker( xs( L"elite crate" ), &options::visuals::world::color_elite_crate, 1 );
				g_framework.color_picker( xs( L"military crate" ), &options::visuals::world::color_military_crate, 1 );
				g_framework.color_picker( xs( L"food crate" ), &options::visuals::world::color_food_crate, 1 );
				g_framework.color_picker( xs( L"medical crate" ), &options::visuals::world::color_medical_crate, 1 );
				g_framework.color_picker( xs( L"normal crate" ), &options::visuals::world::color_normal_crate, 1 );
				g_framework.color_picker( xs( L"helicopter crate" ), &options::visuals::world::color_helicopter_crate, 1 );
				g_framework.color_picker( xs( L"bradley crate" ), &options::visuals::world::color_bradley_crate, 1 );
				g_framework.color_picker( xs( L"supply drop crate" ), &options::visuals::world::color_supply_drops, 1 );
				g_framework.color_picker( xs( L"supply signals" ), &options::visuals::world::color_supply_signals, 1 );
				g_framework.color_picker( xs( L"hackable locked crate" ), &options::visuals::world::color_hackable_locked_crate, 1 );
				g_framework.color_picker( xs( L"stashes" ), &options::visuals::world::color_stashes, 1 );

				g_framework.create_label( xs( L"traps" ), 1, true );
				g_framework.color_picker( xs( L"landmine" ), &options::visuals::world::color_landmines, 1 );
				g_framework.color_picker( xs( L"npc turrets" ), &options::visuals::world::color_npc_turrets, 1 );
				g_framework.color_picker( xs( L"auto-turrets" ), &options::visuals::world::color_auto_turrets, 1 );
				g_framework.color_picker( xs( L"shotgun trap" ), &options::visuals::world::color_shotgun_traps, 1 );
				g_framework.color_picker( xs( L"tesla coil" ), &options::visuals::world::color_tesla_coil, 1 );
				g_framework.color_picker( xs( L"flame turret" ), &options::visuals::world::color_flame_turrets, 1 );
				g_framework.color_picker( xs( L"sam-site" ), &options::visuals::world::color_sam_site, 1 );
				g_framework.color_picker( xs( L"land spikes" ), &options::visuals::world::color_land_spikes, 1 );
				g_framework.color_picker( xs( L"bear trap" ), &options::visuals::world::color_bear_trap, 1 );
			}
			else {
				g_framework.create_label( xs( L"items" ), 1, true );
				g_framework.color_picker( xs( L"weapons" ), &options::visuals::world::color_weapons, 1 );
				g_framework.color_picker( xs( L"melee weapons" ), &options::visuals::world::color_melee_weapons, 1 );
				g_framework.color_picker( xs( L"heal" ), &options::visuals::world::color_heal, 1 );
				g_framework.color_picker( xs( L"everything" ), &options::visuals::world::color_everything, 1 );
				g_framework.color_picker( xs( L"drone" ), &options::visuals::world::color_drone, 1 );
				g_framework.color_picker( xs( L"recycler" ), &options::visuals::world::color_recycler, 1 );

				g_framework.create_label( xs( L"ore collectibles" ), 1, true );
				g_framework.color_picker( xs( L"sulfur collectible" ), &options::visuals::world::color_sulfur_collectibles, 1 );
				g_framework.color_picker( xs( L"metal collectible" ), &options::visuals::world::color_metal_collectibles, 1 );
				g_framework.color_picker( xs( L"wood collectible" ), &options::visuals::world::color_wood_collectibles, 1 );
				g_framework.color_picker( xs( L"stone collectible" ), &options::visuals::world::color_stone_collectibles, 1 );

				g_framework.create_label( xs( L"ores" ), 1, true );
				g_framework.color_picker( xs( L"sulfur ore" ), &options::visuals::world::color_sulfur_ore, 1 );
				g_framework.color_picker( xs( L"metal ore" ), &options::visuals::world::color_metal_ore, 1 );
				g_framework.color_picker( xs( L"stone ore" ), &options::visuals::world::color_stone_ore, 1 );

				g_framework.create_label( xs( L"barrels" ), 1, true );
				g_framework.color_picker( xs( L"oil barrel" ), &options::visuals::world::color_oil_barrel, 1 );
				g_framework.color_picker( xs( L"regular barrels" ), &options::visuals::world::color_regular_barrels, 1 );
				g_framework.color_picker( xs( L"diesel barrels" ), &options::visuals::world::color_diesel_fuel, 1 );					

				g_framework.create_label( xs( L"storages" ), 1, true );
				g_framework.color_picker( xs( L"tool cupboard" ), &options::visuals::world::color_tool_cupboard, 1 );
				g_framework.color_picker( xs( L"box storages" ), &options::visuals::world::color_box_storages, 1 );
				g_framework.color_picker( xs( L"vending machines" ), &options::visuals::world::color_vending_machine, 1 );
			}

			g_framework.create_label( xs( L"food" ), 2, true );
			g_framework.color_picker( xs( L"hemp" ), &options::visuals::world::color_hemp, 2 );
			g_framework.color_picker( xs( L"mushroom" ), &options::visuals::world::color_mushroom, 2 );
			g_framework.color_picker( xs( L"pumpkin" ), &options::visuals::world::color_pumpkin, 2 );
			g_framework.color_picker( xs( L"corn" ), &options::visuals::world::color_corns, 2 );
			g_framework.color_picker( xs( L"berry" ), &options::visuals::world::color_berrys, 2 );
			g_framework.color_picker( xs( L"potato" ), &options::visuals::world::color_potatos, 2 );

			g_framework.create_label( xs( L"animals" ), 2, true );
			g_framework.color_picker( xs( L"chicken" ), &options::visuals::world::color_chickens, 2 );
			g_framework.color_picker( xs( L"boars" ), &options::visuals::world::color_boars, 2 );
			g_framework.color_picker( xs( L"wolves" ), &options::visuals::world::color_wolves, 2 );
			g_framework.color_picker( xs( L"deer" ), &options::visuals::world::color_deers, 2 );
			g_framework.color_picker( xs( L"polar bear" ), &options::visuals::world::color_polar_bears, 2 );
			g_framework.color_picker( xs( L"bear" ), &options::visuals::world::color_bears, 2 );
			g_framework.color_picker( xs( L"sharks" ), &options::visuals::world::color_sharks, 2 );
			g_framework.color_picker( xs( L"horses" ), &options::visuals::world::color_horses, 2 );

			g_framework.create_label( xs( L"vehicles" ), 2, true );
			g_framework.color_picker( xs( L"mini-copter" ), &options::visuals::world::color_minicopter, 2 );
			g_framework.color_picker( xs( L"scrap helicopter" ), &options::visuals::world::color_scrap_helicopter, 2 );
			g_framework.color_picker( xs( L"rhib" ), &options::visuals::world::color_rhib, 2 );
			g_framework.color_picker( xs( L"kayak" ), &options::visuals::world::color_kayak, 2 );
			g_framework.color_picker( xs( L"small motorboat" ), &options::visuals::world::color_small_motorboat, 2 );
			g_framework.color_picker( xs( L"solo submarine" ), &options::visuals::world::color_solo_submarine, 2 );
			g_framework.color_picker( xs( L"duo submarine" ), &options::visuals::world::color_duo_submarine, 2 );
			g_framework.color_picker( xs( L"bradley tank" ), &options::visuals::world::color_bradley_apc, 2 );

			g_framework.create_label( xs( L"helicopters" ), 2, true );
			g_framework.color_picker( xs( L"attack helicopter" ), &options::visuals::world::color_attack_helicopter, 2 );
			g_framework.color_picker( xs( L"chinook helicopter" ), &options::visuals::world::color_chinook_helicopter, 2 );
			g_framework.color_picker( xs( L"patrol helicopter" ), &options::visuals::world::color_patrol_helicopter, 2 );

			g_framework.create_label( xs( L"cars" ), 2, true );
			g_framework.color_picker( xs( L"modular car" ), &options::visuals::world::color_modular_car, 2 );
			g_framework.color_picker( xs( L"2 module car" ), &options::visuals::world::color_two_modules_car, 2 );
		}
		else {
			g_framework.slider_int( xs( L"helicopter max distance" ), &options::visuals::world::max_helicopter_distance, 0, max_distance, 1 );
			g_framework.slider_int( xs( L"ores max distance" ), &options::visuals::world::max_ores_distance, 0, max_distance, 1 );
			g_framework.slider_int( xs( L"corpse max distance" ), &options::visuals::world::max_corpse_distance, 0, max_distance, 1 );
			g_framework.slider_int( xs( L"ore collectibles max distance" ), &options::visuals::world::max_ore_collectibles_distance, 0, max_distance, 1 );
			g_framework.slider_int( xs( L"loot containers max distance" ), &options::visuals::world::max_container_distance, 0, max_distance, 1 );
			g_framework.slider_int( xs( L"food collectibles max distance" ), &options::visuals::world::max_food_collectibles_distance, 0, max_distance, 1 );
			g_framework.slider_int( xs( L"respawn points max distance" ), &options::visuals::world::max_respawn_points_distance, 0, max_distance, 1 );
			g_framework.slider_int( xs( L"barrels max distance" ), &options::visuals::world::max_barrels_distance, 0, max_distance, 1 );
			g_framework.slider_int( xs( L"cars max distance" ), &options::visuals::world::max_cars_distance, 0, max_distance, 1 );

			g_framework.slider_int( xs( L"items max distance" ), &options::visuals::world::max_item_distance, 0, max_distance, 2 );
			g_framework.slider_int( xs( L"grenades max distance" ), &options::visuals::world::max_grenade_distance, 0, max_distance, 2 );
			g_framework.slider_int( xs( L"animal max distance" ), &options::visuals::world::max_animal_distance, 0, max_distance, 2 );
			g_framework.slider_int( xs( L"traps max distance" ), &options::visuals::world::max_trap_distance, 0, max_distance, 2 );
			g_framework.slider_int( xs( L"stashes max distance" ), &options::visuals::world::max_stash_distance, 0, max_distance, 2 );
			g_framework.slider_int( xs( L"storages max distance" ), &options::visuals::world::max_storage_distance, 0, max_distance, 2 );
			g_framework.slider_int( xs( L"vehicle max distance" ), &options::visuals::world::max_vehicle_distance, 0, max_distance, 2 );
			g_framework.slider_int( xs( L"raid max distance" ), &options::visuals::world::max_raid_distance, 0, max_distance, 2 );
		}
	}
}
	
void c_menu::draw_combat_tab( ) {
	g_framework.create_check_box( xs( L"aim during cycle" ), &options::aimbot::exploits::combat::aim_bolt_cycle, 1 );
	g_framework.create_check_box( xs( L"can aim on jugger clothes" ), &options::aimbot::exploits::combat::unlock_aim_on_jugger, 1 );
	g_framework.create_check_box( xs( L"burst weapons" ), &options::aimbot::exploits::combat::burst_weapons, 1 );
	g_framework.create_check_box( xs( L"automatic weapons" ), &options::aimbot::exploits::combat::automatic_weapons, 1 );
	g_framework.create_check_box( xs( L"semi-automatic weapons" ), &options::aimbot::exploits::combat::semi_automatic_weapons, 1 );
	//g_framework.create_check_box( xs( L"reduce shotgun spread" ), &options::aimbot::exploits::combat::no_shot_gun_spread, 1 );
	g_framework.create_check_box( xs( L"reduce spread" ), &options::aimbot::exploits::combat::no_spread, 1 );
	g_framework.slider_float( xs( L"spread amount" ), &options::aimbot::exploits::combat::reduce_spread, 0.0f, 100.f, 1 );
	g_framework.create_check_box( xs( L"remove sway" ), &options::aimbot::exploits::combat::no_sway, 1 );
	g_framework.create_check_box( xs( L"modify eoka chance" ), &options::aimbot::exploits::combat::modify_eoka_chance, 1 );
	g_framework.slider_float( xs( L"eoka chance" ), &options::aimbot::exploits::combat::eoka_chance, 0.0f, 100.f, 1 );
	g_framework.create_check_box( xs( L"reduce recoil" ), &options::aimbot::exploits::combat::no_recoil, 1 );
	g_framework.slider_float( xs( L"recoil amount" ), &options::aimbot::exploits::combat::reduce_recoil, 0.0f, 100.f, 1 );
	g_framework.create_check_box( xs( L"extended melee" ), &options::aimbot::exploits::combat::extended_melee, 1 );
	g_framework.slider_float( xs( L"attack radius" ), &options::aimbot::exploits::combat::attack_radius, 0.0f, 2.5f, 1 );
	g_framework.create_check_box( xs( L"big bullets" ), &options::aimbot::exploits::combat::big_bullets, 1 );
	g_framework.slider_float( xs( L"bullet size" ), &options::aimbot::exploits::combat::bullet_size, 0.0f, 100.f, 1 );
	g_framework.create_check_box( xs( L"quick bullets" ), &options::aimbot::exploits::combat::quick_bullets, 1 );
	g_framework.slider_int( xs( L"bullet speed" ), &options::aimbot::exploits::combat::bullet_speed, 50, 145, 1 );
	g_framework.create_check_box( xs( L"spoof hit distance" ), &options::aimbot::exploits::combat::spoof_hit_distance, 1 );
	g_framework.slider_float( xs( L"bullet distance" ), &options::aimbot::exploits::combat::bullet_distance, 1.0f, 1000.f, 1 );
	//g_framework.create_check_box( xs( L"pierce" ), &options::aimbot::exploits::combat::pierce, 1 );
	g_framework.create_check_box( xs( L"attack in air" ), &options::aimbot::exploits::combat::modify_can_attack, 1 );
	g_framework.create_check_box( xs( L"attack in vehicles" ), &options::aimbot::exploits::combat::can_attack_in_vehicles, 1 );
	g_framework.create_check_box( xs( L"unlock viewangles" ), &options::aimbot::exploits::combat::unlock_view_angles, 1 );
	g_framework.create_check_box( xs( L"fake fire" ), &options::aimbot::exploits::misc::fake_fire, 1 );
	g_framework.create_check_box( xs( L"fake fire always on" ), &options::aimbot::exploits::misc::fake_fire_on, 1 );	
	g_framework.slider_float( xs( L"fire delay" ), &options::aimbot::exploits::misc::delay_fake_fire, 0.0f, 1000.f, 1 );		

	g_framework.create_check_box( xs( L"teleport to player head" ), &options::aimbot::exploits::combat::head_teleportation, 2 );

	if( options::aimbot::exploits::combat::head_teleportation )
		g_framework.create_hotkey( xs( L"teleport key" ), &options::aimbot::exploits::combat::teleport_key, 2 );

	if( !options::aimbot::exploits::misc::fake_fire_on )
		g_framework.create_hotkey( xs( L"fake fire key" ), &options::aimbot::exploits::misc::fake_fire_key, 2 );
}

void c_menu::draw_time_tab( ) {
	g_framework.create_check_box( xs( L"rapid-fire" ), &options::aimbot::exploits::time::rapid_fire, 1 );
	g_framework.slider_float( xs( L"fire speed" ), &options::aimbot::exploits::time::rapid_fire_speed, 0.0f, 20.f, 1 );
	g_framework.create_check_box( xs( L"instant revive" ), &options::aimbot::exploits::time::instant_revive, 1 );
	g_framework.create_check_box( xs( L"always revive target" ), &options::aimbot::exploits::time::always_revive_target, 1 );
	g_framework.create_check_box( xs( L"instant charge compound" ), &options::aimbot::exploits::time::instant_charge_compound, 1 );

	g_framework.create_label( xs( L"additional" ), 2, true );
	g_framework.create_check_box( xs( L"fake-lag" ), &options::aimbot::exploits::time::fake_lag, 2 );
	g_framework.create_check_box( xs( L"fake-lag always on" ), &options::aimbot::exploits::time::fake_lag_on, 2 );
	g_framework.slider_float( xs( L"fake-lag amount" ), &options::aimbot::exploits::time::fake_lag_amount, 0.0f, 1.f, 2 );
	if( !options::aimbot::exploits::time::fake_lag_on )
		g_framework.create_hotkey( xs( L"fake-lag key" ), &options::aimbot::exploits::time::fake_lag_key, 2 );

	g_framework.create_check_box( xs( L"fast heal" ), &options::aimbot::exploits::time::fast_heal, 2 );
	g_framework.create_check_box( xs( L"fast switch" ), &options::aimbot::exploits::time::fast_switch, 2 );
	g_framework.create_check_box( xs( L"fast loot" ), &options::aimbot::exploits::time::fast_loot, 2 );

	g_framework.create_check_box( xs( L"fast bow" ), &options::aimbot::exploits::time::fast_bow, 2 );

	if( !options::aimbot::exploits::time::always_revive_target )
		g_framework.create_hotkey( xs( L"revive key" ), &options::aimbot::exploits::time::revive_key, 1 );
}

void c_menu::draw_movement_tab( ) {
	g_framework.create_check_box( xs( L"walk through trees" ), &options::aimbot::exploits::movement::walk_through_trees, 1 );
	g_framework.create_check_box( xs( L"walk through ai" ), &options::aimbot::exploits::movement::walk_through_players, 1 );

	g_framework.create_check_box( xs( L"modify clothing speed" ), &options::aimbot::exploits::movement::modify_clothing_speed, 1 );
	g_framework.slider_float( xs( L"clothing speed" ), &options::aimbot::exploits::movement::clothing_speed, 0.f, 2.5f, 1 );			

	g_framework.create_check_box( xs( L"always sprint" ), &options::aimbot::exploits::movement::always_sprint, 1 );
				
	g_framework.create_check_box( xs( L"speed hack" ), &options::aimbot::exploits::movement::omni_sprint, 1 );
	g_framework.slider_float( xs( L"[s] speed" ), &options::aimbot::exploits::movement::omni_sprint_speed, 100.0f, 300.f, 1 );
	g_framework.create_check_box( xs( L"infinite jump" ), &options::aimbot::exploits::movement::infinite_jump, 1 );
	g_framework.create_check_box( xs( L"silent walk(shift)" ), &options::aimbot::exploits::movement::silent_walk, 1 );
	g_framework.create_check_box( xs( L"climb assistance" ), &options::aimbot::exploits::movement::climb_assist, 1 );
	g_framework.create_check_box( xs( L"no slow down on melee" ), &options::aimbot::exploits::movement::disable_slow_down_melee, 1 );
	g_framework.create_check_box( xs( L"high jump" ), &options::aimbot::exploits::movement::high_jump, 1 );
	g_framework.slider_float( xs( L"jump height" ), &options::aimbot::exploits::movement::jump_height, 100.0f, 250.f, 1 );
	g_framework.create_check_box( xs( L"fly-hack" ), &options::aimbot::exploits::movement::fly_hack, 1 );
	g_framework.slider_int( xs( L"fly speed" ), &options::aimbot::exploits::movement::fly_speed, 0.0f, 1000.f, 1 );
	g_framework.create_check_box( xs( L"anti fly violation" ), &options::aimbot::exploits::movement::stopper_fly, 1 );

	g_framework.create_hotkey( xs( L"fly key" ), &options::aimbot::exploits::movement::fly_key, 1 );

	g_framework.create_hotkey( xs( L"ignore stopper key" ), &options::aimbot::exploits::movement::ignore_key, 1 );

	g_framework.create_check_box( xs( L"increase height" ), &options::aimbot::exploits::movement::increase_height, 2 );
	g_framework.slider_float( xs( L"height amount" ), &options::aimbot::exploits::movement::height_amount, 1.8f, 7.5f, 2 );
	g_framework.create_hotkey( xs( L"height key" ), &options::aimbot::exploits::movement::increase_height_key, 2 );

	if( options::aimbot::exploits::movement::omni_sprint )
		g_framework.create_hotkey( xs( L"speed-hack bind" ), &options::aimbot::exploits::movement::omni_sprint_key, 2 );
}

void c_menu::draw_misc_tab( ) {
	g_framework.create_check_box( xs( L"enable friend system" ), &options::friend_system, 1 );
	g_framework.create_hotkey( xs( L"add friend key" ), &options::add_friend_key, 1 );

	g_framework.create_label( xs( L"disablers" ), 1 );
	g_framework.create_check_box( xs( L"disarm landmines" ), &options::aimbot::exploits::misc::disarm_landmines, 1 );
	g_framework.create_check_box( xs( L"disable recycler" ), &options::aimbot::exploits::misc::stop_recycler, 1 );

	g_framework.create_check_box( xs( L"interactive debug" ), &options::aimbot::exploits::misc::interactive_debug, 2 );
	g_framework.create_check_box( xs( L"gesture spam" ), &options::aimbot::exploits::misc::gesture_spam, 2 );
	g_framework.create_combo_box( default_gestures, &options::aimbot::exploits::misc::gesture_type, 2 );

	g_framework.create_check_box( xs( L"debug camera" ), &options::aimbot::exploits::misc::debug_camera, 1 );
	g_framework.create_hotkey( xs( L"debug camera key" ), &options::aimbot::exploits::misc::debug_camera_key, 1 );

	g_framework.create_check_box( xs( L"disable land damage" ), &options::aimbot::exploits::misc::disable_land_damage, 1 );
	g_framework.create_check_box( xs( L"always hit weak spot" ), &options::aimbot::exploits::misc::always_hit_weak_spot, 1 );

	g_framework.create_check_box( xs( L"change fov" ), &options::aimbot::exploits::misc::fov_changer, 1 );
	g_framework.slider_int( xs( L"fov amount" ), &options::aimbot::exploits::misc::fov_amount, 0.0f, 150, 1 );
	g_framework.create_check_box( xs( L"zoom" ), &options::aimbot::exploits::misc::zoom_fov, 1 );
	g_framework.slider_int( xs( L"zoom fov amount" ), &options::aimbot::exploits::misc::zoom_fov_amount, 0.0f, 40.f, 1 );
		
	g_framework.create_check_box( xs( L"keep wounded alive" ), &options::aimbot::exploits::misc::keep_wounded_alive, 1 );
				
	g_framework.create_check_box( xs( L"use max view" ), &options::aimbot::exploits::misc::use_max_view, 1 );

	g_framework.create_check_box( xs( L"view offset height" ), &options::aimbot::exploits::misc::long_neck, 1 );
	g_framework.slider_float( xs( L"height" ), &options::aimbot::exploits::misc::long_neck_height, 0.f, 1.50f, 1 );
	g_framework.create_check_box( xs( L"view offset right" ), &options::aimbot::exploits::misc::long_neck_right, 1 );
	g_framework.slider_float( xs( L"right" ), &options::aimbot::exploits::misc::long_neck_right_amount, 0.f, 1.50f, 1 );
	g_framework.create_check_box( xs( L"view offset left" ), &options::aimbot::exploits::misc::long_neck_left, 1 );
	g_framework.slider_float( xs( L"left" ), &options::aimbot::exploits::misc::long_neck_left_amount, 0.f, 1.50f, 1 );

	g_framework.create_check_box( xs( L"spin-bot" ), &options::aimbot::exploits::misc::spin_bot, 1 );

	g_framework.create_check_box( xs( L"suicide" ), &options::aimbot::exploits::misc::suicide, 1 );

	g_framework.create_check_box( xs( L"fake admin" ), &options::aimbot::exploits::misc::fake_admin, 1 );			

	g_framework.create_label( xs( L"view" ), 2 );
	g_framework.create_check_box( xs( L"no view lower" ), &options::aimbot::exploits::misc::no_lower, 2 );
	g_framework.create_check_box( xs( L"no view bob" ), &options::aimbot::exploits::misc::no_viewmodel_bob, 2 );
	g_framework.create_check_box( xs( L"no flash" ), &options::aimbot::exploits::misc::no_flash, 2 );

	g_framework.create_check_box( xs( L"remove water blur" ), &options::visuals::world::clear_underwater, 2 );

	g_framework.create_check_box( xs( L"watermark" ), &options::visuals::show_watermark, 2 );				
				
	g_framework.create_label( xs( L"bind related" ), 2 );
	g_framework.create_hotkey( xs( L"height key" ), &options::aimbot::exploits::misc::view_height, 2 );
	g_framework.create_hotkey( xs( L"right key" ), &options::aimbot::exploits::misc::view_right, 2 );
	g_framework.create_hotkey( xs( L"left key" ), &options::aimbot::exploits::misc::view_left, 2 );
	g_framework.create_hotkey( xs( L"zoom key" ), &options::aimbot::exploits::misc::zoom_key, 2 );

	g_framework.create_hotkey( xs( L"suicide key" ), &options::aimbot::exploits::misc::suicide_key, 2 );
}

void c_menu::draw_modulations_tab( ) {
	g_framework.create_check_box( xs( L"modify rain" ), &options::visuals::world::modify_rain, 1 );
	g_framework.slider_float( xs( L"rain amount" ), &options::visuals::world::rain_amount, 0.f, 5.f, 1 );

	//g_framework.create_check_box( xs( L"night stars" ), &options::visuals::world::night_stars, 1 );
	g_framework.create_check_box( xs( L"change aspect ratio" ), &options::visuals::world::aspect_changer, 1 );
	g_framework.slider_float( xs( L"aspect ratio" ), &options::visuals::world::aspect_value, 0.1f, 1.92f, 1 );
				
	g_framework.create_check_box( xs( L"change time duration" ), &options::aimbot::exploits::time::time_modifier, 1 );
	g_framework.slider_float( xs( L"time" ), &options::aimbot::exploits::time::time_speed, 0.0f, 24.f, 1 );

	g_framework.create_check_box( xs( L"mie changer" ), &options::visuals::world::mie_changer, 1 );
	if( options::visuals::world::mie_changer )
		g_framework.slider_float( xs( L"mie amount" ), &options::visuals::world::mie_amount, 1.0f, 100.f, 1 );	

	g_framework.create_check_box( xs( L"rayleigh changer" ), &options::visuals::world::rayleigh_changer, 1 );
	if( options::visuals::world::rayleigh_changer )
		g_framework.slider_float( xs( L"rayleigh amount" ), &options::visuals::world::rayleigh_amount, 1.0f, 100.f, 1 );

	g_framework.create_check_box( xs( L"stars brightness" ), &options::visuals::world::stars, 1 );
	if( options::visuals::world::stars )
		g_framework.slider_float( xs( L"stars amount" ), &options::visuals::world::brightness_stars_amount, 1.0f, 5000.f, 1 );
				
	g_framework.create_check_box( xs( L"stars size" ), &options::visuals::world::stars_size, 1 );
	if( options::visuals::world::stars_size )
		g_framework.slider_float( xs( L"stars amount size" ), &options::visuals::world::size_stars_amount, 1.0f, 50.f, 1 );

	g_framework.create_check_box( xs( L"change ambient" ), &options::visuals::world::ambient, 1 );
	if( options::visuals::world::ambient ) { 
		g_framework.slider_float( xs( L"ambient value" ), &options::visuals::world::ambient_value, 1.0f, 15.f, 1 );
		g_framework.slider_float( xs( L"light value" ), &options::visuals::world::light_value, 1.0f, 15.f, 1 );
	}

	g_framework.create_check_box( xs( L"change world color" ), &options::visuals::world::change_world_color, 1 );
	g_framework.color_picker( xs( L"sky color" ), &options::visuals::world::sky_color, 1 );
	g_framework.color_picker( xs( L"ambient color" ), &options::visuals::world::ambient_color, 1 );
}

void c_menu::draw_automations_tab( ) {
	g_framework.create_check_box( xs( L"auto pickup items" ), &options::aimbot::exploits::misc::auto_pickup_items, 1 );
	if( options::aimbot::exploits::misc::auto_pickup_items )
		g_framework.create_check_box( xs( L"pickup everything" ), &options::aimbot::exploits::misc::pickup_everything, 1 );
	g_framework.create_hotkey( xs( L"pickup key" ), &options::aimbot::exploits::misc::pickup_key, 1 );
	g_framework.slider_float( xs( L"item distance" ), &options::aimbot::exploits::misc::max_item_distance, 0.f, 15.f, 1 );
	g_framework.create_check_box( xs( L"always pickup items" ), &options::aimbot::exploits::misc::always_pickup, 1 );

	g_framework.create_check_box( xs( L"auto pickup collectibles" ), &options::aimbot::exploits::misc::auto_pickup, 1 );
	g_framework.slider_float( xs( L"collectible distance" ), &options::aimbot::exploits::misc::max_collectible_distance, 0.f, 15.f, 1 );

	g_framework.create_check_box( xs( L"always set code" ), &options::aimbot::exploits::misc::always_codelock, 1 );
	g_framework.create_check_box( xs( L"auto code lock" ), &options::aimbot::exploits::misc::auto_codelock, 1 );
	g_framework.slider_int( xs( L"code" ), &options::aimbot::exploits::misc::code_lock_code, 1000, 9999, 1 );
	g_framework.slider_float( xs( L"lock distance" ), &options::aimbot::exploits::misc::max_lock_distance, 0.f, 15.f, 1 );
	g_framework.create_hotkey( xs( L"code key" ), &options::aimbot::exploits::misc::codelock_key, 1 );

	g_framework.create_check_box( xs( L"always grade" ), &options::aimbot::exploits::misc::always_grade, 1 );
	g_framework.create_check_box( xs( L"auto grade" ), &options::aimbot::exploits::misc::auto_grade, 1 );
	g_framework.slider_float( xs( L"grade distance" ), &options::aimbot::exploits::misc::max_grade_distance, 0.f, 100.f, 1 );
	g_framework.create_combo_box( grade_tier_list, &options::aimbot::exploits::misc::grade_tier, 1 );
	g_framework.create_hotkey( xs( L"grade key" ), &options::aimbot::exploits::misc::grade_key, 1 );

	g_framework.create_check_box( xs( L"auto untie crates" ), &options::aimbot::exploits::misc::auto_untie_crates, 1 );

	g_framework.create_label( xs( L"farm" ), 2 );
	g_framework.create_check_box( xs( L"auto farm barrels" ), &options::aimbot::exploits::misc::auto_farm_barrels, 2 );
	g_framework.create_check_box( xs( L"auto farm ores" ), &options::aimbot::exploits::misc::auto_farm_ores, 2 );
	g_framework.create_check_box( xs( L"auto farm trees" ), &options::aimbot::exploits::misc::auto_farm_trees, 2 );
	g_framework.create_check_box( xs( L"farm hotspot" ), &options::aimbot::exploits::misc::auto_farm_only_hotspot, 2 );
}

/* draw menu */
void c_menu::draw_menu( ) {
	if( g_framework.get_ins( ) )
		g_framework.open = !g_framework.open;

	if( g_framework.open
		&& g_framework.alpha < 255.f )
		g_framework.alpha = std::lerp( g_framework.alpha, 255.f, 0.35f );
	else if( !g_framework.open
		&& g_framework.alpha > 0.f )
		g_framework.alpha = std::lerp( g_framework.alpha, 0.f, 0.35f );

	g_framework.in_alpha = g_framework.alpha > 5.f;
	options::accent_color.a = g_framework.alpha;

	if( !g_framework.in_alpha ) {
		g_framework.pressed_key = false;
		g_framework.active_hotkey = g_framework.active_picker = -1;
		return;
	}

	g_framework.begin( );
	g_framework.create_tab( xs( L"aimbot" ), 0, 1 );
	g_framework.create_tab( xs( L"players" ), 1, 2 );
	g_framework.create_tab( xs( L"world" ), 2, 3 );
	g_framework.create_tab( xs( L"weapon mods" ), 3, 4 );
	g_framework.create_tab( xs( L"time" ), 4, 5 );
	g_framework.create_tab( xs( L"movement" ), 5, 6 );
	g_framework.create_tab( xs( L"misc" ), 6, 7 );
	g_framework.create_tab( xs( L"modulations" ), 7, 8 );
	g_framework.create_tab( xs( L"automations" ), 8, 9 );
	g_framework.create_tab( xs( L"main" ), 9, 10 );

	// vis check in aim
	// omni-sprint

	switch( g_framework.selected_tab ) {
	case 0:
		draw_aimbot_tab( );
	break;
	case 1:
		draw_visuals_tab( );
	break;
	case 2:
		draw_worlds_tab( );
	break;
	case 3:
		draw_combat_tab( );
	break;
	case 4:
		draw_time_tab( );
	break;
	case 5:
		draw_movement_tab( );
	break;
	case 6:
		draw_misc_tab( );
	break;
	case 7:
		draw_modulations_tab( );
	break;
	case 8:
		draw_automations_tab( );
	break;
	case 9:
		draw_menu_tab( );
	break;
	}

	g_framework.end( );
}
