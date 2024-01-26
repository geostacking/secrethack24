#include "movement_related.hpp"
#include "../features/visuals/esp.hpp"
#include "../features/aimbot/aimbot.hpp"

namespace _player_walk_movement {
	void client_input_hook( player_walk_movement* self, void* input, model_state* state ) {
		auto orig_player_walk_movement_client_input = reinterpret_cast<void( * )( player_walk_movement*, void*, model_state* )>( client_input );

		spoof_ret( orig_player_walk_movement_client_input, self, input, state );

		g_esp.apply_fly_violation( );

		state->remove_flag( model_state_flags::in_fly );
		if( options::aimbot::exploits::misc::interactive_debug )
			state->add_flag( model_state_flags::is_mounted );

		if( options::aimbot::exploits::movement::omni_sprint ) {
			state->add_flag( model_state_flags::in_sprint );
			state->set_sprinting( true );

			if( g_sdk.get_key( options::aimbot::exploits::movement::omni_sprint_key ) )
				g_sdk.set_timescale( options::aimbot::exploits::movement::omni_sprint_speed / 100.f );
			else
				g_sdk.set_timescale( 1.f );
		}
		else
			g_sdk.set_timescale( 1.f );

		if( options::aimbot::exploits::movement::silent_walk
			&& g_sdk.get_key( key_code::LeftShift ) ) {
			state->set_onLadder( true );
			state->add_flag( model_state_flags::on_ladder );
		}

		if( options::aimbot::exploits::combat::modify_can_attack )
		{
			state->add_flag( model_state_flags::on_ground );
			self->set_is_grounded( 1.f );
		}

		if( self->flying( )
			|| !g_esp.not_in_fly_action )
			state->add_flag( model_state_flags::in_sprint );

		if( options::visuals::world::night_stars )
			state->add_flag( model_state_flags::asleep );

		if( options::aimbot::exploits::movement::climb_assist ) {
			self->set_ground_angle_new( 0.f );
		}

		bool has_triggered_stopper[2]{ false };
		has_triggered_stopper[0] = g_esp.current_vertical_fly >= g_esp.max_vertical_fly;
		has_triggered_stopper[1] = g_esp.current_horizontal_fly >= g_esp.max_horizontal_fly;

		float speed = options::aimbot::exploits::movement::fly_speed / 100.f;

		player_eyes* eyes = g_cheat.local_eyes;
		if( eyes ) {
			vec3_t movement_direction = vec3_t( );

			// get movement direction..
			{
				vec4_t rotation = eyes->get_body_rotation( );
				if( !rotation.is_zero( ) ) {
					const vec3_t vecright = vec3_t( 1.f, 0.f, 0.f );
					const vec3_t vecforward = vec3_t( 0.f, 0.f, 1.f );
					const vec3_t vecup = vec3_t( 0.f, 1.f, 0.f );

					if( g_sdk.get_key( key_code::W ) )
						movement_direction += g_math.quatmult( &vecforward, &rotation );
					if( g_sdk.get_key( key_code::S ) )
						movement_direction -= g_math.quatmult( &vecforward, &rotation );
					if( g_sdk.get_key( key_code::A ) )
						movement_direction -= g_math.quatmult( &vecright, &rotation );
					if( g_sdk.get_key( key_code::D ) )
						movement_direction += g_math.quatmult( &vecright, &rotation );
					if( g_sdk.get_key( key_code::Space ) )
						movement_direction.y += 1.f;
					if( g_sdk.get_key( key_code::LeftShift ) )
						movement_direction.y -= 1.f;
				}
			}

			if( options::aimbot::exploits::movement::fly_hack ) {
				bool is_moving = g_sdk.get_key( key_code::W ) || g_sdk.get_key( key_code::A ) ||
					g_sdk.get_key( key_code::S ) || g_sdk.get_key( key_code::D ) || g_sdk.get_key( key_code::Space );

				if( g_sdk.get_key( options::aimbot::exploits::movement::fly_key ) ) {
					g_esp.not_in_fly_action = false;

					self->set_flying( true );
					self->set_capsule_center( -1000.f );

					if( !movement_direction.is_zero( ) )
						self->set_targetmovement( movement_direction * speed );
				}
				else {
					self->set_capsule_center( 0.9f );
					self->set_flying( false );
					g_esp.not_in_fly_action = true;
				}
			}

			if( ( has_triggered_stopper[ 0 ] || has_triggered_stopper[ 1 ] ) )
				g_esp.not_in_fly_action = false;

			base_player* local = g_cheat.local;
			if( local ) {
				if( options::aimbot::exploits::movement::stopper_fly
					&& !g_esp.not_in_fly_action
					&& !g_sdk.get_key( options::aimbot::exploits::movement::ignore_key ) )
				{
					vec3_t current = g_cheat.local_pos;

					vec3_t previous_position = current;

					player_ticks* last_sent_tick = local->get_lastSentTick( );

					if( last_sent_tick ) {
						// no fall damage when teleporting.
						self->set_was_falling( true );
						self->set_previous_velocity( vec3_t( ) );

						vec3_t old = last_sent_tick->get_position( );

						float height = ( current.y < old.y ? current.y : old.y ) - 0.75f;

						if( has_triggered_stopper[0] )
							previous_position = vec3_t( previous_position.x,
								height, previous_position.z );
						else if( has_triggered_stopper[1] )
							previous_position = vec3_t( old.x, height, old.z );

						self->set_targetmovement( vec3_t( ) );

						if( !previous_position.is_zero( ) )
							self->teleport_to( previous_position );// teleport to our previous position.
					}
				}
			}
		}

		if( g_esp.not_in_fly_action ) {
			if( options::aimbot::exploits::misc::suicide
				&& g_sdk.get_key( options::aimbot::exploits::misc::suicide_key ) ) {
				self->set_was_falling( true );
				self->set_previous_velocity( vec3_t( 0, -1000, 0 ) );
				self->set_ground_time( 0.f );
			}
			else {
				if( options::aimbot::exploits::misc::disable_land_damage )
				{
					self->set_was_falling( true );
					self->set_previous_velocity( vec3_t( ) );
				}
			}

			auto next_ground_time = self->get_ground_time( ) + 0.5f;

			if( options::aimbot::exploits::movement::infinite_jump )
			{
				self->set_jump_time( 0.f );
				self->set_ground_time( next_ground_time );
				self->set_land_time( 0.f );
			}
			self->set_gravity_multiplier( options::aimbot::exploits::movement::high_jump ? options::aimbot::exploits::movement::jump_height / 100.f : 1.f );
		}

		if( options::aimbot::exploits::movement::always_sprint ) {
			state->set_sprinting( true );

			vec3_t vel = self->get_targetmovement( );

			float max_speed = ( self->swimming( ) || state->get_ducked( ) ) ? 1.7f : 5.5f;

			if( vel.length( ) > 0.f ) {
				vec3_t target_vel = vec3_t( vel.x / vel.length( ) * max_speed, vel.y, vel.z / vel.length( ) * max_speed );
				self->set_targetmovement( target_vel );
			}
		}

		if( options::aimbot::exploits::movement::increase_height ) {
			if( g_sdk.get_key( options::aimbot::exploits::movement::increase_height_key ) )
				self->set_capsule_height( options::aimbot::exploits::movement::height_amount );
			else
				self->set_capsule_height( 1.8f );
		}

		if( options::aimbot::exploits::combat::head_teleportation
			&& g_sdk.get_key( options::aimbot::exploits::combat::teleport_key ) ) {
			base_player* target = g_aimbot.target;
			if( target ) {
				player_model* model = target->get_model( );
				if( model ) {
					vec3_t target_pos = model->get_position( );
					if( !target_pos.is_zero( )
						&& target_pos.distance( g_cheat.local_pos ) < 5.f )
						self->teleport_to( vec3_t( target_pos.x, target_pos.y + 1.f, target_pos.z ) );
				}
			}
		}
	}
}