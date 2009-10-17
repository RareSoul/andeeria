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
SDName: boss_Akilzon
SD%Complete: 75%
SDComment: Missing timer for Call Lightning and Sound ID's
SQLUpdate: 
#Temporary fix for Soaring Eagles

EndScriptData */

#include "precompiled.h"
#include "zulaman.h"
#include "Spell.h"
#include "Weather.h"

enum
{
    //"Your death gonna be quick, strangers. You shoulda never have come to this place..."
    SAY_EVENT1              = -1568024,
    SAY_EVENT2              = -1568025,
    SAY_AGGRO               = -1568026,
    SAY_SUMMON              = -1568027,
    SAY_SUMMON_ALT          = -1568028,
    SAY_ENRAGE              = -1568029,
    SAY_SLAY1               = -1568030,
    SAY_SLAY2               = -1568031,
    SAY_DEATH               = -1568032,
    EMOTE_STORM             = -1568033,

    SPELL_STATIC_DISRUPTION = 43622,
    SPELL_STATIC_VISUAL     = 45265,

    SPELL_CALL_LIGHTNING    = 43661,
    SPELL_GUST_OF_WIND      = 43621,

    SPELL_ELECTRICAL_STORM  = 43648,
    SPELL_STORMCLOUD_VISUAL = 45213,

    SPELL_BERSERK           = 45078,

    MOB_SOARING_EAGLE       = 24858,
    MOB_TEMP_TRIGGER        = 23920,
    MAX_EAGLE_COUNT         = 6,

    SE_LOC_X_MAX            = 400,
    SE_LOC_X_MIN            = 335,
    SE_LOC_Y_MAX            = 1435,
    SE_LOC_Y_MIN            = 1370
};

struct MANGOS_DLL_DECL boss_akilzonAI : public ScriptedAI
{
    boss_akilzonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }
    ScriptedInstance* m_pInstance;

    uint64 TargetGUID;
    uint64 CycloneGUID;
    uint64 CloudGUID;

    uint32 m_uiStaticDisruptTimer;
    uint32 m_uiGustOfWindTimer;
    uint32 m_uiCallLightTimer;
    uint32 m_uiStormTimer;
    uint32 SDisruptAOEVisual_Timer;
    uint32 SummonEagles_Timer;
    uint32 Enrage_Timer;

    uint32 StormCount;
    uint32 StormSequenceTimer;

    bool isRaining;

    void Reset()
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_AKILZON, NOT_STARTED);

        m_uiStaticDisruptTimer = urand(10000, 20000); //10 to 20 seconds (bosskillers)
        m_uiCallLightTimer = urand(15000, 25000);
        m_uiGustOfWindTimer = urand(20000, 30000);//20 to 30 seconds(bosskillers)
        m_uiStormTimer = 60*1000; //60 seconds(bosskillers)
        Enrage_Timer = 10*60*1000; //10 minutes till enrage(bosskillers)
        SDisruptAOEVisual_Timer = 99999;
        SummonEagles_Timer = 99999;

        TargetGUID = 0;
        CloudGUID = 0;
        CycloneGUID = 0;

        StormCount = 0;
        StormSequenceTimer = 0;

        isRaining = false;

        DespawnSummons(MOB_SOARING_EAGLE);
        SetWeather(WEATHER_STATE_FINE, 0.0f);        
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        m_creature->SetInCombatWithZone();
        
        if(m_pInstance)
            m_pInstance->SetData(TYPE_AKILZON, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
        
        if(m_pInstance)
            m_pInstance->SetData(TYPE_AKILZON, DONE);
        DespawnSummons(MOB_SOARING_EAGLE);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void DespawnSummons(uint32 entry)
    {
        std::list<Creature*> templist;
        float x, y, z;
        m_creature->GetPosition(x, y, z);

        {
            CellPair pair(MaNGOS::ComputeCellPair(x, y));
            Cell cell(pair);
            cell.data.Part.reserved = ALL_DISTRICT;
            cell.SetNoCreate();

            AllCreaturesOfEntryInRange check(m_creature, entry, 100);
            MaNGOS::CreatureListSearcher<AllCreaturesOfEntryInRange> searcher(m_creature, templist, check);

            TypeContainerVisitor<MaNGOS::CreatureListSearcher<AllCreaturesOfEntryInRange>, GridTypeMapContainer> cSearcher(searcher);

            CellLock<GridReadGuard> cell_lock(cell, pair);
            cell_lock->Visit(cell_lock, cSearcher, *(m_creature->GetMap()));
        }

        for(std::list<Creature*>::iterator i = templist.begin(); i != templist.end(); ++i)
        {
            (*i)->SetVisibility(VISIBILITY_OFF);
            (*i)->setDeathState(JUST_DIED);
        }
    }

    Player* SelectRandomPlayer(float range = 0.0f, bool alive = true)
    {
        Map *map = m_creature->GetMap();
        if (!map->IsDungeon()) 
            return NULL;

        Map::PlayerList const &PlayerList = map->GetPlayers();
        if (PlayerList.isEmpty())
            return NULL;
        
        std::list<Player*> temp;
        std::list<Player*>::iterator j;
		
        for(Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
			if((range == 0.0f || m_creature->IsWithinDistInMap(i->getSource(), range))
				&& (!alive || i->getSource()->isTargetableForAttack()))
				temp.push_back(i->getSource());

		if (temp.size())
        {
			j = temp.begin();
		    advance(j, rand()%temp.size());
		    return (*j);
		}
        return NULL;

    }

    void SetWeather(uint32 weather, float grade)
    {
        Map *map = m_creature->GetMap();
        if (!map->IsDungeon()) 
            return;

        WorldPacket data(SMSG_WEATHER, (4+4+4));
        data << uint32(weather) << (float)grade << uint8(0);

        ((InstanceMap*)map)->SendToPlayers(&data);
    }

    void HandleStormSequence(Unit *Cloud) // 1: begin, 2-9: tick, 10: end
    {
        if(StormCount < 10 && StormCount > 1)
        {
            // deal damage
            int32 bp0 = 800;
            for(uint8 i = 2; i < StormCount; ++i)
                bp0 *= 2;

            CellPair p(MaNGOS::ComputeCellPair(m_creature->GetPositionX(), m_creature->GetPositionY()));
            Cell cell(p);
            cell.data.Part.reserved = ALL_DISTRICT;
            cell.SetNoCreate();

            std::list<Unit *> tempUnitMap;

            {
                MaNGOS::AnyAoETargetUnitInObjectRangeCheck u_check(m_creature, m_creature, 999);
                MaNGOS::UnitListSearcher<MaNGOS::AnyAoETargetUnitInObjectRangeCheck> searcher(m_creature, tempUnitMap, u_check);

                TypeContainerVisitor<MaNGOS::UnitListSearcher<MaNGOS::AnyAoETargetUnitInObjectRangeCheck>, WorldTypeMapContainer > world_unit_searcher(searcher);
                TypeContainerVisitor<MaNGOS::UnitListSearcher<MaNGOS::AnyAoETargetUnitInObjectRangeCheck>, GridTypeMapContainer >  grid_unit_searcher(searcher);

                CellLock<GridReadGuard> cell_lock(cell, p);
                cell_lock->Visit(cell_lock, world_unit_searcher, *(m_creature->GetMap()));
                cell_lock->Visit(cell_lock, grid_unit_searcher, *(m_creature->GetMap()));
    }

            for(std::list<Unit*>::iterator i = tempUnitMap.begin(); i != tempUnitMap.end(); ++i)
    {
                if(!Cloud->IsWithinDistInMap(*i, 15))
        {
                    float x, y, z;
                    (*i)->GetPosition(x, y, z);
                    x = rand()%2 ? x + rand()%5 : x - rand()%5;
                    y = rand()%2 ? y + rand()%5 : y - rand()%5;
                    z = Cloud->GetPositionZ() + 2 - rand()%4; 

                    if(Creature *trigger = m_creature->SummonCreature(MOB_TEMP_TRIGGER, x, y, z, 0, TEMPSUMMON_TIMED_DESPAWN, 2000))
                    {
						trigger->SetMonsterMoveFlags(MONSTER_MOVE_LEVITATING);
                        trigger->StopMoving();
                        trigger->CastSpell(trigger, 37248, true);
                        trigger->CastCustomSpell(*i, 43137, &bp0, NULL, NULL, true, 0, 0, m_creature->GetGUID());
        }
    }
            }

            // visual
            float x, y, z;
            for(uint8 i = 0; i < StormCount; ++i)
            {
                Cloud->GetPosition(x, y, z);
                x = rand()%2 ? x + rand()%10 : x - rand()%10;
                y = rand()%2 ? y + rand()%10 : y - rand()%10;
                z = z + 2 - rand()%4; 
                
                if(Creature *trigger = m_creature->SummonCreature(MOB_TEMP_TRIGGER, x, y, z, 0, TEMPSUMMON_TIMED_DESPAWN, 2000))
                {
                    trigger->SetMonsterMoveFlags(MONSTER_MOVE_LEVITATING);
                    trigger->StopMoving();
                    trigger->CastSpell(trigger, 37248, true);
                }

                Cloud->GetPosition(x, y, z);
                x = rand()%2 ? x + 10 + rand()%10 : x - 10 - rand()%10;
                y = rand()%2 ? y + 10 + rand()%10 : y - 10 - rand()%10;
                
                if(Unit *trigger = m_creature->SummonCreature(MOB_TEMP_TRIGGER, x, y, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 2000))
                {
                    trigger->SetMaxHealth(9999999);
                    trigger->SetHealth(9999999);
                    trigger->CastSpell(trigger, 43661, true);
                }
            }
        }

        StormCount++;
        
        if (StormCount > 10)
        {
            StormCount = 0; // finish
            SummonEagles_Timer = 5000;
            m_creature->InterruptNonMeleeSpells(false);
            Cloud->RemoveAurasDueToSpell(45213);
            CloudGUID = 0;
            
            if (Unit* Cyclone = Unit::GetUnit(*m_creature, CycloneGUID))
                Cyclone->RemoveAurasDueToSpell(25160);
            
            SetWeather(WEATHER_STATE_FINE, 0.0f);
            isRaining = false;
        }

        StormSequenceTimer = 1000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(StormCount)
        {
            Unit* target = Unit::GetUnit(*m_creature, CloudGUID);
            if(!target || !target->isAlive())
            {
                EnterEvadeMode();
                return;
            }
            else if(Unit* Cyclone = Unit::GetUnit(*m_creature, CycloneGUID))
                Cyclone->CastSpell(target, 25160, true); // keep casting or...

            if(StormSequenceTimer < uiDiff)
            {
                HandleStormSequence(target);
            }else StormSequenceTimer -= uiDiff;

            return;
        }

        if (Enrage_Timer < uiDiff) 
        {
            DoScriptText(SAY_ENRAGE, m_creature);
            m_creature->CastSpell(m_creature, SPELL_BERSERK, true);
            Enrage_Timer = 600000;
        }else Enrage_Timer -= uiDiff;

        if (m_uiStaticDisruptTimer < uiDiff) 
        {
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 1);
            if(!target)
                target = m_creature->getVictim();
   
            TargetGUID = target->GetGUID();
            m_creature->CastSpell(target, SPELL_STATIC_DISRUPTION, false);
            m_creature->SetInFront(m_creature->getVictim());
            m_uiStaticDisruptTimer = urand(7000, 14000); // < 20s

            float dist = m_creature->GetDistance(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());
            if (dist < 5.0f) 
                dist = 5.0f;

            SDisruptAOEVisual_Timer = 1000 + floor(dist / 30 * 1000.0f);
        }else m_uiStaticDisruptTimer -= uiDiff;

        if (SDisruptAOEVisual_Timer < uiDiff) 
        {
            Unit* SDVictim = Unit::GetUnit((*m_creature), TargetGUID);
            if(SDVictim && SDVictim->isAlive())
                SDVictim->CastSpell(SDVictim, SPELL_STATIC_VISUAL, true);
            
            SDisruptAOEVisual_Timer = 99999;
            TargetGUID = 0;
        }else SDisruptAOEVisual_Timer -= uiDiff;

        if (m_uiGustOfWindTimer < uiDiff)
        {
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 1);
            if (!target)
                target = m_creature->getVictim();

            DoCast(target, SPELL_GUST_OF_WIND);
            m_uiGustOfWindTimer = (20+rand()%10)*1000; //20 to 30 seconds(bosskillers)
        }else m_uiGustOfWindTimer -= uiDiff;

        if (m_uiCallLightTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_CALL_LIGHTNING);
            m_uiCallLightTimer = urand(15000, 25000);
        }else m_uiCallLightTimer -= uiDiff;

        if (!isRaining && m_uiStormTimer < urand(8000,13000)) // Guen: niezupelnie rozmumiem o co tu chodzi ale zostawiam tak jak jest xD
        {
            SetWeather(WEATHER_STATE_HEAVY_RAIN, 0.9999f);
            isRaining = true;
        }

        if (m_uiStormTimer < uiDiff)
        {
            Unit* target = SelectRandomPlayer(50);
            if(!target) 
                target = m_creature->getVictim();
            
            float x, y, z;
            target->GetPosition(x, y, z);
            Creature *Cloud = m_creature->SummonCreature(MOB_TEMP_TRIGGER, x, y, m_creature->GetPositionZ() + 10, 0, TEMPSUMMON_TIMED_DESPAWN, 15000);
            
            if(Cloud)
            {
                DoScriptText(EMOTE_STORM, m_creature);
                CloudGUID = Cloud->GetGUID();
                Cloud->SetMonsterMoveFlags(MONSTER_MOVE_LEVITATING);
                Cloud->StopMoving();
                Cloud->SetFloatValue(OBJECT_FIELD_SCALE_X, 3.0f);
                Cloud->setFaction(35);
                Cloud->SetMaxHealth(9999999);
                Cloud->SetHealth(9999999);
                Cloud->CastSpell(Cloud, 45213, true); // cloud visual
                m_creature->StopMoving();
                Cloud->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->CastSpell(Cloud, 43501, false); // siphon soul
            }

            Unit *Cyclone = m_creature->SummonCreature(MOB_TEMP_TRIGGER, x, y, z, 0, TEMPSUMMON_TIMED_DESPAWN, 15000);
            if(Cyclone)
            {
                Cyclone->CastSpell(Cyclone, 25160, true); // wind visual
                CycloneGUID = Cyclone->GetGUID();
            }
            
            m_uiStormTimer = 60000; //60 seconds(bosskillers)
            StormCount = 1;
            StormSequenceTimer = 0;
        }else m_uiStormTimer -= uiDiff;

        if (SummonEagles_Timer < uiDiff) 
        {
            DoScriptText(SAY_SUMMON, m_creature);

            float x, y, z;
            m_creature->GetPosition(x, y, z);
            for (uint8 i = 0; i < 3 + rand()%1; i++) 
        {
                if(Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                {
                    x = target->GetPositionX() + 10 + rand()%20;
                    y = target->GetPositionY() + 10 + rand()%20;
                    z = target->GetPositionZ() + 20 + rand()%5 + 10;
                    if(z > 95) z = 95 - rand()%5;
                }

                Creature *pCreature = m_creature->SummonCreature(MOB_SOARING_EAGLE, x, y, z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                if (pCreature)
        {
                    pCreature->AddThreat(m_creature->getVictim(), 1.0f);
                    pCreature->AI()->AttackStart(m_creature->getVictim());
                }
            }
            SummonEagles_Timer = 999999;
        }else SummonEagles_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_akilzon(Creature* pCreature)
{
    return new boss_akilzonAI(pCreature);
}

enum
{
    SPELL_EAGLE_SWOOP       = 44732,
    POINT_ID_RANDOM         = 1,
    TIMER_RETURN            = 750
};

struct MANGOS_DLL_DECL mob_soaring_eagleAI : public ScriptedAI
{
    mob_soaring_eagleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiEagleSwoopTimer;
    uint32 m_uiReturnTimer;
    bool m_bCanMoveToRandom;
    bool m_bCanCast;

    void Reset()
    {
        m_uiEagleSwoopTimer = urand(2000, 6000);
        m_uiReturnTimer = TIMER_RETURN;
        m_bCanMoveToRandom = false;
        m_bCanCast = true;
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
        }
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if (uiType != POINT_MOTION_TYPE)
            return;

        m_bCanCast = true;
    }

    void DoMoveToRandom()
    {
        if (!m_pInstance)
            return;

        if (Creature* pAzkil = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_AKILZON)))
        {
            float fX, fY, fZ;
            pAzkil->GetRandomPoint(pAzkil->GetPositionX(), pAzkil->GetPositionY(), pAzkil->GetPositionZ()+15.0f, 30.0f, fX, fY, fZ);

            m_creature->GetMotionMaster()->MovePoint(POINT_ID_RANDOM, fX, fY, fZ);

            m_bCanMoveToRandom = false;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bCanMoveToRandom)
        {
            if (m_uiReturnTimer < uiDiff)
            {
                DoMoveToRandom();
                m_uiReturnTimer = TIMER_RETURN;
            }else m_uiReturnTimer -= uiDiff;
        }

        if (!m_bCanCast)
            return;

        if (m_uiEagleSwoopTimer < uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
            {
                DoCast(pTarget,SPELL_EAGLE_SWOOP);

                //below here is just a hack and must be removed
                //use for research about SPELL_EFFECT_149, not implemented in time of writing script.
                //cast -> move to target -> return to random point at same Z as original location?
                //is spellInfo->Speed releted to TIMER_RETURN -value?
                float fX, fY, fZ;
                pTarget->GetContactPoint(m_creature, fX, fY, fZ);
                m_creature->SendMonsterMove(fX, fY, fZ, 0, MONSTER_MOVE_WALK, TIMER_RETURN);
                m_creature->GetMap()->CreatureRelocation(m_creature, fX, fY, fZ, m_creature->GetAngle(pTarget));
            }

            m_uiEagleSwoopTimer = urand(4000, 6000);
            m_bCanMoveToRandom = true;
            m_bCanCast = false;
        }else m_uiEagleSwoopTimer -= uiDiff;
    }
};

//Soaring Eagle
CreatureAI* GetAI_mob_soaring_eagle(Creature* pCreature)
{
    return new mob_soaring_eagleAI(pCreature);
}

void AddSC_boss_akilzon()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_akilzon";
    newscript->GetAI = &GetAI_boss_akilzon;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_soaring_eagle";
    newscript->GetAI = &GetAI_mob_soaring_eagle;
    newscript->RegisterSelf();
}
