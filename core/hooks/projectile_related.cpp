#include "projectile_related.hpp"
#include "../features/aimbot/aimbot.hpp"
#include "../features/visuals/esp.hpp"

namespace projectileshoot
{
	void on_projectile_shoot( void* ecx ) {
		g_esp.time_since_last_shot = g_sdk.get_fixedTime( );
		g_esp.just_shot = true;
		g_esp.did_reload = false;

		int layers = ( ( int )Layers::ProjectileLineOfSightCheckTerrain | ( int )Layers::Terrain | ( int )Layers::EntityLineOfSightCheck | ( int )Layers::Construction );

		base_player* local = g_cheat.local;
		if( !local )
			return;

		item* weapon = local->get_held_item( );
		if( !weapon )
			return;
		
		if( weapon->is_melee( )
			&& options::aimbot::ignore_melee )
			return;

		base_entity* base_projectile = weapon->get_held_entity( );
		if( !base_projectile )
			return;

		//base_projectile::Magazine* magazine = weapon->get_primaryMagazine( );
		//if( !magazine )
		//	return;

		//item_definition* ammo = magazine->get_ammotype( );
		//if( !ammo )
		//	return;

		//item_mod_projectile* mod_proj = ammo->get_component< item_mod_projectile* >( g_il2mgr.type_object( xs( "" ), xs( "ItemModProjectile" ) ) );
		//if( !mod_proj )
		//	return;

		base_player* target = g_aimbot.target;
		if( !target )
			return;

		uintptr_t createdProjectiles = g_il2mgr.value( xs( "BaseProjectile" ), xs( "createdProjectiles" ) );
		if( !createdProjectiles )
			return;

		uintptr_t projectiles = *reinterpret_cast< uintptr_t* >( ( uintptr_t )ecx + 0x18 );
		if( !projectiles )
			return;

		uintptr_t created_list = *reinterpret_cast< uintptr_t* >( base_projectile + createdProjectiles );
		if( !created_list )
			return;

		const uintptr_t projectile_list = *reinterpret_cast< uintptr_t* >( created_list + 0x10 );
		const int projectile_list_size = *reinterpret_cast< int* >( projectile_list + 0x18 );

		const uintptr_t shoot_list = *reinterpret_cast< uintptr_t* >( projectiles + 0x10 );
		const int shoot_list_size = *reinterpret_cast< int* >( shoot_list + 0x18 );

		vec3_t original_velocity = vec3_t( ), manipulation_angle = vec3_t( ), backup_start_position = vec3_t( );
		bool visible = false, same_angle = false, can_manip = false;
		vec3_t aim_velocity = vec3_t( );

		for( int i = 0; i < shoot_list_size; i++ )
		{
			uintptr_t projectile_shoot = *reinterpret_cast< uintptr_t* >( shoot_list + 0x20 + ( i * sizeof( uint64_t ) ) );
			if( !projectile_shoot )
				continue;

			vec3_t& start_position = *reinterpret_cast< vec3_t* >( projectile_shoot + 0x14 );
			manipulation_angle = start_position;
			backup_start_position = manipulation_angle;

			if( !g_esp.manipulation_angle.is_zero( ) )
			{	
				manipulation_angle += g_esp.manipulation_angle;
				visible = g_sdk.is_visible( manipulation_angle, g_aimbot.bone_position, 0.18f, layers );

				if( manipulation_angle == backup_start_position )
					same_angle = true;

				can_manip = !same_angle && visible;
				g_esp.manipulator_ready = can_manip;

				if( can_manip ) {
					start_position = manipulation_angle;
				}
			}
			original_velocity = *reinterpret_cast< vec3_t* >( projectile_shoot + 0x20 );
			
			vec3_t direction = ( ( options::aimbot::should_simulate_movement ? g_aimbot.shoot_position : g_aimbot.bone_position ) - start_position ).normalized( );
			aim_velocity = ( direction * original_velocity.length( ) );
		
			*reinterpret_cast< vec3_t* >( projectile_shoot + 0x20 ) = aim_velocity;
		}

		bool can_aim = g_sdk.get_key( g_aimbot.aim_key );

		for( int i = 0; i < projectile_list_size; i++ )
		{
			projectile* this_projectile = *reinterpret_cast< projectile** >( projectile_list + 0x20 + ( i * sizeof( uint64_t ) ) );
			if( !this_projectile )
				continue;

			if( options::aimbot::exploits::combat::no_spread ) {
				item_mod_projectile* mod = this_projectile->get_item_mod_projectile( );
				float spread = options::aimbot::exploits::combat::reduce_spread / 100.f;
				if( mod ) 
				{
					mod->set_projectile_spread( spread );
					mod->set_projectile_velocity_spread( spread );
				}
			}

			float ricochet_chance = this_projectile->get_ricochet_chance( );
			if( options::aimbot::exploits::combat::ricochet )
				this_projectile->set_ricochet_chance( 100.f );
			else
				this_projectile->set_ricochet_chance( ricochet_chance );

			if( options::aimbot::aim_type == 3 
				&& can_aim
				|| can_manip ) {
				this_projectile->set_current_velocity( aim_velocity );
				this_projectile->set_initial_velocity( aim_velocity );
			}

			float thickness = this_projectile->get_thickness( );
			if( options::aimbot::exploits::combat::big_bullets )
				this_projectile->set_thickness( options::aimbot::exploits::combat::bullet_size / 100.f );
			else
				this_projectile->set_thickness( thickness );

			if( can_manip ) {
				visible = g_sdk.is_visible( manipulation_angle, g_aimbot.bone_position, 0.18f, layers );

				if( visible ) {
					transform* transform = this_projectile->get_transform( );
					if( transform )
						transform->set_position( manipulation_angle );
				}
			}
		}
	}

	void write_to_stream_hook( void* ecx, void* stream )
	{
		auto orig_projectileshoot_write_to_stream = reinterpret_cast< void( * )( void*, void* ) >( o_projectile_shoot );

		on_projectile_shoot( ecx );

		spoof_ret( orig_projectileshoot_write_to_stream, ecx, stream );
	}
}

namespace playerprojectileattack {
	void on_projectile_attack( void* ecx ) {
		uintptr_t player_attack = *reinterpret_cast< uintptr_t* >( ( uintptr_t )ecx + 0x10 );
		if( !player_attack )
			return;

		uintptr_t attack = *reinterpret_cast< uintptr_t* >( player_attack + 0x10 );
		if( !attack )
			return;

		int& hit_bone = *reinterpret_cast< int* >( attack + 0x30 );
		int& hit_part_id = *reinterpret_cast< int* >( attack + 0x64 );

		if ( options::aimbot::exploits::combat::override_hitbox ) {
			switch( options::aimbot::exploits::combat::overriden_hitbox ) {
			case 0:
				hit_bone = 698017942;
				hit_part_id = 2173623152;
				break;
			case 1:
				hit_bone = 2811218643;
				hit_part_id = 1750816991;
				break;
			case 2:
				hit_bone = 1031402764;
				hit_part_id = 1750816991;
				break;
			}
		}
	}

	void write_to_stream_hook( void* ecx, void* stream )
	{
		auto orig_projectileshoot_write_to_stream = reinterpret_cast< void( * )( void*, void* ) >( projectile_attack );

		on_projectile_attack( ecx );

		spoof_ret( orig_projectileshoot_write_to_stream, ecx, stream );
	}
}

namespace playerprojectileupdate {
	void on_projectile_update( void* ecx ) {
		/*if( !ecx 
			|| !options::aimbot::bullet_tp ) {
			std::cout << "xd" << std::endl;
			std::cout << __LINE__ << std::endl;
			return;
		}

		projectile* proj = *reinterpret_cast< projectile** >( ecx );
		if( !proj ) {
			std::cout << "xd" << std::endl;
			std::cout << __LINE__ << std::endl;
			return;
		}

		transform* proj_tr = proj->get_transform( );
		if( !proj_tr ) {
			std::cout << "xd" << std::endl;
			std::cout << __LINE__ << std::endl;
			return;
		}

		if( !proj->is_authoritative( ) ) {
			std::cout << "xd" << std::endl;
			std::cout << __LINE__ << std::endl;
			return;
		}
		std::cout << proj->get_thickness( ) << std::endl;*/
		//base_player* target = g_aimbot.target;
		//if( !target ) {
		//	std::cout << "xd" << std::endl;
		//	std::cout << __LINE__ << std::endl;
		//	return;
		//}

		//float offset = 0.f;

		//auto _transform = target->get_bone_transform( head );
		//if( !_transform ) {
		//	std::cout << "xd" << std::endl;
		//	std::cout << __LINE__ << std::endl;
		//	return;
		//}

		//auto target_pos = _transform->get_position( );
		//if( target_pos.is_zero( ) ) {
		//	std::cout << "xd" << std::endl;
		//	std::cout << __LINE__ << std::endl;
		//	return;
		//}

		//bool mounted = target->get_mountable( );
		//vec3_t proj_pos = proj_tr->get_position( );
		//if( proj_pos.is_zero( ) ) {
		//	std::cout << "xd" << std::endl;
		//	std::cout << __LINE__ << std::endl;
		//	return;
		//}

		//float dist = target_pos.distance( proj_pos );

		//bool auto_shoot_check = mounted && options::aimbot::bullet_tp_mounted ? dist < 10.f : dist < 1.60f;
		//bool updatedist = mounted && options::aimbot::bullet_tp_mounted ? dist < 15.f : dist < 10.f;
		//if( !updatedist
		//	|| !auto_shoot_check
		//	|| !g_sdk.is_visible( target_pos, proj_pos ) ) {
		//	std::cout << "xd" << std::endl;
		//	std::cout << __LINE__ << std::endl;
		//}

		//for( projectile* this_proj = proj; proj->is_alive( ); 
		//	this_proj->update_velocity( 0.0001 ) ) {
		//	auto dist = target_pos.distance( this_proj->get_transform( )->get_position( ) );

		//	hit_test* new_hit_test = proj->get_hit_test( );

		//	vec3_t new_pos = target_pos;
		//	//auto new_pos = vec3_t::move_towards( proj_tr->get_position( ), target_pos, 1.0f );
		//	Ray r( proj_tr->get_position( ), new_pos );
		//	new_hit_test->set_attack_ray( r );
		//	new_hit_test->set_hit_transform( _transform );
		//	new_hit_test->set_hit_entity( target );
		//	new_hit_test->set_hit_hitpoint( _transform->inverse_tranform_point( new_pos ) );
		//	new_hit_test->set_hit_normal( vec3_t( 0.f, 0.f, 0.f ) );
		//	new_hit_test->set_did_hit( true );
		//	proj->do_hit( new_hit_test, new_pos, vec3_t( 0.f, 0.f, 0.f ) );

		//	if( g_sdk.get_time( ) - this_proj->get_launch_time( ) < this_proj->get_traveled_time( ) + 0.03125f )
		//		break;
		//}
	}

	void write_to_stream_hook( void* ecx, void* stream )
	{
		auto orig_projectileshoot_write_to_stream = reinterpret_cast< void( * )( void*, void* ) >( projectile_update );

		on_projectile_update( ecx );

		spoof_ret( orig_projectileshoot_write_to_stream, ecx, stream );
	}
}

namespace playerprojectilericochet {
	void on_projectile_ricochet( void* ecx ) {
		if( !ecx
			|| !options::aimbot::exploits::combat::ricochet )
			return;

		//vec3_t direction = ( ( options::aimbot::should_simulate_movement ? g_aimbot.shoot_position : g_aimbot.bone_position ) - start_position ).normalized( );
		//aim_velocity = ( direction * original_velocity.length( ) );
		//
		//*reinterpret_cast< vec3_t* >( projectile_shoot + 0x20 ) = aim_velocity;

		vec3_t& player_ricochet = *reinterpret_cast< vec3_t* >( ( uintptr_t )ecx + 0x2C );

		player_ricochet = options::aimbot::should_simulate_movement ? g_aimbot.shoot_position : g_aimbot.bone_position;
	}

	void write_to_stream_hook( void* ecx, void* stream )
	{
		auto orig_projectileshoot_write_to_stream = reinterpret_cast< void( * )( void*, void* ) >( projectile_ricochet );

		on_projectile_ricochet( ecx );

		spoof_ret( orig_projectileshoot_write_to_stream, ecx, stream );
	}
}