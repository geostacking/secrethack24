#include "hooker.hpp"

// create global definition for hooks class.
c_hooks g_hooks;

void c_hooks::init_hooks( ) {
	perf_ui::performance_ui_update = *reinterpret_cast< uintptr_t* >( g_il2mgr.method( xs( "PerformanceUI" ), xs( "Update" ), 0, xs( "Facepunch" ) ) );

	client::on_network_message = *reinterpret_cast< uintptr_t* >( g_il2mgr.method( xs( "Client" ), xs( "OnNetworkMessage" ), 1, xs( "" ) ) );

	g_il2mgr.hook( &client::on_network_message_hook, xs( "Client" ), xs( "OnNetworkMessage" ), 1, xs( "" ) );

	g_il2mgr.hook( &perf_ui::performance_ui_update_hook, xs( "PerformanceUI" ), xs( "Update" ), 0, xs( "Facepunch" ) );
	g_il2mgr.hook( &perf_ui::ongui_hook, xs( "DevControls" ), xs( "OnGUI" ), 0, xs( "" ) );

	basemelee::process_attack = g_il2mgr.hook_virt( xs( "BaseMelee" ), xs( "ProcessAttack" ), &basemelee::process_attack_hook, 1 );
	baseplayer::client_input = g_il2mgr.hook_virt( xs( "BasePlayer" ), xs( "ClientInput" ), &baseplayer::client_input_hook, 1 );
	baseplayer::fix_culling = g_il2mgr.hook_virt( xs( "BasePlayer" ), xs( "VisUpdateUsingCulling" ), &baseplayer::fix_culling_hook, 2 );
	baseplayer::on_attacked = g_il2mgr.hook_virt( xs( "BasePlayer" ), xs( "OnAttacked" ), &baseplayer::on_attacked_hook, 1 );
	item_icon_hk::try_to_move = g_il2mgr.hook_virt( xs( "ItemIcon" ), xs( "TryToMove" ), &item_icon_hk::try_to_move_hook, 0 );

	_player_walk_movement::client_input = g_il2mgr.hook_virt( xs( "PlayerWalkMovement" ), xs( "ClientInput" ), &_player_walk_movement::client_input_hook, 2 );

	playerprojectileattack::projectile_attack = g_il2mgr.hook_virt( xs( "PlayerProjectileAttack" ), 
		xs( "WriteToStream" ), &playerprojectileattack::write_to_stream_hook, 1, xs( "ProtoBuf" ) );

	projectileshoot::o_projectile_shoot = g_il2mgr.hook_virt( xs( "ProjectileShoot" ),
		xs( "WriteToStream" ), &projectileshoot::write_to_stream_hook, 1, xs( "ProtoBuf" ) );

	//playerprojectilericochet::projectile_ricochet = g_il2mgr.hook_virt( xs( "PlayerProjectileRicochet" ), 
	//	xs( "WriteToStream" ), &playerprojectilericochet::write_to_stream_hook, 1, xs( "ProtoBuf" ) );

	playerprojectileupdate::projectile_update = g_il2mgr.hook_virt( xs( "PlayerProjectileUpdate" ),
		xs( "WriteToStream" ), &playerprojectileupdate::write_to_stream_hook, 1, xs( "ProtoBuf" ) );

	// maybe hook Projectile Update through g_il2mgr.hook since we are just going to change hittest, etc.

	/*
	
	we should hook:
	FlintStrikeWeapon DoAttack()
	BaseMelee DoAttack()
	DoBowAttack()
	BlockSprint()
	BowIsAiming()

	*/
}