#include "player_related.hpp" 
#include "../features/aimbot/aimbot.hpp"
#include "../features/visuals/esp.hpp"

std::unordered_map< std::uint8_t*, std::pair< std::vector< std::uint8_t >, float > > sound_cache{ };

void read_wav_to_file_memory( std::string fname, BYTE** pb, DWORD* fsize )
{
	std::ifstream f( fname, std::ios::binary );

	f.seekg( 0, std::ios::end );
	int lim = f.tellg( );
	*fsize = lim;

	*pb = new BYTE[ lim ];
	f.seekg( 0, std::ios::beg );

	f.read( ( char* )*pb, lim );

	f.close( );
};

void modify_volume_sound( char* bytes, ptrdiff_t file_size, float volume )
{
	int offset = 0;
	for ( int i = 0; i < file_size / 2; i++ )
	{
		if( bytes[ i ] == 'd'
			&& bytes[ i + 1 ] == 'a'
			&& bytes[ i + 2 ] == 't'
			&& bytes[ i + 3 ] == 'a' )
		{
			offset = i;
			break;
		}
	}

	if( !offset )
		return;

	char* data_offset = ( bytes + offset );
	DWORD sample_bytes = *( DWORD* )( data_offset + 4 );
	DWORD samples = sample_bytes / 2;

	SHORT* sample = ( SHORT* )( data_offset + 8 );
	for ( DWORD i = 0; i < samples; i++ )
	{
		SHORT sh_sample = *sample;
		sh_sample = ( SHORT )( sh_sample * volume );
		*sample = sh_sample;
		sample++;
		if ( ( ( char* )sample ) >= ( bytes + file_size - 1 ) )
			break;
	}
};

void play_sound_from_memory( uint8_t* bytes, size_t size, float volume )
{
	if ( !sound_cache.count( bytes ) )
		sound_cache[ bytes ].first.resize( size );

	auto& current = sound_cache.at( bytes );
	std::uint8_t* stored_bytes = current.first.data( );

	if( current.second != volume )
	{
		std::memcpy( stored_bytes, bytes, size );
		current.second = volume;
		modify_volume_sound( ( char* )stored_bytes, size, volume );
	}

	PlaySoundA( ( char* )stored_bytes, NULL, SND_ASYNC | SND_MEMORY );
}

namespace baseplayer
{
	void handle_exploits( base_player* local_player, item* held_item )
	{
		if( options::aimbot::exploits::time::fast_switch )
		{
			held_item->set_deploy_delay( 0.f );
		}

		if( held_item->is_melee( ) )
		{
			if( options::aimbot::exploits::movement::disable_slow_down_melee )
			{
				held_item->set_block_sprint( false );
			}
			if( options::aimbot::exploits::combat::extended_melee )
			{
				held_item->set_attack_max_distance( options::aimbot::exploits::combat::attack_radius );
			}
		}
		else if( held_item->is_gun( ) ) {
			if( options::aimbot::exploits::combat::aim_bolt_cycle )
			{
				held_item->set_aiming_while_cycling( false );
			}
			if( options::aimbot::exploits::time::fast_bow )
			{
				held_item->set_attack_ready( true );
				held_item->set_was_aiming( true );
			}
			if( options::aimbot::exploits::combat::modify_eoka_chance ) {
				float eoka_chance = options::aimbot::exploits::combat::eoka_chance / 100.f;

				held_item->set_always_eoka( eoka_chance );

				if( eoka_chance > 0.9f
					&& g_sdk.get_key( key_code::Mouse0 ) )
					held_item->set_did_spark_this_frame( true );
			}
			if( options::aimbot::exploits::time::rapid_fire )
			{
				held_item->set_repeat_delay( options::aimbot::exploits::time::rapid_fire_speed / 100.f );
			}

			if( options::aimbot::exploits::time::instant_charge_compound )
			{
				held_item->set_current_hold_progress( 1.5f );
				held_item->set_string_bonus_damage( 1000.f );
				held_item->set_movement_penalty_ramp_up_time( 1000.f );
			}

			if( options::aimbot::exploits::misc::fake_fire
				&& ( g_sdk.get_key( options::aimbot::exploits::misc::fake_fire_key ) || options::aimbot::exploits::misc::fake_fire_on ) )
			{
				static float last_shoot = 0.f;
				float delay = options::aimbot::exploits::misc::delay_fake_fire / 10.f;

				if( ( last_shoot - local_player->lastSentTickTime( ) ) < delay ) {
					held_item->send_broadcast_projectile( Signal::Attack, xs( L"" ) );
					last_shoot = local_player->lastSentTickTime( );
				}
			}

			if( options::aimbot::auto_reload )
			{
				if( !g_esp.did_reload )
					g_esp.time_since_last_shot = ( g_sdk.get_fixedTime( ) - g_esp.fixed_time_last_shot );

				if( !g_esp.just_shot
					&& ( g_esp.time_since_last_shot > 0.2f ) )
				{
					held_item->server_rpc( xs( L"StartReload" ) );
					held_item->send_signal_broadcast( Signal::Reload, xs( L"" ) );
					g_esp.just_shot = true;
				}
				if( g_esp.time_since_last_shot >
					( held_item->get_reload_time( ) - ( held_item->get_reload_time( ) / 10.f ) )
					&& !g_esp.did_reload )
				{
					held_item->server_rpc( xs( L"Reload" ) );
					g_esp.did_reload = true;
					g_esp.time_since_last_shot = 0;
				}

				g_esp.fixed_time_last_shot = g_sdk.get_fixedTime( );
			}

			if( options::aimbot::exploits::combat::burst_weapons ) {
				held_item->set_is_burst( true );
			}
			if( options::aimbot::exploits::combat::automatic_weapons ) {
				held_item->set_is_automatic( true );
			}
			else if( options::aimbot::exploits::combat::semi_automatic_weapons ) {
				held_item->set_is_automatic( false );
			}

			float to_reduce = options::aimbot::exploits::combat::reduce_recoil / 100.f;

			held_item->set_recoil_min_max( options::aimbot::exploits::combat::no_recoil ?
				to_reduce : 1.f );

			if( options::aimbot::exploits::combat::no_sway )
			{
				held_item->set_aim_sway( 0.f );
				held_item->set_aim_sway_speed( 0.f );
			}

			if( options::aimbot::exploits::combat::no_spread )
			{
				float spread = options::aimbot::exploits::combat::reduce_spread / 100.f;

				//std::cout << "1:" << held_item->get_aim_cone( )  << std::endl;
				//std::cout << "2:" << held_item->get_aim_cone_penalty_per_shot( )  << std::endl;
				//std::cout << "3:" << held_item->get_aim_cone_penalty( )  << std::endl;
				//std::cout << "4:" << held_item->get_hip_aim_cone( )  << std::endl;
				//std::cout << "5:" << held_item->get_stance_penalty( )  << std::endl;
				//std::cout << "6:" << held_item->get_aim_cone_penalty_max( )  << std::endl;
				//std::cout << "7:" << held_item->get_aim_penalty_recover_time( )  << std::endl;
				//std::cout << "8:" << held_item->get_aim_penalty_recover_delay( )  << std::endl;
				//std::cout << "9:" << held_item->get_aim_sight_aim_cone_scale( )  << std::endl;
				//std::cout << "10:" << held_item->get_aim_sight_aim_cone_offset( )  << std::endl;
				//std::cout << "11:" << held_item->get_hip_aim_cone_scale( )  << std::endl;
				//std::cout << "12:" << held_item->get_hip_aim_cone_offset( )  << std::endl;

				held_item->set_aim_cone( spread );
				held_item->set_aim_cone_penalty_per_shot( spread );
				held_item->set_aim_cone_penalty( spread );
				held_item->set_hip_aim_cone( spread );
				held_item->set_stance_penalty( spread );
				held_item->set_aim_cone_penalty_max( spread );
				held_item->set_aim_penalty_recover_time( spread );
				held_item->set_aim_penalty_recover_delay( spread );
				held_item->set_aim_sight_aim_cone_scale( spread );
				held_item->set_aim_sight_aim_cone_offset( spread );
				held_item->set_hip_aim_cone_scale( spread );
				held_item->set_hip_aim_cone_offset( spread );
			}

			if( options::aimbot::exploits::combat::quick_bullets )
			{
				float speed = ( options::aimbot::exploits::combat::bullet_speed / 100.f );

				held_item->set_projectile_velocity_scale( speed );
			}
			if( options::aimbot::exploits::combat::spoof_hit_distance )
			{
				float distance = options::aimbot::exploits::combat::bullet_distance;

				held_item->set_projectile_distance_scale( distance );
			}
		}
	}

	void handle_rpc_exploits( base_player* local_player, item* held_item ) {
		base_player* best_target = g_aimbot.target;
		float distance = FLT_MAX;
		static float last_syringe = 0.f;

		if( !last_syringe )
			last_syringe = local_player->lastSentTickTime( );

		if( best_target ) {
			player_model* model = best_target->get_model( );
			if( !model )
				return;

			distance = g_cheat.local_pos.distance( model->get_position( ) );

			if( options::aimbot::exploits::time::instant_revive
				|| options::aimbot::exploits::misc::keep_wounded_alive )
			{
				if( best_target->is_knocked( ) ) {
					if( distance < 10.0f )
					{
						if( ( g_sdk.get_key( options::aimbot::exploits::time::revive_key ) || options::aimbot::exploits::time::always_revive_target )
							&& options::aimbot::exploits::time::instant_revive ) {
							best_target->server_rpc( xs( L"RPC_Assist" ) );
						}
						if( options::aimbot::exploits::misc::keep_wounded_alive )
						{
							best_target->server_rpc( xs( L"RPC_KeepAlive" ) );
						}
					}
				}
			}
		}

		if( held_item->is_heal( ) ) {
			if( options::aimbot::exploits::time::fast_heal )
			{
				if( ( local_player->lastSentTickTime( ) - last_syringe ) > 0.7f ) {
					if( distance < 5.f )
						held_item->server_rpc( xs( L"UseOther" ) );
					else
						held_item->server_rpc( xs( L"UseSelf" ) );
					last_syringe = local_player->lastSentTickTime( );
				}
			}
		}
	}

	void auto_fire( item* item, base_player* local_player ) {
		if( !item
			|| !local_player )
			return;

		if( item->has_attack_cooldown( )
			|| item->get_next_attack_time( ) >= g_sdk.get_time( )
			|| item->get_time_since_deploy( ) < item->get_deploy_delay( ) )
			return;

		base_projectile::Magazine* magazine = item->get_primaryMagazine( );
		if( !magazine )
			return;

		int contents = magazine->contents( );
		if( !contents )
			return;

		if( options::aimbot::auto_fire_type == 1 ) {
			item->launch_projectile( );
			item->update_ammo_display( );
			item->shot_fired( );
			item->did_attack_clientside( );
			item->begin_cycle( );
		}
		else {
			int needed_shots = static_cast< int >( ( ( g_sdk.realtimeSinceStartup( ) - local_player->lastSentTickTime( ) ) ) / item->get_repeat_delay( ) );

			if( needed_shots < 1 )
				needed_shots = 1;
			else {
				for( int i = 0; i < needed_shots; i++ )
				{
					item->launch_projectile( );
					item->update_ammo_display( );
					item->shot_fired( );
					item->did_attack_clientside( );
					item->begin_cycle( );
				}
			}
		}
	}

	void client_input_hook( base_player* this_player, input_state* state )
	{
		auto orig_baseplayer_client_input = reinterpret_cast< void( * )( base_player*, input_state* ) >( client_input );
		if( this_player != g_cheat.local )
			return spoof_ret( orig_baseplayer_client_input, this_player, state );

		while( !g_cheat.camera_ptr )
			get_main_camera( );

		camera* camera = g_cheat.camera_ptr;
		if( camera ) {
			if( options::visuals::world::aspect_changer )
				camera->set_aspect_ratio( options::visuals::world::aspect_value );
		}

		// reset this incase.
		if( ( g_sdk.get_key( key_code::End )
			|| !options::friend_system )
			&& !g_esp.m_friends.empty( ) )
			g_esp.m_friends.clear( );

		g_cheat.cam_pos = g_sdk.get_camera_pos( );

		g_render.ddraw_get( );

		if( !this_player->get_life_state( ) ) {
			float fake_lag = options::aimbot::exploits::time::fake_lag_amount;

			bool has_fake_lag = options::aimbot::exploits::time::fake_lag 
				&& ( g_sdk.get_key( options::aimbot::exploits::time::fake_lag_key ) || options::aimbot::exploits::time::fake_lag_on );

			bool manipulation_activated = g_sdk.get_key( options::aimbot::manipulation_key ) && options::aimbot::manipulation
				&& g_aimbot.target;

			if( manipulation_activated )
				this_player->set_clientTickInterval( 1.f );
			else {
				if( has_fake_lag )
					this_player->set_clientTickInterval( fake_lag );
				else
					this_player->set_clientTickInterval( 0.05f );
			}

			float desync_time = ( g_sdk.realtimeSinceStartup( ) - this_player->lastSentTickTime( ) ) - 0.03125f * 3.f;

			float num = 1.5f;
			float num2 = g_esp.eye_clientframes / 60.f;
   			float num3 = g_esp.eye_serverframes * max( g_sdk.get_deltaTime( ), max( g_sdk.get_smooth_delta_time( ), g_sdk.get_fixed_delta_time( ) ) );
			float num4 = ( desync_time + num2 + num3 ) * num;
			float num5 = this_player->get_max_speed( ) + this_player->get_parent_velocity( ).length( );

			float max_dist_allowed = this_player->get_bounds_padding( ) + num4 * num5;

			g_esp.maximum_eye_height = max_dist_allowed;
			g_cheat.current_lag = this_player->get_clientTickInterval( );

			item* weapon = this_player->get_held_item( );
			
			model_state* model_state = this_player->get_modelstate( );
			
			player_walk_movement* movement = this_player->get_movement( );
			player_input* input = this_player->get_input( );
			player_eyes* eyes = this_player->get_eyes( );
			if( eyes )
				g_cheat.local_eyes = eyes;

			base_player* target = g_aimbot.target;
			if( target ) {
				vec3_t head_pos = target->get_bone_position( head );

				vec3_t eye = g_esp.eye_manipulation;
				if( eye.is_zero( )
					&& eyes )
					eye = eyes->get_position( );

				if( !eye.is_zero( )
					&& options::aimbot::bullet_tp ) {
					if ( !g_sdk.is_visible( head_pos, eye ) ) {
						if ( g_sdk.is_visible( eye, head_pos + vec3_t( 0, 1.30f, 0 ) ) )
							g_esp.bullet_tp_target = head_pos + vec3_t( 0, 1.30f, 0 );

						if ( !g_sdk.is_visible( eye, head_pos + vec3_t( 0, 1.30f, 0 ) ) 
							&& g_sdk.is_visible( eye, head_pos + vec3_t( 1.30f, 0, 0 ) ) )
							g_esp.bullet_tp_target = head_pos + vec3_t( 1.30f, 0, 0 );

						if ( !g_sdk.is_visible( eye, head_pos + vec3_t( 1.30f, 0, 0 ) ) 
							&& g_sdk.is_visible( eye, head_pos + vec3_t( -1.30f, 0, 0 ) ) )
							g_esp.bullet_tp_target = head_pos + vec3_t( -1.30f, 0, 0 );

						if ( !g_sdk.is_visible( eye, head_pos + vec3_t( -1.30f, 0, 0 ) )
							&& g_sdk.is_visible( eye, head_pos + vec3_t( 0, 0, 1.30f ) ) )
							g_esp.bullet_tp_target = head_pos + vec3_t( 0, 0, 1.30f );

						if ( !g_sdk.is_visible( eye, head_pos + vec3_t( 0, 0, 1.30f ) )
							&& g_sdk.is_visible( eye, head_pos + vec3_t( 0, 0, -1.30f ) ) )
							g_esp.bullet_tp_target = head_pos + vec3_t( 0, 0, -1.30f );
					}
					else
						g_esp.bullet_tp_target.clear( );
				}
				else
					g_esp.bullet_tp_target.clear( );

				if( manipulation_activated )
					g_esp.find_manipulate_angle( );
				else
					g_esp.manipulation_angle.clear( );
			}
			else
				g_esp.manipulation_angle.clear( );

			g_esp.render_saved_positions( );

			if( eyes
				&& model_state
				&& movement ) {
				static bool has_debug_camera = false;
				static float x_value{ }, y_value{ }, w_value{ };

				if( options::aimbot::exploits::misc::debug_camera ) {
					static int delay = 0;

					if( g_sdk.get_key( options::aimbot::exploits::misc::debug_camera_key )
						&& --delay < 25 ) {
						has_debug_camera = !has_debug_camera;
						delay = 25;
					}

					static vec4_t rotation_before_debug = vec4_t( );
					static vec2_t view_before_debug = vec2_t( );

					if( has_debug_camera ) {
						bool has_pressed_left = g_sdk.get_key( key_code::A );

						bool has_pressed_right = g_sdk.get_key( key_code::D );

						bool has_pressed_down = g_sdk.get_key( key_code::S );

						bool has_pressed_up = g_sdk.get_key( key_code::W );

						bool has_pressed_shift = g_sdk.get_key( key_code::LeftShift );

						bool has_pressed_space = g_sdk.get_key( key_code::Space );

						//bool has_pressed_h = g_sdk.get_key( key_code::H );

						//bool has_pressed_k = g_sdk.get_key( key_code::K );

						if( has_pressed_left )
							x_value -= 0.25f;
						else if( has_pressed_right )
							x_value += 0.25f;

						if( has_pressed_down )
							w_value -= 0.25f;
						else if( has_pressed_up )
							w_value += 0.25f;

						if( has_pressed_shift )
							y_value -= 0.25f;
						else if( has_pressed_space )
							y_value += 0.25f;

						if( !rotation_before_debug.is_zero( ) )
							eyes->set_body_rotation( rotation_before_debug );

						if( input ) {
							if( !view_before_debug.is_zero( ) )
								input->set_view_angles( view_before_debug );
						}

						if( movement ) {
							movement->set_ground_time( 1.f );
							movement->set_jump_time( 0.f );
							movement->set_land_time( 0.f );
							movement->set_targetmovement( vec3_t( ) );
						}

						eyes->set_view_offset( vec3_t( x_value, y_value, w_value ) );
					}
					else {
						rotation_before_debug = eyes->get_body_rotation( );
						if( input )
							view_before_debug = input->get_view_angles( );

						x_value = w_value = y_value = 0; 
					}
				}
				else
					has_debug_camera = false;

				if( !has_debug_camera ) {
					bool has_up_view_offset = options::aimbot::exploits::misc::long_neck
						&& g_sdk.get_key( options::aimbot::exploits::misc::view_height );

					bool has_right_view_offset = options::aimbot::exploits::misc::long_neck_right
						&& g_sdk.get_key( options::aimbot::exploits::misc::view_right );

					bool has_left_view_offset = options::aimbot::exploits::misc::long_neck_left
						&& g_sdk.get_key( options::aimbot::exploits::misc::view_left );

					float height_view_offset = ( options::aimbot::exploits::misc::long_neck_height );
					if( options::aimbot::exploits::misc::use_max_view )
						height_view_offset = g_esp.maximum_eye_height;

					float left_view_offset = ( options::aimbot::exploits::misc::long_neck_left_amount );

					float right_view_offset = ( options::aimbot::exploits::misc::long_neck_right_amount );

					if( has_up_view_offset	
						&& ( has_left_view_offset || has_right_view_offset ) )
					{
						// we need this so shots are registered.
						this_player->set_clientTickInterval( fake_lag < 1.f ? 1.f : fake_lag );

						eyes->set_view_offset( vec3_t( has_left_view_offset ? -left_view_offset : right_view_offset, height_view_offset, 0 ) );
					}
					else if( has_up_view_offset )
					{
						if( options::aimbot::exploits::misc::use_max_view )
							this_player->set_clientTickInterval( 1.f );
						else {
							if( height_view_offset >= 0.4f )
								this_player->set_clientTickInterval( fake_lag * ( height_view_offset - 0.5f ) );
						}

						eyes->set_view_offset( vec3_t( 0, height_view_offset, 0 ) );
					}
					else if( has_right_view_offset )
					{
						if( right_view_offset >= 0.4f )
							this_player->set_clientTickInterval( fake_lag * ( right_view_offset - 0.5f ) );

						eyes->set_view_offset( vec3_t( right_view_offset, 0, 0 ) );
					}
					else if( has_left_view_offset )
					{
						if( left_view_offset >= 0.4f )
							this_player->set_clientTickInterval( fake_lag * ( left_view_offset - 0.5f ) );

						eyes->set_view_offset( vec3_t( -left_view_offset, 0, 0 ) );
					}
					else {
						if( !has_fake_lag )
							this_player->set_clientTickInterval( 0.05f );
					}
				}
			}

			if( weapon )
			{
				g_esp.melee_max_dist = weapon->get_attack_max_distance( ) + 2.f;

				if( options::aimbot::kill_aura ) {
					vec3_t pos = reinterpret_cast< base_player* >( g_aimbot.target )->get_bone_position( head );

					if( pos.distance( g_cheat.local_pos ) < g_esp.melee_max_dist )
						g_esp.do_melee_attack( pos, g_aimbot.target, true );
				}

				if( options::aimbot::exploits::misc::auto_farm_ores ) {
					if( options::aimbot::exploits::misc::auto_farm_only_hotspot ) {
						base_entity* hotspot = g_esp.get_closest_object_of_class( xs( "OreHotSpot" ), g_esp.melee_max_dist );
						if( hotspot ) {
							transform* transform = hotspot->get_transform( );
							if( transform ) {
								vec3_t pos = transform->get_position( );

								g_esp.do_melee_attack( pos, hotspot, false );
							}
						}
					}
					else {
						base_entity* ore = g_esp.get_closest_object_of_class( xs( "OreResourceEntity" ), g_esp.melee_max_dist );
						if( ore ) {
							transform* transform = ore->get_transform( );
							if( transform ) {
								vec3_t pos = transform->get_position( );

								// increase the height a bit.
								pos.y += 0.25f;

								g_esp.do_melee_attack( pos, ore, false );
							}
						}
					}
				}

				if( eyes ) {
					vec3_t eye_pos = eyes->get_position( );
					if( options::aimbot::exploits::misc::auto_farm_trees ) {
						if( options::aimbot::exploits::misc::auto_farm_only_hotspot ) {
							base_entity* marker = g_esp.get_closest_object_of_class( xs( "TreeMarker" ), g_esp.melee_max_dist );
							if( marker ) {
								transform* transform = marker->get_transform( );
								if( transform ) {
									vec3_t pos = transform->get_position( );
									pos.y = eye_pos.y;

									g_esp.do_melee_attack( pos, marker, false );
								}
							}
						}
						else {
							base_entity* tree = g_esp.get_closest_object_of_class( xs( "TreeEntity" ), g_esp.melee_max_dist );
							if( tree ) {
								transform* transform = tree->get_transform( );
								if( transform ) {
									vec3_t pos = transform->get_position( );
									pos.y = eye_pos.y;

									g_esp.do_melee_attack( pos, tree, false );
								}
							}
						}
					}
				}

				base_view_model* view = base_view_model::get_view( );
				if( view ) {
					viewmodel_bob* bob = view->get_bob( );

					viewmodel_lower* lower = view->get_lower( );

					if( bob
						&& options::aimbot::exploits::misc::no_viewmodel_bob ) {
						bob->set_bob_amount_run( 0.f );
						bob->set_bob_amount_walk( 0.f );
						bob->set_bob_speed_run( 0.f );
						bob->set_bob_speed_walk( 0.f );
					}

					if( lower
						&& options::aimbot::exploits::misc::no_lower )
						lower->set_should_lower( false );
				}

				//base_projectile* proj = reinterpret_cast< base_projectile* >( held_weapon );

				//if( proj ) {
				//	std::cout << "ayo" << std::endl;
				//	if( g_sdk.get_key( key_code::N ) ) {
				//		base_projectile::Magazine* magazine = proj->get_primaryMagazine( );
				//		if( magazine ) {
				//			proj->launch_projectile( );
				//			magazine->contents( )--;
				//			proj->update_ammo_display( );
				//			proj->shot_fired( );
				//			proj->did_attack_clientside( );
				//		}
				//	}
				//}

				if( options::aimbot::auto_fire
					&& ( /*g_esp.manipulator_ready &&*/ !g_esp.manipulation_angle.is_zero( ) ) ) {

					if( !weapon->is_melee( ) )
						auto_fire( weapon, this_player );
					else {
						if( !options::aimbot::ignore_melee ) {
							if( weapon->can_throw_as_projectile( ) )
								weapon->do_throw( );
						}
					}
				}

				handle_exploits( this_player, weapon );
				handle_rpc_exploits( this_player, weapon );
			}

			if( ( g_sdk.get_key( options::aimbot::exploits::misc::pickup_key ) || options::aimbot::exploits::misc::always_pickup ) ) {
				if( options::aimbot::exploits::misc::auto_pickup_items ) {
					base_entity* entity = g_esp.get_closest_object_of_class( xs( "DroppedItem" ), options::aimbot::exploits::misc::max_item_distance );
					if( entity ) {
						bool can_pickup = false;

						item* this_item = entity->get_world_item( );
						if( this_item ) {
							std::string text = g_sdk.ws2s( this_item->get_weapon_name( ) );
							if( options::aimbot::exploits::misc::pickup_everything )
								can_pickup = true;

							if( this_item->is_gun( ) )
								can_pickup = true;
							else if( this_item->is_melee( ) )
								can_pickup = true;
							else if( this_item->is_heal( ) )
								can_pickup = true;
							else if( text.find( "scrap" ) != std::string::npos )
								can_pickup = true;
							else if( text.find( "ammo" ) != std::string::npos ) {
								if( text.find( "incendiary" ) != std::string::npos )
									can_pickup = true;
								else if( text.find( "hv" ) != std::string::npos )
									can_pickup = true;
								else if( text.find( "explosive" ) != std::string::npos )
									can_pickup = true;
								else 
									can_pickup = true;
							}
						}
						if( can_pickup )
							entity->server_rpc( xs( L"Pickup" ) );
					}
				}
				if( options::aimbot::exploits::misc::auto_pickup ) {
					base_entity* collectible = g_esp.get_closest_object_of_class( xs( "CollectibleEntity" ), options::aimbot::exploits::misc::max_collectible_distance );
					if( collectible )
						collectible->server_rpc( xs( L"Pickup" ) );
				}
			}
			if( options::aimbot::exploits::misc::auto_grade
				&& ( g_sdk.get_key( options::aimbot::exploits::misc::grade_key ) || options::aimbot::exploits::misc::always_grade ) ) {
				building_block* build_block = ( building_block* )g_esp.get_closest_object_of_class( xs( "BuildingBlock" ), options::aimbot::exploits::misc::max_grade_distance );
				if( build_block )
					if( build_block->can_upgrade( options::aimbot::exploits::misc::grade_tier, 0, this_player ) ) {
						if( build_block->can_afford( options::aimbot::exploits::misc::grade_tier, 0, this_player ) )
							build_block->upgrade( options::aimbot::exploits::misc::grade_tier, 0, this_player );
					}
			}
			if( options::aimbot::exploits::misc::auto_codelock 
				&& ( g_sdk.get_key( options::aimbot::exploits::misc::codelock_key ) || options::aimbot::exploits::misc::always_pickup ) ) {
				auto code = std::to_wstring( options::aimbot::exploits::misc::code_lock_code );

				base_entity* code_lock = g_esp.get_closest_object_of_class( xs( "CodeLock" ), options::aimbot::exploits::misc::max_lock_distance );
				if( code_lock ) {
					this_player->server_rpc( ( uintptr_t )code_lock, xs( L"RPC_ChangeCode" ), code.c_str( ), false );
					this_player->server_rpc( ( uintptr_t )code_lock, xs( L"TryLock" ) );
					this_player->server_rpc( ( uintptr_t )code_lock, xs( L"RPC_Lock" ) );
				}
			}
		}

		spoof_ret( orig_baseplayer_client_input, this_player, state );

		g_cheat.global_time = g_sdk.get_time( );

		if( this_player )
		{
			if( options::aimbot::exploits::misc::gesture_spam ) {
				switch( options::aimbot::exploits::misc::gesture_type ) {
				case 0:
					this_player->send_signal_broadcast( Signal::Gesture, xs( L"clap" ) );
					break;
				case 1:
					this_player->send_signal_broadcast( Signal::Gesture, xs( L"friendly" ) );
					break;
				case 2:
					this_player->send_signal_broadcast( Signal::Gesture, xs( L"thumbsdown" ) );
					break;
				case 3:
					this_player->send_signal_broadcast( Signal::Gesture, xs( L"thumbsup" ) );
					break;
				case 4:
					this_player->send_signal_broadcast( Signal::Gesture, xs( L"ok" ) );
					break;
				case 5:
					this_player->send_signal_broadcast( Signal::Gesture, xs( L"point" ) );
					break;
				case 6:
					this_player->send_signal_broadcast( Signal::Gesture, xs( L"shrug" ) );
					break;
				case 7:
					this_player->send_signal_broadcast( Signal::Gesture, xs( L"victory" ) );
					break;
				case 8:
					this_player->send_signal_broadcast( Signal::Gesture, xs( L"wave" ) );
					break;
				case 9:
					this_player->send_signal_broadcast( Signal::Gesture, xs( L"dance.cabbagepatch" ) );
					break;
				case 10:
					this_player->send_signal_broadcast( Signal::Gesture, xs( L"dance.twist" ) );
					break;
				}
			}

			if( options::aimbot::exploits::misc::spin_bot )
			{
				vec3_t angle{ };
				angle.x = std::sinf( ( g_sdk.system_time( ) * 180.f ) * 3.14f ) * 180.f; // this will be our yaw.
				angle.y = std::sinf( ( g_sdk.system_time( ) * 360.f ) * 3.14f ) * 360.f; // pitch
				angle.z = std::sinf( ( g_sdk.system_time( ) * 90.f ) * 3.14f ) * 90.f; // roll

				input_message* input_message = state->get_input_message( );
				if( input_message )
					input_message->set_aim_angles( angle );
			}

			if( options::aimbot::exploits::misc::fake_admin )
				this_player->add_flag( player_flags::is_admin );
		}
	}

	void fix_culling_hook( base_player* baseplayer, float dist, bool visibility )
	{
		auto orig_fix_culling = reinterpret_cast< void( * )( base_player*, float, bool ) >( fix_culling );
		if( options::visuals::chams )
			return spoof_ret( orig_fix_culling, baseplayer, 150.f, true );

		spoof_ret( orig_fix_culling, baseplayer, dist, visibility );
	}

	void on_attacked_hook( void* instance, hit_info* this_hit_info )
	{
		auto orig_on_attacked = reinterpret_cast< void( * )( void*, hit_info* ) >( on_attacked );

		base_entity* hit_entity = this_hit_info->get_hit_entity( );

		base_player* local = g_cheat.local;
        if( !local )
			return spoof_ret( orig_on_attacked, instance, this_hit_info );

		player_eyes* eyes = g_cheat.local_eyes;
        if( !eyes )
            return spoof_ret( orig_on_attacked, instance, this_hit_info );

        vec3_t head_pos = local->get_bone_position( head );

		bool has_up_view_offset = options::aimbot::exploits::misc::long_neck
			&& g_sdk.get_key( options::aimbot::exploits::misc::view_height );

		bool has_right_view_offset = options::aimbot::exploits::misc::long_neck_right
			&& g_sdk.get_key( options::aimbot::exploits::misc::view_right );

		bool has_left_view_offset = options::aimbot::exploits::misc::long_neck_left
			&& g_sdk.get_key( options::aimbot::exploits::misc::view_left );

		float height_view_offset = ( options::aimbot::exploits::misc::long_neck_height / 100.f );

		float left_view_offset = ( options::aimbot::exploits::misc::long_neck_left_amount / 100.f );

		float right_view_offset = ( options::aimbot::exploits::misc::long_neck_right_amount / 100.f );

		if( !g_esp.manipulation_angle.is_zero( ) )
			head_pos += g_esp.manipulation_angle;
		else {
			if( g_sdk.get_key( options::aimbot::exploits::misc::view_height ) )
				head_pos.y += height_view_offset;
		
			if( g_sdk.get_key( options::aimbot::exploits::misc::view_left ) )
				head_pos.x -= left_view_offset;
			else if( g_sdk.get_key( options::aimbot::exploits::misc::view_right ) )
				head_pos.x += right_view_offset;
		}

		if( hit_entity ) {
			g_esp.m_shots.emplace_back( c_esp::shot_record_t( g_sdk.get_time( ), head_pos, this_hit_info->get_hit_end( ), 
				reinterpret_cast< base_player* >( hit_entity ) ) );

			if( options::aimbot::hit_sound ) {
				DWORD file_size;
				BYTE* file_bytes;

				std::string sound_directory = std::string( );
				switch( options::aimbot::sound_type ) {
				case 0:
					sound_directory = xs( "C:\\plop.wav" );
					break;
				case 1:
					sound_directory = xs( "C:\\plop1.wav" );
					break;
				case 2:
					sound_directory = xs( "C:\\bell.wav" );
					break;
				case 3:
					sound_directory = xs( "C:\\bepis.wav" );
					break;
				case 4:
					sound_directory = xs( "C:\\fatality.wav" );
					break;
				case 5:
					sound_directory = xs( "C:\\phonk.wav" );
					break;
				case 6:
					sound_directory = xs( "C:\\sparkle.wav" );
					break;
				case 7:
					sound_directory = xs( "C:\\cock.wav" );
					break;
				case 8:
					sound_directory = xs( "C:\\bameware.wav" );
					break;
				case 9:
					sound_directory = xs( "C:\\dopium.wav" );
					break;
				}
				read_wav_to_file_memory( sound_directory, &file_bytes, &file_size );
				if( file_bytes )
					play_sound_from_memory( ( uint8_t* )file_bytes, file_size, options::aimbot::hit_sound_volume / 100.f );
			}
		}

		spoof_ret( orig_on_attacked, instance, this_hit_info );
	}
}

namespace item_icon_hk {
	void try_to_move_hook( item_icon* this_item )
	{
		auto orig_try_to_move = reinterpret_cast< void( * )( item_icon* ) >( try_to_move );

		if( options::aimbot::exploits::time::fast_loot ) {
			if( this_item->queued_for_looting( ) )
				this_item->run_timed_action( );
		}

		spoof_ret( orig_try_to_move, this_item );
	}
}