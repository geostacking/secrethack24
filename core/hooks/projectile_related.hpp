#pragma once
#include "../includes/hinclude.hpp"
#include "../features/options.hpp"

namespace projectileshoot
{
	inline uintptr_t o_projectile_shoot = NULL;

	void on_projectile_shoot( void* ecx );
	void write_to_stream_hook( void* ecx, void* stream );
}

namespace playerprojectilericochet {
	inline uintptr_t projectile_ricochet = NULL;

	void on_projectile_ricochet( void* ecx );
	void write_to_stream_hook( void* ecx, void* stream );
}
namespace playerprojectileupdate {
	inline uintptr_t projectile_update = NULL;

	void on_projectile_update( void* ecx );
	void write_to_stream_hook( void* ecx, void* stream );
}

namespace playerprojectileattack
{
	inline uintptr_t projectile_attack = NULL;

	enum BoneHitbox : int
	{
		Head = 698017942,
		sstomach = -523945340,
		cchest = 827230707,
		lleg = 1286912989,
		rleg = -1963356626,
		rhand = -393310151,
		lhand = -1358607930
	};

	void on_projectile_attack( void* ecx );

	void write_to_stream_hook( void* ecx, void* stream );
}
