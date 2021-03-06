/*
    C-Dogs SDL
    A port of the legendary (and fun) action/arcade cdogs.
    Copyright (C) 1995 Ronny Wester
    Copyright (C) 2003 Jeremy Chin
    Copyright (C) 2003-2007 Lucas Martin-King

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    This file incorporates work covered by the following copyright and
    permission notice:

    Copyright (c) 2013-2015, Cong Xu
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
*/
#include "mission.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "files.h"
#include "game_events.h"
#include "gamedata.h"
#include "map.h"
#include "map_new.h"
#include "objs.h"
#include "palette.h"
#include "particle.h"
#include "pickup.h"
#include "defs.h"
#include "pic_manager.h"
#include "actors.h"
#include "triggers.h"


int StrKeycard(const char *s)
{
	S2T(FLAGS_KEYCARD_YELLOW, "yellow");
	S2T(FLAGS_KEYCARD_GREEN, "green");
	S2T(FLAGS_KEYCARD_BLUE, "blue");
	S2T(FLAGS_KEYCARD_RED, "red");
	return 0;
}

const char *MapTypeStr(MapType t)
{
	switch (t)
	{
	case MAPTYPE_CLASSIC:
		return "Classic";
	case MAPTYPE_STATIC:
		return "Static";
	default:
		return "";
	}
}
MapType StrMapType(const char *s)
{
	if (strcmp(s, "Classic") == 0)
	{
		return MAPTYPE_CLASSIC;
	}
	else if (strcmp(s, "Static") == 0)
	{
		return MAPTYPE_STATIC;
	}
	return MAPTYPE_CLASSIC;
}


void MissionInit(Mission *m)
{
	memset(m, 0, sizeof *m);
	m->WallMask = colorBattleshipGrey;
	m->FloorMask = colorGravel;
	m->RoomMask = colorDoveGray;
	m->AltMask = colorOfficeGreen;
	CArrayInit(&m->Objectives, sizeof(MissionObjective));
	CArrayInit(&m->Enemies, sizeof(int));
	CArrayInit(&m->SpecialChars, sizeof(int));
	CArrayInit(&m->MapObjectDensities, sizeof(MapObjectDensity));
	CArrayInit(&m->Weapons, sizeof(const GunDescription *));
}
void MissionCopy(Mission *dst, const Mission *src)
{
	if (src == NULL)
	{
		return;
	}
	MissionTerminate(dst);
	MissionInit(dst);
	if (src->Title)
	{
		CSTRDUP(dst->Title, src->Title);
	}
	if (src->Description)
	{
		CSTRDUP(dst->Description, src->Description);
	}
	dst->Type = src->Type;
	dst->Size = src->Size;

	dst->WallStyle = src->WallStyle;
	dst->FloorStyle = src->FloorStyle;
	dst->RoomStyle = src->RoomStyle;
	dst->ExitStyle = src->ExitStyle;
	dst->KeyStyle = src->KeyStyle;
	dst->DoorStyle = src->DoorStyle;

	CArrayCopy(&dst->Objectives, &src->Objectives);
	for (int i = 0; i < (int)src->Objectives.size; i++)
	{
		MissionObjective *smo = CArrayGet(&src->Objectives, i);
		MissionObjective *dmo = CArrayGet(&dst->Objectives, i);
		if (smo->Description)
		{
			CSTRDUP(dmo->Description, smo->Description);
		}
	}
	CArrayCopy(&dst->Enemies, &src->Enemies);
	CArrayCopy(&dst->SpecialChars, &src->SpecialChars);
	CArrayCopy(&dst->MapObjectDensities, &src->MapObjectDensities);

	dst->EnemyDensity = src->EnemyDensity;
	CArrayCopy(&dst->Weapons, &src->Weapons);

	memcpy(dst->Song, src->Song, sizeof dst->Song);

	dst->WallMask = src->WallMask;
	dst->FloorMask = src->FloorMask;
	dst->RoomMask = src->RoomMask;
	dst->AltMask = src->AltMask;

	switch (dst->Type)
	{
	case MAPTYPE_STATIC:
		CArrayCopy(&dst->u.Static.Tiles, &src->u.Static.Tiles);
		CArrayCopy(&dst->u.Static.Items, &src->u.Static.Items);
		CArrayCopy(&dst->u.Static.Wrecks, &src->u.Static.Wrecks);
		CArrayCopy(&dst->u.Static.Characters, &src->u.Static.Characters);
		CArrayCopy(&dst->u.Static.Objectives, &src->u.Static.Objectives);
		CArrayCopy(&dst->u.Static.Keys, &src->u.Static.Keys);

		dst->u.Static.Start = src->u.Static.Start;
		dst->u.Static.Exit = src->u.Static.Exit;
		break;
	default:
		memcpy(&dst->u, &src->u, sizeof dst->u);
		break;
	}
}
void MissionTerminate(Mission *m)
{
	if (m == NULL) return;
	CFREE(m->Title);
	CFREE(m->Description);
	for (int i = 0; i < (int)m->Objectives.size; i++)
	{
		MissionObjective *mo = CArrayGet(&m->Objectives, i);
		CFREE(mo->Description);
	}
	CArrayTerminate(&m->Objectives);
	CArrayTerminate(&m->Enemies);
	CArrayTerminate(&m->SpecialChars);
	CArrayTerminate(&m->MapObjectDensities);
	CArrayTerminate(&m->Weapons);
	switch (m->Type)
	{
	case MAPTYPE_CLASSIC:
		break;
	case MAPTYPE_STATIC:
		CArrayTerminate(&m->u.Static.Tiles);
		CArrayTerminate(&m->u.Static.Items);
		CArrayTerminate(&m->u.Static.Wrecks);
		CArrayTerminate(&m->u.Static.Characters);
		CArrayTerminate(&m->u.Static.Objectives);
		CArrayTerminate(&m->u.Static.Keys);
		break;
	}
}


// +-------------+
// |  Door info  |
// +-------------+

// note that the horz pic in the last pair is a TILE pic, not an offset pic!

static struct DoorPic officeDoors[6] = { {OFSPIC_DOOR, OFSPIC_VDOOR},
{OFSPIC_HDOOR_YELLOW, OFSPIC_VDOOR_YELLOW},
{OFSPIC_HDOOR_GREEN, OFSPIC_VDOOR_GREEN},
{OFSPIC_HDOOR_BLUE, OFSPIC_VDOOR_BLUE},
{OFSPIC_HDOOR_RED, OFSPIC_VDOOR_RED},
{109, OFSPIC_VDOOR_OPEN}
};

static struct DoorPic dungeonDoors[6] = { {OFSPIC_DOOR2, OFSPIC_VDOOR2},
{OFSPIC_HDOOR2_YELLOW, OFSPIC_VDOOR2_YELLOW},
{OFSPIC_HDOOR2_GREEN, OFSPIC_VDOOR2_GREEN},
{OFSPIC_HDOOR2_BLUE, OFSPIC_VDOOR2_BLUE},
{OFSPIC_HDOOR2_RED, OFSPIC_VDOOR2_RED},
{342, OFSPIC_VDOOR2_OPEN}
};

static struct DoorPic pansarDoors[6] = { {OFSPIC_HDOOR3, OFSPIC_VDOOR3},
{OFSPIC_HDOOR3_YELLOW, OFSPIC_VDOOR3_YELLOW},
{OFSPIC_HDOOR3_GREEN, OFSPIC_VDOOR3_GREEN},
{OFSPIC_HDOOR3_BLUE, OFSPIC_VDOOR3_BLUE},
{OFSPIC_HDOOR3_RED, OFSPIC_VDOOR3_RED},
{P2 + 148, OFSPIC_VDOOR2_OPEN}
};

static struct DoorPic alienDoors[6] = { {OFSPIC_HDOOR4, OFSPIC_VDOOR4},
{OFSPIC_HDOOR4_YELLOW, OFSPIC_VDOOR4_YELLOW},
{OFSPIC_HDOOR4_GREEN, OFSPIC_VDOOR4_GREEN},
{OFSPIC_HDOOR4_BLUE, OFSPIC_VDOOR4_BLUE},
{OFSPIC_HDOOR4_RED, OFSPIC_VDOOR4_RED},
{P2 + 163, OFSPIC_VDOOR2_OPEN}
};


static struct DoorPic *doorStyles[] = {
	officeDoors,
	dungeonDoors,
	pansarDoors,
	alienDoors
};

#define DOORSTYLE_COUNT (sizeof doorStyles / sizeof(struct DoorPic *))
int GetDoorstyleCount(void) { return DOORSTYLE_COUNT; }


// +-------------+
// |  Exit info  |
// +-------------+


static int exitPics[] = {
	375, 376,	// hazard stripes
	380, 381	// yellow plates
};

// Every exit has TWO pics, so actual # of exits == # pics / 2!
#define EXIT_COUNT (sizeof( exitPics)/sizeof( int)/2)
int GetExitCount(void) { return EXIT_COUNT; }


// +----------------+
// |  Mission info  |
// +----------------+


struct MissionOld dogFight1 = {
	"",
	"",
	WALL_STYLE_STONE, FLOOR_STYLE_STONE, FLOOR_STYLE_WOOD, 0, 1, 1,
	32, 32,
	50, 25,
	4, 2,
	0, 0, 0, 0,
	0,
	{
	 {"", 0, 0, 0, 0, 0}
	 },

	0, {0},
	0, {0},
	8,
	{8, 9, 10, 11, 12, 13, 14, 15},
	{10, 10, 10, 10, 10, 10, 10, 10},

	0, 0,
	"", "",
	14, 13, 22, 1
};

struct MissionOld dogFight2 = {
	"",
	"",
	WALL_STYLE_STEEL, FLOOR_STYLE_BLUE, FLOOR_STYLE_WHITE, 0, 0, 0,
	64, 64,
	50, 50,
	10, 3,
	0, 0, 0, 0,
	0,
	{
	 {"", 0, 0, 0, 0, 0}
	 },

	0, {0},
	0, {0},
	8,
	{0, 1, 2, 3, 4, 5, 6, 7},
	{10, 10, 10, 10, 10, 10, 10, 10},

	0, 0,
	"", "",
	5, 2, 9, 4
};


// +-----------------+
// |  Campaign info  |
// +-----------------+

#include "files.h"
#include <missions/bem.h>
#include <missions/ogre.h>


static CampaignSettingOld df1 =
{
	"Dogfight in the dungeon",
	"", "",
	1, &dogFight1,
	0, NULL
};

static CampaignSettingOld df2 =
{
	"Cubicle wars",
	"", "",
	1, &dogFight2,
	0, NULL
};


// +-----------------------+
// |  And now the code...  |
// +-----------------------+

static void SetupBadguysForMission(Mission *mission)
{
	int i;
	CharacterStore *s = &gCampaign.Setting.characters;

	CharacterStoreResetOthers(s);

	if (s->OtherChars.size == 0)
	{
		return;
	}

	for (i = 0; i < (int)mission->Objectives.size; i++)
	{
		MissionObjective *mobj = CArrayGet(&mission->Objectives, i);
		if (mobj->Type == OBJECTIVE_RESCUE)
		{
			CharacterStoreAddPrisoner(s, mobj->Index);
			break;	// TODO: multiple prisoners
		}
	}

	for (i = 0; i < (int)mission->Enemies.size; i++)
	{
		CharacterStoreAddBaddie(s, *(int *)CArrayGet(&mission->Enemies, i));
	}

	for (i = 0; i < (int)mission->SpecialChars.size; i++)
	{
		CharacterStoreAddSpecial(
			s, *(int *)CArrayGet(&mission->SpecialChars, i));
	}
}

int SetupBuiltinCampaign(int idx)
{
	switch (idx)
	{
	case 0:
		ConvertCampaignSetting(&gCampaign.Setting, &BEM_campaign);
		break;
	case 1:
		ConvertCampaignSetting(&gCampaign.Setting, &OGRE_campaign);
		break;
	default:
		ConvertCampaignSetting(&gCampaign.Setting, &OGRE_campaign);
		return 0;
	}
	return 1;
}

int SetupBuiltinDogfight(int idx)
{
	switch (idx)
	{
	case 0:
		ConvertCampaignSetting(&gCampaign.Setting, &df1);
		break;
	case 1:
		ConvertCampaignSetting(&gCampaign.Setting, &df2);
		break;
	default:
		ConvertCampaignSetting(&gCampaign.Setting, &df1);
		return 0;
	}
	return 1;
}

static void SetupObjectives(struct MissionOptions *mo, Mission *mission)
{
	for (int i = 0; i < (int)mission->Objectives.size; i++)
	{
		MissionObjective *mobj = CArrayGet(&mission->Objectives, i);
		ObjectiveDef o;
		memset(&o, 0, sizeof o);
		assert(i < OBJECTIVE_MAX_OLD);
		// Set objective colours based on type
		o.color = ObjectiveTypeColor(mobj->Type);
		o.blowupObject = IntMapObject(mobj->Index);
		o.pickupClass = IntPickupClass(mobj->Index);
		CArrayPushBack(&mo->Objectives, &o);
	}
}

static void SetupWeapons(CArray *to, CArray *from)
{
	CArrayCopy(to, from);
}

void SetupMission(
	int buildTables, Mission *m, struct MissionOptions *mo, int missionIndex)
{
	MissionOptionsInit(mo);
	mo->index = missionIndex;
	mo->missionData = m;
	mo->doorPics =
	    doorStyles[abs(m->DoorStyle) % DOORSTYLE_COUNT];
	mo->keyStyle = m->KeyStyle;

	mo->exitPic = exitPics[2 * (abs(m->ExitStyle) % EXIT_COUNT)];
	mo->exitShadow = exitPics[2 * (abs(m->ExitStyle) % EXIT_COUNT) + 1];

	ActorsInit();
	ObjsInit();
	MobObjsInit();
	PickupsInit();
	ParticlesInit(&gParticles);
	SetupObjectives(mo, m);
	SetupBadguysForMission(m);
	SetupWeapons(&mo->Weapons, &m->Weapons);
	// TODO: store colours instead and request during map load
	/*RecolourPics(
		abs(m->WallColor) % COLORRANGE_COUNT,
		abs(m->FloorColor) % COLORRANGE_COUNT,
		abs(m->RoomColor) % COLORRANGE_COUNT,
		abs(m->AltColor) % COLORRANGE_COUNT);*/
	if (buildTables)
	{
		BuildTranslationTables(gPicManager.palette);
	}
}

void MissionEnd(void)
{
	ActorsTerminate();
	ObjsTerminate();
	MobObjsTerminate();
	PickupsTerminate();
	ParticlesTerminate(&gParticles);
	RemoveAllWatches();
	for (int i = 0; i < (int)gPlayerDatas.size; i++)
	{
		PlayerData *p = CArrayGet(&gPlayerDatas, i);
		p->Id = -1;
	}
	gMission.HasStarted = false;
}

void MissionSetMessageIfComplete(struct MissionOptions *options)
{
	if (CanCompleteMission(options))
	{
		GameEvent msg = GameEventNew(GAME_EVENT_MISSION_COMPLETE);
		GameEventsEnqueue(&gGameEvents, msg);
	}
}

void UpdateMissionObjective(
	struct MissionOptions *options, int flags, ObjectiveType type,
	int player, Vec2i pos)
{
	if (!(flags & TILEITEM_OBJECTIVE))
	{
		return;
	}
	int idx = ObjectiveFromTileItem(flags);
	MissionObjective *mobj = CArrayGet(&options->missionData->Objectives, idx);
	if (mobj->Type != type)
	{
		return;
	}
	GameEvent e = GameEventNew(GAME_EVENT_UPDATE_OBJECTIVE);
	e.u.UpdateObjective.ObjectiveIndex = idx;
	e.u.UpdateObjective.Update = 1;
	e.u.UpdateObjective.PlayerIndex = player;
	e.u.UpdateObjective.Pos = pos;
	GameEventsEnqueue(&gGameEvents, e);
}

bool CanCompleteMission(const struct MissionOptions *options)
{
	int i;

	// Death is the only escape from PVP and quick play
	if (IsPVP(gCampaign.Entry.Mode))
	{
		return GetNumPlayers(true, false, false) <= 1;
	}
	else if (gCampaign.Entry.Mode == GAME_MODE_QUICK_PLAY)
	{
		return GetNumPlayers(true, false, false) == 0;
	}

	// Check all objective counts are enough
	for (i = 0; i < (int)options->Objectives.size; i++)
	{
		const ObjectiveDef *o = CArrayGet(&options->Objectives, i);
		MissionObjective *mobj =
			CArrayGet(&options->missionData->Objectives, i);
		if (o->done < mobj->Required)
		{
			return 0;
		}
	}

	return 1;
}

bool IsMissionComplete(const struct MissionOptions *options)
{
	int rescuesRequired = 0;

	if (!CanCompleteMission(options))
	{
		return 0;
	}

	// Check if dogfight is complete
	if (IsPVP(gCampaign.Entry.Mode) && GetNumPlayers(true, false, false) <= 1)
	{
		// Also check that only one player has lives left
		int numPlayersWithLives = 0;
		for (int i = 0; i < (int)gPlayerDatas.size; i++)
		{
			const PlayerData *p = CArrayGet(&gPlayerDatas, i);
			if (p->Lives > 0)
			{
				numPlayersWithLives++;
			}
		}
		if (numPlayersWithLives <= 1)
		{
			return true;
		}
	}

	// Check that all surviving players are in exit zone
	for (int i = 0; i < (int)gPlayerDatas.size; i++)
	{
		const PlayerData *p = CArrayGet(&gPlayerDatas, i);
		if (!IsPlayerAlive(p))
		{
			continue;
		}
		const TActor *player = CArrayGet(&gActors, p->Id);
		if (!MapIsTileInExit(&gMap, &player->tileItem))
		{
			return 0;
		}
	}

	// Find number of rescues required
	// TODO: support multiple rescue objectives
	for (int i = 0; i < (int)options->missionData->Objectives.size; i++)
	{
		MissionObjective *mobj =
			CArrayGet(&options->missionData->Objectives, i);
		if (mobj->Type == OBJECTIVE_RESCUE)
		{
			rescuesRequired = mobj->Required;
			break;
		}
	}
	// Check that enough prisoners are in exit zone
	if (rescuesRequired > 0)
	{
		int prisonersRescued = 0;
		for (int i = 0; i < (int)gActors.size; i++)
		{
			TActor *a = CArrayGet(&gActors, i);
			if (!a->isInUse)
			{
				continue;
			}
			if (CharacterIsPrisoner(&gCampaign.Setting.characters, ActorGetCharacter(a)) &&
				MapIsTileInExit(&gMap, &a->tileItem))
			{
				prisonersRescued++;
			}
		}
		if (prisonersRescued < rescuesRequired)
		{
			return 0;
		}
	}

	return 1;
}

int KeycardCount(int flags)
{
	int count = 0;
	if (flags & FLAGS_KEYCARD_RED) count++;
	if (flags & FLAGS_KEYCARD_BLUE) count++;
	if (flags & FLAGS_KEYCARD_GREEN) count++;
	if (flags & FLAGS_KEYCARD_YELLOW) count++;
	return count;
}


struct EditorInfo GetEditorInfo(void)
{
	struct EditorInfo ei;
	ei.keyCount = KEYSTYLE_COUNT;
	ei.doorCount = DOORSTYLE_COUNT;
	ei.exitCount = EXIT_COUNT;
	return ei;
}
