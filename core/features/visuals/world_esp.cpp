#include "world_esp.hpp"
#include "esp.hpp"
#include "../options.hpp"
#include "../../utilities/memory.hpp"
#include "../../game/offsets.hpp"

c_world_esp g_world;

void c_world_esp::render( vec4_t render_position, std::wstring class_name,
	std::wstring obj_name, int distance, base_combat_entity* combat_entity,
	event_type event, base_entity* base_entity )
{
	if( !options::visuals::world::master_switch 
		|| event != event_type::re_paint
		|| distance >= 5000 )
		return;

	std::wstring dist_str = std::to_wstring( static_cast< int >( distance ) );
	std::wstring text = std::wstring( );
	color_t color = color_t( 0, 0, 0, 0 );

	float closest_distance = FLT_MAX;
		
	base_player* local = g_cheat.local;
	if( !local )
		return;

	static float last_sent_tick = local->lastSentTickTime( );

	//g_render.draw_text( render_position, obj_name, color_t( 255, 255, 255 ) );
	if( options::visuals::prefab_name_debug ) {
		render_position.y += 15;
		g_render.draw_text( render_position, obj_name.c_str( ), color_t( 255, 255, 255 ), false, 10, g_esp.font_flags );
		render_position.y -= 15;
	}

	if( options::visuals::class_name_debug ) {
		render_position.y += options::visuals::prefab_name_debug ? 30 : 15;

		g_render.draw_text( vec4_t( render_position.x, render_position.y + 40, render_position.z, render_position.w ), 
			class_name.c_str( ), color_t( 255 ,255, 255 ), false, 10, g_esp.font_flags );

		render_position.y -= options::visuals::prefab_name_debug ? 30 : 15;
	}

	if( distance < options::visuals::world::max_grenade_distance
		&& options::visuals::world::show_grenades ) {
		if( wcsstr( obj_name.c_str( ), xs( L"grenade.f1.deployed.prefab" ) ) )
		{
			color = color_t( 177, 232, 95 );
			text = xs( L"f1 grenade" );
		}
		if( wcsstr( obj_name.c_str( ), xs( L"grenade.flashbang.deployed.prefab" ) ) )
		{
			color = color_t( 165, 183, 201 );
			text = xs( L"flashbang" );
		}
		if( wcsstr( obj_name.c_str( ), xs( L"grenade.molotov.deployed.prefab" ) ) )
		{
			color = color_t( 199, 138, 74 );
			text = xs( L"molotov" );
		}
		if( wcsstr( obj_name.c_str( ), xs( L"grenade.beancan.deployed.prefab" ) ) )
		{
			color = color_t( 220, 220, 220 );
			text = xs( L"bean can" );
		}
	}
	if( distance < options::visuals::world::max_corpse_distance ) {
		bool npc_corpse = class_name == xs( L"NPCPlayerCorpse" );
		if( ( class_name == xs( L"PlayerCorpse" ) || npc_corpse )
			&& options::visuals::world::show_corpse )
		{
			color = options::visuals::world::color_corpse;
			text = npc_corpse ? xs( L"npc corpse" ) : xs( L"player corpse" );
		}
		if( wcsstr( obj_name.c_str( ), xs( L"item_drop_backpack.prefab" ) )
			&& options::visuals::world::show_backpack )
		{
			color = options::visuals::world::color_backpack;
			text = xs( L"backpack" );
		}
	}
	if( distance < options::visuals::world::max_ores_distance
		&& class_name == xs( L"OreResourceEntity" ) ) {
		if( ( wcsstr( obj_name.c_str( ), xs( L"metal-ore.prefab" ) )
			|| wcsstr( obj_name.c_str( ), xs( L"ore_metal.prefab" ) ) )
			&& options::visuals::world::show_metal_ore ) {
			color = options::visuals::world::color_metal_ore;
			text = xs( L"metal ore" );
		}
		else if( ( wcsstr( obj_name.c_str( ), xs( L"sulfur-ore.prefab" ) )
			|| wcsstr( obj_name.c_str( ), xs( L"ore_sulfur.prefab" ) ) )
			&& options::visuals::world::show_sulfur_ore ) {
			color = options::visuals::world::color_sulfur_ore;
			text = xs( L"sulfur ore" );
		}
		else if( ( wcsstr( obj_name.c_str( ), xs( L"stone-ore.prefab" ) )
			|| wcsstr( obj_name.c_str( ), xs( L"ore_stone.prefab" ) ) )
			&& options::visuals::world::show_stone_ore ) {
			color = options::visuals::world::color_stone_ore;
			text = xs( L"stone ore" );
		}
	}
	if( distance < options::visuals::world::max_item_distance ) {
		item* item = base_entity->get_world_item( );
		if( item ) {
			if( class_name == xs( L"DroppedItem" ) )
			{
				int item_amt = item->get_amount( );
				std::wstring item_amount = std::to_wstring( item_amt );

				text = item->get_weapon_name( );
				if( options::visuals::world::show_item_amount 
					&& item_amt > 1 )
					text += xs( L" x" ) + item_amount;

				if( text.find( xs( L"arrow" ) ) != std::string::npos 
					|| text.find( xs( L"nails" ) ) != std::string::npos ) {
					color = color_t( 143, 165, 199 );
					if( !options::visuals::world::show_ammo_nails_arrows )
						return;
				}
				else if( item->is_gun( ) ) {
					color = color_t( 145, 187, 255 );
					if( !options::visuals::world::show_weapons )
						return;
				}
				else if( item->is_melee( ) ) {
					color = color_t( 145, 156, 255 );
					if( !options::visuals::world::show_melee_weapons )
						return;
				}
				else if( item->is_heal( )
					&& !options::visuals::world::show_heal ) {
					color = color_t( 188, 255, 112 );
					return;
				}
				else if( !options::visuals::world::show_everything ) {
					color = color_t( 230, 247, 255 );
					return;
				}
				else if( text.find( xs( L"scrap" ) ) != std::string::npos )
					color = color_t( 222, 113, 60 );
				else if( text.find( xs( L"ammo" ) ) != std::string::npos ) {
					if( text.find( xs( L"incendiary" ) ) != std::string::npos )
						color = color_t( 255, 169, 112 );
					else if( text.find( xs( L"hv" ) ) != std::string::npos )
						color = color_t( 112, 169, 255 ); 
					else if( text.find( xs( L"explosive" ) ) != std::string::npos )
						color = color_t( 255, 119, 28 );
					else 
						color = color_t( 159, 167, 196 );
				}
				else if( text.find( xs( L"blue" ) ) != std::string::npos )
					color = color_t( 173, 199, 255 );
				else if( text.find( xs( L"white" ) ) != std::string::npos )
					color = color_t( 255, 255, 255 );
				else if( text.find( xs( L"green" ) ) != std::string::npos )
					color = color_t( 199, 255, 173 );
				else if( text.find( xs( L"red" ) ) != std::string::npos )
					color = color_t( 255, 110, 139 );
				else if( text.find( xs( L"purple" ) ) != std::string::npos )
					color = color_t( 156, 110, 255 );
				else if( text.find( xs( L"yellow" ) ) != std::string::npos )
					color = color_t( 217, 197, 95 );
				else
					color = color_t( 217, 237, 255 );
			}
		}
		if( class_name == xs( L"Drone" )
			&& options::visuals::world::show_drone )
		{
			color = options::visuals::world::color_drone;
			text = xs( L"drone" );
		}
		if( class_name == xs( L"Recycler" ) )
		{  
			if( base_entity 
				&& options::aimbot::exploits::misc::stop_recycler
				&& distance < 5.f 
				&& ( local->lastSentTickTime( ) - last_sent_tick ) > 2.5 ) {
				base_entity->server_rpc( xs( L"SVSwitch" ) );
				last_sent_tick = local->lastSentTickTime( );
			}

			if( options::visuals::world::show_recycler ) {
				color = options::visuals::world::color_recycler;
				text = xs( L"recycler" );
			}
		}
	}
	if( distance < options::visuals::world::max_respawn_points_distance )
	{
		if( class_name == xs( L"SleepingBag" ) )
		{
			color = color_t( 175, 175, 175 );
			text = xs( L"sleeping bag" );
		}
		if( wcsstr( obj_name.c_str( ), xs( L"bed_deployed.prefab" ) ) )
		{
			color = color_t( 225, 225, 225 );
			text = xs( L"bed" );
		}
	}
	if( distance < options::visuals::world::max_container_distance )
	{
		if( class_name == xs( L"HackableLockedCrate" )
			&& options::visuals::world::show_hackable_locked_crate )
		{
			text = xs( L"hackable crate" );
			color = options::visuals::world::color_hackable_locked_crate;
		}
		else if( class_name == xs( L"LootContainer" ) )	
		{
			if( wcsstr( obj_name.c_str( ), xs( L"crate_elite.prefab" ) )
				&& options::visuals::world::show_elite_crate )
			{
				text = xs( L"elite crate" );
				color = options::visuals::world::color_elite_crate;
			}
			else if( wcsstr( obj_name.c_str( ), xs( L"crate_basic.prefab" ) )
				&& options::visuals::world::show_small_crate )
			{
				text = xs( L"small crate" );
				color = options::visuals::world::color_small_crate;
			}
			else if( wcsstr( obj_name.c_str( ), xs( L"crate_normal_2.prefab" ) )
				&& options::visuals::world::show_normal_crate )
			{
				text = xs( L"crate" );
				color = options::visuals::world::color_normal_crate;
			}
			else if( wcsstr( obj_name.c_str( ), xs( L"crate_underwater_advanced.prefab" ) )
				&& options::visuals::world::show_underwater_crate )
			{
				text = xs( L"advanced underwater crate" );
				color = color_t( 3, 136, 252 );

				if( options::aimbot::exploits::misc::auto_untie_crates
					&& distance < 5.f )
					base_entity->server_rpc( xs( L"RPC_FreeCrate" ) ); 
			}
			else if( wcsstr( obj_name.c_str( ), xs( L"crate_underwater_basic.prefab" ) )
				&& options::visuals::world::show_underwater_crate )
			{
				text = xs( L"underwater crate" );
				color = color_t( 113, 159, 199 );

				if( options::aimbot::exploits::misc::auto_untie_crates
					&& distance < 5.f )
					base_entity->server_rpc( xs( L"RPC_FreeCrate" ) ); 
			}
			else if( wcsstr( obj_name.c_str( ), xs( L"crate_tools.prefab" ) )
				&& options::visuals::world::show_tool_box )
			{
				text = xs( L"toolbox" );
				color = options::visuals::world::color_tool_box;
			}
			else if( wcsstr( obj_name.c_str( ), xs( L"crate_normal.prefab" ) )
				&& options::visuals::world::show_military_crate )
			{
				text = xs( L"military crate" );
				color = options::visuals::world::color_military_crate;
			}
			else if( ( wcsstr( obj_name.c_str( ), xs( L"minecart.prefab" ) ) )
				&& options::visuals::world::show_mine_crate )
			{
				text = xs( L"mine crate" );
				color = options::visuals::world::color_mine_crate;
			}
			else if( ( wcsstr( obj_name.c_str( ), xs( L"crate_foodbox.prefab" ) ) 
				|| wcsstr( obj_name.c_str( ), xs( L"crate_normal_2_food.prefab" ) ) )
				&& options::visuals::world::show_food_crate )
			{
				text = xs( L"food crate" );
				color = options::visuals::world::color_food_crate;
			}
			//else if( wcsstr( obj_name.c_str( ), xs( L"crate_bradley.prefab" ) ) )
			//{
			//	text = xs( L"bradley crate" );
			//	color = color_t( 206, 255, 150 ) ;
			//}
			//else if( wcsstr( obj_name.c_str( ), xs( L"codelockedhackablecrate.prefab" ) ) )
			//{
			//	text = xs( L"hackable crate" );
			//	color = color_t( 206, 255, 150 ) ;
			//}
			//else if( wcsstr( obj_name.c_str( ), xs( L"codelockedhackablecrate_oilrig.prefab" ) ) )
			//{
			//	text = xs( L"hackable oil-rig crate" );
			//	color = color_t( 206, 255, 150 ) ;
			//}
			else if( wcsstr( obj_name.c_str( ), xs( L"crate_normal_2_medic.prefab" ) )
				&& options::visuals::world::show_medical_crate )
			{
				text = xs( L"medic crate" );
				color = options::visuals::world::color_medical_crate;
			}
			//else if( wcsstr( obj_name.c_str( ), xs( L"codelockedhackablecrate.prefab" ) ) )
			//{
			//	text = xs( L"hackable crate" );
			//	color = color_t( 206, 255, 150 ) ;
			//}
			//else if( wcsstr( obj_name.c_str( ), xs( L"codelockedhackablecrate_oilrig.prefab" ) ) )
			//{
			//	text = xs( L"hackable oil-rig crate" );
			//	color = color_t( 206, 255, 150 ) ;
			//}
		}
		else if( class_name == xs( L"SupplyDrop" )
			&& options::visuals::world::show_supply_drops )
		{
			color = options::visuals::world::color_supply_drops;
			text = xs( L"supply drop" );
		}
		else if( class_name == xs( L"SupplySignal" )
			&& options::visuals::world::show_supply_signals )
		{
			color = options::visuals::world::color_supply_signals;
			text = xs( L"supply signal" );
		}
		else if( ( wcsstr( obj_name.c_str( ), xs( L"bradley_crate" ) ) && wcsstr( obj_name.c_str( ), xs( L".prefab" ) ) )
			&& options::visuals::world::show_bradley_crate )
		{
			text = xs( L"bradley crate" );
			color = options::visuals::world::color_bradley_crate;
		}
		else if( wcsstr( obj_name.c_str( ), xs( L"heli_crate.prefab" ) )
			&& options::visuals::world::show_helicopter_crate )
		{
			text = xs( L"helicopter crate" );
			color = options::visuals::world::color_helicopter_crate;
		}
		//else if( class_name == xs( L"MobileMapMarker" )
			//&& class_name != xs( L"HackableLockedCrate" ) )
		//{
			//text = xs( L"marker" );
					
			//g_render.draw_text( render_position, text, color_t( 255, 255, 255 ), false, 10, font_flags_t::dropshadow );
		//}
	}
	if( distance < options::visuals::world::max_food_collectibles_distance )
	{
		if( wcsstr( obj_name.c_str( ), xs( L"hemp/hemp" ) )
			&& options::visuals::world::show_hemp ) {
			text = xs( L"hemp" );
			color = options::visuals::world::color_hemp;
		}
		else if( wcsstr( obj_name.c_str( ), xs( L"mushroom-cluster-6.prefab" ) )
			&& options::visuals::world::show_mushroom ) {
			text = xs( L"mushroom" );
			color = options::visuals::world::color_mushroom;
		}
		else if( wcsstr( obj_name.c_str( ), xs( L"pumpkin-collectable.prefab" ) )
			&& options::visuals::world::show_pumpkin ) {
			text = xs( L"pumpkin" );
			color = options::visuals::world::color_pumpkin;
		}
		else if( wcsstr( obj_name.c_str( ), xs( L"corn-collectable.prefab" ) )
			&& options::visuals::world::show_corns ) {
			text = xs( L"corn" );
			color = options::visuals::world::color_corns;
		}
		else if( wcsstr( obj_name.c_str( ), xs( L"berry-" ) )
			&& options::visuals::world::show_berrys ) {
			text = xs( L"berry" );
			color = color_t( 143, 128, 217 );
			if( wcsstr( obj_name.c_str( ), xs( L"white" ) ) )
				color = color_t( 209, 209, 209 );
			if( wcsstr( obj_name.c_str( ), xs( L"yellow" ) ) )
				color = color_t( 235, 196, 89 );
			if( wcsstr( obj_name.c_str( ), xs( L"blue" ) ) )
				color = color_t( 115, 171, 255 );
			if( wcsstr( obj_name.c_str( ), xs( L"red" ) ) )
				color = color_t( 255, 46, 99 );
			if( wcsstr( obj_name.c_str( ), xs( L"black" ) ) )
				color = color_t( 150, 150, 150 );
			if( wcsstr( obj_name.c_str( ), xs( L"green" ) ) )
				color = color_t( 117, 255, 117 );
		}
		else if( wcsstr( obj_name.c_str( ), xs( L"potato-collectable.prefab" ) )
			&& options::visuals::world::show_potatos ) {
			text = xs( L"potato" );
			color = options::visuals::world::color_potatos;
		}
	}

	if( distance < options::visuals::world::max_ore_collectibles_distance )
	{
		if( wcsstr( obj_name.c_str( ), xs( L"metal-collectable.prefab" ) )
			&& options::visuals::world::show_metal_collectibles ) {
			text = xs( L"metal collectible" );
			color = options::visuals::world::color_metal_collectibles;
		}
		else if( wcsstr( obj_name.c_str( ), xs( L"wood-collectable.prefab" ) ) 
			&& options::visuals::world::show_wood_collectibles ) {
			text = xs( L"wood collectible" );
			color = options::visuals::world::color_wood_collectibles;
		}
		else if( wcsstr( obj_name.c_str( ), xs( L"stone-collectable.prefab" ) )
			&& options::visuals::world::show_stone_collectibles ) {
			text = xs( L"stone collectible" );
			color = options::visuals::world::color_stone_collectibles;
		}
		else if( wcsstr( obj_name.c_str( ), xs( L"sulfur-collectable.prefab" ) )
			&& options::visuals::world::show_sulfur_collectibles ) {
			text = xs( L"sulfur collectible" );
			color = options::visuals::world::color_sulfur_collectibles;
		}
	}

	//std::string stash_state = std::string( );
	//stash_container* stash_container = reinterpret_cast< stash_container* >( combat_entity );

	//if( stash_container ) {
	//	float buried_offset = stash_container->get_burried_offset( );
	//	if( buried_offset == 2048 )
	//		stash_state = " | buried";
	//	else if( buried_offset == 8 )
	//		stash_state = " | being looted";
	//	else
	//		stash_state = " | not buried";

	//	stash_state += std::to_string( buried_offset );

	//	if( distance < options::visuals::world::max_stash_distance 
	//		&& options::visuals::world::show_stashes ) {
	//		if( class_name == xs( L"StashContainer" ) )
	//		{
	//			text = xs( L"stash" ) + stash_state;
	//			
	//			g_render.draw_text( render_position, text, color_t( 255, 255, 255 ), false, 10, font_flags_t::dropshadow );
	//		}
	//	}
	//}

	if( distance < options::visuals::world::max_barrels_distance )
	{
		bool is_barrel = false;

		if( class_name == xs( L"LootContainer" ) )
		{
			if( ( wcsstr( obj_name.c_str( ), xs( L"loot-barrel-1.prefab" ) )
				|| wcsstr( obj_name.c_str( ), xs( L"loot-barrel-2.prefab" ) )
				|| ( wcsstr( obj_name.c_str( ), xs( L"loot_barrel_1.prefab" ) )
				|| wcsstr( obj_name.c_str( ), xs( L"loot_barrel_2.prefab" ) ) ) ) )
			{
				if( options::visuals::world::show_regular_barrels ) {
					text = xs( L"barrel" );
					if( wcsstr( obj_name.c_str( ), xs( L"loot-barrel-1.prefab" ) )
						|| wcsstr( obj_name.c_str( ), xs( L"loot_barrel_1.prefab" ) ) )
						color = color_t( 135, 177, 222 );
					else
						color = color_t( 227, 193, 113 );
				}
				is_barrel = true;
			}
			else if( wcsstr( obj_name.c_str( ), xs( L"oil_barrel.prefab" ) ) )
			{
				if( options::visuals::world::show_oil_barrel ) {
					text = xs( L"oil barrel" );
					color = options::visuals::world::color_oil_barrel;
				}
				is_barrel = true;
			}
		}
		else if( ( wcsstr( obj_name.c_str( ), xs( L"diesel" ) ) && wcsstr( obj_name.c_str( ), xs( L".prefab" ) ) ) )
		{
			if( options::visuals::world::show_diesel_fuel ) {
				text = xs( L"diesel fuel" );
				color = options::visuals::world::color_diesel_fuel;
			}
			is_barrel = true;
		}

		if( options::aimbot::exploits::misc::auto_farm_barrels ) {
			if( is_barrel ) {
				transform* transform = base_entity->get_transform( );
				if( transform
					&& distance < g_esp.melee_max_dist )
					g_esp.do_melee_attack( transform->get_position( ), base_entity, false );
			}
		}
	}
	if( distance < options::visuals::world::max_cars_distance )
	{
		if( !wcsstr( obj_name.c_str( ), xs( L"lift" ) ) ) {
			if( wcsstr( obj_name.c_str( ), xs( L"modularcar" ) )
				&& options::visuals::world::show_modular_car )
			{
				text = xs( L"modular car" );
				color = options::visuals::world::color_modular_car;
			}
			else if( wcsstr( obj_name.c_str( ), xs( L"2module_camper" ) )
				&& options::visuals::world::show_two_modules_car )
			{
				text = xs( L"two module car" );
				color = options::visuals::world::color_two_modules_car;
			}
		}
	}
	if( distance < options::visuals::world::max_storage_distance )
	{
		//std::string upkeep_time = std::to_string( static_cast< int >( base_entity->get_protected_minutes( ) / 60 ) );

		if( class_name == xs( L"BuildingPrivlidge" )
			&& options::visuals::world::show_tool_cupboard )
		{
			color = options::visuals::world::color_tool_cupboard;
			text = xs( L"tool cupboard" );
		}
		if( class_name == xs( L"VendingMachine" )
			&& options::visuals::world::show_vending_machine )
		{
			color = options::visuals::world::color_vending_machine;
			text = xs( L"vending machine" );
		}
		if( class_name == xs( L"BoxStorage" )
			&& options::visuals::world::show_box_storages )
		{
			color = options::visuals::world::color_box_storages;
			text = xs( L"box storage" );
		}
	}

	if( distance < options::visuals::world::max_stash_distance 
		&& options::visuals::world::show_stashes ) {
		if( class_name == xs( L"StashContainer" ) )
		{
			color = options::visuals::world::color_stashes;
			text = xs( L"stash" );
		}
	}
	if( distance < options::visuals::world::max_animal_distance ) {
		if( class_name == xs( L"Boar" )
			&& options::visuals::world::show_boars )
		{
			text = xs( L"boar" );
			color = options::visuals::world::color_boars;
		}
		if( class_name == xs( L"Chicken" )
			&& options::visuals::world::show_chickens )
		{	
			color = options::visuals::world::color_chickens;
			text = xs( L"chicken" );
		}
		if( class_name == xs( L"Stag" ) 
			&& options::visuals::world::show_deers )
		{
			color = options::visuals::world::color_deers;
			text = xs( L"deer" );
		}
		if( class_name == xs( L"Wolf" )
			&& options::visuals::world::show_wolves )
		{
			color = options::visuals::world::color_wolves;
			text = xs( L"wolf" );
		}
		if( class_name == xs( L"Bear" )
			&& options::visuals::world::show_bears )
		{
			color = options::visuals::world::color_bears;
			text = xs( L"bear" );
		}
		if( class_name == xs( L"Polarbear" )
			&& options::visuals::world::show_polar_bears )
		{
			color = options::visuals::world::color_polar_bears;
			text = xs( L"polar bear" );
		}
		if( class_name == xs( L"RidableHorse" )
			&& options::visuals::world::show_horses )
		{
			color = options::visuals::world::color_horses;
			text = xs( L"horse" );
		}
		if( wcsstr( obj_name.c_str( ), xs( L"simpleshark.prefab" ) )
			&& options::visuals::world::show_sharks )
		{
			color = options::visuals::world::color_sharks;
			text = xs( L"shark" );
		}
	}
	if( distance < options::visuals::world::max_vehicle_distance )
	{
		if( wcsstr( obj_name.c_str( ), xs( L"submarineduo.entity.prefab" ) )
			&& options::visuals::world::show_duo_submarine )
		{
			color = options::visuals::world::color_duo_submarine;
			text = xs( L"duo submarine" );
		}
		if( wcsstr( obj_name.c_str( ), xs( L"submarinesolo.entity.prefab" ) )
			&& options::visuals::world::show_solo_submarine )
		{
			color = options::visuals::world::color_solo_submarine;
			text = xs( L"solo submarine" );
		}
		if( class_name == xs( L"MotorRowboat" )
			&& options::visuals::world::show_small_motorboat )
		{
			color = options::visuals::world::color_small_motorboat;
			text = xs( L"motor boat" );
		}
		if( wcsstr( obj_name.c_str( ), xs( L"kayak.prefab" ) )
			&& options::visuals::world::show_kayak )
		{
			color = options::visuals::world::color_kayak;
			text = xs( L"kayak" );
		}
		if( class_name == xs( L"RHIB" )
			&& options::visuals::world::show_rhib )
		{
			color = options::visuals::world::color_rhib;
			text = xs( L"rhib" );
		}
		if( wcsstr( obj_name.c_str( ), xs( L"bradleyapc.prefab" ) )
			&& options::visuals::world::show_bradley_apc )
		{
			color = options::visuals::world::color_bradley_apc;
			text = xs( L"bradley apc" );
		}
		if( class_name == xs( L"MiniCopter" )
			&& options::visuals::world::show_minicopter )
		{
			color = options::visuals::world::color_rhib;
			text = xs( L"minicopter" );
		}
		if( class_name == xs( L"ScrapTransportHelicopter" )
			&& options::visuals::world::show_scrap_helicopter )
		{
			color = options::visuals::world::color_scrap_helicopter;
			text = xs( L"scrap helicopter" );
		}
	}
	if( distance < options::visuals::world::max_helicopter_distance ) {

		if( class_name == xs( L"PatrolHelicopter" )
			&& options::visuals::world::show_patrol_helicopter )
		{
			color = options::visuals::world::color_patrol_helicopter;
			text = xs( L"patrol helicopter" );
		}
		if( wcsstr( obj_name.c_str( ), xs( L"attackhelicopter.entity.prefab" ) )
			&& options::visuals::world::show_attack_helicopter )
		{
			color = options::visuals::world::color_attack_helicopter;
			text = xs( L"attack helicopter" );
		}
		if( class_name == xs( L"CH47Helicopter" )
			&& options::visuals::world::show_chinook_helicopter )
		{
			color = options::visuals::world::color_chinook_helicopter;
			text = xs( L"chinook helicopter" );
		}
	}
	if( distance < options::visuals::world::max_trap_distance )
	{
		if( class_name == xs( L"Landmine" ) 
			&& options::visuals::world::show_landmines )
		{
			color = options::visuals::world::color_landmines;
			text = xs( L"landmine" );

			base_player* local = g_cheat.local;

			if( local ) {
				if( base_entity 
					&& options::aimbot::exploits::misc::disarm_landmines
					&& distance < 5.f 
					&& ( local->lastSentTickTime( ) - last_sent_tick ) > 0.05 ) {
					base_entity->server_rpc( L"RPC_Disarm" );
					last_sent_tick = local->lastSentTickTime( );
				}
			}
		}
		if( class_name == xs( L"NPCAutoTurret" )
			&& options::visuals::world::show_npc_turrets )
		{
			color = options::visuals::world::color_npc_turrets;
			text = xs( L"npc turret" );
		}
		if( wcsstr( obj_name.c_str( ), xs( L"spikes.floor.prefab" ) )
			&& options::visuals::world::show_land_spikes )
		{
			color = options::visuals::world::color_land_spikes;
			text = xs( L"floor spikes" );
		}
		if( class_name == xs( L"FlameTurret" )
			&& options::visuals::world::show_flame_turrets )
		{
			color = options::visuals::world::color_flame_turrets;
			text = xs( L"flame turret" );
		}
		if( class_name == xs( L"TeslaCoil" )
			&& options::visuals::world::show_tesla_coil )
		{
			color = options::visuals::world::color_tesla_coil;
			text = xs( L"tesla coil" );
		}
		if( class_name == xs( L"AutoTurret" )
			&& options::visuals::world::show_auto_turrets )
		{
			color = options::visuals::world::color_auto_turrets;
			text = xs( L"turret" );
		}
		if( class_name == xs( L"GunTrap" )
			&& options::visuals::world::show_shotgun_traps )
		{
			color = options::visuals::world::color_shotgun_traps;
			text = xs( L"shotgun trap" );
		}
		if( class_name == xs( L"SamSite" )
			&& options::visuals::world::show_sam_site )
		{
			color = options::visuals::world::color_sam_site;
			text = xs( L"sam-site" );
		}
		if( class_name == xs( L"BearTrap" )
			&& options::visuals::world::show_bear_trap )
		{
			color = options::visuals::world::color_bear_trap;
			text = xs( L"bear trap" );
		}
	}

	if( !text.empty( )
		&& color.a > 0 ) {
		std::transform( text.begin( ), text.end( ), text.begin( ), ::tolower );

		bool show_distance = options::visuals::world::show_distance;
		bool show_health = options::visuals::world::show_health 
			&& combat_entity
			&& combat_entity->get_health( ) > 0;

		if( show_distance || show_health )
			text += xs( L"[" );

		if( show_distance )
			text += dist_str + xs( L"m" );
		
		if( show_health ) {
			std::wstring health_str = std::to_wstring( static_cast< int >( combat_entity->get_health( ) ) ) + xs( L"hp" );

			text += show_distance ? xs( L"|" ) + health_str : health_str;
		}

		if( show_distance || show_health )
			text += xs( L"]" );

		g_render.draw_text( render_position, text.c_str( ), color, false, 10, g_esp.font_flags );
	}
}