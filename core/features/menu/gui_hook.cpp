#include "gui_hook.hpp"
#include "../../includes/hinclude.hpp"
#include "../visuals/esp.hpp"
#include "../../game/offsets.hpp"
#include "../options.hpp"
#include "../aimbot/aimbot.hpp"
#include "framework/gui_framework.hpp"
#include <filesystem>
#include <shlobj.h>
#include "menu_tabs.hpp"

void entity_loop( )
{
	std::vector< std::pair< base_player*, entity_type > > player_list;
	
	event_type event_type = g_sdk.get_event_type( g_sdk.get_current( ) );

	auto clear_list = [ & ]( bool get_client_entities = false, bool reset_local = false ) -> void
	{
		if( get_client_entities )
			g_cheat.client_entities = g_il2mgr.value( xs( "BaseNetworkable" ), xs( "clientEntities" ), false );

		if( reset_local ) {
			g_cheat.local = nullptr;
			g_cheat.local_eyes = nullptr;
		}
			
		/* fix for re-joining servers xd */
		player_list.clear( );
		g_cheat.player_list.clear( );
		g_cheat.local_pos.clear( );
		g_cheat.cam_pos.clear( );
	};

	if( !g_cheat.client_entities )
		return clear_list( true, true );

	uintptr_t entity_realm = *reinterpret_cast< uintptr_t* >( g_cheat.client_entities + 0x10 );
	if( !entity_realm )
		return clear_list( true, true );

	uintptr_t buffer_list = *reinterpret_cast< uintptr_t* >( entity_realm + 0x28 );
	if( !buffer_list )
		return clear_list( false, true );

	uintptr_t object_list = *reinterpret_cast< uintptr_t* >( buffer_list + 0x18 );
	if( !object_list )
		return clear_list( false, true );

	int object_list_size = *reinterpret_cast< int* >( buffer_list + 0x10 );

	if( !g_cheat.local )
		clear_list( );

	for( int i = 0; i < object_list_size; i++ )
	{
		base_entity* current_object = *reinterpret_cast< base_entity** >( object_list + ( 0x20 + ( i * sizeof( uint64_t ) ) ) );
		if( !current_object )
			continue;

		uintptr_t base_object = *reinterpret_cast< uintptr_t* >( ( uintptr_t )current_object + 0x10 );
		if( !base_object )
			continue;

		uintptr_t object = *reinterpret_cast< uintptr_t* >( base_object + 0x30 );
		if( !object )
			continue;

		uintptr_t object_class = *reinterpret_cast< uintptr_t* >( object + 0x30 );
		if( !object_class )
			continue;

		uintptr_t entity = *reinterpret_cast< uintptr_t* >( object_class + 0x18 );
		if( !entity )
			continue;

		std::string class_name = current_object->get_class_name( );

		base_player* player = *reinterpret_cast< base_player** >( entity + 0x28 );
		if( player ) {
			if( i == 0 )
				continue;

			const uint16_t tag = *reinterpret_cast< uint16_t* >( object + 0x54 );
			if( tag == 6 )
			{
				static entity_type type = entity_type::player;

				if( ( class_name.length ( ) > 9
					&& ( class_name[ 9 ] == 'N' )
					|| ( class_name[ 0 ] == 'S' && class_name[ 1 ] == 'c' ) ) 
					|| ( class_name[ 0 ] == 'H' && class_name[ 1 ] == 'T' ) 
					|| ( class_name[ 0 ] == 'N' && class_name[ 3 ] == 'S' )
					|| ( class_name[ 0 ] == 'V' && class_name[ 7 ] == 'V' ) ) // Scientist
					type = scientist;
				else if( ( class_name[ 0 ] == 'T' && class_name[ 6 ] == 'D' ) ) // TunnelDweller
					type = tunnel_dweller;
				else 
					type = entity_type::player;

				player_list.emplace_back( std::make_pair( player, type ) );
			}
		}

		base_entity* base_ent = *reinterpret_cast< base_entity** >( base_object + 0x28 );
		if( !base_ent )
			continue;

		transform* transform = base_ent->get_transform( );
		if( !transform )
			continue;

		vec3_t position = transform->get_position( );
		if( position.is_zero( ) )
			continue;

		float distance = g_cheat.local_pos.distance( position );

		uintptr_t object_name_ptr = *reinterpret_cast< uintptr_t* >( object + 0x60 );
		if( !object_name_ptr )
			continue;

		std::string obj_name = g_sdk.read_ascii( object_name_ptr, 128 );

		vec2_t screen_pos = g_sdk.world_to_screen( position );
		if( screen_pos.is_zero( ) )
			continue;

		vec4_t render_position = vec4_t( screen_pos.x, screen_pos.y, 150, 55 );
			
		base_combat_entity* combat_entity = reinterpret_cast< base_combat_entity* >( current_object );

		g_world.render( render_position, g_sdk.s2ws( class_name ), g_sdk.s2ws( obj_name ), distance, combat_entity, event_type, base_ent );
	}

	base_player* local_ent = local_player::Entity( );
	if( !local_ent ) {
		g_cheat.local = nullptr;
		g_cheat.local_eyes = nullptr;
	}
	else
	{
		if( local_ent->has_flag( player_flags::is_connected ) )
			g_cheat.local = local_ent;
		else {
			g_cheat.local = nullptr;
			g_cheat.local_eyes = nullptr;
		}

		base_player* local = g_cheat.local;
		if( local ) {
			if( !local_ent->get_life_state( ) ) {
				player_model* model = local->get_model( );
				if( model )
					g_cheat.local_pos = model->get_position( );
			}
		}
	}

	g_cheat.player_list = player_list;
}

namespace perf_ui {
	void ongui_hook( )
	{
		g_cheat.mouse_pos = g_sdk.get_mouse_pos( );
		g_cheat.screen_size = vec2_t( g_sdk.get_screen_width( ), g_sdk.get_screen_height( ) ); // k to the y to the s
		if( !g_cheat.screen_size.is_zero( ) )
			g_cheat.screen_center = g_cheat.screen_size / 2;

		uintptr_t current_event = g_sdk.get_current( );
		if( !current_event )
			return;

		event_type event_type = g_sdk.get_event_type( current_event );
		if( event_type != event_type::re_paint )
			return;

		g_cheat.draw_skin = g_sdk.get_draw_skin( );
		if( !g_cheat.draw_label )
			g_cheat.draw_label = *reinterpret_cast< uintptr_t* >( g_cheat.draw_skin + 0x38 );
		else {
			g_sdk.set_draw_font_size( g_cheat.draw_label, 12 );
			g_sdk.set_draw_alignment( g_cheat.draw_label, 0x0 );
			g_sdk.set_draw_color( color_t( 255, 255, 255, 255 ) );
		}

		if( !g_cheat.circuit_chams_bundle )
			g_cheat.circuit_chams_bundle = g_sdk.load_bundle_file( xs( L"C:\\secrethack24\\circuit_chams_bundle.vmt" ) );

		if( !g_cheat.chams_bundle )
			g_cheat.chams_bundle = g_sdk.load_bundle_file( xs( L"C:\\secrethack24\\chams_bundle.vmt" ) );

		if( !g_cheat.wireframe_chams_bundle )
			g_cheat.wireframe_chams_bundle = g_sdk.load_bundle_file( xs( L"C:\\secrethack24\\wire_chams.vmt" ) );

		if( !g_cheat.asset_bundle )
			g_cheat.asset_bundle = g_sdk.load_bundle_file( xs( L"C:\\secrethack24\\chams.vmt" ) );

		if( !g_cheat.font_bundle )
			g_cheat.font_bundle = g_sdk.load_bundle_file( xs( L"C:\\secrethack24\\font_bundle.bndl" ) );
		else {
			static uintptr_t object = g_il2mgr.type_object( "UnityEngine", "Font" );

			uintptr_t font_name = g_sdk.load_asset( g_cheat.font_bundle, xs( L"ubuntu-medium.ttf" ), object );
			if( font_name )
				*reinterpret_cast< uintptr_t* >( g_cheat.draw_skin + 0x18 ) = font_name;
		}

		g_cheat.pulsation.rainbow( 5.f );
		g_cheat.rainbow = color_t( g_cheat.pulsation.r, 127 + g_cheat.pulsation.g * 0.25f, g_cheat.pulsation.g );

		if( options::rainbow_accent )
			options::accent_color = g_cheat.rainbow;

		// draw our menu.
		g_menu.draw_menu( );

		// set-up shader before draw.
		g_render.set_up_draw_shader( );

		std::string config_direction = std::filesystem::current_path( ).string( );

		static std::vector< std::string > cfg_files;

		cfg_files.clear( );
		for( const auto& entry : std::filesystem::directory_iterator( config_direction ) )
		{
			if( entry.is_regular_file( ) 
				&& entry.path( ).extension( ) == ".cfg" )
				cfg_files.push_back( entry.path( ).filename( ).string( ) );
		}

		if( options::cfg_index >= 0 ) {
			std::string file_name = cfg_files.at( options::cfg_index );

			if( options::load_cfg
				&& options::cfg_index < cfg_files.size( ) )
			{
				g_cfg.load_cfg( file_name );
			}

			if( options::save_cfg
				&& options::cfg_index < cfg_files.size( ) )
			{
				g_cfg.save_cfg( file_name );
			}
		}

		if( options::aimbot::exploits::misc::no_flash )
			g_game.set_flash_duration( 0.f );
		else
			g_game.set_flash_duration( 2.5f );

		entity_loop( );

		// esp font flags.
		{
			switch( options::font_flags )
			{
			case 1:
				g_esp.font_flags = font_flags_t::dropshadow;
				break;
			case 2:
				g_esp.font_flags = font_flags_t::outline;
				break;
			default:
				g_esp.font_flags = font_flags_t::none;
				break;
			}
		}

		auto render_water_mark =[ & ]( ) -> void // tf is that lambda:P
		{
			std::wstring total_text = xs( L"secret-hack24: RUST");

			g_render.outline_box( vec2_t( 2, 3 ), vec2_t( 110, 20 ), color_t( 48, 48, 48 ) );
			g_render.draw_filled_rect( vec4_t( 2, 3, 110, 20 ), color_t( 10, 10, 10 ) );
			g_render.draw_filled_rect( vec4_t( 2, 3, 2, 20 ), options::accent_color.alpha( 255.f, true ) );
			g_render.draw_text( vec4_t( 7, 4, 150, 20 ), total_text.c_str( ), color_t( 225, 225, 225 ), false, 11, g_esp.font_flags );
		};

		if( options::visuals::show_watermark )
			render_water_mark( );

		if( options::visuals::world::mie_changer )
			g_game.set_mie( options::visuals::world::mie_amount );
		if( options::visuals::world::rayleigh_changer )
			g_game.set_rayleigh( options::visuals::world::rayleigh_amount );
		if( options::visuals::world::modify_rain )
			g_game.set_rain( options::visuals::world::rain_amount );

		g_esp.change_sky_ambient_color( );
		base_player* local_player = g_cheat.local;

		if( !local_player )
			g_esp.reset_fly_hack( );

		if( local_player )
		{			
			// reset this.
			g_esp.not_in_fly_action = true;

			if( options::aimbot::exploits::movement::modify_clothing_speed )
				local_player->set_clothing_speed_reduce( -options::aimbot::exploits::movement::clothing_speed );

			if( options::aimbot::exploits::combat::unlock_aim_on_jugger )
				local_player->set_clothing_blocks_aiming( false );

			// spacing handle lol.
			{
				if( options::visuals::show_health_text 
					&& g_esp.esp_spacing[ 1 ] < 13 )
					g_esp.esp_spacing[ 1 ] += 13;
				else if( !options::visuals::show_health_text )
					g_esp.esp_spacing[ 1 ] = 0;
			}

			if( options::visuals::draw_radar )
			{
				float width = 200, height = 200;

				// check if we have menu open.
				if( g_framework.in_alpha ) {
					vec3_t cursor = g_cheat.mouse_pos;

					if( g_sdk.get_key( key_code::Mouse0 ) 
						&& g_framework.is_hovering( g_esp.radar_position, vec2_t( width, 25 ) ) )
					{
						cursor.y = g_cheat.screen_size.y - cursor.y;

						g_esp.radar_position.x = cursor.x - width / 2.f;
						g_esp.radar_position.y = cursor.y - 10.f;
					}
				}

				g_render.draw_filled_rect( vec4_t( g_esp.radar_position.x, g_esp.radar_position.y, width, height ), color_t( 0, 0, 0, 255 ) );
				g_render.outline_box( vec2_t( g_esp.radar_position.x, g_esp.radar_position.y ), vec2_t( width, height ), color_t( 48, 48, 48, 255 ) );
			}

			if( options::visuals::rgb_chams )
				options::visuals::chams_color = options::visuals::chams_visible = g_cheat.rainbow;

			for( int i = 0; i < g_cheat.player_list.size( ); i++ )
			{
				std::pair< base_player*, entity_type > player = g_cheat.player_list.at( i ); // wow that's so ugly
				if( !player.first )
					continue;

				g_esp.context_instance( player.first, player.second );
			}

			g_aimbot.process( );

			if( g_aimbot.target ) {
				if( options::visuals::show_hotbar )
				{
					g_esp.draw_hotbar( g_aimbot.target ); 
				}
				if( options::visuals::show_clothes )
				{
					g_esp.draw_clothes( g_aimbot.target );
				}
			}

			if( g_sdk.get_key( options::add_friend_key )
				&& options::friend_system )
				g_esp.m_friends.push_back( g_aimbot.target );

			model_state* model_ = local_player->get_modelstate( );
			player_walk_movement* movement = local_player->get_movement( );
			player_eyes* eyes = local_player->get_eyes( );

			float speed = options::aimbot::exploits::movement::fly_speed / 100.f;

			g_esp.show_reload_bar( ); // render reload bar BEFORE desync bar, so spacing works fine:p
			g_esp.show_desync_bar( );

			g_esp.draw_bullet_tracers( );

			//g_sdk.set_admin_ambient_mp( options::visuals::world::rayleigh_value );
			//g_sdk.set_admin_ref_mp( options::visuals::world::rayleigh_value );

			if( local_player->has_flag( player_flags::is_connected ) ) {
				if( options::visuals::local_trails )
					g_esp.draw_local_trails( );
				// ----------------------------- //
				if( options::visuals::show_last_sent_tick )
					g_esp.draw_last_sent_tick( );
			}

			int indicator_spacing = 0;

			if( options::visuals::indicators ) {
				if( options::visuals::fly_hack_indicator )
					g_esp.fly_hack_bar( );

				if( g_aimbot.target )
				{
					std::wstring text = xs( L"aimbot[t" );
					if( g_aimbot.is_in_aim )
						text += L"|b]";
					else
						text += L"]";

					g_render.draw_text( vec4_t( g_cheat.screen_center.x - 50, g_cheat.screen_center.y + ( 15 + 
						( g_esp.indicators_spacing[ 0 ] + g_esp.indicators_spacing[ 1 ] ) ), 150, 20 ),
						text.c_str( ), color_t( g_cheat.pulsation.r, 125, g_cheat.pulsation.b - 125.f ), true, 10, g_esp.font_flags );

					indicator_spacing += 10;
				}

				if( g_cheat.current_lag > 0.1f )
				{
					std::wstring text = xs( L"lag" );
					text += xs( L"(" );
					text += g_sdk.s2ws( g_math.remove_trailing_zeros( g_cheat.current_lag ) );
					text += xs( L"s)" );

					g_render.draw_text( vec4_t( g_cheat.screen_center.x - 50, g_cheat.screen_center.y + ( 15 + indicator_spacing 
						+ ( g_esp.indicators_spacing[ 0 ] + g_esp.indicators_spacing[ 1 ] ) ), 150, 20 ),
						text.c_str( ), g_cheat.rainbow, true, 10, g_esp.font_flags );

					indicator_spacing += 10;
				}

				if( !g_esp.not_in_fly_action )
				{
					std::wstring text = std::wstring( );
					bool unsafe[ 3 ]{ false };
					unsafe[ 1 ] = ( g_esp.current_vertical_fly - g_esp.max_vertical_fly ) > 2.f;
					unsafe[ 2 ] = ( g_esp.current_horizontal_fly - g_esp.max_horizontal_fly ) > 2.f;

					if( ( unsafe[ 1 ] ) )
						text += L"*";
					text += xs( L"noclipping" );
					if( ( unsafe[ 2 ] ) )
						text += L"*";

					color_t color = color_t( g_cheat.pulsation.r - 127, g_cheat.pulsation.g * 0.25f, g_cheat.pulsation.g * 0.25f );
					if( unsafe[ 1 ] || unsafe[ 2 ] )
						color = color_t( g_cheat.pulsation.r * g_cheat.pulsation.g * 0.1f, 127, 0 );

					g_render.draw_text( vec4_t( g_cheat.screen_center.x - 50, g_cheat.screen_center.y + ( 15 + indicator_spacing 
						+ ( g_esp.indicators_spacing[ 0 ] + g_esp.indicators_spacing[ 1 ] ) ), 150, 20 ),
						text.c_str( ), color, true, 10, g_esp.font_flags );

					indicator_spacing += 10;
				}

				if( !g_esp.manipulation_angle.is_zero( ) )
				{
					std::wstring text = std::wstring( );
					text += xs( L"manipulator" );
					if( g_esp.manipulator_ready )
						text += xs( L"[r]" );
					else
						text += xs( L"[c]" );

					color_t color = color_t( 255 / g_cheat.current_lag, 255 * g_cheat.current_lag, 255 - ( 125 * g_cheat.current_lag ) );

					g_render.draw_text( vec4_t( g_cheat.screen_center.x - 50, g_cheat.screen_center.y + ( 15 + indicator_spacing 
						+ ( g_esp.indicators_spacing[ 0 ] + g_esp.indicators_spacing[ 1 ] ) ), 150, 20 ),
						text.c_str( ), color, true, 10, g_esp.font_flags );
				}
			}

			g_game.set_underwater_blur( !options::visuals::world::clear_underwater );

			if( options::aimbot::exploits::misc::zoom_fov
				&& g_sdk.get_key( options::aimbot::exploits::misc::zoom_key ) )
				g_game.set_graphics_fov( options::aimbot::exploits::misc::zoom_fov_amount );
			else if( options::aimbot::exploits::misc::fov_changer )
				g_game.set_graphics_fov( options::aimbot::exploits::misc::fov_amount );
			else
				g_game.set_graphics_fov( 90.f );

			base_mountable* mountable = local_player->get_mountable( );

			if( mountable )
			{
				if( options::aimbot::exploits::combat::can_attack_in_vehicles )
					mountable->set_can_wield_item( true );

				if( options::aimbot::exploits::combat::unlock_view_angles )
					mountable->set_ignore_vehicle_parent( true );
			}

			g_sdk.ignore_layer_collision( layer::PlayerMovement, layer::Tree, options::aimbot::exploits::movement::walk_through_trees );
			g_sdk.ignore_layer_collision( layer::PlayerMovement, layer::AI, options::aimbot::exploits::movement::walk_through_players );
		}

		if( options::aimbot::draw_fov )
		{
			g_render.draw_circle( g_cheat.screen_center, options::aimbot::fov_amount, 
				options::aimbot::fov_circle_color );
		}

		if( options::aimbot::draw_crosshair ) {
			g_render.draw_line( vec2_t( g_cheat.screen_center.x - 7.f, g_cheat.screen_center.y ), 
				vec2_t( g_cheat.screen_center.x - 2.f, g_cheat.screen_center.y ), color_t( g_cheat.pulsation.r, g_cheat.pulsation.g, 255 ) ); // left middle

			g_render.draw_line( vec2_t( g_cheat.screen_center.x + 6.f, g_cheat.screen_center.y ),
				vec2_t( g_cheat.screen_center.x + 1.f, g_cheat.screen_center.y ), color_t( g_cheat.pulsation.r, ( 0.5f + g_cheat.pulsation.g * 0.25f ), g_cheat.pulsation.g ) ); // right middle

			g_render.draw_line( vec2_t( g_cheat.screen_center.x, g_cheat.screen_center.y - 7.f ),
				vec2_t( g_cheat.screen_center.x, g_cheat.screen_center.y - 2.f ), color_t( g_cheat.pulsation.b, 255, g_cheat.pulsation.b ) ); // middle down

			g_render.draw_line( vec2_t( g_cheat.screen_center.x, g_cheat.screen_center.y + 6.f ),
				vec2_t( g_cheat.screen_center.x, g_cheat.screen_center.y + 1.f ), color_t( g_cheat.pulsation.r, g_cheat.pulsation.g, g_cheat.pulsation.b ) ); // middle up
		}

		return; // yeah
	}

	void performance_ui_update_hook( void* instance )
	{
		if( first_loaded )
		{
			static uintptr_t game_object = methods::object_new( g_il2mgr.init_class( xs( "GameObject" ), xs( "UnityEngine" ) ) );
			g_sdk.create( game_object, xs( L"" ) );

			static uintptr_t dev_controls = g_il2mgr.type_object( xs( "" ), xs( "DevControls" ) );
			g_sdk.add_component( game_object, dev_controls );
			g_sdk.dont_destroy_on_load( game_object );

			first_loaded = false;
		}

		static auto update_performance_ui = reinterpret_cast< void( * )( void* ) >( performance_ui_update );
		spoof_ret( update_performance_ui, instance );
	}
}