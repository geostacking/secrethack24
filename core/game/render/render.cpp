#include "../../includes/hinclude.hpp"
#include "../offsets.hpp"
#include "render.hpp"

static uintptr_t draw_shader = NULL;
static bool has_drew_once = true;

c_renderer g_render;

void c_renderer::get_draw_shader( ) {
	draw_shader = methods::object_new( g_il2mgr.init_class( xs( "Material" ), xs( "UnityEngine" ) ) );
}

void c_renderer::set_up_draw_shader( ) {
	if( !draw_shader )
		return get_draw_shader( );

	static uintptr_t shader = g_sdk.find( xs( L"Hidden/Internal-Colored" ) );
	if( !shader
		|| !has_drew_once )
		return;

	g_sdk.create_shader( draw_shader, shader );
	g_sdk.set_material_int( draw_shader, xs( L"_SrcBlend" ), 5 );
	g_sdk.set_material_int( draw_shader, xs( L"_DstBlend" ), 10 );
	g_sdk.set_material_int( draw_shader, xs( L"_Cull" ), 0 );
	g_sdk.set_material_int( draw_shader, xs( L"_ZWrite" ), 0 );
	g_sdk.dont_destroy_on_load( draw_shader );
	has_drew_once = false;
}

void c_renderer::begin_gl_draw( begin_mode mode ) {
	if( !draw_shader )
		return get_draw_shader( );

	g_sdk.push_matrix( );
	g_sdk.set_pass( draw_shader, 0 );
	g_sdk.begin( mode );
}

void c_renderer::draw_circle( vec2_t pos, float radius, color_t color ) {
	if( !draw_shader )
		return get_draw_shader( );

	begin_gl_draw( begin_mode::line_strip );
	g_sdk.set_gl_color( color );
	for( float num = 0.f; num < PIX2 * 2.f; num += 0.05f ) {
		g_sdk.vertex( vec3_t( cos( num ) * radius + pos.x, sin( num ) * radius + pos.y, 0 ) );
		g_sdk.vertex( vec3_t( cos( num + 0.05f ) * radius + pos.x, sin( num + 0.05f ) * radius + pos.y, 0 ) );
	}
	g_sdk.end( );
	g_sdk.pop_matrix( );
}

void c_renderer::gl_triangle( vec2_t pos, vec2_t pos1, vec2_t pos2, color_t col )
{
	if( !draw_shader )
		return get_draw_shader( );

	begin_gl_draw( begin_mode::triangles );
	g_sdk.set_gl_color( col );
	g_sdk.vertex( vec3_t( pos.x, pos.y, 0 ) );
	g_sdk.vertex( vec3_t( pos1.x, pos1.y, 0 ) );
	g_sdk.vertex( vec3_t( pos2.x, pos2.y, 0 ) );

	g_sdk.end( );
	g_sdk.pop_matrix( );
}

void c_renderer::draw_line( vec2_t start, vec2_t end, color_t color ) {
	if( !draw_shader )
		return get_draw_shader( );

	begin_gl_draw( begin_mode::lines );
	g_sdk.set_gl_color( color );
	g_sdk.vertex( vec3_t( start.x, start.y, 0 ) );
	g_sdk.vertex( vec3_t( end.x, end.y, 0 ) );

	g_sdk.end( );
	g_sdk.pop_matrix( );
}

void c_renderer::get_n_white_texture( ) {
	g_cheat.white_texture = g_sdk.get_white_texture( );
}

void c_renderer::horizontal_line( vec2_t pos, float size, color_t clr )
{
	if( !g_cheat.white_texture )
		return get_n_white_texture( );

	g_sdk.set_draw_color( clr );

	g_sdk.draw_texture( vec4_t( pos.x, pos.y, size, 1 ), g_cheat.white_texture );
}

void c_renderer::vertical_line( vec2_t pos, float size, color_t clr )
{
	if( !g_cheat.white_texture )
		return get_n_white_texture( );

	g_sdk.set_draw_color( clr );

	g_sdk.draw_texture( vec4_t( pos.x, pos.y, 1, size ), g_cheat.white_texture );
}

void c_renderer::outline_box( vec2_t position, vec2_t size, color_t color, float girth )
{
	if( !g_cheat.white_texture )
		return get_n_white_texture( );

	g_sdk.set_draw_color( color );

	g_sdk.draw_texture( vec4_t( position.x, position.y, girth, size.y ), g_cheat.white_texture );
	g_sdk.draw_texture( vec4_t( position.x + size.x, position.y, girth, size.y + 1 ), g_cheat.white_texture );
	g_sdk.draw_texture( vec4_t( position.x, position.y, size.x, girth ), g_cheat.white_texture );
	g_sdk.draw_texture( vec4_t( position.x, position.y + size.y, size.x, girth ), g_cheat.white_texture );
}

void c_renderer::draw_filled_rect( vec2_t position, vec2_t size, color_t color )
{
	if( !g_cheat.white_texture )
		return get_n_white_texture( );

	g_sdk.set_draw_color( color );
	g_sdk.draw_texture( vec4_t( position.x, position.y, size.x, size.y ), g_cheat.white_texture );
}

void c_renderer::draw_filled_rect( float x, float y, float width, float height, color_t color )
{
	if( !g_cheat.white_texture )
		return get_n_white_texture( );

	g_sdk.set_draw_color( color );
	g_sdk.draw_texture( vec4_t( x, y, width, height ), g_cheat.white_texture );
}

void c_renderer::draw_gradient( float x, float y, float width, float height, color_t start_color, color_t end_color )
{
	for( int i = 0; i < height; i++ )
	{
		color_t new_color = color_t( 
			start_color.r + ( i * ( end_color.r - start_color.r ) / height ),
			start_color.g + ( i * ( end_color.g - start_color.g ) / height ),
			start_color.b + ( i * ( end_color.b - start_color.b ) / height )
		);

		draw_filled_rect( vec4_t( x, y + i, width, 1 ), color_t( new_color.r, new_color.g, new_color.b, start_color.a ) );
	}
}

void c_renderer::draw_filled_rect( vec4_t position, color_t color )
{
	if( !g_cheat.white_texture )
		return get_n_white_texture( );

	g_sdk.set_draw_color( color );
	g_sdk.draw_texture( position, g_cheat.white_texture );
}

void c_renderer::line( vec2_t point1, vec2_t point2, color_t color )
{
	if( !g_cheat.white_texture )
		return get_n_white_texture( );

	g_sdk.set_draw_color( color );
	if( ( point1.x - point2.x ) < 1.f )
		g_sdk.draw_texture( vec4_t( point1.x, point1.y, point1.x - point2.x, 1.f ), g_cheat.white_texture );
	else
		g_sdk.draw_texture( vec4_t( point1.x, point1.y, 1.f, point1.y - point2.y ), g_cheat.white_texture );
}

void c_renderer::label( vec4_t position, u_string text, color_t color, bool centered, int font_size )
{
	if( !g_cheat.draw_label )
		return;

	g_sdk.set_draw_font_size( g_cheat.draw_label, font_size );

	g_sdk.set_draw_alignment( g_cheat.draw_label, centered ? 0x4 : 0x0 );

	uintptr_t method = offsets::gui_label;
	if( !method )
		return;

	uintptr_t method_ptr = *reinterpret_cast< uintptr_t* >( method );
	if( !method_ptr )
		return;

	auto label = reinterpret_cast< void( * )( vec4_t, u_string, uintptr_t ) >( method_ptr );

	g_sdk.set_draw_color( color );
	spoof_ret( label, position, text, g_cheat.draw_label );
}

void c_renderer::draw_text( vec4_t position, u_string text, color_t color, bool centered, int font_size, font_flags_t flags, color_t outline_color ) {
	// measure.
	outline_color.a *= color.a;

	if( flags & outline ) {
		label( vec4_t( position.x, position.y - 1, position.z, position.w ), text, outline_color, centered, font_size ); //up
		label( vec4_t( position.x, position.y + 1, position.z, position.w ), text, outline_color, centered, font_size ); //down
		label( vec4_t( position.x + 1, position.y, position.z, position.w ), text, outline_color, centered, font_size ); //right
		label( vec4_t( position.x - 1, position.y, position.z, position.w ), text, outline_color, centered, font_size ); //left
		label( vec4_t( position.x + 1, position.y + 1, position.z, position.w ), text, outline_color, centered, font_size ); //down-right
		label( vec4_t( position.x - 1, position.y + 1, position.z, position.w ), text, outline_color, centered, font_size ); //down-left
		label( vec4_t( position.x + 1, position.y - 1, position.z, position.w ), text, outline_color, centered, font_size ); //up-right
		label( vec4_t( position.x - 1, position.y - 1, position.z, position.w ), text, outline_color, centered, font_size ); //up-left
	}
	if( flags & dropshadow ) {
		label( vec4_t( position.x + 1, position.y + 1, position.z, position.w ), text, outline_color, centered, font_size );
	}

	label( position, text, color, centered, font_size );
}

void c_renderer::ddraw_line( vec3_t start, vec3_t end, color_t color, float duration, bool distanceFade, bool ztest )
{
	uintptr_t method = offsets::ddraw_line;
	if( !method )
		return;

	uintptr_t method_ptr = *reinterpret_cast< uintptr_t* >( method );
	if( !method_ptr )
		return;

	auto line = reinterpret_cast< void( * )( vec3_t, vec3_t, color_t, float, bool, bool ) >( method_ptr );
	spoof_ret( line, start, end, color, duration, distanceFade, ztest );
}

void c_renderer::ddraw_sphere( vec3_t start, vec3_t end, color_t color, float duration, bool distanceFade )
{
	uintptr_t method = offsets::ddraw_sphere;
	if( !method )
		return;

	uintptr_t method_ptr = *reinterpret_cast< uintptr_t* >( method );
	if( !method_ptr )
		return;

	auto sphere = reinterpret_cast< void( * )( vec3_t, vec3_t, color_t, float, bool ) >( method_ptr );
	spoof_ret( sphere, start, end, color, duration, distanceFade );
}

void c_renderer::ddraw_arrow( vec3_t start, vec3_t end, float size, color_t color, float duration )
{
	uintptr_t method = offsets::ddraw_arrow;
	if( !method )
		return;

	uintptr_t method_ptr = *reinterpret_cast< uintptr_t* >( method );
	if( !method_ptr )
		return;

	auto arrow = reinterpret_cast< void( * )( vec3_t, vec3_t, float, color_t, float ) >( method_ptr );
	spoof_ret( arrow, start, end, size, color, duration );
}

void c_renderer::ddraw_text( u_string text, vec3_t pos, color_t color, float duration )
{
	uintptr_t method = offsets::ddraw_text;
	if( !method )
		return;

	uintptr_t method_ptr = *reinterpret_cast< uintptr_t* >( method );
	if( !method_ptr )
		return;

	auto drawtext = reinterpret_cast< void( * )( u_string, vec3_t, color_t, float ) >( method_ptr );
	spoof_ret( drawtext, text, pos, color, duration );
}

void c_renderer::ddraw_get( )
{
	uintptr_t method = offsets::ddraw_get;
	if( !method )
		return;

	uintptr_t method_ptr = *reinterpret_cast< uintptr_t* >( method );
	if( !method_ptr )
		return;

	auto Get = reinterpret_cast< void( * )( ) >( method_ptr );
	spoof_ret( Get );
}