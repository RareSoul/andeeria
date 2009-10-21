#ifndef __MANGOS_CREATUREGROUP_H
#define __MANGOS_CREATUREGROUP_H

#include "Common.h"
#include "Policies/Singleton.h"
#include <map>
#include <string>
#include "Creature.h"

struct FormationMember
{
	uint32 leaderGuid;
	uint32 memberGuid;

	float dist;
	float angle;

	uint8 groupAI;
};

class CreatureGroupManager
{
	public:
		void LoadCreatureFormation();
		void UpdateCreatureGroup(uint32 gruopId, Creature *member);
		void DestroyGroup(uint32 groupId, uint64 memberGuid);
};

class CreatureGroup
{
	UNORDERED_MAP<uint64, Creature*>CreatureGroupMembers;
	Creature *m_leader;

	public:
		CreatureGroup() : m_leader(NULL) {}
		~CreatureGroup(){ sLog.outDebug("Destroying group"); }
		void AddMember(Creature *);
		void RemoveMember(uint64 guid);
		void MoveToLeader();
		void GroupAttack(Creature *);
		void SetDestination(Creature *);
		bool isEmpty() {return CreatureGroupMembers.empty();}
};

extern UNORDERED_MAP<uint32, CreatureGroup*> CreatureGroupHolder;
extern UNORDERED_MAP<uint32, FormationMember*> CreatureGroupMap;

#define formationMgr MaNGOS::Singleton<CreatureGroupManager>::Instance()
#endif