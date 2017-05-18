#include "../main.h"
#include "../Natives.h"

// native GetActorSpawnInfo(actorid, &skinid, &Float:fX, &Float:fY, &Float:fZ, &Float:fAngle);
AMX_DECLARE_NATIVE(Natives::GetActorSpawnInfo)
{
	CHECK_PARAMS(6, LOADED);

	const int actorid = CScriptParams::Get()->ReadInt();
	if (actorid < 0 || actorid > MAX_PLAYERS) return 0;

	CActor *pActor = pNetGame->pActorPool->pActor[actorid];
	if (!pActor) return 0;

	CScriptParams::Get()->Add(pActor->iSkinID, pActor->vecSpawnPos, pActor->fSpawnAngle);
	return 1;
}

// native GetActorSkin(actorid);
AMX_DECLARE_NATIVE(Natives::GetActorSkin)
{
	CHECK_PARAMS(1, LOADED);

	const int actorid = CScriptParams::Get()->ReadInt();
	if (actorid < 0 || actorid > MAX_PLAYERS) return 0;

	CActor *pActor = pNetGame->pActorPool->pActor[actorid];
	if (!pActor) return 0;

	return pActor->iSkinID;
}


// native GetActorAnimation(actorid, animlib[], animlibsize = sizeof(animlib), animname[], animnamesize = sizeof(animname), &Float:fDelta, &loop, &lockx, &locky, &freeze, &time)
AMX_DECLARE_NATIVE(Natives::GetActorAnimation)
{
	CHECK_PARAMS(11, LOADED);

	const int actorid = CScriptParams::Get()->ReadInt();
	if (actorid < 0 || actorid > MAX_PLAYERS) return 0;

	CActor *pActor = pNetGame->pActorPool->pActor[actorid];
	if (!pActor) return 0;

	CScriptParams::Get()->Add(&pActor->anim.szAnimLib[0], &pActor->anim.szAnimName[0], pActor->anim.fDelta, pActor->anim.byteLoop, pActor->anim.byteLockX,
		pActor->anim.byteLockY, pActor->anim.byteFreeze, pActor->anim.iTime);
	return 1;
}
