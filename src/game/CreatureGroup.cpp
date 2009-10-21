#include "Creature.h"
#include "CreatureGroup.h"
#include "ObjectMgr.h"
#include "ProgressBar.h"
#include "Policies/SingletonImp.h"

#define MAX_DESYNC 5.0f

INSTANTIATE_SINGLETON_1(CreatureGroupManager);
UNORDERED_MAP<uint32, CreatureGroup*> CreatureGroupHolder;
UNORDERED_MAP<uint32, FormationMember*> CreatureGroupMap;