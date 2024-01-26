#include <cmath>
#include "../aimbot/aimbot.hpp"

#include <Windows.h>
#include "../options.hpp"

#include "../../utilities/memory.hpp"
#include "../menu/framework/gui_framework.hpp"
#include "esp.hpp"

c_esp g_esp;

void c_esp::context_instance( base_player* player, entity_type type )
{
    player_model* model = player->get_model( );
    base_player* local = g_cheat.local;
    if( !local
        || player == local )
        return;

    bool npc = model->is_npc( );

	if( ( !options::visuals::draw_visuals
        || ( !options::visuals::can_show_sleepers 
		&& player->is_sleeping( ) ) 
		|| ( !options::visuals::can_show_knocked
		&& player->is_knocked( ) )
		|| ( !options::visuals::can_show_npc && npc )
		|| player->get_life_state( ) ) ) {
		return;
	}

    bool visible = player->is_visible( player->get_bone_position( head ) );

    if( npc ) {
        if( visible ) {
            name_color = options::visuals::npc_vis_name_color;
            box_color = options::visuals::npc_vis_box_color;

            skeleton_color = options::visuals::npc_vis_skeleton_color;
            radar_color = options::visuals::npc_vis_radar_color;
            oof_color = options::visuals::npc_vis_oof_color;
            snap_lines_color = options::visuals::npc_vis_snap_lines_color;
            distance_color = options::visuals::npc_vis_distance_color;
            weapon_color = options::visuals::npc_vis_weapon_color;
            view_direction_color = options::visuals::npc_vis_view_direction_color;
            knocked_color = options::visuals::npc_vis_knocked_color;
            ducking_color = options::visuals::npc_vis_ducking_color;
            custom_health_color = options::visuals::npc_vis_custom_health_color;
            health_text_color = options::visuals::npc_vis_health_text_color;
        }
        else {
            name_color = options::visuals::npc_name_color;
            box_color = options::visuals::npc_box_color;

            skeleton_color = options::visuals::npc_skeleton_color;
            radar_color = options::visuals::npc_radar_color;
            oof_color = options::visuals::npc_oof_color;
            snap_lines_color = options::visuals::npc_snap_lines_color;
            distance_color = options::visuals::npc_distance_color;
            weapon_color = options::visuals::npc_weapon_color;
            view_direction_color = options::visuals::npc_view_direction_color;
            knocked_color = options::visuals::npc_knocked_color;
            ducking_color = options::visuals::npc_ducking_color;
            custom_health_color = options::visuals::npc_custom_health_color;
            health_text_color = options::visuals::npc_health_text_color;
        }
    }
    else {
        if( visible ) {
            name_color = options::visuals::vis_name_color;
            box_color = options::visuals::vis_box_color;

            skeleton_color = options::visuals::vis_skeleton_color;
            radar_color = options::visuals::vis_radar_color;
            oof_color = options::visuals::vis_oof_color;
            snap_lines_color = options::visuals::vis_snap_lines_color;
            distance_color = options::visuals::vis_distance_color;
            weapon_color = options::visuals::vis_weapon_color;
            view_direction_color = options::visuals::vis_view_direction_color;
            knocked_color = options::visuals::vis_knocked_color;
            ducking_color = options::visuals::vis_ducking_color;
            custom_health_color = options::visuals::vis_custom_health_color;
            health_text_color = options::visuals::vis_health_text_color;
        }
        else {
            name_color = options::visuals::name_color;
            box_color = options::visuals::box_color;

            skeleton_color = options::visuals::skeleton_color;
            radar_color = options::visuals::radar_color;
            oof_color = options::visuals::oof_color;
            snap_lines_color = options::visuals::snap_lines_color;
            distance_color = options::visuals::distance_color;
            weapon_color = options::visuals::weapon_color;
            view_direction_color = options::visuals::view_direction_color;
            knocked_color = options::visuals::knocked_color;
            ducking_color = options::visuals::ducking_color;
            custom_health_color = options::visuals::custom_health_color;
            health_text_color = options::visuals::health_text_color;
        }
    }

	float dist_to = g_cheat.local_pos.distance( model->get_position( ) );
	if( dist_to > options::visuals::max_player_distance )
		return;

    bbox = get_bounds( player, 5.f );

    if( !bbox.on_screen ) {
        if( options::visuals::offscreen_arrows ) {
            draw_out_of_fov_arrows( player );
        }
    }
	else {
        if( options::visuals::show_name ) {
			draw_name( player, type );
		}
        if( options::visuals::box_type > 0 ) { 
			draw_box( player ); 
		}
        if( options::visuals::show_health )
		{
			draw_health( player );
		}
        if( options::visuals::draw_bones )
		{
			draw_bones( player );
		}
        if( options::visuals::show_lines 
            || options::aimbot::draw_target )
		{
			draw_snap_lines( player );
		}
        draw_view_direction( player );
        if( options::visuals::show_health_text )
        {
            draw_health_text( player );
        }
		draw_weapon( player ); // custom shit
		if( options::visuals::show_distance ) {
			draw_distance( player );
		}
        if( options::visuals::show_ducking_flag )
        {
            draw_ducking_flag( player );
        }
        if( options::visuals::show_knocked_flag )
        {
            draw_knocked_flag( player );
        }
        if( options::visuals::chams )
		{
			invoke_chams( player );
		}
	}

    if( options::visuals::draw_radar ) {
        draw_radar( player );
    }
}

void c_esp::draw_bullet_tracers( )
{
    if( !options::visuals::draw_tracers )
        return m_shots.clear( );

	if( m_shots.empty( ) )
		return;

    int font_size = 12;

    // keep this limited to 5 instances.
    while( m_shots.size( ) > 5 )
        m_shots.pop_back( );

    color_t color = options::visuals::tracers_color;

	for( size_t i = 0; i < m_shots.size( ); i++ )
	{
		auto& tr = m_shots[ i ];

        static float alpha = 0.f;

		float delta = g_sdk.get_time( ) - tr.m_time;
		if( ( delta > 1.0f
            || std::abs( delta ) > 3.f ) ) {

            alpha = std::lerp( alpha, 0.f, 0.025f );

            if( alpha < 0.1f )
			    m_shots.erase( m_shots.begin( ) + i );
        }
        else if( alpha < 1.f )
            alpha = std::lerp( alpha, 1.f, 0.025f );

        // measure alpha.
        color.a *= alpha;

        base_player* player = tr.entity;
        if( !player )
            continue;

        g_render.ddraw_line( tr.m_start, tr.m_end, color, 3.5f, false );
	}
}

vec2_t rotate_point( vec2_t to_rotate, vec2_t center, float angle )
{
    angle *= ( M_PI / 180.f );

    float ang_cos = cos( angle );
    float ang_sin = sin( angle );

    vec2_t to_return = vec2_t( );

    to_return.x = ( ( ang_cos * ( to_rotate.x - center.x ) - ang_sin * ( to_rotate.y - center.y ) ) + center.x );
    to_return.y = ( ( ang_sin * ( to_rotate.x - center.x ) + ang_cos * ( to_rotate.y - center.y ) ) + center.y );

    return to_return;
}

void c_esp::draw_local_trails( ) {
    base_player* local = g_cheat.local;
    if( !local )
        return;

    player_ticks* last_sent_tick = local->get_lastSentTick( );
    if( !last_sent_tick )
        return;

    g_render.ddraw_line( last_sent_tick->get_position( ), g_cheat.local_pos, options::visuals::trails_color, 2.5f, true, true );
}

void c_esp::draw_last_sent_tick( ) {
    base_player* local = g_cheat.local;
    if( !local )
        return;

    player_ticks* last_sent_tick = local->get_lastSentTick( );
    if( !last_sent_tick )
        return;

    g_render.ddraw_sphere( last_sent_tick->get_position( ), g_cheat.local_pos, color_t( 255, 0, 0 ), 2.5f, false );
}

void c_esp::draw_radar( base_player* player )
{
    vec2_t size = vec2_t( 4, 4 );
    float width = 200, height = 200;

    vec2_t screen_position = vec2_t( ), rotated_point = vec2_t( );
    vec2_t center = vec2_t( radar_position.x + ( width / 2 ), radar_position.y + 100 );
    vec2_t center_position = vec2_t( center.x, center.y );
        
    player_model* model = player->get_model( );
    if( !model )
        return;

    base_player* local = g_cheat.local;
    if( !local )
        return;

    player_input* input = local->get_input( );
    if( !input )
        return;

    vec3_t pos = model->get_position( );
    if( pos.is_zero( ) )
        return;

    vec2_t view_angles = input->get_view_angles( );
    if( view_angles.is_zero( ) )
        return;

    screen_position = vec2_t( ( -( g_cheat.local_pos.x - pos.x ) + center.x ),
        ( g_cheat.local_pos.z - pos.z ) + center.y );

    rotated_point = rotate_point( screen_position, center_position, ( 360.0f - view_angles.y ) );

    if( ( rotated_point.x < radar_position.x
        || rotated_point.y < radar_position.y
        || rotated_point.x > radar_position.x + ( width - size.x )
        || rotated_point.y > radar_position.y + ( height - size.y ) ) )
        return;

    g_render.draw_filled_rect( vec2_t( rotated_point.x, rotated_point.y ), vec2_t( 4, 4 ), radar_color );
    g_render.outline_box( vec2_t( rotated_point.x, rotated_point.y ), vec2_t( 4, 4 ), color_t( 10, 10, 10, 175 ) );

    g_render.draw_filled_rect( vec4_t( center.x, center.y, 4, 4 ), options::accent_color.alpha( 255, true ) );
    g_render.outline_box( vec2_t( center.x, center.y ), vec2_t( 4, 4 ), color_t( 10, 10, 10, 175 ) );
}

void c_esp::render_saved_positions( )
{
    base_player* local = g_cheat.local;
    if( !local )
        return;

    player_model* model = local->get_model( );
    if( !model )
        return;

    vec3_t local_pos = model->get_position( );
    if( local_pos.is_zero( ) )
        return;

    // make position higher than us.
    local_pos.y += 2;

    static int delay = 0;

    if( g_sdk.get_key_down( ( key_code )options::visuals::save_pos_key )
        && delay < 10 ) {
        delay = 10;
        m_positions.push_back( { options::visuals::is_home_pos, local_pos, options::visuals::pos_color } );
    }

    // remove delay by 1.
    --delay;

    if( g_sdk.get_key( options::visuals::remove_positions_key )
        && !m_positions.empty( ) )
        m_positions.pop_back( );

    for( int i = 0; i < m_positions.size( ); i++ ) {
        auto& pos = m_positions.at( i );
        if( pos.m_location.is_zero( ) )
            continue;

        vec2_t world_pos = g_sdk.world_to_screen( pos.m_location );
        if( world_pos.is_zero( ) )
            continue;

        std::wstring text = std::wstring( );

        if( pos.is_home )
            text = xs( L"home pos" );
        else
            text = xs( L"saved position" );

        text += xs( L"[" ) + std::to_wstring( i );
        text += xs( L"|" ) + std::to_wstring( static_cast< int >( g_cheat.local_pos.distance( pos.m_location ) ) ) + xs( L"m" );
        text += xs( L"]" );

        g_render.draw_text( vec4_t( world_pos.x, world_pos.y, 150, 20 ), text.c_str( ), pos.color, false, 12, font_flags );
    }
}

void c_esp::change_sky_ambient_color( ) {
    static uintptr_t sky = g_il2mgr.init_class( "TOD_Sky" );
    if( !sky )
        return;

	uintptr_t night = memory::read_chain< uintptr_t >( sky, { 0xB8, 0x0, 0x10, 0x20, 0x58 } );
	uintptr_t day = memory::read_chain< uintptr_t >( sky, { 0xB8, 0x0, 0x10, 0x20, 0x50 } );
	uintptr_t stars = memory::read_chain< uintptr_t >( sky, { 0xB8, 0x0, 0x10, 0x20, 0x70 } );
				
	if( stars ) {
		if( options::visuals::world::stars )
			*reinterpret_cast< int* >( stars + 0x14 ) = options::visuals::world::brightness_stars_amount;
		if( options::visuals::world::stars_size )
			*reinterpret_cast< int* >( stars + 0x10 ) = options::visuals::world::size_stars_amount;
	}

	if( options::aimbot::exploits::time::time_modifier )
        g_game.set_admin_time( options::aimbot::exploits::time::time_speed );

	if( night
		&& day 
		&& options::visuals::world::ambient ) {

		*reinterpret_cast< float* >( night + 0x48 ) = options::visuals::world::light_value;
		*reinterpret_cast< float* >( night + 0x50 ) = options::visuals::world::ambient_value;
		*reinterpret_cast< float* >( day + 0x48 ) = options::visuals::world::light_value;
		*reinterpret_cast< float* >( day + 0x50 ) = options::visuals::world::ambient_value;
	}

    color_t ambient = options::visuals::world::ambient_color;
    color_t sky_clr = options::visuals::world::sky_color;

    // modify alpha so it's not so bright as it is.

    if( options::visuals::world::change_world_color ) {
        uintptr_t components = *reinterpret_cast< uintptr_t* >( sky + 0xA8 );
        if( !components )
            return;

        uintptr_t scattering = *reinterpret_cast< uintptr_t* >( components + 0x1A0 );
        if( !scattering )
            return;

        uintptr_t sky_color = *reinterpret_cast< uintptr_t* >( day + 0x28 );
        if( !sky_color )
            return;

        uintptr_t ambient_color = *reinterpret_cast< uintptr_t* >( day + 0x40 );
        if( !ambient_color )
            return;

        uintptr_t sky_gradient = *reinterpret_cast< uintptr_t* >( sky_color + 0x10 );
        if( !sky_gradient )
            return;

        uintptr_t ambient_gradient = *reinterpret_cast< uintptr_t* >( ambient_color + 0x10 );
        if( !ambient_gradient )
            return;

        *reinterpret_cast< color_t* >( sky_gradient ) = color_t( sky_clr.r, sky_clr.g, sky_clr.b, 255.f );

        *reinterpret_cast< color_t* >( ambient_gradient ) = color_t( ambient.r, ambient.g, ambient.b, 255.f );
    }
}

void c_esp::get_sphere_points( float radius, int sectors, std::vector< vec3_t >& _array, float max = 1.5f ) {
    float x, z, current = 0;

    if( _array.size( ) > sectors ) {
        _array.erase( std::unique( _array.begin( ), _array.end( ) ), _array.end( ) );
        std::cout << "s:" << _array.size( ) << std::endl;
        return;
    }

    std::cout << _array.size( ) << std::endl;

    while( _array.size( ) < sectors ) {
        x = sin( current ) * radius;
        z = cos( current ) * radius;

        for( float y = -max; y < max; y += 0.1f ) {
            _array.push_back( vec3_t( -x, y, z ) );
            _array.push_back( vec3_t( x, -y, z ) );
            _array.push_back( vec3_t( x, y, -z ) );
            _array.push_back( vec3_t( -x, -y, z ) );
            _array.push_back( vec3_t( -x, -y, -z ) );

            if( options::aimbot::use_max_eye_points ) {
                _array.push_back( vec3_t( -x * maximum_eye_height / 2, -y * maximum_eye_height, -z ) );
                _array.push_back( vec3_t( -x * maximum_eye_height / 2, -y * maximum_eye_height / 2, -z ) );
                _array.push_back( vec3_t( -( x / 2 ) * maximum_eye_height, -y * maximum_eye_height / 2, -z ) );

                _array.push_back( vec3_t( x * maximum_eye_height, -y * -maximum_eye_height, -( z / 2 ) * maximum_eye_height ) );
                _array.push_back( vec3_t( -x * maximum_eye_height, -y * maximum_eye_height, -z ) );
                _array.push_back( vec3_t( ( x / 2 ) * maximum_eye_height, -y * -maximum_eye_height, -( z / 2 ) * maximum_eye_height ) );
                _array.push_back( vec3_t( -x * maximum_eye_height, y * maximum_eye_height, -z ) );
                _array.push_back( vec3_t( x * maximum_eye_height, y * maximum_eye_height, z ) );
                _array.push_back( vec3_t( -( x / 2 ) * maximum_eye_height, -y * maximum_eye_height, z ) );
                _array.push_back( vec3_t( ( x / 2 ) * maximum_eye_height, y * maximum_eye_height, z ) );
                _array.push_back( vec3_t( -x * maximum_eye_height, -y * maximum_eye_height, -( z / 2 ) * maximum_eye_height ) );
                _array.push_back( vec3_t( x * maximum_eye_height, -y * -maximum_eye_height, -( z / 2 ) * maximum_eye_height ) );
                _array.push_back( vec3_t( -x * maximum_eye_height, y * maximum_eye_height, -( z / 2 ) * maximum_eye_height ) );
                _array.push_back( vec3_t( ( x / 2 ) * maximum_eye_height, -y * maximum_eye_height, -( z / 2 ) * maximum_eye_height ) );
                _array.push_back( vec3_t( -( x / 2 ) * maximum_eye_height, -y * maximum_eye_height, -( z / 2 ) * maximum_eye_height ) );
                _array.push_back( vec3_t( ( x / 2 ) * maximum_eye_height, y * maximum_eye_height, -( z / 2 ) * maximum_eye_height ) );
            }

            if( options::aimbot::use_half_points ) {
                _array.push_back( vec3_t( -x / 1.5f, y / 1.5f, z ) );
                _array.push_back( vec3_t( x, -y / 1.5f, z ) );
                _array.push_back( vec3_t( x / y, y / 2, z ) );
                _array.push_back( vec3_t( x / 2.f, y, z / max ) );
                _array.push_back( vec3_t( x / 2.f, y, z / max ) );
                _array.push_back( vec3_t( -x / 2.f, y / 2.f, -z / 2.f ) );
                _array.push_back( vec3_t( x / 1.5f, y / 2.f, z / y ) );
                _array.push_back( vec3_t( x, y / 2.f, z / 1.5f ) );
                _array.push_back( vec3_t( x / 2.f, y / 2.f, z / 1.5f ) );
            }
        }

        current += max;
    }
}

bool noclip_test( vec3_t old_position, vec3_t new_position, float radius, float backtracking, bool sphere ) {
    int num = 429990145;

    vec3_t normalized = ( new_position - old_position ).normalized( );
    vec3_t vector = old_position - normalized * backtracking;
    float magnitude = ( new_position - vector ).unity_length( );
    bool flag = g_sdk.physics_sphere_cast( Ray( vector, normalized ), radius, magnitude + radius, num );
    if( sphere )
        flag = g_sdk.physics_sphere_cast( Ray( vector, normalized ), radius, magnitude, num );
    
    return flag;
}

float noclip_radius( float margin )
{
    return 0.5f - margin;
}

bool c_esp::validate_eye_pos( vec3_t to_check )
{
    base_player* local = g_cheat.local;
    if( !local )
        return false;

    player_eyes* eyes = g_cheat.local_eyes;
    if ( !eyes )
        return false;

    bool flag = true;
    if( to_check.is_zero( ) )
        return false;

    float desync_time = ( g_sdk.realtimeSinceStartup( ) - local->lastSentTickTime( ) );

    vec3_t current_position = eyes->get_position( );

    vec3_t parent_vel = local->get_parent_velocity( );

    float num = 1.5f;
    float num2 = eye_clientframes / 60.f;
   	float num3 = eye_serverframes * max( g_sdk.get_deltaTime( ), max( g_sdk.get_smooth_delta_time( ), g_sdk.get_fixed_delta_time( ) ) );
    float num4 = ( desync_time + num2 + num3 ) * num;
    float num5 = local->max_velocity( ) + parent_vel.length( );

    float max_dist_allowed = local->get_bounds_padding( ) + num4 * num5;
    float eye_pos_dist = current_position.distance( to_check );
    if( eye_pos_dist > max_dist_allowed )
        flag = false;

    float num8 = std::abs( local->get_mount_velocity( ).y + parent_vel.y );
    float num9 = local->get_bounds_padding( ) + num4 * num8 + local->get_jumpheight( );
    float num10 = std::abs( current_position.y - to_check.y );
    if( num10 > num9 )
        flag = false;

    //int layer = ( 2162688 | 8388608 | 134217728 ); // 8388608 = eye_terraincheck / 134217728 = eye_vehiclecheck

    //vec3_t center = eyes->get_center( );
    //if( !g_sdk.line_of_sight( center, current_position, layer, eye_losradius )
    //    || !g_sdk.line_of_sight( current_position, to_check, layer, eye_losradius ) )
    //        flag = false;

    if( eye_pos_dist > 0.06f )
    {
        if ( noclip_test( current_position, to_check, noclip_radius( eye_noclip_margin ), eye_noclip_backtracking, true ) )
            flag = false;
    }
    else if ( eye_pos_dist > 0.01f 
        && noclip_test( current_position, to_check, 0.01f, eye_noclip_backtracking, true ) )
        flag = false;

    return flag;
}

static std::vector< vec3_t > generate_angles( int max_angles, bool has_desync )  {
    float inc = 3.14f * ( 3 - sqrt( 5 ) );
    float off = 2.f / max_angles;

    std::vector< vec3_t > total_angles{ };
    for( auto i = 0; i < max_angles; i++ ) {
        vec3_t angle = vec3_t( );
        if( has_desync )
            angle.y = i * off - 1 + ( off / 2.f );

        float r = sqrt( 1 - angle.y * angle.y );
        double phi = i * inc;

        angle.x = cos( phi ) * r;
        angle.z = sin( phi ) * r;
        total_angles.push_back( angle );
    }

    return total_angles;
}

void c_esp::find_manipulate_angle( ) {
    std::vector< vec3_t > _array = { };
    int layers = ( ( int )Layers::ProjectileLineOfSightCheckTerrain | ( int )Layers::Terrain | ( int )Layers::EntityLineOfSightCheck | ( int )Layers::Construction );

    auto reset_angles =[ & ]( )  -> void {
        _array.clear( );
        manipulation_angle.clear( );
    };

    base_player* target = g_aimbot.target;
    if( !target )
        return reset_angles( );

    base_player* local = g_cheat.local;
    if( !local )
        return reset_angles( );

    transform* transform = local->get_transform( );
    if( !transform )
        return reset_angles( );

    player_eyes* eyes = g_cheat.local_eyes;
    if( !eyes )
        return reset_angles( );

    vec3_t re_p = transform->get_position( ) + transform->up( ) * ( player_eyes::get_eyeOffset( ).y + eyes->get_viewoffset( ).y );

    vec3_t choice = vec3_t( );

    vec3_t head_pos = target->get_bone_position( head );

    if( !options::aimbot::visible_manipulation ) {
        if ( !bullet_tp_target.is_zero( ) ) {
            if( g_sdk.is_visible( re_p, bullet_tp_target, 0.f, layers ) )
                return manipulation_angle.clear( );
        }
        else {
            if( g_sdk.is_visible( re_p, head_pos, 0.f, layers ) )
                return manipulation_angle.clear( );
        }
    }

    vec3_t right = eyes->get_movementright( );
    vec3_t forward = eyes->get_movementforward( );
    vec3_t center = eyes->get_center( );

    float max_v_multiplier = 6.2f;
    float v_maximum_eye_height = maximum_eye_height / max_v_multiplier;

    float manip_height = options::aimbot::exploits::misc::use_max_view ? v_maximum_eye_height : 1.6f;
    if( options::aimbot::generate_points )
        _array = generate_angles( options::aimbot::manipulation_angles, true );

    const int angle_amt = 820;

    std::array< vec3_t, angle_amt > arr =
    {
        vec3_t( v_maximum_eye_height / 2, -maximum_eye_height / 2, 0.f ),
        vec3_t( -v_maximum_eye_height, v_maximum_eye_height, 0.f ),
        vec3_t( v_maximum_eye_height, -( v_maximum_eye_height / 2 ), 0.f ),
        vec3_t( v_maximum_eye_height, -v_maximum_eye_height / 2, 0.f ),
        vec3_t( 0.f, maximum_eye_height, 0.f ),
        vec3_t( 0.f, -maximum_eye_height / 2, 0.f ),
        vec3_t( 0.f, -maximum_eye_height, 0.f ),
        vec3_t( 0.75f, -0.75f, 0.f ),
        vec3_t( 1.5f, -0.75f, 0.f ),
        vec3_t( 1.5f, -v_maximum_eye_height, 0.f ),
        vec3_t( -1.5f, v_maximum_eye_height, 0.f ),
        vec3_t( 1.5f, v_maximum_eye_height, 0.f ),
        vec3_t( 1.5f, -( v_maximum_eye_height / 2 ), 0.f ),
        vec3_t( 1.5f, -v_maximum_eye_height / 2, 0.f ),
        vec3_t( 1.5f, -maximum_eye_height, 0.f ),
        vec3_t( -1.5f, maximum_eye_height, 0.f ),
        vec3_t( 1.5f, maximum_eye_height, 0.f ),
        vec3_t( v_maximum_eye_height / 2, -maximum_eye_height, maximum_eye_height / 2 ),
        vec3_t( 1.5f, -( maximum_eye_height / 2 ), 0.f ),
        vec3_t( 1.5f, -maximum_eye_height / 2, 0.f ),
        vec3_t( -( maximum_eye_height / 2 ), maximum_eye_height / 2, 0.f ),
        vec3_t( v_maximum_eye_height, -maximum_eye_height, 0.f ),
        vec3_t( -v_maximum_eye_height, maximum_eye_height, 0.f ),
        vec3_t( v_maximum_eye_height, maximum_eye_height, 0.f ),
        vec3_t( v_maximum_eye_height, -( maximum_eye_height / 2 ), 0.f ),
        vec3_t( v_maximum_eye_height, -maximum_eye_height / 2, 0.f ),
        vec3_t( -( right.x * 4.f ), -maximum_eye_height, -( right.z * 4.f ) ),
        vec3_t( -( right.x / 2.f ), -maximum_eye_height, -( right.z * 4.f ) ),
        vec3_t( -( right.x * 4.f ), -maximum_eye_height, -( right.z / 2.f ) ),
        vec3_t( -( right.x / 2.f ), -maximum_eye_height, -( right.z / 2.f ) ),
        vec3_t( -( right.x ), -maximum_eye_height, -( right.z * 4.f ) ),
        vec3_t( -right.x, -maximum_eye_height, -( right.z * 4.f ) ),
        vec3_t( -( right.x * 4.f ), maximum_eye_height, -( right.z * 4.f ) ),
        vec3_t( -( right.x / 2.f ), maximum_eye_height, -( right.z * 4.f ) ),
        vec3_t( -( right.x * 4.f ), maximum_eye_height, -( right.z / 2.f ) ),
        vec3_t( -( right.x / 2.f ), maximum_eye_height, -( right.z / 2.f ) ),
        vec3_t( -( right.x ), maximum_eye_height, -( right.z * 4.f ) ),
        vec3_t( -right.x, maximum_eye_height, -( right.z * 4.f ) ),
        vec3_t( -( right.x * 4.f ), -( maximum_eye_height / 2 ), -( right.z * 4.f ) ),
        vec3_t( -( right.x / 2.f ), -( maximum_eye_height / 2 ), -( right.z * 4.f ) ),
        vec3_t( -( right.x * 4.f ), -( maximum_eye_height / 2 ), -( right.z / 2.f ) ),
        vec3_t( -( right.x / 2.f ), -( maximum_eye_height / 2 ), -( right.z / 2.f ) ),
        vec3_t( -( right.x ), -( maximum_eye_height / 2 ), -( right.z * 4.f ) ),
        vec3_t( -right.x, -( maximum_eye_height / 2 ), -( right.z * 4.f ) ),
        vec3_t( -( right.x * 4.f ), ( maximum_eye_height / 2 ), -( right.z * 4.f ) ),
        vec3_t( -( right.x / 2.f ), ( maximum_eye_height / 2 ), -( right.z * 4.f ) ),
        vec3_t( -( right.x * 4.f ), ( maximum_eye_height / 2 ), -( right.z / 2.f ) ),
        vec3_t( -( right.x / 2.f ), ( maximum_eye_height / 2 ), -( right.z / 2.f ) ),
        vec3_t( -( right.x ), ( maximum_eye_height / 2 ), -( right.z * 4.f ) ),
        vec3_t( -right.x, ( maximum_eye_height / 2 ), -( right.z * 4.f ) ),
        vec3_t( 0.f, ( v_maximum_eye_height / 5 ), 0.f ),
        vec3_t( 0.f, v_maximum_eye_height, 0.f ),
        vec3_t( 0.f, -manip_height / 2, 0.f ),
        vec3_t( 0.f, -manip_height, 0.f ),
        vec3_t( 0.f, 1.5f, 0.f ),
        vec3_t( 0.f, -0.75f, 0.f ),
        vec3_t( 0.f, -1.5f, 0.f ),
        vec3_t( v_maximum_eye_height / 2, -manip_height / 2, 0.f ),
        vec3_t( v_maximum_eye_height, -manip_height / 2, 0.f ),
        vec3_t( 0.f, -v_maximum_eye_height / 2, 0.f ),
        vec3_t( 0.f, -v_maximum_eye_height, 0.f ),
        vec3_t( v_maximum_eye_height / 2, -maximum_eye_height, 0.f ),
        vec3_t( v_maximum_eye_height, -v_maximum_eye_height, 0.f ),
        vec3_t( 0.f, ( v_maximum_eye_height / 10 ), 0.f ),
        vec3_t( 0.f, ( v_maximum_eye_height / 4 ), 0.f ), // medium
        vec3_t( 0.f, ( v_maximum_eye_height / 2 ), 0.f ), // large
        vec3_t( right.x, -v_maximum_eye_height / 2, right.z ),
        vec3_t( right.x, -v_maximum_eye_height, right.z ),
        vec3_t( v_maximum_eye_height, -maximum_eye_height / 2, right.x ),
        vec3_t( -( right.x / 2 ), -v_maximum_eye_height / 2, right.z ),
        vec3_t( -( right.x / 2 ), -v_maximum_eye_height, right.z ),
        vec3_t( ( right.x / 2 ), -v_maximum_eye_height / 2, right.z ),
        vec3_t( ( right.x / 2 ), -v_maximum_eye_height, right.z ),
        vec3_t( ( right.x / 2 ), ( v_maximum_eye_height / 5 ), right.z ),
        vec3_t( ( right.x / 2 ), ( v_maximum_eye_height / 3 ), right.z ), // medium
        vec3_t( ( right.x / 2 ), ( v_maximum_eye_height / 2 ), right.z ), // large
        vec3_t( -( right.x / 2 ), ( v_maximum_eye_height / 5 ), right.z ),
        vec3_t( -( right.x / 2 ), ( v_maximum_eye_height / 3 ), right.z ), // medium
        vec3_t( -( right.x / 2 ), ( v_maximum_eye_height / 2 ), right.z ), // large
        vec3_t( 1.5f, -( v_maximum_eye_height / 2 ), center.z / 2 ),
        vec3_t( -right.x, ( v_maximum_eye_height / 5 ), right.z ),
        vec3_t( -right.x, ( v_maximum_eye_height / 3 ), right.z ), // medium
        vec3_t( -right.x, ( v_maximum_eye_height / 2 ), right.z ), // large
        vec3_t( right.x, ( v_maximum_eye_height / 5 ), right.z ),
        vec3_t( right.x, ( v_maximum_eye_height / 3 ), right.z ), // medium
        vec3_t( right.x, ( v_maximum_eye_height / 2 ), right.z ), // large
        vec3_t( ( right.x / 2 ), ( v_maximum_eye_height / 10 ), right.z ),
        vec3_t( ( right.x / 2 ), ( v_maximum_eye_height / 4 ), right.z ), // medium
        vec3_t( -( right.x / 2 ), ( v_maximum_eye_height / 10 ), right.z ),
        vec3_t( -( right.x / 2 ), ( v_maximum_eye_height / 4 ), right.z ), // medium
        vec3_t( -right.x, ( v_maximum_eye_height / 10 ), right.z ),
        vec3_t( -right.x, ( v_maximum_eye_height / 4 ), right.z ), // medium
        vec3_t( right.x, ( v_maximum_eye_height / 10 ), right.z ),
        vec3_t( right.x, ( v_maximum_eye_height / 4 ), right.z ), // medium
        vec3_t( -( manip_height / 2 ), -manip_height, 0.f ),
        vec3_t( v_maximum_eye_height, -manip_height / 2, ( right.z / 2 ) ),
        vec3_t( ( manip_height / 2 ), -manip_height / 2, 0.f ),
        vec3_t( -v_maximum_eye_height, -manip_height / 2, 0.f ),
        vec3_t( v_maximum_eye_height, manip_height / 2, 0.f ),
        vec3_t( -( manip_height / 2 ), -manip_height / 2, 0.f ),
        vec3_t( -right.x, -v_maximum_eye_height / 2, right.z ),
        vec3_t( -right.x, -v_maximum_eye_height, right.z ),
        vec3_t( v_maximum_eye_height, -v_maximum_eye_height, v_maximum_eye_height ),
        vec3_t( v_maximum_eye_height, -v_maximum_eye_height, -v_maximum_eye_height ),
        vec3_t( -v_maximum_eye_height, v_maximum_eye_height, v_maximum_eye_height ),
        vec3_t( -( maximum_eye_height / 2 ), -v_maximum_eye_height, -v_maximum_eye_height ),
        vec3_t( -v_maximum_eye_height, -v_maximum_eye_height, -v_maximum_eye_height ),
        vec3_t( v_maximum_eye_height, -( v_maximum_eye_height / 2 ), -v_maximum_eye_height ),
        vec3_t( v_maximum_eye_height, -( v_maximum_eye_height / 2 ), -( v_maximum_eye_height / 2 ) ),
        vec3_t( -( maximum_eye_height / 2 ), -( v_maximum_eye_height / 2 ), -( v_maximum_eye_height / 2 ) ),
        vec3_t( ( maximum_eye_height / 2 ), -( v_maximum_eye_height / 2 ), -( v_maximum_eye_height / 2 ) ),
        vec3_t( -( maximum_eye_height / 2 ), ( v_maximum_eye_height / 2 ), -( v_maximum_eye_height / 2 ) ),
        vec3_t( -( maximum_eye_height / 2 ), -( v_maximum_eye_height / 2 ), ( v_maximum_eye_height / 2 ) ),
        vec3_t( ( maximum_eye_height / 2 ), ( v_maximum_eye_height / 2 ), ( v_maximum_eye_height / 2 ) ),
        vec3_t( -( maximum_eye_height / 2 ), -v_maximum_eye_height, -( v_maximum_eye_height / 2 ) ),
        vec3_t( v_maximum_eye_height, -v_maximum_eye_height, -( v_maximum_eye_height / 2 ) ),
        vec3_t( ( maximum_eye_height / 2 ), -v_maximum_eye_height, -v_maximum_eye_height ),
        vec3_t( v_maximum_eye_height, v_maximum_eye_height, 0.f ),
        vec3_t( v_maximum_eye_height, -v_maximum_eye_height, -maximum_eye_height ),
        vec3_t( v_maximum_eye_height / 2, -v_maximum_eye_height, v_maximum_eye_height ),
        vec3_t( -v_maximum_eye_height / 2, -v_maximum_eye_height, v_maximum_eye_height / 2 ),
        vec3_t( -v_maximum_eye_height, v_maximum_eye_height / 2, v_maximum_eye_height ),
        vec3_t( right.x * maximum_eye_height, v_maximum_eye_height, 0.f ),
        vec3_t( right.x * maximum_eye_height, right.y * v_maximum_eye_height, 0.f ),
        vec3_t( right.x * maximum_eye_height, ( right.y / 2 ) * v_maximum_eye_height, ( right.z / 2 ) * v_maximum_eye_height ),
        vec3_t( right.x * maximum_eye_height, ( right.y * v_maximum_eye_height ) / 2, ( right.z ) * v_maximum_eye_height / 2 ),
        vec3_t( right.x * maximum_eye_height, ( right.y * v_maximum_eye_height ) / 2, 0.f ),
        vec3_t( right.x * maximum_eye_height / 2, ( right.y * -( v_maximum_eye_height / 2 ) ) / 2, 0.f ),
        vec3_t( right.x * maximum_eye_height, -( right.y * v_maximum_eye_height ) / 2, right.z ),
        vec3_t( right.x * ( maximum_eye_height / 2 ), -( right.y * v_maximum_eye_height ) / 2, right.z ),
        vec3_t( -( right.x * ( maximum_eye_height / 2 ) ), -( right.y * v_maximum_eye_height ) / 2, right.z ),
        vec3_t( -( right.x * -( maximum_eye_height / 2 ) ), -( right.y * v_maximum_eye_height ) / 2, right.z ),
        vec3_t( right.x * maximum_eye_height, ( right.y / 2 ) * v_maximum_eye_height, 0.f ),
        vec3_t( forward.x * maximum_eye_height, v_maximum_eye_height, 0.f ),
        vec3_t( forward.x * maximum_eye_height, forward.y * v_maximum_eye_height, 0.f ),
        vec3_t( forward.x * maximum_eye_height, ( forward.y / 2 ) * v_maximum_eye_height, ( forward.z / 2 ) * v_maximum_eye_height ),
        vec3_t( forward.x * maximum_eye_height, ( forward.y * v_maximum_eye_height ) / 2, ( forward.z ) * v_maximum_eye_height / 2 ),
        vec3_t( forward.x * maximum_eye_height, ( forward.y * v_maximum_eye_height ) / 2, 0.f ),
        vec3_t( forward.x * maximum_eye_height / 2, ( forward.y * -( v_maximum_eye_height / 2 ) ) / 2, 0.f ),
        vec3_t( forward.x * maximum_eye_height, -( forward.y * v_maximum_eye_height ) / 2, forward.z ),
        vec3_t( forward.x * ( maximum_eye_height / 2 ), -( forward.y * v_maximum_eye_height ) / 2, forward.z ),
        vec3_t( -( forward.x * ( maximum_eye_height / 2 ) ), -( forward.y * v_maximum_eye_height ) / 2, forward.z ),
        vec3_t( -( forward.x * -( maximum_eye_height / 2 ) ), -( forward.y * v_maximum_eye_height ) / 2, forward.z ),
        vec3_t( forward.x * maximum_eye_height, ( forward.y / 2 ) * v_maximum_eye_height, 0.f ),
        vec3_t( v_maximum_eye_height / 2, -v_maximum_eye_height, forward.z ),
        vec3_t( forward.x * maximum_eye_height, manip_height, forward.z * maximum_eye_height ),
        vec3_t( center.x, maximum_eye_height, 0.f ),
        vec3_t( ( center.x * -( v_maximum_eye_height / 2 ) ), -( maximum_eye_height ), 0.f ),
        vec3_t( ( center.x * -( v_maximum_eye_height ) ), -( maximum_eye_height ), 0.f ),
        vec3_t( ( center.x * ( v_maximum_eye_height / 2 ) ), -( maximum_eye_height ), 0.f ),
        vec3_t( ( center.x * -v_maximum_eye_height ), -( maximum_eye_height ), 0.f ),
        vec3_t( ( center.x * -( v_maximum_eye_height / 2 ) ), -( maximum_eye_height / 2 ), 0.f ),
        vec3_t( ( center.x * -( v_maximum_eye_height ) ), -( maximum_eye_height / 2 ), 0.f ),
        vec3_t( ( center.x * ( v_maximum_eye_height / 2 ) ), -( maximum_eye_height / 2 ), 0.f ),
        vec3_t( ( center.x * -v_maximum_eye_height ), -( maximum_eye_height / 2 ), 0.f ),
        vec3_t( ( center.x * -( v_maximum_eye_height / 2 ) ), ( maximum_eye_height / 2 ), 0.f ),
        vec3_t( ( center.x * -( v_maximum_eye_height ) ), ( maximum_eye_height / 2 ), 0.f ),
        vec3_t( ( center.x * ( v_maximum_eye_height / 2 ) ), ( maximum_eye_height / 2 ), 0.f ),
        vec3_t( ( center.x * -v_maximum_eye_height ), ( maximum_eye_height / 2 ), 0.f ),
        vec3_t( -center.x, maximum_eye_height, 0.f ),
        vec3_t( -( center.x * -( v_maximum_eye_height / 2 ) ), -( maximum_eye_height ), 0.f ),
        vec3_t( -( center.x * -( v_maximum_eye_height ) ), -( maximum_eye_height ), 0.f ),
        vec3_t( -( center.x * ( v_maximum_eye_height / 2 ) ), -( maximum_eye_height ), 0.f ),
        vec3_t( -( center.x * -v_maximum_eye_height ), -( maximum_eye_height ), 0.f ),
        vec3_t( -( center.x * -( v_maximum_eye_height / 2 ) ), -( maximum_eye_height / 2 ), 0.f ),
        vec3_t( -( center.x * -( v_maximum_eye_height ) ), -( maximum_eye_height / 2 ), 0.f ),
        vec3_t( -( center.x * ( v_maximum_eye_height / 2 ) ), -( maximum_eye_height / 2 ), 0.f ),
        vec3_t( -( center.x * -v_maximum_eye_height ), -( maximum_eye_height / 2 ), 0.f ),
        vec3_t( -( center.x * -( v_maximum_eye_height / 2 ) ), ( maximum_eye_height / 2 ), 0.f ),
        vec3_t( -( center.x * -( v_maximum_eye_height ) ), ( maximum_eye_height / 2 ), 0.f ),
        vec3_t( -( center.x * ( v_maximum_eye_height / 2 ) ), ( maximum_eye_height / 2 ), 0.f ),
        vec3_t( -( center.x * -v_maximum_eye_height ), ( maximum_eye_height / 2 ), 0.f ),
        vec3_t( v_maximum_eye_height / 2, -v_maximum_eye_height / 2, -maximum_eye_height ),
        vec3_t( v_maximum_eye_height / 2, -maximum_eye_height, right.z ),
        vec3_t( v_maximum_eye_height, -maximum_eye_height / 2, center.x ),
        vec3_t( v_maximum_eye_height, -v_maximum_eye_height, right.z ),
        vec3_t( -v_maximum_eye_height, v_maximum_eye_height, right.z ),
        vec3_t( v_maximum_eye_height, v_maximum_eye_height, right.z ),
        vec3_t( -( center.x / 2 ), -v_maximum_eye_height / 2, right.z ),
        vec3_t( -( center.x / 2 ), -v_maximum_eye_height, right.z ),
        vec3_t( v_maximum_eye_height, -maximum_eye_height / 2, right.z ),
        vec3_t( ( center.x / 2 ), -v_maximum_eye_height / 2, right.z ),
        vec3_t( ( center.x / 2 ), -v_maximum_eye_height, right.z ),
        vec3_t( ( center.x / 2 ), ( v_maximum_eye_height / 5 ), right.z ),
        vec3_t( ( center.x / 2 ), ( v_maximum_eye_height / 3 ), right.z ), // medium
        vec3_t( ( center.x / 2 ), ( v_maximum_eye_height / 2 ), right.z ), // large
        vec3_t( -( center.x / 2 ), ( v_maximum_eye_height / 5 ), right.z ),
        vec3_t( -( center.x / 2 ), ( v_maximum_eye_height / 3 ), right.z ), // medium
        vec3_t( -( center.x / 2 ), ( v_maximum_eye_height / 2 ), right.z ), // large
        vec3_t( -center.x, ( v_maximum_eye_height / 5 ), right.z ),
        vec3_t( -center.x, ( v_maximum_eye_height / 3 ), right.z ), // medium
        vec3_t( -center.x, ( v_maximum_eye_height / 2 ), right.z ), // large
        vec3_t( center.x, ( v_maximum_eye_height / 5 ), right.z ),
        vec3_t( center.x, ( v_maximum_eye_height / 3 ), right.z ), // medium
        vec3_t( center.x, ( v_maximum_eye_height / 2 ), right.z ), // large
        vec3_t( ( center.x / 2 ), ( v_maximum_eye_height / 10 ), right.z ),
        vec3_t( ( center.x / 2 ), ( v_maximum_eye_height / 4 ), right.z ), // medium
        vec3_t( -( center.x / 2 ), ( v_maximum_eye_height / 10 ), right.z ),
        vec3_t( -( center.x / 2 ), ( v_maximum_eye_height / 4 ), right.z ), // medium
        vec3_t( -center.x, ( v_maximum_eye_height / 10 ), right.z ),
        vec3_t( -center.x, ( v_maximum_eye_height / 4 ), right.z ), // medium
        vec3_t( center.x, ( v_maximum_eye_height / 10 ), right.z ),
        vec3_t( center.x, ( v_maximum_eye_height / 4 ), right.z ), // medium
        vec3_t( v_maximum_eye_height / 2, -v_maximum_eye_height, right.z ),
        vec3_t( center.x * maximum_eye_height, v_maximum_eye_height, 0.f ),
        vec3_t( center.x * maximum_eye_height, forward.y * v_maximum_eye_height, 0.f ),
        vec3_t( center.x * maximum_eye_height, ( forward.y / 2 ) * v_maximum_eye_height, ( center.z / 2 ) * v_maximum_eye_height ),
        vec3_t( center.x * maximum_eye_height, ( forward.y * v_maximum_eye_height ) / 2, ( center.z ) * v_maximum_eye_height / 2 ),
        vec3_t( center.x * maximum_eye_height, ( forward.y * v_maximum_eye_height ) / 2, 0.f ),
        vec3_t( center.x * maximum_eye_height / 2, ( forward.y * -( v_maximum_eye_height / 2 ) ) / 2, 0.f ),
        vec3_t( center.x * maximum_eye_height, -( forward.y * v_maximum_eye_height ) / 2, center.z ),
        vec3_t( center.x * ( maximum_eye_height / 2 ), -( forward.y * v_maximum_eye_height ) / 2, center.z ),
        vec3_t( -( center.x * ( maximum_eye_height / 2 ) ), -( forward.y * v_maximum_eye_height ) / 2, center.z ),
        vec3_t( -( center.x * -( maximum_eye_height / 2 ) ), -( forward.y * v_maximum_eye_height ) / 2, center.z ),
        vec3_t( center.x * maximum_eye_height, ( forward.y / 2 ) * v_maximum_eye_height, 0.f ),
        vec3_t( v_maximum_eye_height / 2, -v_maximum_eye_height, center.z ),
        vec3_t( center.x * maximum_eye_height, manip_height, center.z * maximum_eye_height ),
        vec3_t( center.x * ( maximum_eye_height / 2 ), -manip_height / 2, center.z * 2 ),
        vec3_t( -( center.x * ( maximum_eye_height / 2 ) ), 0.f, -( center.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( center.x * maximum_eye_height ), -manip_height, -( center.z * maximum_eye_height ) ),
        vec3_t( center.x * maximum_eye_height, v_maximum_eye_height, center.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( -center.x * ( maximum_eye_height ), ( v_maximum_eye_height ), -center.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( center.x * maximum_eye_height, -v_maximum_eye_height, center.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( -center.x * ( maximum_eye_height ), ( -v_maximum_eye_height ), -center.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( ( center.x / 2 ) * maximum_eye_height, v_maximum_eye_height, ( center.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( center.x / 2 ) * ( maximum_eye_height / 2 ), ( v_maximum_eye_height ), -( center.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( center.x / 2 ) * -( maximum_eye_height / 2 ), ( v_maximum_eye_height ), -( center.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( center.x / 2 ) * ( maximum_eye_height / 2 ), -( v_maximum_eye_height ), -( center.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( center.x / 2 ) * ( maximum_eye_height ), ( v_maximum_eye_height / 2 ), -( center.z / 2 ) * ( maximum_eye_height / 2 ) ),
        vec3_t( -( center.x / 2 ) * -( maximum_eye_height ), ( v_maximum_eye_height / 2 ), -( center.z / 2 ) * ( maximum_eye_height / 2 ) ),
        vec3_t( -( center.x / 2 ) * ( maximum_eye_height ), ( v_maximum_eye_height / 2 ), -( center.z / 2 ) * -( maximum_eye_height / 2 ) ),
        vec3_t( -( center.x / 2 ) * ( maximum_eye_height ), ( v_maximum_eye_height ), -( center.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( center.x / 2 ) * -( maximum_eye_height / 2 ), ( v_maximum_eye_height / 2 ), -( center.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( center.x / 2 ) * ( maximum_eye_height / 2 ), -( v_maximum_eye_height / 2 ), -( center.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( center.x / 2 ) * -( maximum_eye_height / 2 ), ( v_maximum_eye_height / 2 ), -( center.z / 2 ) * - ( maximum_eye_height / 2 ) ),
        vec3_t( -( center.x / 2 ) * ( maximum_eye_height ), ( v_maximum_eye_height ), -( center.z / 2 ) * ( maximum_eye_height / 2 ) ),
        vec3_t( -( center.x / 2 ) * ( maximum_eye_height ), ( v_maximum_eye_height ), -( center.z / 2 ) * -( maximum_eye_height / 2 ) ),
        vec3_t( -( center.x / 2 ) * -( maximum_eye_height / 2 ), -( v_maximum_eye_height ), -( center.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -center.x * ( maximum_eye_height ), 1.5f, -center.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( ( center.x / 2 ) * maximum_eye_height, -v_maximum_eye_height, ( center.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( center.x / 2 ) * ( maximum_eye_height ), ( -v_maximum_eye_height ), -( center.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( center.x * ( maximum_eye_height / 4 ), 0.f, center.z * ( maximum_eye_height / 4 ) ),
        vec3_t( center.x * ( maximum_eye_height / manip_height ), 0.f, center.z * ( maximum_eye_height / 1.5f ) ),
        vec3_t( -( center.x * ( maximum_eye_height / 1.5f ) ), 0.f, -( center.z * ( maximum_eye_height / 1.5f ) ) ),
        vec3_t( center.x * maximum_eye_height, 1.5f, center.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( center.x * maximum_eye_height, -1.5f, center.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( -center.x * ( maximum_eye_height ), -1.5f, -center.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( ( center.x / 2 ) * maximum_eye_height, 1.5f, ( center.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( center.x / 2 ) * ( maximum_eye_height ), 1.5f, -( center.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( ( center.x / 2 ) * maximum_eye_height, -1.5f, ( center.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( center.x / 2 ) * ( maximum_eye_height ), -1.5f, -( center.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( center.x * maximum_eye_height, manip_height, center.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( center.x * maximum_eye_height, -manip_height, center.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( -( center.x / 2 ) * ( maximum_eye_height ), manip_height, -( center.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( center.x / 2 ) * ( maximum_eye_height ), -manip_height, -( center.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( right.x * -( v_maximum_eye_height / 2 ) ), -( maximum_eye_height ), 0.f ),
        vec3_t( -( right.x * -( v_maximum_eye_height ) ), -( maximum_eye_height ), 0.f ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), -( maximum_eye_height ), 0.f ),
        vec3_t( -( right.x * -v_maximum_eye_height ), -( maximum_eye_height ), 0.f ),
        vec3_t( -( right.x * -( v_maximum_eye_height / 2 ) ), -( maximum_eye_height / 2 ), 0.f ),
        vec3_t( -( right.x * -( v_maximum_eye_height ) ), -( maximum_eye_height / 2 ), 0.f ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), -( maximum_eye_height / 2 ), 0.f ),
        vec3_t( -( right.x * -v_maximum_eye_height ), -( maximum_eye_height / 2 ), 0.f ),
        vec3_t( -( right.x * -( v_maximum_eye_height / 2 ) ), ( maximum_eye_height / 2 ), 0.f ),
        vec3_t( -( right.x * -( v_maximum_eye_height ) ), ( maximum_eye_height / 2 ), 0.f ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), ( maximum_eye_height / 2 ), 0.f ),
        vec3_t( -( right.x * -v_maximum_eye_height ), ( maximum_eye_height / 2 ), 0.f ),
        vec3_t( right.x, maximum_eye_height, 0.f ),
        vec3_t( ( right.x * -( v_maximum_eye_height / 2 ) ), -( maximum_eye_height ), 0.f ),
        vec3_t( ( right.x * -( v_maximum_eye_height ) ), -( maximum_eye_height ), 0.f ),
        vec3_t( ( right.x * ( v_maximum_eye_height / 2 ) ), -( maximum_eye_height ), 0.f ),
        vec3_t( ( right.x * -v_maximum_eye_height ), -( maximum_eye_height ), 0.f ),
        vec3_t( ( right.x * -( v_maximum_eye_height / 2 ) ), -( maximum_eye_height / 2 ), 0.f ),
        vec3_t( ( right.x * -( v_maximum_eye_height ) ), -( maximum_eye_height / 2 ), 0.f ),
        vec3_t( ( right.x * ( v_maximum_eye_height / 2 ) ), -( maximum_eye_height / 2 ), 0.f ),
        vec3_t( ( right.x * -v_maximum_eye_height ), -( maximum_eye_height / 2 ), 0.f ),
        vec3_t( ( right.x * -( v_maximum_eye_height / 2 ) ), ( maximum_eye_height / 2 ), 0.f ),
        vec3_t( ( right.x * -( v_maximum_eye_height ) ), ( maximum_eye_height / 2 ), 0.f ),
        vec3_t( ( right.x * ( v_maximum_eye_height / 2 ) ), ( maximum_eye_height / 2 ), 0.f ),
        vec3_t( ( right.x * -v_maximum_eye_height ), ( maximum_eye_height / 2 ), 0.f ),
        vec3_t( right.x * maximum_eye_height, 0.f, right.z * maximum_eye_height ),
        vec3_t( right.x * ( maximum_eye_height / 2 ), manip_height / 2, right.z * ( maximum_eye_height / 2 ) ),
        vec3_t( right.x * ( maximum_eye_height / 2 ), -manip_height / 2, right.z * 2 ),
        vec3_t( -( right.x * ( maximum_eye_height / 2 ) ), manip_height / 2, -( right.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * maximum_eye_height ), manip_height, -( right.z * maximum_eye_height ) ),
        vec3_t( -( right.x * ( maximum_eye_height / 2 ) ), -manip_height / 2, -( right.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * maximum_eye_height ), 0.f, -( forward.z * maximum_eye_height ) ),
        vec3_t( forward.x * ( maximum_eye_height / 2 ), manip_height / 2, forward.z * ( maximum_eye_height / 2 ) ),
        vec3_t( forward.x * ( maximum_eye_height / 2 ), -manip_height / 2, forward.z * ( maximum_eye_height / 2 ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / 2 ) ), manip_height / 2, -( forward.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / 2 ) ), -manip_height / 2, -( forward.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -right.x * ( maximum_eye_height ), manip_height, -right.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( -right.x * ( maximum_eye_height ), -manip_height, -right.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( ( right.x / 2 ) * maximum_eye_height, manip_height, ( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( ( right.x / 2 ) * maximum_eye_height, -manip_height, ( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( ( forward.x / 2 ) * ( maximum_eye_height / 2 ), manip_height, ( forward.z / 2 ) * ( maximum_eye_height / 2 ) ),
        vec3_t( -( ( forward.x / 2 ) * ( maximum_eye_height / 2 ) ), manip_height, -( ( forward.z / 2 ) * maximum_eye_height / 2 ) ),
        vec3_t( forward.x * ( maximum_eye_height / 2 ), -manip_height / 2, forward.z * 2 ),
        vec3_t( -( forward.x * ( maximum_eye_height / 2 ) ), 0.f, -( forward.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * maximum_eye_height ), -manip_height, -( forward.z * maximum_eye_height ) ),
        vec3_t( forward.x * maximum_eye_height, v_maximum_eye_height, forward.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( -forward.x * ( maximum_eye_height ), ( v_maximum_eye_height ), -forward.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( forward.x * maximum_eye_height, -v_maximum_eye_height, forward.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( -forward.x * ( maximum_eye_height ), ( -v_maximum_eye_height ), -forward.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( ( forward.x / 2 ) * maximum_eye_height, v_maximum_eye_height, ( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( maximum_eye_height / 2 ), ( v_maximum_eye_height ), -( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * -( maximum_eye_height / 2 ), ( v_maximum_eye_height ), -( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( maximum_eye_height / 2 ), -( v_maximum_eye_height ), -( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( maximum_eye_height ), ( v_maximum_eye_height / 2 ), -( forward.z / 2 ) * ( maximum_eye_height / 2 ) ),
        vec3_t( -( forward.x / 2 ) * -( maximum_eye_height ), ( v_maximum_eye_height / 2 ), -( forward.z / 2 ) * ( maximum_eye_height / 2 ) ),
        vec3_t( -( forward.x / 2 ) * ( maximum_eye_height ), ( v_maximum_eye_height / 2 ), -( forward.z / 2 ) * -( maximum_eye_height / 2 ) ),
        vec3_t( -( forward.x / 2 ) * ( maximum_eye_height ), ( v_maximum_eye_height ), -( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * -( maximum_eye_height / 2 ), ( v_maximum_eye_height / 2 ), -( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( maximum_eye_height / 2 ), -( v_maximum_eye_height / 2 ), -( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * -( maximum_eye_height / 2 ), ( v_maximum_eye_height / 2 ), -( forward.z / 2 ) * - ( maximum_eye_height / 2 ) ),
        vec3_t( -( forward.x / 2 ) * ( maximum_eye_height ), ( v_maximum_eye_height ), -( forward.z / 2 ) * ( maximum_eye_height / 2 ) ),
        vec3_t( -( forward.x / 2 ) * ( maximum_eye_height ), ( v_maximum_eye_height ), -( forward.z / 2 ) * -( maximum_eye_height / 2 ) ),
        vec3_t( -( forward.x / 2 ) * -( maximum_eye_height / 2 ), -( v_maximum_eye_height ), -( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -forward.x * ( maximum_eye_height ), 1.5f, -forward.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( ( forward.x / 2 ) * maximum_eye_height, -v_maximum_eye_height, ( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( maximum_eye_height ), ( -v_maximum_eye_height ), -( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( forward.x * ( maximum_eye_height / 4 ), 0.f, forward.z * ( maximum_eye_height / 4 ) ),
        vec3_t( forward.x * ( maximum_eye_height / manip_height ), 0.f, forward.z * ( maximum_eye_height / 1.5f ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / 1.5f ) ), 0.f, -( forward.z * ( maximum_eye_height / 1.5f ) ) ),
        vec3_t( forward.x * maximum_eye_height, 1.5f, forward.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( forward.x * maximum_eye_height, -1.5f, forward.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( -forward.x * ( maximum_eye_height ), -1.5f, -forward.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( ( forward.x / 2 ) * maximum_eye_height, 1.5f, ( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( maximum_eye_height ), 1.5f, -( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( ( forward.x / 2 ) * maximum_eye_height, -1.5f, ( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( maximum_eye_height ), -1.5f, -( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( forward.x * maximum_eye_height, manip_height, forward.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( forward.x * maximum_eye_height, -manip_height, forward.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( -( forward.x / 2 ) * ( maximum_eye_height ), manip_height, -( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( maximum_eye_height ), -manip_height, -( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( 1.5f, -( v_maximum_eye_height / 2 ), forward.z / 2 ),
        vec3_t( forward.x * ( maximum_eye_height / 2 ), 0.75f, forward.z * ( maximum_eye_height / 2 ) ),
        vec3_t( forward.x * maximum_eye_height, 1.5f, forward.z * maximum_eye_height ),
        vec3_t( forward.x * ( maximum_eye_height / 2 ), -0.75f, forward.z * 2.f ),
        vec3_t( forward.x * maximum_eye_height, -1.5f, forward.z * maximum_eye_height ),
        vec3_t( right.x * maximum_eye_height, 1.5f, right.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( -right.x * ( maximum_eye_height ), 1.5f, -right.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( right.x * maximum_eye_height, -1.5f, right.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( -right.x * ( maximum_eye_height ), -1.5f, -right.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( ( right.x / 2 ) * maximum_eye_height, 1.5f, ( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( maximum_eye_height ), 1.5f, -( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( ( right.x / 2 ) * maximum_eye_height, -1.5f, ( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( maximum_eye_height ), -1.5f, -( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( ( right.x / 2 ) * maximum_eye_height, -v_maximum_eye_height, ( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( maximum_eye_height ), ( -v_maximum_eye_height ), -( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( right.x * ( maximum_eye_height / 4 ), 0.f, right.z * ( maximum_eye_height / 4 ) ),
        vec3_t( right.x * ( maximum_eye_height / 2 ), 0.f, right.z * ( maximum_eye_height / 2 ) ),
        vec3_t( right.x * ( maximum_eye_height / manip_height ), 0.f, right.z * ( maximum_eye_height / 1.5f ) ),
        vec3_t( -( right.x * ( maximum_eye_height / 4 ) ), 0.f, -( right.z * ( maximum_eye_height / 4 ) ) ),
        vec3_t( -( right.x * ( maximum_eye_height / 2 ) ), 0.f, -( right.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( maximum_eye_height / 1.5f ) ), 0.f, -( right.z * ( maximum_eye_height / 1.5f ) ) ),
        vec3_t( right.x * maximum_eye_height, manip_height, right.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( right.x * maximum_eye_height, -manip_height, right.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( -( right.x / 2 ) * ( maximum_eye_height ), manip_height, -( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( maximum_eye_height ), -manip_height, -( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( 1.5f, -( v_maximum_eye_height / 2 ), right.z / 2 ),
        vec3_t( right.x * ( maximum_eye_height / 2 ), 0.75f, right.z * ( maximum_eye_height / 2 ) ),
        vec3_t( right.x * maximum_eye_height, 1.5f, right.z * maximum_eye_height ),
        vec3_t( right.x * ( maximum_eye_height / 2 ), -0.75f, right.z * 2.f ),
        vec3_t( right.x * maximum_eye_height, -1.5f, right.z * maximum_eye_height ),
        vec3_t( forward.x * maximum_eye_height, 0.f, forward.z * maximum_eye_height ),
        vec3_t( forward.x * maximum_eye_height, -manip_height, forward.z * maximum_eye_height ),
        vec3_t( -( forward.x * 4.f ), manip_height, -( forward.z * 4.f ) ),
        vec3_t( ( forward.x / 2 ) * ( maximum_eye_height / 2 ), ( v_maximum_eye_height / 1 ), ( forward.z / 2 ) * ( maximum_eye_height / 2 ) ),
        vec3_t( -( ( forward.x / 2 ) * ( maximum_eye_height / 2 ) ), ( v_maximum_eye_height / 1 ), -( ( forward.z / 2 ) * maximum_eye_height / 2 ) ),
        vec3_t( forward.x * ( maximum_eye_height / 2 ), 0.f, forward.z * ( maximum_eye_height / 2 ) ),
        vec3_t( forward.x * ( maximum_eye_height / 1.5f ), 0.f, forward.z * ( maximum_eye_height / 1.5f ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / 4 ) ), 0.f, -( forward.z * ( maximum_eye_height / 4 ) ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / manip_height ) ), 0.f, -( forward.z * ( maximum_eye_height / 1.5f ) ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / 2 ) ), manip_height, -( forward.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( ( forward.x * ( maximum_eye_height / 2 ) ), -manip_height / 2, -( forward.z * maximum_eye_height ) ),
        vec3_t( ( forward.x * -( maximum_eye_height / 2 ) ), manip_height / 2, -( forward.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( ( forward.x * -( maximum_eye_height ) ), -manip_height, ( forward.z * maximum_eye_height / 2 ) ),
        vec3_t( ( forward.x * -( maximum_eye_height ) ), -manip_height, -( forward.z * maximum_eye_height ) ),
        vec3_t( ( forward.x * -( maximum_eye_height ) ), -( manip_height / 2 ), -( forward.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( ( forward.x * -( maximum_eye_height / 2 ) ), -manip_height / 2, -( forward.z * maximum_eye_height ) ),
        vec3_t( -( forward.x * maximum_eye_height ), -manip_height / 2, -( forward.z * maximum_eye_height ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / 2 ) ), -manip_height / 2, -( forward.z * maximum_eye_height ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / 2 ) ), -manip_height, -( forward.z * maximum_eye_height ) ),
        vec3_t( -( forward.x * maximum_eye_height ), manip_height / 2, -( forward.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / 2 ) ), -( manip_height / 2 ), -( forward.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / 2 ) ), -manip_height, -( forward.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( ( forward.x / 2 ) * ( maximum_eye_height / 2 ), -( v_maximum_eye_height / 2 ), ( forward.z / 2 ) * ( maximum_eye_height / 2 ) ),
        vec3_t( -( ( forward.x / 2 ) * ( maximum_eye_height / 2 ) ), v_maximum_eye_height, -( ( forward.z / 2 ) * maximum_eye_height / 2 ) ),
        vec3_t( ( forward.x / 2 ) * ( maximum_eye_height / 2 ), v_maximum_eye_height / 2, ( forward.z / 2 ) * ( maximum_eye_height / 2 ) ),
        vec3_t( -( ( forward.x / 2 ) * ( maximum_eye_height / 2 ) ), ( v_maximum_eye_height / 2 ), -( ( forward.z / 2 ) * maximum_eye_height / 2 ) ),
        vec3_t( ( forward.x / 2 ) * ( maximum_eye_height / 2 ), -v_maximum_eye_height, ( forward.z / 2 ) * ( maximum_eye_height / 2 ) ),
        vec3_t( -( ( forward.x / 2 ) * ( maximum_eye_height / 2 ) ), -( v_maximum_eye_height / 2 ), -( ( forward.z / 2 ) * maximum_eye_height / 2 ) ),
        vec3_t( v_maximum_eye_height / 2, -v_maximum_eye_height, v_maximum_eye_height / 2 ),
        vec3_t( -( v_maximum_eye_height / 2 ), v_maximum_eye_height / 2, 0.f ),
        vec3_t( right.x * v_maximum_eye_height, 0.f, right.z * v_maximum_eye_height ),
        vec3_t( right.x * ( v_maximum_eye_height / 2 ), manip_height / 2, right.z * ( v_maximum_eye_height / 2 ) ),
        vec3_t( right.x * ( v_maximum_eye_height / 2 ), -manip_height / 2, right.z * 2 ),
        vec3_t( v_maximum_eye_height, -maximum_eye_height, maximum_eye_height ),
        vec3_t( v_maximum_eye_height, -maximum_eye_height, -maximum_eye_height ),
        vec3_t( -v_maximum_eye_height, maximum_eye_height, maximum_eye_height ),
        vec3_t( -( v_maximum_eye_height / 2 ), -maximum_eye_height, -maximum_eye_height ),
        vec3_t( -v_maximum_eye_height, -maximum_eye_height, -maximum_eye_height ),
        vec3_t( v_maximum_eye_height, -( maximum_eye_height / 2 ), -maximum_eye_height ),
        vec3_t( v_maximum_eye_height, -( maximum_eye_height / 2 ), -( maximum_eye_height / 2 ) ),
        vec3_t( -( v_maximum_eye_height / 2 ), -( maximum_eye_height / 2 ), -( maximum_eye_height / 2 ) ),
        vec3_t( ( v_maximum_eye_height / 2 ), -( maximum_eye_height / 2 ), -( maximum_eye_height / 2 ) ),
        vec3_t( -( v_maximum_eye_height / 2 ), ( maximum_eye_height / 2 ), -( maximum_eye_height / 2 ) ),
        vec3_t( -( v_maximum_eye_height / 2 ), -( maximum_eye_height / 2 ), ( maximum_eye_height / 2 ) ),
        vec3_t( ( v_maximum_eye_height / 2 ), ( maximum_eye_height / 2 ), ( maximum_eye_height / 2 ) ),
        vec3_t( -( v_maximum_eye_height / 2 ), -maximum_eye_height, -( maximum_eye_height / 2 ) ),
        vec3_t( v_maximum_eye_height, -maximum_eye_height, -( maximum_eye_height / 2 ) ),
        vec3_t( ( v_maximum_eye_height / 2 ), -maximum_eye_height, -maximum_eye_height ),
        vec3_t( v_maximum_eye_height, -maximum_eye_height, -v_maximum_eye_height ),
        vec3_t( v_maximum_eye_height / 2, -maximum_eye_height, maximum_eye_height ),
        vec3_t( -v_maximum_eye_height / 2, -maximum_eye_height, maximum_eye_height / 2 ),
        vec3_t( -v_maximum_eye_height, maximum_eye_height / 2, maximum_eye_height ),
        vec3_t( right.x * v_maximum_eye_height, maximum_eye_height, 0.f ),
        vec3_t( right.x * v_maximum_eye_height, right.y * maximum_eye_height, 0.f ),
        vec3_t( right.x * v_maximum_eye_height, ( right.y / 2 ) * maximum_eye_height, ( right.z / 2 ) * maximum_eye_height ),
        vec3_t( right.x * v_maximum_eye_height, ( right.y * maximum_eye_height ) / 2, ( right.z ) * maximum_eye_height / 2 ),
        vec3_t( right.x * v_maximum_eye_height, ( right.y * maximum_eye_height ) / 2, 0.f ),
        vec3_t( right.x * v_maximum_eye_height / 2, ( right.y * -( maximum_eye_height / 2 ) ) / 2, 0.f ),
        vec3_t( right.x * v_maximum_eye_height, -( right.y * maximum_eye_height ) / 2, right.z ),
        vec3_t( right.x * ( v_maximum_eye_height / 2 ), -( right.y * maximum_eye_height ) / 2, right.z ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), -( right.y * maximum_eye_height ) / 2, right.z ),
        vec3_t( -( right.x * -( v_maximum_eye_height / 2 ) ), -( right.y * maximum_eye_height ) / 2, right.z ),
        vec3_t( right.x * v_maximum_eye_height, ( right.y / 2 ) * maximum_eye_height, 0.f ),
        vec3_t( v_maximum_eye_height / 2, -maximum_eye_height / 2, -v_maximum_eye_height ),
        vec3_t( forward.x * v_maximum_eye_height, maximum_eye_height, 0.f ),
        vec3_t( forward.x * v_maximum_eye_height, forward.y * maximum_eye_height, 0.f ),
        vec3_t( forward.x * v_maximum_eye_height, ( forward.y / 2 ) * maximum_eye_height, ( forward.z / 2 ) * maximum_eye_height ),
        vec3_t( forward.x * v_maximum_eye_height, ( forward.y * maximum_eye_height ) / 2, ( forward.z ) * maximum_eye_height / 2 ),
        vec3_t( forward.x * v_maximum_eye_height, ( forward.y * maximum_eye_height ) / 2, 0.f ),
        vec3_t( forward.x * v_maximum_eye_height / 2, ( forward.y * -( maximum_eye_height / 2 ) ) / 2, 0.f ),
        vec3_t( forward.x * v_maximum_eye_height, -( forward.y * maximum_eye_height ) / 2, forward.z ),
        vec3_t( forward.x * ( v_maximum_eye_height / 2 ), -( forward.y * maximum_eye_height ) / 2, forward.z ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / 2 ) ), -( forward.y * maximum_eye_height ) / 2, forward.z ),
        vec3_t( -( forward.x * -( v_maximum_eye_height / 2 ) ), -( forward.y * maximum_eye_height ) / 2, forward.z ),
        vec3_t( forward.x * v_maximum_eye_height, ( forward.y / 2 ) * maximum_eye_height, 0.f ),
        vec3_t( v_maximum_eye_height / 2, -maximum_eye_height, forward.z ),
        vec3_t( forward.x * v_maximum_eye_height, manip_height, forward.z * v_maximum_eye_height ),
        vec3_t( forward.x * ( v_maximum_eye_height / 2 ), -manip_height / 2, forward.z * 2 ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / 2 ) ), 0.f, -( forward.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * v_maximum_eye_height ), -manip_height, -( forward.z * v_maximum_eye_height ) ),
        vec3_t( forward.x * v_maximum_eye_height, maximum_eye_height, forward.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( -forward.x * ( v_maximum_eye_height ), ( maximum_eye_height ), -forward.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( forward.x * v_maximum_eye_height, -maximum_eye_height, forward.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( -forward.x * ( v_maximum_eye_height ), ( -maximum_eye_height ), -forward.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( ( forward.x / 2 ) * v_maximum_eye_height, maximum_eye_height, ( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( v_maximum_eye_height / 2 ), ( maximum_eye_height ), -( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * -( v_maximum_eye_height / 2 ), ( maximum_eye_height ), -( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( v_maximum_eye_height / 2 ), -( maximum_eye_height ), -( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( v_maximum_eye_height ), ( maximum_eye_height / 2 ), -( forward.z / 2 ) * ( v_maximum_eye_height / 2 ) ),
        vec3_t( -( forward.x / 2 ) * -( v_maximum_eye_height ), ( maximum_eye_height / 2 ), -( forward.z / 2 ) * ( v_maximum_eye_height / 2 ) ),
        vec3_t( -( forward.x / 2 ) * ( v_maximum_eye_height ), ( maximum_eye_height / 2 ), -( forward.z / 2 ) * -( v_maximum_eye_height / 2 ) ),
        vec3_t( -( forward.x / 2 ) * ( v_maximum_eye_height ), ( maximum_eye_height ), -( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * -( v_maximum_eye_height / 2 ), ( maximum_eye_height / 2 ), -( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( v_maximum_eye_height / 2 ), -( maximum_eye_height / 2 ), -( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * -( v_maximum_eye_height / 2 ), ( maximum_eye_height / 2 ), -( forward.z / 2 ) * - ( v_maximum_eye_height / 2 ) ),
        vec3_t( -( forward.x / 2 ) * ( v_maximum_eye_height ), ( maximum_eye_height ), -( forward.z / 2 ) * ( v_maximum_eye_height / 2 ) ),
        vec3_t( -( forward.x / 2 ) * ( v_maximum_eye_height ), ( maximum_eye_height ), -( forward.z / 2 ) * -( v_maximum_eye_height / 2 ) ),
        vec3_t( -( forward.x / 2 ) * -( v_maximum_eye_height / 2 ), -( maximum_eye_height ), -( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -forward.x * ( v_maximum_eye_height ), 1.5f, -forward.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( ( forward.x / 2 ) * v_maximum_eye_height, -maximum_eye_height, ( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( v_maximum_eye_height ), ( -maximum_eye_height ), -( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( forward.x * ( v_maximum_eye_height / 4 ), 0.f, forward.z * ( v_maximum_eye_height / 4 ) ),
        vec3_t( forward.x * ( v_maximum_eye_height / manip_height ), 0.f, forward.z * ( v_maximum_eye_height / 1.5f ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / 1.5f ) ), 0.f, -( forward.z * ( v_maximum_eye_height / 1.5f ) ) ),
        vec3_t( forward.x * v_maximum_eye_height, 1.5f, forward.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( forward.x * v_maximum_eye_height, -1.5f, forward.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( -forward.x * ( v_maximum_eye_height ), -1.5f, -forward.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( ( forward.x / 2 ) * v_maximum_eye_height, 1.5f, ( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( v_maximum_eye_height ), 1.5f, -( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( ( forward.x / 2 ) * v_maximum_eye_height, -1.5f, ( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( v_maximum_eye_height ), -1.5f, -( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( forward.x * v_maximum_eye_height, manip_height, forward.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( forward.x * v_maximum_eye_height, -manip_height, forward.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( -( forward.x / 2 ) * ( v_maximum_eye_height ), manip_height, -( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( v_maximum_eye_height ), -manip_height, -( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( 1.5f, -( maximum_eye_height / 2 ), forward.z / 2 ),
        vec3_t( forward.x * ( v_maximum_eye_height / 2 ), 0.75f, forward.z * ( v_maximum_eye_height / 2 ) ),
        vec3_t( forward.x * v_maximum_eye_height, 1.5f, forward.z * v_maximum_eye_height ),
        vec3_t( forward.x * ( v_maximum_eye_height / 2 ), -0.75f, forward.z * 2.f ),
        vec3_t( forward.x * v_maximum_eye_height, -1.5f, forward.z * v_maximum_eye_height ),
        vec3_t( right.x * v_maximum_eye_height, 1.5f, right.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( -right.x * ( v_maximum_eye_height ), 1.5f, -right.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( right.x * v_maximum_eye_height, -1.5f, right.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( -right.x * ( v_maximum_eye_height ), -1.5f, -right.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( ( right.x / 2 ) * v_maximum_eye_height, 1.5f, ( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( v_maximum_eye_height ), 1.5f, -( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( ( right.x / 2 ) * v_maximum_eye_height, -1.5f, ( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( v_maximum_eye_height ), -1.5f, -( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( ( right.x / 2 ) * v_maximum_eye_height, -maximum_eye_height, ( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( v_maximum_eye_height ), ( -maximum_eye_height ), -( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( right.x * ( v_maximum_eye_height / 4 ), 0.f, right.z * ( v_maximum_eye_height / 4 ) ),
        vec3_t( right.x * ( v_maximum_eye_height / 2 ), 0.f, right.z * ( v_maximum_eye_height / 2 ) ),
        vec3_t( right.x * ( v_maximum_eye_height / manip_height ), 0.f, right.z * ( v_maximum_eye_height / 1.5f ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 4 ) ), 0.f, -( right.z * ( v_maximum_eye_height / 4 ) ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), 0.f, -( right.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 1.5f ) ), 0.f, -( right.z * ( v_maximum_eye_height / 1.5f ) ) ),
        vec3_t( right.x * v_maximum_eye_height, manip_height, right.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( right.x * v_maximum_eye_height, -manip_height, right.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( -( right.x / 2 ) * ( v_maximum_eye_height ), manip_height, -( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( v_maximum_eye_height ), -manip_height, -( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( 1.5f, -( maximum_eye_height / 2 ), right.z / 2 ),
        vec3_t( right.x * ( v_maximum_eye_height / 2 ), 0.75f, right.z * ( v_maximum_eye_height / 2 ) ),
        vec3_t( right.x * v_maximum_eye_height, 1.5f, right.z * v_maximum_eye_height ),
        vec3_t( right.x * ( v_maximum_eye_height / 2 ), -0.75f, right.z * 2.f ),
        vec3_t( right.x * v_maximum_eye_height, -1.5f, right.z * v_maximum_eye_height ),
        vec3_t( forward.x * v_maximum_eye_height, 0.f, forward.z * v_maximum_eye_height ),
        vec3_t( forward.x * v_maximum_eye_height, -manip_height, forward.z * v_maximum_eye_height ),
        vec3_t( ( forward.x / 2 ) * ( v_maximum_eye_height / 2 ), ( maximum_eye_height / 1 ), ( forward.z / 2 ) * ( v_maximum_eye_height / 2 ) ),
        vec3_t( -( ( forward.x / 2 ) * ( v_maximum_eye_height / 2 ) ), ( maximum_eye_height / 1 ), -( ( forward.z / 2 ) * v_maximum_eye_height / 2 ) ),
        vec3_t( forward.x * ( v_maximum_eye_height / 2 ), 0.f, forward.z * ( v_maximum_eye_height / 2 ) ),
        vec3_t( forward.x * ( v_maximum_eye_height / 1.5f ), 0.f, forward.z * ( v_maximum_eye_height / 1.5f ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / 4 ) ), 0.f, -( forward.z * ( v_maximum_eye_height / 4 ) ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / manip_height ) ), 0.f, -( forward.z * ( v_maximum_eye_height / 1.5f ) ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / 2 ) ), manip_height, -( forward.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( ( forward.x * ( v_maximum_eye_height / 2 ) ), -manip_height / 2, -( forward.z * v_maximum_eye_height ) ),
        vec3_t( ( forward.x * -( v_maximum_eye_height / 2 ) ), manip_height / 2, -( forward.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( ( forward.x * -( v_maximum_eye_height ) ), -manip_height, ( forward.z * v_maximum_eye_height / 2 ) ),
        vec3_t( ( forward.x * -( v_maximum_eye_height ) ), -manip_height, -( forward.z * v_maximum_eye_height ) ),
        vec3_t( ( forward.x * -( v_maximum_eye_height ) ), -( manip_height / 2 ), -( forward.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( ( forward.x * -( v_maximum_eye_height / 2 ) ), -manip_height / 2, -( forward.z * v_maximum_eye_height ) ),
        vec3_t( -( forward.x * v_maximum_eye_height ), -manip_height / 2, -( forward.z * v_maximum_eye_height ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / 2 ) ), -manip_height / 2, -( forward.z * v_maximum_eye_height ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / 2 ) ), -manip_height, -( forward.z * v_maximum_eye_height ) ),
        vec3_t( -( forward.x * v_maximum_eye_height ), manip_height / 2, -( forward.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / 2 ) ), -( manip_height / 2 ), -( forward.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / 2 ) ), -manip_height, -( forward.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( ( forward.x / 2 ) * ( v_maximum_eye_height / 2 ), -( maximum_eye_height / 2 ), ( forward.z / 2 ) * ( v_maximum_eye_height / 2 ) ),
        vec3_t( -( ( forward.x / 2 ) * ( v_maximum_eye_height / 2 ) ), maximum_eye_height, -( ( forward.z / 2 ) * v_maximum_eye_height / 2 ) ),
        vec3_t( ( forward.x / 2 ) * ( v_maximum_eye_height / 2 ), maximum_eye_height / 2, ( forward.z / 2 ) * ( v_maximum_eye_height / 2 ) ),
        vec3_t( -( ( forward.x / 2 ) * ( v_maximum_eye_height / 2 ) ), ( maximum_eye_height / 2 ), -( ( forward.z / 2 ) * v_maximum_eye_height / 2 ) ),
        vec3_t( ( forward.x / 2 ) * ( v_maximum_eye_height / 2 ), -maximum_eye_height, ( forward.z / 2 ) * ( v_maximum_eye_height / 2 ) ),
        vec3_t( -( ( forward.x / 2 ) * ( v_maximum_eye_height / 2 ) ), -( maximum_eye_height / 2 ), -( ( forward.z / 2 ) * v_maximum_eye_height / 2 ) ),
        vec3_t( forward.x * ( v_maximum_eye_height / 2 ), -0.7f, forward.z * ( v_maximum_eye_height / 2 ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / 2 ) ), 0.75f, -( forward.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / 2 ) ), -0.75f, -( forward.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / 2 ) ), 1.5f, -( forward.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / 2 ) ), -1.5f, -( forward.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( forward.x * ( maximum_eye_height / 2 ), -0.7f, forward.z * ( maximum_eye_height / 2 ) ),
        vec3_t( -( forward.x * 4.f ), 1.5f, -( forward.z * 4.f ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / 2 ) ), 0.75f, -( forward.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / 2 ) ), -0.75f, -( forward.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / 2 ) ), 1.5f, -( forward.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / 2 ) ), -1.5f, -( forward.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -forward.x * ( maximum_eye_height ), v_maximum_eye_height, -forward.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( forward.x * ( maximum_eye_height / manip_height ), 0.f, forward.z * ( maximum_eye_height / v_maximum_eye_height ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / v_maximum_eye_height ) ), 0.f, -( forward.z * ( maximum_eye_height / v_maximum_eye_height ) ) ),
        vec3_t( -forward.x * ( maximum_eye_height ), -v_maximum_eye_height, -forward.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( -( forward.x / 2 ) * ( maximum_eye_height ), v_maximum_eye_height, -( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( maximum_eye_height ), -v_maximum_eye_height, -( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( v_maximum_eye_height, -( v_maximum_eye_height / 2 ), forward.z / 2 ),
        vec3_t( forward.x * ( maximum_eye_height / 2 ), v_maximum_eye_height / 2, forward.z * ( maximum_eye_height / 2 ) ),
        vec3_t( forward.x * maximum_eye_height, v_maximum_eye_height, forward.z * maximum_eye_height ),
        vec3_t( forward.x * ( maximum_eye_height / 2 ), -v_maximum_eye_height / 2, forward.z * 2.f ),
        vec3_t( forward.x * maximum_eye_height, -v_maximum_eye_height, forward.z * maximum_eye_height ),
        vec3_t( right.x * maximum_eye_height, v_maximum_eye_height, right.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( -right.x * ( maximum_eye_height ), v_maximum_eye_height, -right.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( right.x * maximum_eye_height, -v_maximum_eye_height, right.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( -right.x * ( maximum_eye_height ), -v_maximum_eye_height, -right.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( ( right.x / 2 ) * maximum_eye_height, v_maximum_eye_height, ( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( maximum_eye_height ), v_maximum_eye_height, -( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( maximum_eye_height ), -v_maximum_eye_height, -( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( right.x * ( maximum_eye_height / manip_height ), 0.f, right.z * ( maximum_eye_height / v_maximum_eye_height ) ),
        vec3_t( -( right.x * ( maximum_eye_height / v_maximum_eye_height ) ), 0.f, -( right.z * ( maximum_eye_height / v_maximum_eye_height ) ) ),
        vec3_t( v_maximum_eye_height, -( v_maximum_eye_height / 2 ), right.z / 2 ),
        vec3_t( right.x * ( maximum_eye_height / 2 ), v_maximum_eye_height / 2, right.z * ( maximum_eye_height / 2 ) ),
        vec3_t( right.x * maximum_eye_height, v_maximum_eye_height, right.z * maximum_eye_height ),
        vec3_t( right.x * ( maximum_eye_height / 2 ), -v_maximum_eye_height / 2, right.z * 2.f ),
        vec3_t( right.x * maximum_eye_height, -v_maximum_eye_height, right.z * maximum_eye_height ),
        vec3_t( forward.x * ( maximum_eye_height / v_maximum_eye_height ), 0.f, forward.z * ( maximum_eye_height / v_maximum_eye_height ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / manip_height ) ), 0.f, -( forward.z * ( maximum_eye_height / v_maximum_eye_height ) ) ),
        vec3_t( -forward.x * ( v_maximum_eye_height ), v_maximum_eye_height, -forward.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( forward.x * ( v_maximum_eye_height / manip_height ), 0.f, forward.z * ( v_maximum_eye_height / v_maximum_eye_height ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / v_maximum_eye_height ) ), 0.f, -( forward.z * ( v_maximum_eye_height / v_maximum_eye_height ) ) ),
        vec3_t( forward.x * v_maximum_eye_height, v_maximum_eye_height, forward.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( forward.x * v_maximum_eye_height, -v_maximum_eye_height, forward.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( -forward.x * ( v_maximum_eye_height ), -v_maximum_eye_height, -forward.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( ( forward.x / 2 ) * v_maximum_eye_height, v_maximum_eye_height, ( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( v_maximum_eye_height ), v_maximum_eye_height, -( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( ( forward.x / 2 ) * v_maximum_eye_height, -v_maximum_eye_height, ( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( v_maximum_eye_height ), -v_maximum_eye_height, -( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( v_maximum_eye_height, -( maximum_eye_height / 2 ), forward.z / 2 ),
        vec3_t( forward.x * ( v_maximum_eye_height / 2 ), v_maximum_eye_height / 2, forward.z * ( v_maximum_eye_height / 2 ) ),
        vec3_t( forward.x * v_maximum_eye_height, v_maximum_eye_height, forward.z * v_maximum_eye_height ),
        vec3_t( forward.x * ( v_maximum_eye_height / 2 ), -v_maximum_eye_height / 2, forward.z * 2.f ),
        vec3_t( forward.x * v_maximum_eye_height, -v_maximum_eye_height, forward.z * v_maximum_eye_height ),
        vec3_t( right.x * v_maximum_eye_height, v_maximum_eye_height, right.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( -right.x * ( v_maximum_eye_height ), v_maximum_eye_height, -right.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( right.x * v_maximum_eye_height, -v_maximum_eye_height, right.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( -right.x * ( v_maximum_eye_height ), -v_maximum_eye_height, -right.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( ( right.x / 2 ) * v_maximum_eye_height, v_maximum_eye_height, ( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( v_maximum_eye_height ), v_maximum_eye_height, -( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( ( right.x / 2 ) * v_maximum_eye_height, -v_maximum_eye_height, ( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( v_maximum_eye_height ), -v_maximum_eye_height, -( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( right.x * ( v_maximum_eye_height / manip_height ), 0.f, right.z * ( v_maximum_eye_height / v_maximum_eye_height ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / v_maximum_eye_height ) ), 0.f, -( right.z * ( v_maximum_eye_height / v_maximum_eye_height ) ) ),
        vec3_t( v_maximum_eye_height, -( maximum_eye_height / 2 ), right.z / 2 ),
        vec3_t( right.x * ( v_maximum_eye_height / 2 ), v_maximum_eye_height / 2, right.z * ( v_maximum_eye_height / 2 ) ),
        vec3_t( right.x * v_maximum_eye_height, v_maximum_eye_height, right.z * v_maximum_eye_height ),
        vec3_t( right.x * ( v_maximum_eye_height / 2 ), -v_maximum_eye_height / 2, right.z * 2.f ),
        vec3_t( right.x * v_maximum_eye_height, -v_maximum_eye_height, right.z * v_maximum_eye_height ),
        vec3_t( forward.x * ( v_maximum_eye_height / v_maximum_eye_height ), 0.f, forward.z * ( v_maximum_eye_height / v_maximum_eye_height ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / manip_height ) ), 0.f, -( forward.z * ( v_maximum_eye_height / v_maximum_eye_height ) ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / 2 ) ), v_maximum_eye_height / 2, -( forward.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / 2 ) ), -v_maximum_eye_height / 2, -( forward.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / 2 ) ), v_maximum_eye_height, -( forward.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / 2 ) ), -v_maximum_eye_height, -( forward.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( v_maximum_eye_height, -0.75f, 0.f ),
        vec3_t( -( forward.x * 4.f ), v_maximum_eye_height, -( forward.z * 4.f ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / 2 ) ), v_maximum_eye_height, -( forward.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / 2 ) ), -v_maximum_eye_height, -( forward.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -forward.x * ( maximum_eye_height ), maximum_eye_height, -forward.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( forward.x * ( maximum_eye_height / manip_height ), 0.f, forward.z * ( maximum_eye_height / maximum_eye_height ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / maximum_eye_height ) ), 0.f, -( forward.z * ( maximum_eye_height / maximum_eye_height ) ) ),
        vec3_t( forward.x * maximum_eye_height, maximum_eye_height, forward.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( forward.x * maximum_eye_height, -maximum_eye_height, forward.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( -forward.x * ( maximum_eye_height ), -maximum_eye_height, -forward.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( ( forward.x / 2 ) * maximum_eye_height, maximum_eye_height, ( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( maximum_eye_height ), maximum_eye_height, -( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( ( forward.x / 2 ) * maximum_eye_height, -maximum_eye_height, ( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( maximum_eye_height ), -maximum_eye_height, -( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( forward.x * ( maximum_eye_height / 2 ), maximum_eye_height / 2, forward.z * ( maximum_eye_height / 2 ) ),
        vec3_t( forward.x * maximum_eye_height, maximum_eye_height, forward.z * maximum_eye_height ),
        vec3_t( forward.x * ( maximum_eye_height / 2 ), -maximum_eye_height / 2, forward.z * 2.f ),
        vec3_t( forward.x * maximum_eye_height, -maximum_eye_height, forward.z * maximum_eye_height ),
        vec3_t( right.x * maximum_eye_height, maximum_eye_height, right.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( -right.x * ( maximum_eye_height ), maximum_eye_height, -right.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( right.x * maximum_eye_height, -maximum_eye_height, right.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( -right.x * ( maximum_eye_height ), -maximum_eye_height, -right.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( ( right.x / 2 ) * maximum_eye_height, maximum_eye_height, ( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( maximum_eye_height ), maximum_eye_height, -( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( ( right.x / 2 ) * maximum_eye_height, -maximum_eye_height, ( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( maximum_eye_height ), -maximum_eye_height, -( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( right.x * ( maximum_eye_height / manip_height ), 0.f, right.z * ( maximum_eye_height / maximum_eye_height ) ),
        vec3_t( -( right.x * ( maximum_eye_height / maximum_eye_height ) ), 0.f, -( right.z * ( maximum_eye_height / maximum_eye_height ) ) ),
        vec3_t( right.x * ( maximum_eye_height / 2 ), maximum_eye_height / 2, right.z * ( maximum_eye_height / 2 ) ),
        vec3_t( right.x * maximum_eye_height, maximum_eye_height, right.z * maximum_eye_height ),
        vec3_t( right.x * ( maximum_eye_height / 2 ), -maximum_eye_height / 2, right.z * 2.f ),
        vec3_t( right.x * maximum_eye_height, -maximum_eye_height, right.z * maximum_eye_height ),
        vec3_t( forward.x * ( maximum_eye_height / maximum_eye_height ), 0.f, forward.z * ( maximum_eye_height / maximum_eye_height ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / manip_height ) ), 0.f, -( forward.z * ( maximum_eye_height / maximum_eye_height ) ) ),
        vec3_t( -forward.x * ( v_maximum_eye_height ), maximum_eye_height, -forward.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( forward.x * ( v_maximum_eye_height / manip_height ), 0.f, forward.z * ( v_maximum_eye_height / maximum_eye_height ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / maximum_eye_height ) ), 0.f, -( forward.z * ( v_maximum_eye_height / maximum_eye_height ) ) ),
        vec3_t( -forward.x * ( v_maximum_eye_height ), -maximum_eye_height, -forward.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( -( forward.x / 2 ) * ( v_maximum_eye_height ), maximum_eye_height, -( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( forward.x / 2 ) * ( v_maximum_eye_height ), -maximum_eye_height, -( forward.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( forward.x * ( v_maximum_eye_height / 2 ), maximum_eye_height / 2, forward.z * ( v_maximum_eye_height / 2 ) ),
        vec3_t( forward.x * v_maximum_eye_height, maximum_eye_height, forward.z * v_maximum_eye_height ),
        vec3_t( forward.x * ( v_maximum_eye_height / 2 ), -maximum_eye_height / 2, forward.z * 2.f ),
        vec3_t( forward.x * v_maximum_eye_height, -maximum_eye_height, forward.z * v_maximum_eye_height ),
        vec3_t( right.x * v_maximum_eye_height, maximum_eye_height, right.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( -right.x * ( v_maximum_eye_height ), maximum_eye_height, -right.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( right.x * v_maximum_eye_height, -maximum_eye_height, right.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( -right.x * ( v_maximum_eye_height ), -maximum_eye_height, -right.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( ( right.x / 2 ) * v_maximum_eye_height, maximum_eye_height, ( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( v_maximum_eye_height ), maximum_eye_height, -( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( v_maximum_eye_height ), -maximum_eye_height, -( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( right.x * ( v_maximum_eye_height / manip_height ), 0.f, right.z * ( v_maximum_eye_height / maximum_eye_height ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / maximum_eye_height ) ), 0.f, -( right.z * ( v_maximum_eye_height / maximum_eye_height ) ) ),
        vec3_t( right.x * ( v_maximum_eye_height / 2 ), maximum_eye_height / 2, right.z * ( v_maximum_eye_height / 2 ) ),
        vec3_t( right.x * v_maximum_eye_height, maximum_eye_height, right.z * v_maximum_eye_height ),
        vec3_t( right.x * ( v_maximum_eye_height / 2 ), -maximum_eye_height / 2, right.z * 2.f ),
        vec3_t( right.x * v_maximum_eye_height, -maximum_eye_height, right.z * v_maximum_eye_height ),
        vec3_t( forward.x * ( v_maximum_eye_height / maximum_eye_height ), 0.f, forward.z * ( v_maximum_eye_height / maximum_eye_height ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / manip_height ) ), 0.f, -( forward.z * ( v_maximum_eye_height / maximum_eye_height ) ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / 2 ) ), maximum_eye_height / 2, -( forward.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / 2 ) ), -maximum_eye_height / 2, -( forward.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / 2 ) ), maximum_eye_height, -( forward.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * ( v_maximum_eye_height / 2 ) ), -maximum_eye_height, -( forward.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * 4.f ), maximum_eye_height, -( forward.z * 4.f ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / 2 ) ), maximum_eye_height / 2, -( forward.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / 2 ) ), -maximum_eye_height / 2, -( forward.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / 2 ) ), maximum_eye_height, -( forward.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * ( maximum_eye_height / 2 ) ), -maximum_eye_height, -( forward.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( forward.x * maximum_eye_height ), manip_height, -( forward.z * maximum_eye_height ) ),
        vec3_t( -( right.x * maximum_eye_height ), 0.f, -( right.z * maximum_eye_height ) ),
        vec3_t( right.x * ( maximum_eye_height / 2 ), -manip_height / 2, right.z * ( maximum_eye_height / 2 ) ),
        vec3_t( -forward.x * ( maximum_eye_height ), manip_height, -forward.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( -forward.x * ( maximum_eye_height ), -manip_height, -forward.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( ( forward.x / 2 ) * maximum_eye_height, manip_height, ( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( ( forward.x / 2 ) * maximum_eye_height, -manip_height, ( forward.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( ( right.x / 2 ) * ( maximum_eye_height / 2 ), manip_height, ( right.z / 2 ) * ( maximum_eye_height / 2 ) ),
        vec3_t( -( ( right.x / 2 ) * ( maximum_eye_height / 2 ) ), manip_height, -( ( right.z / 2 ) * maximum_eye_height / 2 ) ),
        vec3_t( right.x * maximum_eye_height, manip_height, right.z * maximum_eye_height ),
        vec3_t( -( right.x * maximum_eye_height ), -manip_height, -( right.z * maximum_eye_height ) ),
        vec3_t( -right.x * ( maximum_eye_height ), ( v_maximum_eye_height ), -right.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( -right.x * ( maximum_eye_height ), ( -v_maximum_eye_height ), -right.z * ( maximum_eye_height ) ) * 0.9f,
        vec3_t( -( right.x / 2 ) * ( maximum_eye_height / 2 ), ( v_maximum_eye_height ), -( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * -( maximum_eye_height / 2 ), ( v_maximum_eye_height ), -( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( maximum_eye_height / 2 ), -( v_maximum_eye_height ), -( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( maximum_eye_height ), ( v_maximum_eye_height / 2 ), -( right.z / 2 ) * ( maximum_eye_height / 2 ) ),
        vec3_t( -( right.x / 2 ) * -( maximum_eye_height ), ( v_maximum_eye_height / 2 ), -( right.z / 2 ) * ( maximum_eye_height / 2 ) ),
        vec3_t( -( right.x / 2 ) * ( maximum_eye_height ), ( v_maximum_eye_height / 2 ), -( right.z / 2 ) * -( maximum_eye_height / 2 ) ),
        vec3_t( -( right.x / 2 ) * ( maximum_eye_height ), ( v_maximum_eye_height ), -( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * -( maximum_eye_height / 2 ), ( v_maximum_eye_height / 2 ), -( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( maximum_eye_height / 2 ), -( v_maximum_eye_height / 2 ), -( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * -( maximum_eye_height / 2 ), ( v_maximum_eye_height / 2 ), -( right.z / 2 ) * - ( maximum_eye_height / 2 ) ),
        vec3_t( -( right.x / 2 ) * ( maximum_eye_height ), ( v_maximum_eye_height ), -( right.z / 2 ) * ( maximum_eye_height / 2 ) ),
        vec3_t( -( right.x / 2 ) * ( maximum_eye_height ), ( v_maximum_eye_height ), -( right.z / 2 ) * -( maximum_eye_height / 2 ) ),
        vec3_t( -( right.x / 2 ) * -( maximum_eye_height / 2 ), -( v_maximum_eye_height ), -( right.z / 2 ) * ( maximum_eye_height ) ),
        vec3_t( right.x * maximum_eye_height, -manip_height, right.z * maximum_eye_height ),
        vec3_t( -( right.x * 4.f ), manip_height, -( right.z * 4.f ) ),
        vec3_t( ( right.x / 2 ) * ( maximum_eye_height / 2 ), ( v_maximum_eye_height / 1 ), ( right.z / 2 ) * ( maximum_eye_height / 2 ) ),
        vec3_t( -( ( right.x / 2 ) * ( maximum_eye_height / 2 ) ), ( v_maximum_eye_height / 1 ), -( ( right.z / 2 ) * maximum_eye_height / 2 ) ),
        vec3_t( right.x * ( maximum_eye_height / 1.5f ), 0.f, right.z * ( maximum_eye_height / 1.5f ) ),
        vec3_t( -( right.x * ( maximum_eye_height / manip_height ) ), 0.f, -( right.z * ( maximum_eye_height / 1.5f ) ) ),
        vec3_t( -( right.x * ( maximum_eye_height / 2 ) ), manip_height, -( right.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( ( right.x * ( maximum_eye_height / 2 ) ), -manip_height / 2, -( right.z * maximum_eye_height ) ),
        vec3_t( ( right.x * -( maximum_eye_height / 2 ) ), manip_height / 2, -( right.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( ( right.x * -( maximum_eye_height ) ), -manip_height, ( right.z * maximum_eye_height / 2 ) ),
        vec3_t( ( right.x * -( maximum_eye_height ) ), -manip_height, -( right.z * maximum_eye_height ) ),
        vec3_t( ( right.x * -( maximum_eye_height ) ), -( manip_height / 2 ), -( right.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( ( right.x * -( maximum_eye_height / 2 ) ), -manip_height / 2, -( right.z * maximum_eye_height ) ),
        vec3_t( -( right.x * maximum_eye_height ), -manip_height / 2, -( right.z * maximum_eye_height ) ),
        vec3_t( -( right.x * ( maximum_eye_height / 2 ) ), -manip_height / 2, -( right.z * maximum_eye_height ) ),
        vec3_t( -( right.x * ( maximum_eye_height / 2 ) ), -manip_height, -( right.z * maximum_eye_height ) ),
        vec3_t( -( right.x * maximum_eye_height ), manip_height / 2, -( right.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( maximum_eye_height / 2 ) ), -( manip_height / 2 ), -( right.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( maximum_eye_height / 2 ) ), -manip_height, -( right.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( ( right.x / 2 ) * ( maximum_eye_height / 2 ), -( v_maximum_eye_height / 2 ), ( right.z / 2 ) * ( maximum_eye_height / 2 ) ),
        vec3_t( -( ( right.x / 2 ) * ( maximum_eye_height / 2 ) ), v_maximum_eye_height, -( ( right.z / 2 ) * maximum_eye_height / 2 ) ),
        vec3_t( ( right.x / 2 ) * ( maximum_eye_height / 2 ), v_maximum_eye_height / 2, ( right.z / 2 ) * ( maximum_eye_height / 2 ) ),
        vec3_t( -( ( right.x / 2 ) * ( maximum_eye_height / 2 ) ), ( v_maximum_eye_height / 2 ), -( ( right.z / 2 ) * maximum_eye_height / 2 ) ),
        vec3_t( ( right.x / 2 ) * ( maximum_eye_height / 2 ), -v_maximum_eye_height, ( right.z / 2 ) * ( maximum_eye_height / 2 ) ),
        vec3_t( -( ( right.x / 2 ) * ( maximum_eye_height / 2 ) ), -( v_maximum_eye_height / 2 ), -( ( right.z / 2 ) * maximum_eye_height / 2 ) ),
        vec3_t( forward.x * ( v_maximum_eye_height / 2 ), manip_height / 2, forward.z * ( v_maximum_eye_height / 2 ) ),
        vec3_t( right.x * v_maximum_eye_height, manip_height, right.z * v_maximum_eye_height ),
        vec3_t( -( right.x * v_maximum_eye_height ), -manip_height, -( right.z * v_maximum_eye_height ) ),
        vec3_t( -right.x * ( v_maximum_eye_height ), ( maximum_eye_height ), -right.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( -right.x * ( v_maximum_eye_height ), ( -maximum_eye_height ), -right.z * ( v_maximum_eye_height ) ) * 0.9f,
        vec3_t( -( right.x / 2 ) * ( v_maximum_eye_height / 2 ), ( maximum_eye_height ), -( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * -( v_maximum_eye_height / 2 ), ( maximum_eye_height ), -( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( v_maximum_eye_height / 2 ), -( maximum_eye_height ), -( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( v_maximum_eye_height ), ( maximum_eye_height / 2 ), -( right.z / 2 ) * ( v_maximum_eye_height / 2 ) ),
        vec3_t( -( right.x / 2 ) * -( v_maximum_eye_height ), ( maximum_eye_height / 2 ), -( right.z / 2 ) * ( v_maximum_eye_height / 2 ) ),
        vec3_t( -( right.x / 2 ) * ( v_maximum_eye_height ), ( maximum_eye_height / 2 ), -( right.z / 2 ) * -( v_maximum_eye_height / 2 ) ),
        vec3_t( -( right.x / 2 ) * ( v_maximum_eye_height ), ( maximum_eye_height ), -( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * -( v_maximum_eye_height / 2 ), ( maximum_eye_height / 2 ), -( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * ( v_maximum_eye_height / 2 ), -( maximum_eye_height / 2 ), -( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( -( right.x / 2 ) * -( v_maximum_eye_height / 2 ), ( maximum_eye_height / 2 ), -( right.z / 2 ) * - ( v_maximum_eye_height / 2 ) ),
        vec3_t( -( right.x / 2 ) * ( v_maximum_eye_height ), ( maximum_eye_height ), -( right.z / 2 ) * ( v_maximum_eye_height / 2 ) ),
        vec3_t( -( right.x / 2 ) * ( v_maximum_eye_height ), ( maximum_eye_height ), -( right.z / 2 ) * -( v_maximum_eye_height / 2 ) ),
        vec3_t( -( right.x / 2 ) * -( v_maximum_eye_height / 2 ), -( maximum_eye_height ), -( right.z / 2 ) * ( v_maximum_eye_height ) ),
        vec3_t( right.x * v_maximum_eye_height, -manip_height, right.z * v_maximum_eye_height ),
        vec3_t( ( right.x / 2 ) * ( v_maximum_eye_height / 2 ), ( maximum_eye_height / 1 ), ( right.z / 2 ) * ( v_maximum_eye_height / 2 ) ),
        vec3_t( -( ( right.x / 2 ) * ( v_maximum_eye_height / 2 ) ), ( maximum_eye_height / 1 ), -( ( right.z / 2 ) * v_maximum_eye_height / 2 ) ),
        vec3_t( right.x * ( v_maximum_eye_height / 1.5f ), 0.f, right.z * ( v_maximum_eye_height / 1.5f ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / manip_height ) ), 0.f, -( right.z * ( v_maximum_eye_height / 1.5f ) ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), manip_height, -( right.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( ( right.x * ( v_maximum_eye_height / 2 ) ), -manip_height / 2, -( right.z * v_maximum_eye_height ) ),
        vec3_t( ( right.x * -( v_maximum_eye_height / 2 ) ), manip_height / 2, -( right.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( ( right.x * -( v_maximum_eye_height ) ), -manip_height, ( right.z * v_maximum_eye_height / 2 ) ),
        vec3_t( ( right.x * -( v_maximum_eye_height ) ), -manip_height, -( right.z * v_maximum_eye_height ) ),
        vec3_t( ( right.x * -( v_maximum_eye_height ) ), -( manip_height / 2 ), -( right.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( ( right.x * -( v_maximum_eye_height / 2 ) ), -manip_height / 2, -( right.z * v_maximum_eye_height ) ),
        vec3_t( -( right.x * v_maximum_eye_height ), -manip_height / 2, -( right.z * v_maximum_eye_height ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), -manip_height / 2, -( right.z * v_maximum_eye_height ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), -manip_height, -( right.z * v_maximum_eye_height ) ),
        vec3_t( -( right.x * v_maximum_eye_height ), manip_height / 2, -( right.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), -( manip_height / 2 ), -( right.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), -manip_height, -( right.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( ( right.x / 2 ) * ( v_maximum_eye_height / 2 ), -( maximum_eye_height / 2 ), ( right.z / 2 ) * ( v_maximum_eye_height / 2 ) ),
        vec3_t( -( ( right.x / 2 ) * ( v_maximum_eye_height / 2 ) ), maximum_eye_height, -( ( right.z / 2 ) * v_maximum_eye_height / 2 ) ),
        vec3_t( ( right.x / 2 ) * ( v_maximum_eye_height / 2 ), maximum_eye_height / 2, ( right.z / 2 ) * ( v_maximum_eye_height / 2 ) ),
        vec3_t( -( ( right.x / 2 ) * ( v_maximum_eye_height / 2 ) ), ( maximum_eye_height / 2 ), -( ( right.z / 2 ) * v_maximum_eye_height / 2 ) ),
        vec3_t( ( right.x / 2 ) * ( v_maximum_eye_height / 2 ), -maximum_eye_height, ( right.z / 2 ) * ( v_maximum_eye_height / 2 ) ),
        vec3_t( -( ( right.x / 2 ) * ( v_maximum_eye_height / 2 ) ), -( maximum_eye_height / 2 ), -( ( right.z / 2 ) * v_maximum_eye_height / 2 ) ),
        vec3_t( right.x * ( v_maximum_eye_height / 2 ), -0.7f, right.z * ( v_maximum_eye_height / 2 ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), 0.75f, -( right.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), -0.75f, -( right.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), 1.5f, -( right.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), -1.5f, -( right.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( right.x * ( maximum_eye_height / 2 ), -0.7f, right.z * ( maximum_eye_height / 2 ) ),
        vec3_t( -( forward.x * 4.f ), ( maximum_eye_height / 2 ), -( forward.z * 4.f ) ),
        vec3_t( -( forward.x * 4.f ), -( maximum_eye_height / 2 ), -( forward.z * 4.f ) ),
        vec3_t( -( forward.x * 4.f ), ( v_maximum_eye_height / 2 ), -( forward.z * 4.f ) ),
        vec3_t( -( right.x / 2.f ), ( v_maximum_eye_height / 2 ), -( right.z * 4.f ) ),
        vec3_t( -( right.x * 4.f ), ( v_maximum_eye_height / 2 ), -( right.z / 2.f ) ),
        vec3_t( -( right.x / 2.f ), ( v_maximum_eye_height / 2 ), -( right.z / 2.f ) ),
        vec3_t( -( right.x ), ( v_maximum_eye_height / 2 ), -( right.z * 4.f ) ),
        vec3_t( -right.x, -( v_maximum_eye_height / 2 ), -( right.z * 4.f ) ),
        vec3_t( -( forward.x * 4.f ), -( v_maximum_eye_height / 2 ), -( forward.z * 4.f ) ),
        vec3_t( -( right.x / 2.f ), -( v_maximum_eye_height / 2 ), -( right.z * 4.f ) ),
        vec3_t( -( right.x * 4.f ), -( v_maximum_eye_height / 2 ), -( right.z / 2.f ) ),
        vec3_t( -( right.x / 2.f ), -( v_maximum_eye_height / 2 ), -( right.z / 2.f ) ),
        vec3_t( -( right.x ), -( v_maximum_eye_height / 2 ), -( right.z * 4.f ) ),
        vec3_t( -( right.x * ( maximum_eye_height / 2 ) ), 0.75f, -( right.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( maximum_eye_height / 2 ) ), -0.75f, -( right.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( maximum_eye_height / 2 ) ), 1.5f, -( right.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( maximum_eye_height / 2 ) ), -1.5f, -( right.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( right.x * ( maximum_eye_height / v_maximum_eye_height ), 0.f, right.z * ( maximum_eye_height / v_maximum_eye_height ) ),
        vec3_t( -( right.x * ( maximum_eye_height / manip_height ) ), 0.f, -( right.z * ( maximum_eye_height / v_maximum_eye_height ) ) ),
        vec3_t( right.x * ( v_maximum_eye_height / v_maximum_eye_height ), 0.f, right.z * ( v_maximum_eye_height / v_maximum_eye_height ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / manip_height ) ), 0.f, -( right.z * ( v_maximum_eye_height / v_maximum_eye_height ) ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), v_maximum_eye_height / 2, -( right.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), -v_maximum_eye_height / 2, -( right.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), v_maximum_eye_height, -( right.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), -v_maximum_eye_height, -( right.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * 4.f ), v_maximum_eye_height, -( right.z * 4.f ) ),
        vec3_t( -( right.x * ( maximum_eye_height / 2 ) ), v_maximum_eye_height, -( right.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( maximum_eye_height / 2 ) ), -v_maximum_eye_height, -( right.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( right.x * ( maximum_eye_height / maximum_eye_height ), 0.f, right.z * ( maximum_eye_height / maximum_eye_height ) ),
        vec3_t( -( right.x * ( maximum_eye_height / manip_height ) ), 0.f, -( right.z * ( maximum_eye_height / maximum_eye_height ) ) ),
        vec3_t( right.x * ( v_maximum_eye_height / maximum_eye_height ), 0.f, right.z * ( v_maximum_eye_height / maximum_eye_height ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / manip_height ) ), 0.f, -( right.z * ( v_maximum_eye_height / maximum_eye_height ) ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), maximum_eye_height / 2, -( right.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), -maximum_eye_height / 2, -( right.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), maximum_eye_height, -( right.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( v_maximum_eye_height / 2 ) ), -maximum_eye_height, -( right.z * ( v_maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( maximum_eye_height / 2 ) ), maximum_eye_height / 2, -( right.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( maximum_eye_height / 2 ) ), -maximum_eye_height / 2, -( right.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( maximum_eye_height / 2 ) ), maximum_eye_height, -( right.z * ( maximum_eye_height / 2 ) ) ),
        vec3_t( -( right.x * ( maximum_eye_height / 2 ) ), -maximum_eye_height, -( right.z * ( maximum_eye_height / 2 ) ) ),
    };

    if( options::aimbot::generate_points ) {
        for( int i = 0; i < options::aimbot::manipulation_angles; i++ )
        {
            // reaching out of limit.
            if( i > _array.size( ) )
                break;

            vec3_t current_pos = _array.at( i );
            if( current_pos.y > maximum_eye_height )
                continue;

            vec3_t point = re_p + current_pos;

            bool is_player_visible = g_sdk.is_visible( head_pos, point, eye_losradius, layers );
            if( options::aimbot::bullet_tp
                && !bullet_tp_target.is_zero( ) )
                is_player_visible = g_sdk.is_visible( bullet_tp_target, point, eye_losradius, layers );

            if( !g_sdk.is_visible( point, re_p, eye_losradius, layers )
                || !is_player_visible
                || !validate_eye_pos( point )
                /*|| !validate_eye_pos( re_p )*/ )
                continue;

            if( options::aimbot::visualize_manipulation )
                g_render.ddraw_line( re_p, point, color_t( 255, 255, 255 ), 5.f );

            eye_manipulation = point;
            choice = current_pos;
            if( !choice.is_zero( ) )
                break;
        }
    }
    else {
        for( int i = 0; i < options::aimbot::manipulation_angles; i++ )
        {
            // reaching out of limit.
            if( i > angle_amt )
                break;

            vec3_t current_pos = arr.at( i );
            if( current_pos.y > maximum_eye_height )
                continue;

            vec3_t point = re_p + current_pos;

            bool is_player_visible = g_sdk.is_visible( head_pos, point, eye_losradius, layers );
            if( options::aimbot::bullet_tp
                && !bullet_tp_target.is_zero( ) )
                is_player_visible = g_sdk.is_visible( bullet_tp_target, point, eye_losradius, layers );

            if( !g_sdk.is_visible( point, re_p, eye_losradius, layers )
                || !is_player_visible
                || !validate_eye_pos( point )
                /*|| !validate_eye_pos( re_p )*/ )
                continue;

            if( options::aimbot::visualize_manipulation )
                g_render.ddraw_line( re_p, point, color_t( 255, 255, 255 ), 5.f );

            eye_manipulation = point;
            choice = current_pos;
            if( !choice.is_zero( ) )
                break;
        }
    }
    if( choice.is_zero( ) ) {
        manipulation_angle.clear( );
        return;
    }

    manipulation_angle = choice;
}

void c_esp::invoke_chams( base_player* player )
{
    static uintptr_t shader = { };
    static uintptr_t type_obj = g_il2mgr.type_object( "UnityEngine", "Shader" );
    switch( options::visuals::chams_type ) {
    case 0:
        shader = g_sdk.load_asset( g_cheat.asset_bundle, xs( L"chams.shader" ), type_obj );
        break;
    case 1:
        shader = g_sdk.load_asset( g_cheat.chams_bundle, xs( L"hologramshader.shader" ), type_obj );
        break;
    case 2:
        shader = g_sdk.load_asset( g_cheat.chams_bundle, xs( L"invisible.shader" ), type_obj );
        break;
    case 3:
        shader = g_sdk.load_asset( g_cheat.chams_bundle, xs( L"assets\\sinevfx\\galaxymaterials\\resources\\shaders\\galaxymaterial.shader" ), type_obj );
        break;
    case 4:
        shader = g_sdk.load_asset( g_cheat.chams_bundle, xs( L"chams.shader" ), type_obj );
        break;
    case 5:
        shader = g_sdk.load_asset( g_cheat.wireframe_chams_bundle, xs( L"assets\\bundledassets\\wireframetransparentculled.shader" ), type_obj );
        break;
    case 6:
        shader = g_sdk.load_asset( g_cheat.circuit_chams_bundle, xs( L"nebnel.shader" ), type_obj );
        break;
    }
    if( !shader )
        return;

    player_model* model = player->get_model( );
    if( !model )
        return;

    uintptr_t multi_mesh = model->get_multimesh( );
    if( !multi_mesh )
        return;

    laddy_list< uintptr_t >* renderers = g_sdk.get2renderers( multi_mesh );
    if( !renderers )
        return;

    for( int i = 0; i < renderers->get_size( ); i++ )
    {
        uintptr_t renderer = renderers->get_value( i );
        if( !renderer )
            return;

        const uintptr_t material = g_sdk.get_material( renderer );
        if( !material )
            return;

        if( g_sdk.get_shader( material ) != shader )
            g_sdk.set_shader( material, shader );

        if( options::visuals::chams_type == 5 ) {
            g_sdk.set_color( material, xs( L"_WireColor" ), 
                color_t( options::visuals::chams_color.r * 255.f, options::visuals::chams_color.g * 255.f, options::visuals::chams_color.b * 255.f ) );

            g_sdk.set_color( material, xs( L"_BaseColor" ), 
                color_t( options::visuals::chams_color.r * 255.f, options::visuals::chams_color.g * 255.f, options::visuals::chams_color.b * 255.f ) );

            g_sdk.set_material_int( material, xs( L"_WireThickness" ), 
                1 );
        }

        if( options::visuals::chams_type == 6 ) {
            g_sdk.set_color( material, xs( L"_Color" ), 
                color_t( options::visuals::chams_color.r * 255.f, options::visuals::chams_color.g * 255.f, options::visuals::chams_color.b * 255.f ) );
        }
        if( options::visuals::chams_type == 1 || options::visuals::chams_type == 3 ) {
            g_sdk.set_color( material, xs( L"_InvisCol" ), 
                options::visuals::chams_color );

            g_sdk.set_color( material, xs( L"_VisCol" ), 
                options::visuals::chams_visible );
        }
        if( options::visuals::chams_type == 0
            || options::visuals::chams_type == 4 ) {
            g_sdk.set_color( material, xs( L"_ColorBehind" ), 
                options::visuals::chams_color );

            g_sdk.set_color( material, xs( L"_ColorVisible" ), 
                options::visuals::chams_visible );
        }
    }

    //uintptr_t skin_set = *reinterpret_cast< uintptr_t* >( ( uintptr_t )model + 0x140 );
    //if( !skin_set ) {
    //    skin_set = *reinterpret_cast< uintptr_t* >( ( uintptr_t )model + 0x148 );
    //    return;
    //}
            
    //uintptr_t skins = *reinterpret_cast< uintptr_t* >( skin_set + 0x18 );
    //if( !skins )
    //    return;

    //static uintptr_t sky = g_il2mgr.init_class( "TOD_Sky" );
    //if( !sky )
    //    return;

    //uintptr_t components = *reinterpret_cast< uintptr_t* >( sky + 0xA8 );
    //if( !components )
    //    return;

    //uintptr_t scattering = *reinterpret_cast< uintptr_t* >( components + 0x154 );
    //if( !scattering )
    //    return;

    //int size = *reinterpret_cast< int* >( skins + 0x18 );

    //for ( int idx = 0; idx < size; idx++ )
    //{
    //    uintptr_t skin_set = *reinterpret_cast< uintptr_t* >( skins + 0x20 + ( idx * sizeof( uint64_t ) ) );
    //    if( !skin_set )
    //        continue;

    //    *reinterpret_cast< uintptr_t* >( skin_set + 0x68 ) = scattering; // SkinSet HeadMaterial
    //    *reinterpret_cast< uintptr_t* >( skin_set + 0x70 ) = scattering; // SkinSet BodyMaterial
    //    *reinterpret_cast< uintptr_t* >( skin_set + 0x78 ) = scattering; // SkinSet EyeMaterial
    //}
}

void c_esp::draw_health_text(core::base_player* player) {
    std::string hp_str = std::to_string(static_cast<int>(player->get_health())) + "hp";

    g_render.draw_text(vec4_t(bbox.center, bbox.bottom, bbox.half, 30),
        hp_str, health_text_color,
        true, 11, font_flags);
}

void c_esp::draw_health( base_player* player )
{
    float health = min( 100.f, static_cast< float >( player->get_health( ) ) );

    float box_height = bbox.bottom - bbox.top;

    // calculate hp bar color.
    const float calculate_col = ( std::clamp< float >( health, 25, 75 ) - 25.f ) / 50.f;

    color_t color = color_t( 120.f + ( 135.f * ( 1.f - calculate_col ) ), 50.f + ( 175.f * calculate_col ), 80 );
    if( options::visuals::modify_health_color )
        color = custom_health_color/*.multiply( color_t( 255, 15, 15 ), ( health / 100.f ) )*/;

    // get hp bar height.
	int fill = static_cast< int >( std::round( health * box_height / 100.f ) );

    int thickness = options::visuals::health_thickness;

    g_render.draw_filled_rect( vec4_t( bbox.left - ( 5 + thickness ), bbox.top - 1, thickness + 2, box_height + 3 ), color_t( 0, 0, 0, 150 ) );

    //if( options::visuals::modify_health_color )
    //    g_render.draw_gradient( bbox.left - ( 4 + thickness ), bbox.top + box_height - fill, thickness, fill + 1, color, color_t( 255, 15, 15 ) );
    //else
	g_render.draw_filled_rect( vec4_t( bbox.left - ( 4 + thickness ), bbox.top + box_height - fill, thickness, fill + 1 ), color );
}

void c_esp::draw_hotbar( base_player* target )
{
    if( !target
        || target->get_life_state( ) )
        return;

    // check if we have menu open.
	if( g_framework.in_alpha ) {
		vec3_t cursor = g_cheat.mouse_pos;

		if( g_sdk.get_key( key_code::Mouse0 ) 
			&& g_framework.is_hovering( hotbar_position, vec2_t( info_bar_size.x, 25 ) ) )
		{
			cursor.y = g_cheat.screen_size.y - cursor.y;

			hotbar_position.x = cursor.x - info_bar_size.x / 2.f;
			hotbar_position.y = cursor.y - 10.f;
		}
	}

    float info_y = 0;

    std::wstring name = std::wstring( );

    player_model* model = target->get_model( );
    if( !model )
        return;

    std::vector< item* > belt = target->get_hotbar_items( );

    if( belt.empty( ) )
        return;

    name = model->is_npc( ) ? xs( L"npc" ) : target->get_username( ).substr( 0, 14 );

    std::wstring text = name + xs( L"'s hotbar" );

    g_render.draw_filled_rect( hotbar_position, info_bar_size, g_framework.outline_color );
    g_render.outline_box( hotbar_position, info_bar_size, options::accent_color.alpha( 255, true ) );

    g_render.draw_text( vec4_t( hotbar_position.x + 11, hotbar_position.y, 150, 30 ), text.c_str( ), color_t( 255, 255, 255 ), true, 11, font_flags );

    color_t color = color_t( 220, 220, 220 );

    item* held_item = target->get_held_item( );

    for( size_t i = 0; i < belt.size( ); i++ )
    {
        item* item = belt.at( i );
        if( !item )
            continue;

        int item_amount = item->get_amount( );
	    if( item_amount > 1000 )
            item_amount = 1000;

        std::wstring weapon_name = item->get_weapon_name( );
        if( weapon_name.empty( ) )
            continue;

        if( held_item 
            && item == held_item )
            color = color_t( 255, 255, 255 );

        std::wstring text = weapon_name;
        if( item_amount > 1 )
            text += xs( L" x" ) + std::to_wstring( item_amount );

        std::transform( text.begin( ), text.end( ), text.begin( ), ::tolower );

        g_render.draw_text( vec4_t( hotbar_position.x + 11, ( hotbar_position.y + 25 ) + info_y, 150, 40 ), text.c_str( ), color, true, 11, font_flags );
        info_y += 13;
    }

    info_y = 0; // whaat...
}

void c_esp::draw_clothes( base_player* target )
{
    if( !target
        || target->get_life_state( ) )
        return;

    float info_y = 0;

    // check if we have menu open.
	if( g_framework.in_alpha ) {
		vec3_t cursor = g_cheat.mouse_pos;

		if( g_sdk.get_key( key_code::Mouse0 ) 
			&& g_framework.is_hovering( clothes_position, vec2_t( info_bar_size.x, 25 ) ) )
		{
			cursor.y = g_cheat.screen_size.y - cursor.y;

			clothes_position.x = cursor.x - info_bar_size.x / 2.f;
			clothes_position.y = cursor.y - 10.f;
		}
	}

    std::wstring name = std::wstring( );

    player_model* model = target->get_model( );
    if( !model )
        return;

    std::vector< item* > wear = target->get_wear_items( );
    if( wear.empty( ) )
        return;

    name = model->is_npc( ) ? xs( L"npc" ) : target->get_username( ).substr( 0, 14 );

    std::wstring text = name + xs( L"'s clothes" );

    g_render.draw_filled_rect( clothes_position, info_bar_size, g_framework.outline_color );
    g_render.outline_box( clothes_position, info_bar_size, options::accent_color.alpha( 255, true ) );

    g_render.draw_text( vec4_t( clothes_position.x + 11, clothes_position.y, 150, 30 ), text.c_str( ), color_t( 255, 255, 255 ), true, 11, font_flags );

    color_t color = color_t( 255, 255, 255 );

    for( size_t i = 0; i < wear.size( ); i++ )
    {
        item* item = wear.at( i );
        if( !item )
            continue;

        int item_amount = item->get_amount( );
	    if( item_amount > 1000 )
            item_amount = 1000;

        std::wstring item_name = item->get_weapon_name( );
        if( item_name.empty( ) )
            continue;

        std::wstring text = item_name;
        if( item_amount > 1 )
            text += xs( L" x" ) + std::to_wstring( item_amount );

        std::transform( text.begin( ), text.end( ), text.begin( ), ::tolower );

        g_render.draw_text( vec4_t( clothes_position.x + 11, ( clothes_position.y + 25 ) + info_y, 150, 40 ), text.c_str( ), color, true, 11, font_flags );
        info_y += 13;
    }

    info_y = 0; // whaat...
}

void c_esp::draw_box( base_player* player ) {
    float box_width = bbox.right - bbox.left;
    float box_height = bbox.bottom - bbox.top;
                         
    color_t dropshadow_color = color_t( 0, 0, 0, 150 );

    float box_left_x = bbox.left - 1;
    float box_right_x = bbox.right - 1;

    auto draw_corner_box = [ & ] {
        /* dropshadow */

        // horizontal line ----

        //// left
        //g_render.horizontal_line( vec2_t( box_left_x - 1.f, bbox.top - 1.f ), box_width / 4 + 1, dropshadow_color );

        //// right
        //g_render.horizontal_line( vec2_t( box_right_x + 1.f, bbox.top - 1.f ), -( box_width / 4 + 1 ), dropshadow_color );

        //// vertical line |

        //// left
        //g_render.vertical_line( vec2_t( box_left_x - 1.f, bbox.top ), box_height / 4, dropshadow_color );

        //// right
        //g_render.vertical_line( vec2_t( box_right_x + 1.f, bbox.top - 1.f ), box_height / 4 + 1, dropshadow_color );

        ///* bottom */

        //// horizontal line ----

        //// left
        //g_render.horizontal_line( vec2_t( box_left_x - 1.f, bbox.bottom + 1.f ), box_width / 4 + 1, dropshadow_color );

        //// right
        //g_render.horizontal_line( vec2_t( box_right_x + 2.f, bbox.bottom + 1.f ), -( box_width / 4 + 2 ), dropshadow_color );

        //// vertical line |

        //// left
        //g_render.vertical_line( vec2_t( box_left_x - 1.f, bbox.bottom + 1.f ), -( box_height / 4 + 1 ), dropshadow_color );

        //// right
        //g_render.vertical_line( vec2_t( box_right_x + 1.f, bbox.bottom + 1.f ), -( box_height / 4 + 1 ), dropshadow_color );

        // main part

        // horizontal line ----
        // left
        g_render.horizontal_line( vec2_t( box_left_x, bbox.top ), box_width / 4, box_color );

        // right
        g_render.horizontal_line( vec2_t( box_right_x, bbox.top ), -( box_width / 4 ), box_color );

        // vertical line |
        // left
        g_render.vertical_line( vec2_t( box_left_x, bbox.top ), box_height / 4, box_color );

        // right
        g_render.vertical_line( vec2_t( box_right_x, bbox.top ), box_height / 4, box_color );

        // lower part

        /* bottom */

        // horizontal line ----
        // left
        g_render.horizontal_line( vec2_t( box_left_x, bbox.bottom ), box_width / 4, box_color );

        // right
        g_render.horizontal_line( vec2_t( box_right_x, bbox.bottom ), -( box_width / 4 ), box_color );

        // vertical line |
        // left
        g_render.vertical_line( vec2_t( box_left_x, bbox.bottom ), -( box_height / 4 ), box_color );

        // right
        g_render.vertical_line( vec2_t( box_right_x, bbox.bottom + 1.f ), -( box_height / 4 + 1 ), box_color );
    };

    switch( options::visuals::box_type ) {
    case 1:
        g_render.outline_box( 
            vec2_t( bbox.left - 2, bbox.top - 1 ),
            vec2_t( box_width + 2, box_height + 2 ),
            color_t( 0, 0, 0, 150 ) );

        g_render.outline_box( 
            vec2_t( bbox.left - 1, bbox.top ),
            vec2_t( box_width, box_height ),
            box_color );
        break;
    case 2:
        draw_corner_box( );
        break;
    default:
        break; // :shrug:
    }
}

void c_esp::draw_snap_lines( base_player* player ) {
    vec3_t bone_pos = player->get_bone_position( g_aimbot.aimbot_bone );
    vec2_t pos = g_sdk.world_to_screen( bone_pos );
    if( pos.is_zero( ) )
        return;

    player_model* model = player->get_model( );
    if( !model )
        return;

    if( player == g_aimbot.target 
        && options::aimbot::draw_target ) {
        g_render.draw_line( vec2_t( g_cheat.screen_size.x / 2, g_cheat.screen_size.y / 2 ), pos, options::aimbot::target_line_color );
    }
    else if( options::visuals::show_lines ) {
        g_render.draw_line( vec2_t( g_cheat.screen_size.x / 2, g_cheat.screen_size.y ), vec2_t( bbox.left + ( ( bbox.right - bbox.left ) / 2 ), bbox.bottom ), options::visuals::snap_lines_color );
    }
}

void c_esp::draw_view_direction( base_player* player )
{
    if( !options::visuals::show_view_direction )
        return;

    player_model* model = player->get_model( );
    if( !model )
        return;

    base_player* local = g_cheat.local;
    if( !local )
        return;

    player_eyes* eyes = player->get_eyes( );
    if( !eyes )
        return;

    vec3_t head_pos = player->get_bone_position( head );
    vec2_t head = g_sdk.world_to_screen( head_pos );

    vec2_t forward = g_sdk.world_to_screen( head_pos + ( eyes->get_bodyforward( ) * 2.f ) );

    if( head.is_zero( )
        || forward.is_zero( ) )
        return;

    g_render.draw_line( vec2_t( head.x, head.y ), vec2_t( forward.x, forward.y ), snap_lines_color );
}

void c_esp::draw_out_of_fov_arrows( base_player* player )
{
    base_player* local = g_cheat.local;
    if( !local )
        return;

    player_model* model = player->get_model( );
    if( !model )
        return;

    player_eyes* eyes = g_cheat.local_eyes;
    if( !eyes )
        return;

    vec2_t center = g_cheat.screen_center;

    vec3_t local_pos = g_cheat.local_pos;
    vec3_t player_pos = model->get_position( );
    if( local_pos.is_zero( ) 
        || player_pos.is_zero( ) )
        return;

    vec3_t euler_angles = g_math.to_euler_angles( eyes->get_rotation( ) );

    const vec2_t position = vec2_t( local_pos.x - player_pos.x, local_pos.z - player_pos.z ).normalize( );

    float angle = atan2( position.x, position.y ) * 57.29578f - 180.f - euler_angles.y;

    vec2_t pos_0 = g_math.cos_tan_horizontal( angle, 10.f, center.x, center.y, 140 );
    vec2_t pos_1 = g_math.cos_tan_horizontal( angle + 2.f, 10.f, center.x, center.y, 130 );
    vec2_t pos_2 = g_math.cos_tan_horizontal( angle - 2.f, 10.f, center.x, center.y, 130 );

    g_render.gl_triangle( vec2_t( pos_0.x + 1, pos_0.y + 1 ), vec2_t( pos_1.x + 1, pos_1.y + 1 ), vec2_t( pos_2.x + 1, pos_2.y + 1 ), color_t( 0, 0, 0, 150 ) );
    g_render.gl_triangle( vec2_t( pos_0.x, pos_0.y ), vec2_t( pos_1.x, pos_1.y ), vec2_t( pos_2.x, pos_2.y ), oof_color );
}

void c_esp::draw_knocked_flag( base_player* player ) {
    if( !player->is_knocked( ) )
        return;

    g_render.draw_text( vec4_t( bbox.center, bbox.top - 25.f - ( esp_spacing[ 3 ] + esp_spacing[ 4 ] ), bbox.half, 30 ),
        xs( L"^knocked^" ), knocked_color,
        true, 11, font_flags );
}

void c_esp::draw_ducking_flag( base_player* player ) {
    std::string name = std::string( );

    model_state* model_state = player->get_modelstate( );
    if( !model_state ) {
        esp_spacing[ 4 ] = 0;
        return;
    }

    if( !model_state->get_ducked( ) 
        || player->is_sleeping( )
        || player->is_knocked( ) ) {
        esp_spacing[ 4 ] = 0;
        return;
    }

    g_render.draw_text( vec4_t( bbox.center, bbox.top - 25.f - esp_spacing[ 3 ], bbox.half, 30 ), 
        xs( L"*ducking*" ), ducking_color,
        true, 11, font_flags );

    if( esp_spacing[ 4 ] < 12 )
        esp_spacing[ 4 ] = 12;
}

void c_esp::draw_name( base_player* player, entity_type type ) {
    std::wstring name = std::wstring( );

    player_model* model = player->get_model( );
    if( !model )
        return;

    if( type == scientist )
        name  = xs( L"scientist" );
    else if( type == tunnel_dweller )
        name  = xs( L"tunnel dweller" );
    else if( model->is_npc( ) )
        name = xs( L"npc" );
    else
        name = player->get_username( );

    std::wstring new_name = std::wstring( );

    new_name = name;

    /* added to friend list. */
	for( auto& targets : m_friends )
	{
        if( player == targets )
            new_name = xs( L"[f]" ) + name;
    }

    new_name.substr( 0, 18 ); // limit to 18 chars.

    g_render.draw_text( vec4_t( bbox.center, bbox.top - 25.f, bbox.half, 30 ), 
        new_name.c_str( ), name_color,
        true, 11, font_flags );

    if( esp_spacing[ 3 ] < 12 )
        esp_spacing[ 3 ] = 12;
}

void c_esp::draw_distance(base_player* player) {
    std::wstring dist_str = std::to_wstring(static_cast<int>(g_cheat.local_pos.distance(player->get_model()->get_position()))) + xs(L"m");

    g_render.draw_text(vec4_t(bbox.center, bbox.bottom + (esp_spacing[0] + esp_spacing[1]), bbox.half, 30),
        dist_str.c_str(), distance_color,
        true, 11, font_flags);

    if (esp_spacing[2] < 12)
        esp_spacing[2] = 12;
}

void c_esp::draw_bones( base_player* player )
{
    vec3_t head_pos = player->get_bone_position( head );
	vec3_t spine4_pos = player->get_bone_position( spine4 );
	vec3_t l_clavicle_pos = player->get_bone_position( l_clavicle );
	vec3_t l_upperarm_pos = player->get_bone_position( l_upperarm );
	vec3_t l_forearm_pos = player->get_bone_position( l_forearm );
	vec3_t l_hand_pos = player->get_bone_position( l_hand );
	vec3_t r_clavicle_pos = player->get_bone_position( r_clavicle );
	vec3_t r_upperarm_pos = player->get_bone_position( r_upperarm );
	vec3_t r_forearm_pos = player->get_bone_position( r_forearm );
	vec3_t r_hand_pos = player->get_bone_position( r_hand );
	vec3_t pelvis_pos = player->get_bone_position( pelvis );
	vec3_t l_hip_pos = player->get_bone_position( l_hip );
	vec3_t l_knee_pos = player->get_bone_position( l_knee );
	vec3_t l_ankle_scale_pos = player->get_bone_position( l_ankle_scale );
	vec3_t l_foot_pos = player->get_bone_position( l_foot );
	vec3_t r_hip_pos = player->get_bone_position( r_hip );
	vec3_t r_knee_pos = player->get_bone_position( r_knee );
	vec3_t r_ankle_scale_pos = player->get_bone_position( r_ankle_scale );
	vec3_t r_foot_pos = player->get_bone_position( r_foot );

	vec2_t head = g_sdk.world_to_screen( head_pos );
    vec2_t spine = g_sdk.world_to_screen( spine4_pos ); 
    vec2_t l_clavicle = g_sdk.world_to_screen( l_clavicle_pos );
    vec2_t l_upperarm = g_sdk.world_to_screen( l_upperarm_pos );
    vec2_t l_forearm = g_sdk.world_to_screen( l_forearm_pos );
    vec2_t l_hand = g_sdk.world_to_screen( l_hand_pos );
    vec2_t r_clavicle = g_sdk.world_to_screen( r_clavicle_pos );
    vec2_t r_upperarm = g_sdk.world_to_screen( r_upperarm_pos );
    vec2_t r_forearm = g_sdk.world_to_screen( r_forearm_pos ); 
    vec2_t r_hand = g_sdk.world_to_screen( r_hand_pos ); 
    vec2_t pelvis = g_sdk.world_to_screen( pelvis_pos );
    vec2_t l_hip = g_sdk.world_to_screen( l_hip_pos );
    vec2_t l_knee = g_sdk.world_to_screen( l_knee_pos );
    vec2_t l_ankle_scale = g_sdk.world_to_screen( l_ankle_scale_pos );
    vec2_t l_foot = g_sdk.world_to_screen( l_foot_pos ); 
    vec2_t r_hip = g_sdk.world_to_screen( r_hip_pos );
    vec2_t r_knee = g_sdk.world_to_screen( r_knee_pos );
    vec2_t r_ankle_scale = g_sdk.world_to_screen( r_ankle_scale_pos ); 
    vec2_t r_foot = g_sdk.world_to_screen( r_foot_pos );

	if( head.is_zero( )
        || spine.is_zero( )
        || l_clavicle.is_zero( )
        || l_upperarm.is_zero( )
        || l_forearm.is_zero( )
        || l_hand.is_zero( )
        || r_clavicle.is_zero( )
        || r_upperarm.is_zero( )
        || r_forearm.is_zero( )
        || r_hand.is_zero( )
        || pelvis.is_zero( )
		|| l_hip.is_zero( )
        || l_knee.is_zero( )
        || l_ankle_scale.is_zero( )
        || l_foot.is_zero( )
        || r_hip.is_zero( )
        || r_knee.is_zero( )
        || r_ankle_scale.is_zero( )
        || r_foot.is_zero( ) )
        return;

    player_model* model = player->get_model( );
    if( !model )
        return;

    vec3_t vis = model->get_position( );

    g_render.draw_line( vec2_t( head.x, head.y ), vec2_t( spine.x, spine.y ), skeleton_color );
    g_render.draw_line( vec2_t( spine.x, spine.y ), vec2_t( l_upperarm.x, l_upperarm.y ), skeleton_color );
    g_render.draw_line( vec2_t( l_upperarm.x, l_upperarm.y ), vec2_t( l_forearm.x, l_forearm.y ), skeleton_color );
    g_render.draw_line( vec2_t( l_forearm.x, l_forearm.y ), vec2_t( l_hand.x, l_hand.y ), skeleton_color );
    g_render.draw_line( vec2_t( spine.x, spine.y ), vec2_t( r_upperarm.x, r_upperarm.y ), skeleton_color );
    g_render.draw_line( vec2_t( r_upperarm.x, r_upperarm.y ), vec2_t( r_forearm.x, r_forearm.y ), skeleton_color );
    g_render.draw_line( vec2_t( r_forearm.x, r_forearm.y ), vec2_t( r_hand.x, r_hand.y ), skeleton_color );
    g_render.draw_line( vec2_t( spine.x, spine.y ), vec2_t( pelvis.x, pelvis.y ), skeleton_color );
    g_render.draw_line( vec2_t( pelvis.x, pelvis.y ), vec2_t( l_hip.x, l_hip.y ), skeleton_color );
    g_render.draw_line( vec2_t( l_hip.x, l_hip.y ), vec2_t( l_knee.x, l_knee.y ), skeleton_color );
    g_render.draw_line( vec2_t( l_knee.x, l_knee.y ), vec2_t( l_foot.x, l_foot.y ), skeleton_color );
    g_render.draw_line( vec2_t( pelvis.x, pelvis.y ), vec2_t( r_hip.x, r_hip.y ), skeleton_color );
    g_render.draw_line( vec2_t( r_hip.x, r_hip.y ), vec2_t( r_knee.x, r_knee.y ), skeleton_color );
    g_render.draw_line( vec2_t( r_knee.x, r_knee.y ), vec2_t( r_foot.x, r_foot.y ), skeleton_color );
}

void c_esp::draw_weapon(base_player* player) {
    item* held_item = player->get_held_item();
    if (!held_item) {
        esp_spacing[0] = 0;
        return;
    }

    std::wstring weapon_name = held_item->get_weapon_name();

    std::transform(weapon_name.begin(), weapon_name.end(), weapon_name.begin(), ::tolower);

    if (options::visuals::show_weapon) {
        g_render.draw_text(vec4_t(bbox.center, bbox.bottom + esp_spacing[1], bbox.half, 30),
            weapon_name.c_str(), weapon_color,
            true, 11, font_flags);

        if (esp_spacing[0] < 12)
            esp_spacing[0] = 12;
    }
    else if (!options::visuals::show_weapon
        || weapon_name.empty())
        esp_spacing[0] = 0;
}

void c_esp::reset_fly_hack( ) {
    distance_vertical = distance_horizontal =
    pause_time = current_horizontal_fly = 0.f;
    current_vertical_fly = 0.f;
}

void c_esp::apply_fly_violation( )
{
    base_player* local = g_cheat.local;
    if ( !local
        || local
        && ( local->get_life_state( ) || local->get_on_ladder( ) || local->is_sleeping( ) ) )
        return reset_fly_hack( );

    player_ticks* last_sent_tick = local->get_lastSentTick( );
    if ( !last_sent_tick )
        return reset_fly_hack( );

    vec3_t last_position = last_sent_tick->get_position( );
    if ( last_position.is_zero( ) )
        return reset_fly_hack( );

    vec3_t position = g_cheat.local_pos;
    if ( position.is_zero( ) )
        return reset_fly_hack( );

    player_walk_movement* movement = local->get_movement( );
    if( !movement )
        return reset_fly_hack( );

    if ( movement->swimming( ) )
        return reset_fly_hack( );

    float jump_height = local->get_jumpheight( );
    if( !jump_height )
        return reset_fly_hack( );

    vec3_t vector = ( last_position + position ) * 0.5f;
    if ( g_game.water_level_test( vector - vec3_t( 0.f, flyhack_extrusion, 0.f ), true, true, local ) )
        return reset_fly_hack( );

    if( local->get_mountable( ) )
        return reset_fly_hack( );

    float radius = local->get_radius( );
    float height = local->get_height( false );
    vec3_t vector2 = vector + vec3_t( 0.f, radius - flyhack_extrusion, 0.f );
    vec3_t vector3 = vector + vec3_t( 0.f, height - radius, 0.f );
    float radius2 = radius - flyhack_margin;

    if ( !g_sdk.physics_checkcapsule( vector2, vector3, radius2, 1503731969, QueryTriggerInteraction::Ignore ) )
    {
        bool flag = false;
        vec3_t vector4 = position - last_position;
        float num2 = std::abs( vector4.y );
        float num3 = vector4.length2d( vector4 );
        if ( vector4.y >= 0.f )
            current_vertical_fly += vector4.y;

        if ( num2 < num3 )
            current_horizontal_fly += num3;
    }
    else
    {
        current_vertical_fly = 0.f;
        current_horizontal_fly = 0.f;
    }

    pause_time = max( 0.f, pause_time - g_sdk.get_deltaTime( ) );

    float num4 = max( ( pause_time > 0.f ) ? flyhack_forgiveness_vertical_inertia : flyhack_forgiveness_vertical, 0.f );
    max_vertical_fly = jump_height + num4;
	float num6 = max( ( pause_time > 0.f ) ? flyhack_forgiveness_horizontal_inertia : flyhack_forgiveness_horizontal, 0.f );
    max_horizontal_fly = 5.f + num6;
}

void c_esp::show_reload_bar( ) {
    if( !options::visuals::show_reload_bar ) {
        indicators_spacing[ 0 ] = 0.f;
        return;
    }

    base_player* local_player = g_cheat.local;
    if( !local_player ) {
        indicators_spacing[ 0 ] = 0.f;
        return;
    }

    item* held_item = local_player->get_held_item( );
    if( !held_item ) {
        indicators_spacing[ 0 ] = 0.f;
        return;
    }   

    float max_size = 130;
    float red{ }, green{ }, x{ };

    static float alpha{ };

    if( held_item->has_reload_cooldown( ) ) {
        float reload_dur = held_item->get_next_reload_time( ) - g_cheat.global_time;
        float reload_dur_total = held_item->calculate_cooldown_time( held_item->get_next_reload_time( ), held_item->get_reload_time( ) ) - g_cheat.global_time;

        float percentage = ( ( ( reload_dur / reload_dur_total ) * 100.0f ) + 1.f ) / 100;

        float num = percentage;

		if( num ) {
            num = std::clamp( num, 0.f, 1.f );
		}

        x = ( num * max_size );

        x = std::clamp( x, 1.f, max_size );

        if( alpha < 1.f )
            alpha = std::lerp( alpha, 1.f, 0.15f );
    }
    else {
        alpha = std::lerp( alpha, 0.f, 0.15f );
    }

    if( alpha > 0.15f ) {
        if( indicators_spacing[ 0 ] < 12.f )
            indicators_spacing[ 0 ] = std::lerp( indicators_spacing[ 0 ], 10.f, 0.25f );

        color_t color = options::visuals::reload_bar_color;
        color.a *= alpha;

        g_render.draw_filled_rect( vec4_t( g_cheat.screen_center.x - ( max_size / 2 ), g_cheat.screen_center.y + ( 20.f ), max_size, 4.f ), color_t( 0.f, 0.f, 0.f, 255.f * alpha ) );
        g_render.draw_filled_rect( vec4_t( g_cheat.screen_center.x - ( max_size / 2 ) + 1, g_cheat.screen_center.y + ( 21.f ), x - 2, 2.f ), color );
    }
    else {
        indicators_spacing[ 0 ] = std::lerp( indicators_spacing[ 0 ], 0.f, 0.25f );
    }
}

void c_esp::show_desync_bar( ) {
	if( !options::visuals::show_desync_bar ) {
        indicators_spacing[ 1 ] = std::lerp( indicators_spacing[ 1 ], 0.f, 0.25f );
        return;
    }

    static float alpha = 0.f;
    float max_size = 130;

	float desync_time = ( g_sdk.realtimeSinceStartup( ) - g_cheat.local->lastSentTickTime( ) ) - 0.03125 * 3;
    float desync_amount = ( ( ( desync_time / 0.85f ) * 100.0f ) + 1.f ) / 100;

	float num = desync_amount;

	if( desync_amount < 0.1f )
        num = 0;

	if( num ) {
        num = std::clamp( num, 0.f, 1.f );

        if( alpha < 1.f )
            alpha = std::lerp( alpha, 1.f, 0.15f );
	}
    else {
        alpha = std::lerp( alpha, 0.f, 0.15f );
    }

    float x = ( desync_time * max_size );

    x = std::clamp( x, 1.f, max_size );

    if( alpha > 0.15f ) {
        if( indicators_spacing[ 1 ] < 10.f )
            indicators_spacing[ 1 ] = std::lerp( indicators_spacing[ 1 ], 10.f, 0.25f );

        color_t color = options::visuals::desync_bar_color;
        color.a *= alpha;

        g_render.draw_filled_rect( vec4_t( g_cheat.screen_center.x - ( max_size / 2 ), g_cheat.screen_center.y + ( 20.f + indicators_spacing[ 0 ] ), max_size, 4.f ),
            color_t( 0.f, 0.f, 0.f, 255.f * alpha ) );

        g_render.draw_filled_rect( vec4_t( g_cheat.screen_center.x - ( max_size / 2 ) + 1, g_cheat.screen_center.y + ( 21.f + indicators_spacing[ 0 ] ), x - 2, 2.f ),
            color );
    }
    else {
        indicators_spacing[ 1 ] = std::lerp( indicators_spacing[ 1 ], 0.f, 0.25f );
    }
}

void c_esp::fly_hack_bar( ) {
    static float alpha[ 2 ]{ 0.f };

    if( current_vertical_fly > 0.f
        && alpha[ 1 ] < 1.f )
        alpha[ 1 ] = std::lerp( alpha[ 1 ], 1.f, 0.15f );
    else if( alpha[ 1 ] > 0.f )
        alpha[ 1 ] = std::lerp( alpha[ 1 ], 0.f, 0.15f );

    if( current_horizontal_fly > 0.f
        && alpha[ 0 ] < 1.f )
        alpha[ 0 ] = std::lerp( alpha[ 0 ], 1.f, 0.15f );
    else if( alpha[ 0 ] > 0.f )
        alpha[ 0 ] = std::lerp( alpha[ 0 ], 0.f, 0.15f );

    if( options::visuals::indicators 
        && options::visuals::fly_hack_indicator ) {
	    float vert_percentage = ( current_vertical_fly / max_vertical_fly );
		const float red = vert_percentage * 255.f;
		const float green = 255.f - red;
        vert_percentage = std::clamp( vert_percentage, 0.f, 1.f );

        if( alpha[ 1 ] ) {
		    g_render.draw_filled_rect( vec4_t( g_cheat.screen_center.x - 200, g_cheat.screen_center.y - 426, 402, 6 ), 
                color_t( 0, 0, 0, 255.f * alpha[ 1 ] ) );

		    g_render.draw_filled_rect( vec4_t( g_cheat.screen_center.x - 199, g_cheat.screen_center.y - 425, 400 * vert_percentage, 4 ), 
                color_t( red, green, 0, 255.f * alpha[ 1 ] ) );
        }

		float hor_percentage = ( current_horizontal_fly / max_horizontal_fly );
		const float red_horizontal = hor_percentage * 255.f;
		const float green_horizontal = 255.f - red_horizontal;
        hor_percentage = std::clamp( hor_percentage, 0.f, 1.f );

        if( alpha[ 0 ] ) {
		    g_render.draw_filled_rect( vec4_t( g_cheat.screen_center.x - 200, g_cheat.screen_center.y - 446, 402, 6 ), 
                color_t( 0, 0, 0, 255.f * alpha[ 0 ] ) );

		    g_render.draw_filled_rect( vec4_t( g_cheat.screen_center.x - 199, g_cheat.screen_center.y - 445, 400 * hor_percentage, 4 ), 
                color_t( red_horizontal, green_horizontal, 0, 255.f * alpha[ 0 ] ) );
        }
    }
}

base_entity* c_esp::get_closest_object_of_class( std::string class_to_find, float max_dist )
{
	float best_distance = max_dist;
	base_entity* best_object = NULL;

	if( !g_cheat.client_entities )
		return NULL;

	uintptr_t entity_realm = *reinterpret_cast< uintptr_t* >( g_cheat.client_entities + 0x10 );
	if( !entity_realm )
		return NULL;

	uintptr_t buffer_list = *reinterpret_cast< uintptr_t* >( entity_realm + 0x28 );
	if( !buffer_list )
		return NULL;

	uintptr_t object_list = *reinterpret_cast< uintptr_t* >( buffer_list + 0x18 );
	if( !object_list )
		return NULL;

	int object_list_size = *reinterpret_cast< int* >( buffer_list + 0x10 );
	if( !object_list_size )
		return NULL;

	for( int i = 0; i < object_list_size; i++ )
	{
		base_entity* current_object = *reinterpret_cast< base_entity** >( object_list + ( 0x20 + ( i * sizeof( uint64_t ) ) ) );

		if( !current_object )
			continue;

		std::string class_name = current_object->get_class_name( );

		if( class_name == class_to_find )
		{
			uintptr_t base_mono_object = *reinterpret_cast< uintptr_t* >( ( uintptr_t )current_object + 0x10 );
			if( !base_mono_object )
				continue;

			uintptr_t object = *reinterpret_cast< uintptr_t* >( base_mono_object + 0x30 );
			if( !object )
				continue;

			uintptr_t object_class = *reinterpret_cast< uintptr_t* >( object + 0x30 );
			if( !object_class )
				continue;

			uintptr_t entity_visual = *reinterpret_cast< uintptr_t* >( object_class + sizeof( uint64_t ) );
			if( !entity_visual )
				continue;

			uintptr_t visual_state = *reinterpret_cast< uintptr_t* >( entity_visual + 0x38 );
			if( !visual_state )
				continue;

			vec3_t position = *reinterpret_cast< vec3_t* >( visual_state + 0x90 );

			float distance = g_cheat.local_pos.distance( position );

			if( distance < best_distance )
			{
				best_distance = distance;
				best_object = current_object;
			}
		}
	}

	return best_object;
}

void c_esp::do_melee_attack( vec3_t pos, base_entity* entity, bool is_player )
{
    base_player* local = g_cheat.local;
    if( !local )
        return;

    player_eyes* eyes = g_cheat.local_eyes;
    if( !eyes )
        return;
        
    item* held_item = local->get_held_item( );
    if( !held_item 
        || !held_item->is_melee( ) )
        return;

	if( !entity ) 
		return;

	if( held_item->get_next_attack_time( ) >= g_sdk.get_fixedTime( )
        || held_item->get_deploy_delay( ) > held_item->get_time_since_deploy( ) )
		return;

	transform* trans = is_player ? reinterpret_cast< base_player* >( entity )->get_bone_transform( head ) : entity->get_transform( );
	if( !trans )
		return;

	vec3_t transform_pos = trans->inverse_tranform_point( pos );

	Ray ray = Ray( eyes->get_position( ), ( pos - eyes->get_position( ) ).normalized( ) );

	hit_test* hit = hit_test::New( );
	if( hit ) {
		hit->set_hit_entity( entity );
		hit->set_did_hit( true );
		hit->set_max_distance( melee_max_dist );
		hit->set_hit_transform( trans );
		hit->set_attack_ray( ray );
		hit->set_hit_normal( transform_pos );
		hit->set_hit_hitpoint( transform_pos );
		hit->set_damage_properties( held_item->get_damage_properties( ) );
        held_item->process_attack( hit );
        held_item->start_attack_cooldown( held_item->get_repeat_delay( ) );
	}
}