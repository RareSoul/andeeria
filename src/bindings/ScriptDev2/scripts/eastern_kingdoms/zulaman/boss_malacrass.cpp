/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Boss_Malacrass
SD%Complete: 90
SDComment: Contain adds and adds selection
SDCategory: Zul'Aman
EndScriptData */

#include "precompiled.h"
#include "zulaman.h"

enum
{
    SAY_AGGRO                   = -1568045,
    SAY_ENRAGE                  = -1568046,
    SAY_KILL1                   = -1568047,
    SAY_KILL2                   = -1568048,
    SAY_SOUL_SIPHON             = -1568049,
    SAY_DRAIN_POWER             = -1568050,
    SAY_SPIRIT_BOLTS            = -1568051,
    SAY_ADD_DIED1               = -1568052,
    SAY_ADD_DIED2               = -1568053,
    SAY_ADD_DIED3               = -1568054,
    SAY_DEATH                   = -1568055,

    SPELL_SPIRIT_BOLTS          = 43383,
    SPELL_SIPHON_SOUL           = 43501,
    SPELL_DRAIN_POWER           = 44131,

	MOB_TEMP_TRIGGER        = 23920,
	
    //for various powers he uses after using soul drain
    //Death Knight
    SPELL_DK_DEATH_AND_DECAY    = 61603,
    SPELL_DK_PLAGUE_STRIKE      = 61606,
    SPELL_DK_MARK_OF_BLOOD      = 61600,

    //Druid
    SPELL_DR_THORNS             = 43420,
    SPELL_DR_LIFEBLOOM          = 43421,
    SPELL_DR_MOONFIRE           = 43545,

    //Hunter
    SPELL_HU_EXPLOSIVE_TRAP     = 43444,
    SPELL_HU_FREEZING_TRAP      = 43447,
    SPELL_HU_SNAKE_TRAP         = 43449,

    //Mage
    SPELL_MG_FIREBALL           = 41383,
    SPELL_MG_FROST_NOVA         = 43426,
    SPELL_MG_ICE_LANCE          = 43427,
    SPELL_MG_FROSTBOLT          = 43428,

    //Paladin
    SPELL_PA_CONSECRATION       = 43429,
    SPELL_PA_AVENGING_WRATH     = 43430,
    SPELL_PA_HOLY_LIGHT         = 43451,

    //Priest
    SPELL_PR_HEAL               = 41372,
    SPELL_PR_MIND_BLAST         = 41374,
    SPELL_PR_SW_DEATH           = 41375,
    SPELL_PR_PSYCHIC_SCREAM     = 43432,
    SPELL_PR_MIND_CONTROL       = 43550,
    SPELL_PR_PAIN_SUPP          = 44416,

    //Rogue
    SPELL_RO_WOUND_POISON       = 39665,
    SPELL_RO_BLIND              = 43433,
    SPELL_RO_SLICE_DICE         = 43457,

    //Shaman
    SPELL_SH_CHAIN_LIGHT        = 43435,
    SPELL_SH_FIRE_NOVA          = 43436,
    SPELL_SH_HEALING_WAVE       = 43548,

    //Warlock
    SPELL_WL_CURSE_OF_DOOM      = 43439,
    SPELL_WL_RAIN_OF_FIRE       = 43440,
    SPELL_WL_UNSTABLE_AFFL      = 35183,

    //Warrior
    SPELL_WR_MORTAL_STRIKE      = 43441,
    SPELL_WR_WHIRLWIND          = 43442,
    SPELL_WR_SPELL_REFLECT      = 43443,

    //misc
    //WEAPON_ID                   = 33494,                    //weapon equip id, must be set by database.
    MAX_ACTIVE_ADDS             = 4
};

#define ORIENT                  1.5696
#define POS_Y                   921.2795
#define POS_Z                   33.8883
#define SAY_ADD_DIED_ONE        -1568031
#define SAY_ADD_DIED_TWO        -1568032
#define SAY_ADD_DIED_THREE      -1568033
static float Pos_X[4] = {127.8827, 122.8827, 112.8827, 107.8827};

static uint32 AddEntryList[8]=
{
    //Far Left
    24240, //Alyson Antille
    24241, //Thurg
    //Left
    24242, //Slither
    24243, //Lord Raadan
    //Right
    24244, //Gazakroth
    24245, //Fenstalker
    //Far Right
    24246, //Darkheart
    24247  //Koragg
};

enum AbilityTarget
{
    ABILITY_TARGET_SELF = 0,
    ABILITY_TARGET_VICTIM = 1,
    ABILITY_TARGET_ENEMY = 2,
    ABILITY_TARGET_HEAL = 3,
    ABILITY_TARGET_BUFF = 4,
    ABILITY_TARGET_SPECIAL = 5
};

struct PlayerAbilityStruct
{
    uint32 spell;
    AbilityTarget target;
    uint32 cooldown;
};

static PlayerAbilityStruct PlayerAbility[][3] =
{
    // 1 warrior
    {{SPELL_WR_MORTAL_STRIKE, ABILITY_TARGET_VICTIM, 6000},
    {SPELL_WR_WHIRLWIND, ABILITY_TARGET_SELF, 10000},
    {SPELL_WR_SPELL_REFLECT, ABILITY_TARGET_SELF, 10000}},    
    // 2 paladin
    {{SPELL_PA_CONSECRATION, ABILITY_TARGET_SELF, 10000},
    {SPELL_PA_HOLY_LIGHT, ABILITY_TARGET_HEAL, 10000},
    {SPELL_PA_AVENGING_WRATH, ABILITY_TARGET_SELF, 10000}},
    // 3 hunter
    {{SPELL_HU_EXPLOSIVE_TRAP, ABILITY_TARGET_SELF, 10000},
    {SPELL_HU_FREEZING_TRAP, ABILITY_TARGET_SELF, 10000},
    {SPELL_HU_SNAKE_TRAP, ABILITY_TARGET_SELF, 10000}},
    // 4 rogue
    {{SPELL_RO_WOUND_POISON, ABILITY_TARGET_VICTIM, 3000},
    {SPELL_RO_BLIND, ABILITY_TARGET_ENEMY, 10000},
    {SPELL_RO_SLICE_DICE, ABILITY_TARGET_SELF, 10000}},    
    // 5 priest
    {{SPELL_PR_PAIN_SUPP, ABILITY_TARGET_HEAL, 10000},
    {SPELL_PR_HEAL, ABILITY_TARGET_HEAL, 10000},
    {SPELL_PR_PSYCHIC_SCREAM, ABILITY_TARGET_SELF, 10000}},
    // 5* shadow priest
    {{SPELL_PR_MIND_CONTROL, ABILITY_TARGET_ENEMY, 15000},
    {SPELL_PR_MIND_BLAST, ABILITY_TARGET_ENEMY, 5000},
    {SPELL_PR_SW_DEATH, ABILITY_TARGET_ENEMY, 10000}},
    // 7 shaman
    {{SPELL_SH_CHAIN_LIGHT, ABILITY_TARGET_ENEMY, 8000},
    {SPELL_SH_FIRE_NOVA, ABILITY_TARGET_SELF, 10000},
    {SPELL_SH_HEALING_WAVE, ABILITY_TARGET_HEAL, 10000}},
    // 8 mage
    {{SPELL_MG_FIREBALL, ABILITY_TARGET_ENEMY, 5000},
    {SPELL_MG_FROSTBOLT, ABILITY_TARGET_ENEMY, 5000},
    //{SPELL_MG_FROST_NOVA, ABILITY_TARGET_VICTIM, 1000},
    {SPELL_MG_ICE_LANCE, ABILITY_TARGET_SPECIAL, 2000}},
    // 9 warlock
    {{SPELL_WL_CURSE_OF_DOOM, ABILITY_TARGET_ENEMY, 10000},
    {SPELL_WL_RAIN_OF_FIRE, ABILITY_TARGET_ENEMY, 10000},
    {SPELL_WL_UNSTABLE_AFFL, ABILITY_TARGET_ENEMY, 10000}},
    // 11 druid
    {{SPELL_DR_LIFEBLOOM, ABILITY_TARGET_HEAL, 10000},
    {SPELL_DR_THORNS, ABILITY_TARGET_SELF, 10000},
    {SPELL_DR_MOONFIRE, ABILITY_TARGET_ENEMY, 8000}}
};
struct MANGOS_DLL_DECL boss_malacrassAI : public ScriptedAI
{
    boss_malacrassAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        SelectAddEntry();
        for(uint8 i = 0; i < 4; ++i)
            AddGUID[i] = 0;
        Reset();
    }

    ScriptedInstance* m_pInstance;

    std::list<uint32> m_lAddsEntryList;
    uint64 m_auiAddGUIDs[MAX_ACTIVE_ADDS];

    uint64 AddGUID[4];
    uint32 AddEntry[4];

    uint64 PlayerGUID;

    uint32 SpiritBolts_Timer;
    bool bDrainPower;
    uint32 DrainPower_Timer;
    uint32 SiphonSoul_Timer;
    uint32 PlayerAbility_Timer;
    uint32 CheckAddState_Timer;

    uint32 PlayerClass;

    Unit* SoulDrainTarget;
    void Reset()
    {

        if (!m_pInstance)
            return;

        SpiritBolts_Timer = 20000;
        bDrainPower = false;
        DrainPower_Timer = 60000;
        SiphonSoul_Timer = 100000;
        PlayerAbility_Timer = 99999;
        CheckAddState_Timer = 5000;
		 
        m_pInstance->SetData(TYPE_MALACRASS, NOT_STARTED);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MALACRASS, FAIL);

		CleanAdds();
		
        for(uint8 i = 0; i < MAX_ACTIVE_ADDS; ++i)
        {
            if (Creature* pAdd = (Creature*)Unit::GetUnit(*m_creature, m_auiAddGUIDs[i]))
                pAdd->AI()->EnterEvadeMode();
        }
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
		
		SpawnAdds();

        DoScriptText(SAY_AGGRO, m_creature);

        if (!m_pInstance)
            return;

        for(uint8 i = 0; i < 4; ++i)
        {
            Unit* Temp = Unit::GetUnit((*m_creature),AddGUID[i]);
            if(Temp && Temp->isAlive())
                ((Creature*)Temp)->AI()->AttackStart(m_creature->getVictim());
            else
            {
                EnterEvadeMode();
                break;
            }
        }
        m_pInstance->SetData(TYPE_MALACRASS, IN_PROGRESS);
    }

	void SelectAddEntry()
        {
        std::vector<uint32> AddList;

        for(uint8 i = 0; i < 8; ++i)
            AddList.push_back(AddEntryList[i]);
        
        uint8 i = 0;
        while(AddList.size() > 4)
            {
            AddList.erase(AddList.begin()+rand()%2+i);
            i++;
        }

        i=0;
        for(std::vector<uint32>::iterator itr = AddList.begin(); itr != AddList.end(); ++itr, ++i)
            AddEntry[i] = *itr;
            }

    void SpawnAdds()
        {
        for(uint8 i = 0; i < 4; ++i)
            {
            Creature *pCreature = ((Creature*)Unit::GetUnit((*m_creature), AddGUID[i]));
            if(!pCreature || !pCreature->isAlive())
                {
                if(pCreature) pCreature->setDeathState(DEAD);
                pCreature = m_creature->SummonCreature(AddEntry[i], Pos_X[i], POS_Y, POS_Z, ORIENT, TEMPSUMMON_DEAD_DESPAWN, 0);
                if(pCreature) AddGUID[i] = pCreature->GetGUID();
                }
            else
            {
                pCreature->AI()->EnterEvadeMode();
                pCreature->Relocate(Pos_X[i], POS_Y, POS_Z, ORIENT);
                pCreature->StopMoving();
            }
        }
    }

    void AddDied()
    {
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_ADD_DIED_ONE, m_creature); break;
            case 1: DoScriptText(SAY_ADD_DIED_TWO, m_creature); break;
            case 2: DoScriptText(SAY_ADD_DIED_THREE, m_creature); break;
    }
    }

    void AddsAttack(Unit* pWho)
    {
        for(uint8 i = 0; i < MAX_ACTIVE_ADDS; ++i)
        {
            if (Creature* pAdd = (Creature*)Unit::GetUnit(*m_creature, m_auiAddGUIDs[i]))
            {
                if (!pAdd->getVictim())
                    pAdd->AI()->AttackStart(pWho);
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        DoScriptText(urand(0, 1) ? SAY_KILL1 : SAY_KILL2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
        CleanAdds();

        if (!m_pInstance)
            return;

        m_pInstance->SetData(TYPE_MALACRASS, DONE);
    }

    void CleanAdds()
    {
        for(uint8 i = 0; i < MAX_ACTIVE_ADDS; ++i)
        {
            if (Creature* pAdd = (Creature*)Unit::GetUnit(*m_creature, m_auiAddGUIDs[i]))
            {
                pAdd->AI()->EnterEvadeMode();
                pAdd->setDeathState(JUST_DIED);
            }
        }

        memset(&m_auiAddGUIDs, 0, sizeof(m_auiAddGUIDs));
        m_lAddsEntryList.clear();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(CheckAddState_Timer < diff)
        {
            for(uint8 i = 0; i < 4; ++i)
            {
                Unit* Temp = Unit::GetUnit((*m_creature),AddGUID[i]);
                if(Temp && Temp->isAlive() && !Temp->getVictim())
                    ((Creature*)Temp)->AI()->AttackStart(m_creature->getVictim());
            }
            CheckAddState_Timer = 5000;
        }else CheckAddState_Timer -= diff;
		
        if(m_creature->GetHealth() * 10 < m_creature->GetMaxHealth() * 8 && !bDrainPower)
            bDrainPower = true;
        
        if(bDrainPower)
        {
            if(DrainPower_Timer < diff)
            {
                DoScriptText(SAY_DRAIN_POWER, m_creature);
                m_creature->CastSpell(m_creature, SPELL_DRAIN_POWER, true);                
                DrainPower_Timer = 40000 + rand()%15000;    // must cast in 60 sec, or buff/debuff will disappear
            }else DrainPower_Timer -= diff;
        }

        if(SpiritBolts_Timer < diff)
        {
            if(DrainPower_Timer < 12000)    // channel 10 sec
                SpiritBolts_Timer = 13000;  // cast drain power first
            else
            {
                m_creature->CastSpell(m_creature, SPELL_SPIRIT_BOLTS, false);
                DoScriptText(SAY_SPIRIT_BOLTS, m_creature);
                SpiritBolts_Timer = 40000;
                SiphonSoul_Timer = 10000;  // ready to drain
                PlayerAbility_Timer = 99999;
            }
        }else SpiritBolts_Timer -= diff;

        if(SiphonSoul_Timer < diff)
        {
            Player* target = SelectRandomPlayer(50);
            Unit *trigger = DoSpawnCreature(MOB_TEMP_TRIGGER, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN, 30000);
            if (!target || !trigger) EnterEvadeMode();
            else
            {
				DoScriptText(SAY_SOUL_SIPHON , m_creature);
                trigger->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
                trigger->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                trigger->CastSpell(target, SPELL_SIPHON_SOUL, true);
                trigger->GetMotionMaster()->MoveChase(m_creature);

                PlayerGUID = target->GetGUID();
                PlayerAbility_Timer = 8000 + rand()%2000;
                PlayerClass = target->getClass() - 1;
                if(PlayerClass == 10) PlayerClass = 9; // druid
                if(PlayerClass == 4 && target->HasSpell(15473)) PlayerClass = 5; // shadow priest
                SiphonSoul_Timer = 99999;   // buff lasts 30 sec
            }
        }else SiphonSoul_Timer -= diff;

        if(PlayerAbility_Timer < diff)
        {
            UseAbility();
            PlayerAbility_Timer = 8000 + rand()%2000;
        }else PlayerAbility_Timer -= diff;

        DoMeleeAttackIfReady();
    }

    void UseAbility()
    {
        uint32 random = rand()%3; 
		
		//random = (PlayerClass == 7 ? rand()%4 : rand()%3);
        Unit *target = NULL;
        switch (PlayerAbility[PlayerClass][random].target)
        {
        case ABILITY_TARGET_SELF:
            target = m_creature;
            break;
        case ABILITY_TARGET_VICTIM:
            target = m_creature->getVictim();
            break;
        case ABILITY_TARGET_HEAL:
            target = DoSelectLowestHpFriendly(50, 0);
            break;
        case ABILITY_TARGET_BUFF:
            {
                std::list<Creature*> templist = DoFindFriendlyMissingBuff(50, PlayerAbility[PlayerClass][random].spell);
                if(!templist.empty()) target = *(templist.begin());
            }
            break;
		case ABILITY_TARGET_ENEMY:
        default:
            if (Unit* temp = SelectUnit(SELECT_TARGET_RANDOM, 0))
                target = temp;

            break;
        }
        if (target)
            m_creature->CastSpell(target, PlayerAbility[PlayerClass][random].spell, false);
    }

    Player* SelectRandomPlayer(float range = 0.0f, bool alive = true)
    {
        Map *map = m_creature->GetMap();
        if (!map->IsDungeon()) return NULL;

        Map::PlayerList const &PlayerList = map->GetPlayers();
        if (PlayerList.isEmpty())
            return NULL;
        
        std::list<Player*> temp;
        std::list<Player*>::iterator j;
		
        for(Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if((range == 0.0f || m_creature->IsWithinDistInMap(i->getSource(), range))
                && (!alive || i->getSource()->isAlive()))
                    temp.push_back(i->getSource());

        if (temp.size()) 
        {
            j = temp.begin();
		    advance(j, rand()%temp.size());
            return (*j);
        }
        return NULL;
    }
};

CreatureAI* GetAI_boss_malacrass(Creature* pCreature)
{
    return new boss_malacrassAI(pCreature);
}

//common AI for adds
struct MANGOS_DLL_DECL boss_malacrass_addAI : public ScriptedAI
{
    boss_malacrass_addAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset() { }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
    }

    void MoveInLineOfSight(Unit* pWho)
    {
    }

    void KilledUnit(Unit* pVictim)
    {
        if (!m_pInstance)
            return;

        if (Creature* pMalacrass = (Creature*)Unit::GetUnit(*m_creature, m_pInstance->GetData64(DATA_MALACRASS)))
            ((boss_malacrassAI*)pMalacrass->AI())->KilledUnit(pVictim);
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance)
            return;

        if (Unit* pMalacrass = Unit::GetUnit(*m_creature, m_pInstance->GetData64(DATA_MALACRASS)))
        {
            switch(urand(0, 2))
            {
                case 0: DoScriptText(SAY_ADD_DIED1, pMalacrass); break;
                case 1: DoScriptText(SAY_ADD_DIED2, pMalacrass); break;
                case 2: DoScriptText(SAY_ADD_DIED3, pMalacrass); break;
            }
        }
    }

    bool IsEnemyPlayerInRangeForSpell(uint32 uiSpellId)
    {
        SpellEntry const* pSpell = GetSpellStore()->LookupEntry(uiSpellId);

        //if spell not valid
        if (!pSpell)
            return false;

        //spell known, so lookup using rangeIndex
        SpellRangeEntry const* pSpellRange = GetSpellRangeStore()->LookupEntry(pSpell->rangeIndex);

        //not valid, so return
        if (!pSpellRange)
            return false;

        std::list<HostileReference*>& lThreatList = m_creature->getThreatManager().getThreatList();

        for(std::list<HostileReference*>::iterator iter = lThreatList.begin(); iter != lThreatList.end(); ++iter)
        {
            Unit* pTarget = Unit::GetUnit((*m_creature), (*iter)->getUnitGuid());

            if (pTarget && pTarget->GetTypeId() == TYPEID_PLAYER)
            {
                //if target further away than maxrange or closer than minrange, statement is false
                if (m_creature->IsInRange(pTarget, pSpellRange->minRange, pSpellRange->maxRange))
                    return true;
            }
        }

        return false;
    }
};

enum
{
    SPELL_BLOODLUST = 43578,
    SPELL_CLEAVE    = 15496
};

struct MANGOS_DLL_DECL mob_thurgAI : public boss_malacrass_addAI
{
    mob_thurgAI(Creature* pCreature) : boss_malacrass_addAI(pCreature) { Reset(); }

    uint32 m_uiBloodlustTimer;
    uint32 m_uiCleaveTimer;

    void Reset()
    {
        m_uiBloodlustTimer = 15000;
        m_uiCleaveTimer = 10000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBloodlustTimer < uiDiff)
        {
            std::list<Creature*> lTempList = DoFindFriendlyMissingBuff(50.0f, SPELL_BLOODLUST);

            if (!lTempList.empty())
            {
                Unit* pTarget = *(lTempList.begin());
                if (pTarget) DoCast(pTarget, SPELL_BLOODLUST);
            }

            m_uiBloodlustTimer = 12000;
        }
        else
            m_uiBloodlustTimer -= uiDiff;

        if (m_uiCleaveTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_CLEAVE);
            m_uiCleaveTimer = 12000;
        }
        else
            m_uiCleaveTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_thurg(Creature* pCreature)
{
    return new mob_thurgAI(pCreature);
}

enum
{
    SPELL_ARCANE_TORRENT = 33390,
    SPELL_FLASH_HEAL     = 43575,
    SPELL_DISPEL_MAGIC   = 43577
};

const float RANGE_FRIENDLY_TARGET = 40.0;

struct MANGOS_DLL_DECL mob_alyson_antilleAI : public boss_malacrass_addAI
{
    mob_alyson_antilleAI(Creature* pCreature) : boss_malacrass_addAI(pCreature) { Reset(); }

    uint32 m_uiArcaneTorrentTimer;
    uint32 m_uiFlashHealTimer;
    uint32 m_uiDispelMagicTimer;

    void Reset()
    {
        m_uiArcaneTorrentTimer = 0;
        m_uiFlashHealTimer = 2500;
        m_uiDispelMagicTimer = 10000;
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho)
            return;

        if (m_creature->Attack(pWho, false))
        {
            m_creature->AddThreat(pWho, 0.0f);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            m_creature->GetMotionMaster()->MoveChase(pWho, 20.0f);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiArcaneTorrentTimer < uiDiff)
        {
            if (IsEnemyPlayerInRangeForSpell(SPELL_ARCANE_TORRENT))
            {
                DoCast(m_creature, SPELL_ARCANE_TORRENT);
                m_uiArcaneTorrentTimer = 60000;
            }
            else
                m_uiArcaneTorrentTimer = 1000;
        }
        else
            m_uiArcaneTorrentTimer -= uiDiff;

        if (m_uiFlashHealTimer < uiDiff)
        {
            //this will fail if we previously was following target and pTarget is now different than before
            if (Unit* pTarget = DoSelectLowestHpFriendly(RANGE_FRIENDLY_TARGET*2, 30000))
            {
                if (pTarget->IsWithinDistInMap(m_creature, RANGE_FRIENDLY_TARGET))
                {
                    DoCast(pTarget, SPELL_FLASH_HEAL);

                    //if not already chasing, start chase
                    if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() != TARGETED_MOTION_TYPE)
                        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim(), 20.0f);
                }
                else
                {
                    //if chasing, start follow target instead
                    if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == TARGETED_MOTION_TYPE)
                    {
                        m_creature->GetMotionMaster()->MovementExpired();
                        m_creature->GetMotionMaster()->MoveFollow(pTarget, 20.0f, 0.0f);
                    }
                }
            }

            m_uiFlashHealTimer = 2500;
        }
        else
            m_uiFlashHealTimer -= uiDiff;

        if (m_uiDispelMagicTimer < uiDiff)
        {
            Unit* pTarget = NULL;
            std::list<Creature*> lTempList = DoFindFriendlyCC(RANGE_FRIENDLY_TARGET);

            if (!lTempList.empty())
                pTarget = *(lTempList.begin());
            else
            {
                if (Unit* temp = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    pTarget = temp;
            }

            if (pTarget)
                DoCast(pTarget, SPELL_DISPEL_MAGIC, false);

            m_uiDispelMagicTimer = 12000;
        }
        else
            m_uiDispelMagicTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_alyson_antille(Creature* pCreature)
{
    return new mob_alyson_antilleAI(pCreature);
}

enum
{
    SPELL_FIREBOLT = 43584
};

struct MANGOS_DLL_DECL mob_gazakrothAI : public boss_malacrass_addAI
{
    mob_gazakrothAI(Creature* pCreature) : boss_malacrass_addAI(pCreature){ Reset(); }

    uint32 m_uiFireboltTimer;

    void Reset()
    {
        m_uiFireboltTimer = 1000;
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho)
            return;

        if (m_creature->Attack(pWho, false))
        {
            m_creature->AddThreat(pWho, 0.0f);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            m_creature->GetMotionMaster()->MoveChase(pWho, 20.0f);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFireboltTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_FIREBOLT);
            m_uiFireboltTimer = 1000;
        }
        else
            m_uiFireboltTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_gazakroth(Creature* pCreature)
{
    return new mob_gazakrothAI(pCreature);
}

enum
{
    SPELL_FLAME_BREATH = 43582,
    SPELL_THUNDERCLAP  = 43583
};

struct MANGOS_DLL_DECL mob_lord_raadanAI : public boss_malacrass_addAI
{
    mob_lord_raadanAI(Creature* pCreature) : boss_malacrass_addAI(pCreature) { Reset(); }

    uint32 m_uiFlameBreathTimer;
    uint32 m_uiThunderclapTimer;

    void Reset()
    {
        m_uiFlameBreathTimer = 8000;
        m_uiThunderclapTimer = 13000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiThunderclapTimer < uiDiff)
        {
            if (IsEnemyPlayerInRangeForSpell(SPELL_THUNDERCLAP))
            {
                DoCast(m_creature->getVictim(), SPELL_THUNDERCLAP);
                m_uiThunderclapTimer = 12000;
            }
            else
                m_uiThunderclapTimer = 1000;
        }
        else
            m_uiThunderclapTimer -= uiDiff;

        if (m_uiFlameBreathTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_FLAME_BREATH);
            m_uiFlameBreathTimer = 12000;
        }
        else
            m_uiFlameBreathTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_lord_raadan(Creature* pCreature)
{
    return new mob_lord_raadanAI(pCreature);
}

enum
{
    SPELL_PSYCHIC_WAIL = 43590
};

struct MANGOS_DLL_DECL mob_darkheartAI : public boss_malacrass_addAI
{
    mob_darkheartAI(Creature* pCreature) : boss_malacrass_addAI(pCreature) { Reset(); }

    uint32 m_uiPsychicWailTimer;

    void Reset()
    {
        m_uiPsychicWailTimer = 8000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiPsychicWailTimer < uiDiff)
        {
            if (IsEnemyPlayerInRangeForSpell(SPELL_PSYCHIC_WAIL))
            {
                DoCast(m_creature->getVictim(), SPELL_PSYCHIC_WAIL);
                m_uiPsychicWailTimer = 12000;
            }
            else
                m_uiPsychicWailTimer = 1000;
        }
        else
            m_uiPsychicWailTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_darkheart(Creature* pCreature)
{
    return new mob_darkheartAI(pCreature);
}

enum
{
    SPELL_VENOM_SPIT = 43579
};

struct MANGOS_DLL_DECL mob_slitherAI : public boss_malacrass_addAI
{
    mob_slitherAI(Creature* pCreature) : boss_malacrass_addAI(pCreature) { Reset(); }

    uint32 m_uiVenomSpitTimer;

    void Reset()
    {
        m_uiVenomSpitTimer = 4000;
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho)
            return;

        if (m_creature->Attack(pWho, false))
        {
            m_creature->AddThreat(pWho, 0.0f);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            m_creature->GetMotionMaster()->MoveChase(pWho, 20.0f);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiVenomSpitTimer < uiDiff)
        {
            if (Unit* pVictim = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pVictim, SPELL_VENOM_SPIT);

            m_uiVenomSpitTimer = 2500;
        }
        else
            m_uiVenomSpitTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_slither(Creature* pCreature)
{
    return new mob_slitherAI(pCreature);
}

enum
{
    SPELL_VOLATILE_INFECTION = 43586
};

struct MANGOS_DLL_DECL mob_fenstalkerAI : public boss_malacrass_addAI
{
    mob_fenstalkerAI(Creature* pCreature) : boss_malacrass_addAI(pCreature) { Reset(); }

    uint32 m_uiVolatileInfectionTimer;

    void Reset()
    {
        m_uiVolatileInfectionTimer = 15000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiVolatileInfectionTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_VOLATILE_INFECTION);
            m_uiVolatileInfectionTimer = 12000;
        }
        else
            m_uiVolatileInfectionTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_fenstalker(Creature* pCreature)
{
    return new mob_fenstalkerAI(pCreature);
}

enum
{
    SPELL_COLD_STARE  = 43593,
    SPELL_MIGHTY_BLOW = 43592,
};

struct MANGOS_DLL_DECL mob_koraggAI : public boss_malacrass_addAI
{
    mob_koraggAI(Creature* pCreature) : boss_malacrass_addAI(pCreature) { Reset(); }

    uint32 m_uiColdStareTimer;
    uint32 m_uiMightyBlowTimer;

    void Reset()
    {
        m_uiColdStareTimer = 15000;
        m_uiMightyBlowTimer = 10000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiMightyBlowTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_MIGHTY_BLOW);
            m_uiMightyBlowTimer = 12000;
        }
        else
            m_uiMightyBlowTimer -= uiDiff;

        if (m_uiColdStareTimer < uiDiff)
        {
            if (Unit* pVictim = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pVictim, SPELL_COLD_STARE);

            m_uiColdStareTimer = 12000;
        }
        else
            m_uiColdStareTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_koragg(Creature* pCreature)
{
    return new mob_koraggAI(pCreature);
}

void AddSC_boss_malacrass()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_malacrass";
    newscript->GetAI = &GetAI_boss_malacrass;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_thurg";
    newscript->GetAI = &GetAI_mob_thurg;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_gazakroth";
    newscript->GetAI = &GetAI_mob_gazakroth;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_lord_raadan";
    newscript->GetAI = &GetAI_mob_lord_raadan;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_darkheart";
    newscript->GetAI = &GetAI_mob_darkheart;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_slither";
    newscript->GetAI = &GetAI_mob_slither;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_fenstalker";
    newscript->GetAI = &GetAI_mob_fenstalker;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_koragg";
    newscript->GetAI = &GetAI_mob_koragg;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_alyson_antille";
    newscript->GetAI = &GetAI_mob_alyson_antille;
    newscript->RegisterSelf();
}
