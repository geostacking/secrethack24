#include "hooker.hpp"
#include "../features/visuals/esp.hpp"

namespace client {
	void on_network( void* ecx ) {
		if( !options::visuals::world::show_raid )
			return;

		uintptr_t effect_network = g_il2mgr.init_class( xs( "EffectNetwork" ) );
		if( !effect_network ) {
			std::cout << __LINE__ << std::endl;
			return;
		}

		uintptr_t effect = *reinterpret_cast< uintptr_t* >( effect_network + 0xB8 );
		if( !effect ) {
			std::cout << __LINE__ << std::endl;
			return;
		}

		//public Vector3 worldPos; // 0x64
		//public string pooledString; // 0x90

		vec3_t world_pos = *reinterpret_cast< vec3_t* >( effect + 0x64 );
		if( world_pos.is_zero( ) ) {
			std::cout << __LINE__ << std::endl;
			return;
		}

		u_string* string = *reinterpret_cast< u_string** >( effect + 0x90 );
		if( !string ) {
			std::cout << __LINE__ << std::endl;
			return;
		}

		vec2_t screen_pos = g_sdk.world_to_screen( world_pos );
		if( screen_pos.is_zero( ) ) {
			std::cout << __LINE__ << std::endl;
			return;
		}

		uintptr_t current_event = g_sdk.get_current( );
		if( !current_event )
			return;

		event_type event_type = g_sdk.get_event_type( current_event );
		if( event_type != event_type::re_paint )
			return;

		wchar_t* name = string->get_buffer( );

		float distance = g_cheat.local_pos.distance( world_pos );
		std::wstring text = std::wstring( );
		color_t color = color_t( );

		std::wcout << name << std::endl;

		if( distance < options::visuals::world::max_raid_distance )
		{
			if( wcsstr( name, xs( L"c4_explosion.prefab" ) ) ) {
				text = xs( L"c4" );
				color = color_t( 255, 0, 0 );
			}
			else if( wcsstr( name, xs( L"satchel-charge-explosion.prefab" ) ) ) {
				text = xs( L"satchel" );
				color = color_t( 242, 164, 63 );
			}
			else if( wcsstr( name, xs( L"beancan_grenade_explosion.prefab" ) ) ) {
				text = xs( L"bean can grenade" );
				color = color_t( 255, 114, 59 );
			}
			else if( wcsstr( name, xs( L"rocket_explosion.prefab" ) ) ) {
				text = xs( L"rocket" );
				color = color_t( 255, 114, 59 );
			}
		}
		if( !text.empty( ) )
		{
			vec4_t render_pos = vec4_t( screen_pos.x, screen_pos.y, 150, 50 );

			g_render.draw_text( render_pos, text.c_str( ), color, false, 12, g_esp.font_flags );
		}
	}
	void on_network_message_hook( void* self, void* packet ) {
		auto orig_client_on_network_message = reinterpret_cast< void( * )( void*, void* )>( on_network_message );

		on_network( self );

		spoof_ret( orig_client_on_network_message, self, packet );
	}
}