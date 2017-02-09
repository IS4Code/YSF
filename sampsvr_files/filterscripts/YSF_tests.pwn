#include <a_samp>
#define FILTERSCRIPT
#include <zcmd>
#include <sscanf2>
#include <YSF>
#tryinclude <streamer>

#define XCOLOR_ORANGE			FF9900
#define XCOLOR_DEFAULT 			A9C4E4
#define XCOLOR_SAMP 			EE5555
#define XCOLOR_LIME 			99FF00

new
	g_szFormatString[4096], tmp[4096];

#define strcatf(%1,%3) \
    strcat(%1, (format(g_szFormatString, sizeof(g_szFormatString), %3), g_szFormatString))

#define isnull(%1) \
	((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))

public OnFilterScriptInit()
{
    TestObjectMaterial();
    TestActors();
	return 1;
}

stock TestObjectMaterial()
{
    new modelid, txdname[64], texturename[64], materialcolor, text[144], materialsize, fontface[32], fontsize, bold, fontcolor, backcolor, textalignment, formatstr[32];
	new obj = CreateObject(1225, 5267.7, -4655.9, 500.3, 30.5, 69.3, 180.4, 500.0);
	new objtext = CreateObject(3500, 10.1, 20.9, 30.7, 30.5, 69.3, 180.4, 285.2);

	printf("\n--- material test ---");
	for(new i = 0; i != 16; i++)
	{
		format(formatstr, sizeof(formatstr), "fediszno_%d", random(256));
		SetObjectMaterial(obj, i, 1225 + i, "kismalac", formatstr, 0xAABBCCDD);
		GetObjectMaterial(obj, i, modelid, txdname, sizeof(txdname), texturename, sizeof(texturename), materialcolor);

		printf("material: %d - modelid: %d, txd: '%s' - texture: '%s', color: %x", i, modelid, txdname, texturename, materialcolor);
	}

	printf("\n--- material text test ---");
	for(new i = 0; i != 16; i++)
	{
		SetObjectMaterialText(objtext, "eszmaloskibli", i, _, "Arial", 26 + i, 1, 0xAABBCCDD, 0xDDEEFFAA, 1);
		GetObjectMaterialText(objtext, i, text, sizeof(text), materialsize, fontface, sizeof(fontface), fontsize, bold, fontcolor, backcolor, textalignment);

		printf("material: %d <%s> - materialsize: %d, fontface: %s, fontsize: %d, bold: %d, fontcolor: %x, backcolor: %x, alignment: %d", i, text, materialsize, fontface, fontsize, bold, fontcolor, backcolor, textalignment);
	}
}

stock TestActors()
{
	new act; CreateActor(217, -86.185, 643.7, 26.98, 90.01); CreateActor(217, -87.185, 643.7, 26.98, 90.01); act = CreateActor(217, -86.185, 643.7, 26.98, 90.01);
	SetActorVirtualWorld(act, 123);
	ApplyActorAnimation(act, "PAULNMAC", "wank_out", 4.0, true, false, false, 0, 0);
	SetActorHealth(act, 87.8);
	SetActorInvulnerable(random(2));
	
	printf("\n--- actor test (%d) ---", act);

	new Float:Health, skinid, Float:fSpawn[4], szAnimLib[64], szAnimName[64], Float:fDelta, loop, lockx, locky, freeze, time;
	GetActorHealth(act, Health);
	GetActorSpawnInfo(act, skinid, fSpawn[0], fSpawn[1], fSpawn[2], fSpawn[3]);
	GetActorAnimation(act, szAnimLib, _, szAnimName, _, fDelta, loop, lockx, locky, freeze, time);

	printf("health: %.2f, skinid: %d, Spawn: %.3f, %.3f, %.3f, %3.f", Health, skinid, fSpawn[0], fSpawn[1], fSpawn[2], fSpawn[3]);
	printf("Animlib: %s, Animname: %s, fDelta: %.3f, loop: %d, lockx: %d, locky: %d, freeze: %d, time: %d", szAnimLib, szAnimName, fDelta, loop, lockx, locky, freeze, time);

}
