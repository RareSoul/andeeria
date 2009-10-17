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
SDName: Boss_Zuljin
SD%Complete: 90
SDComment:
SDCategory: Zul'Aman
EndScriptData */

#include "precompiled.h"
#include "zulaman.h"
#include "Spell.h"

enum
{
    SAY_INTRO                       = -1568056,
    SAY_AGGRO                       = -1568057,
    SAY_BEAR_TRANSFORM              = -1568058,
    SAY_EAGLE_TRANSFORM             = -1568058,
    SAY_LYNX_TRANSFORM              = -1568060,
    SAY_DRAGONHAWK_TRANSFORM        = -1568061,
    SAY_FIRE_BREATH                 = -1568062,
    SAY_BERSERK                     = -1568053,
    SAY_KILL1                       = -1568064,
    SAY_KILL2                       = -1568065,
    SAY_DEATH                       = -1568066,


    // Troll Form
    SPELL_WHIRLWIND                 = 17207,
    SPELL_GRIEVOUS_THROW            = 43093,                //removes debuff after full healed

    // Bear Form
    SPELL_CREEPING_PARALYSIS        = 42398,                //should cast on the whole raid
    SPELL_OVERPOWER                 = 43456,                //use after melee attack dodged

    // Eagle Form
    SPELL_ENERGY_STORM              = 43983,                //enemy area aura, trigger 42577
    SPELL_ZAP_INFORM                = 42577,
    SPELL_ZAP_DAMAGE                = 43137,                //1250 damage
    SPELL_SUMMON_CYCLONE            = 43112,                //summon four feather vortex
    CREATURE_FEATHER_VORTEX         = 24136,
    SPELL_CYCLONE_VISUAL            = 43119,                //trigger 43147 visual
    SPELL_CYCLONE_PASSIVE           = 43120,                //trigger 43121 (4y aoe) every second

    // Lynx Form
    SPELL_CLAW_RAGE_HASTE           = 42583,
    SPELL_CLAW_RAGE_TRIGGER         = 43149,
    SPELL_CLAW_RAGE_DAMAGE          = 43150,
    SPELL_LYNX_RUSH_HASTE           = 43152,
    SPELL_LYNX_RUSH_DAMAGE          = 43153,

    // Dragonhawk Form
    SPELL_FLAME_WHIRL               = 43213,                //trigger two spells
    SPELL_FLAME_BREATH              = 43215,
    SPELL_SUMMON_PILLAR             = 43216,                //summon 24187
    CREATURE_COLUMN_OF_FIRE         = 24187,
    SPELL_PILLAR_TRIGGER            = 43218,                //trigger 43217

    // Cosmetic
    SPELL_SPIRIT_AURA               = 42466,
    SPELL_SIPHON_SOUL               = 43501,

    // Transforms
    SPELL_SHAPE_OF_THE_BEAR         = 42594,
    SPELL_SHAPE_OF_THE_EAGLE        = 42606,
    SPELL_SHAPE_OF_THE_LYNX         = 42607,
    SPELL_SHAPE_OF_THE_DRAGONHAWK   = 42608,

    SPELL_BERSERK                   = 45078,

    WEAPON_ID                       = 33975,

    PHASE_BEAR                      = 0,
    PHASE_EAGLE                     = 1,
    PHASE_LYNX                      = 2,
    PHASE_DRAGONHAWK                = 3,
    PHASE_TROLL                     = 4,
};


//coords for going for changing form
const float CENTER_X = 120.148811;
const float CENTER_Y = 703.713684;
const float CENTER_Z = 45.111477;
/* ############################
   ############################ */

struct TransformStruct
{
    uint32 sound;
    uint64 text;
    uint32 spell, unaura;
};

static TransformStruct Transform[] =
{
    {SAY_BEAR_TRANSFORM, SPELL_SHAPE_OF_THE_BEAR, SPELL_WHIRLWIND},
    {SAY_EAGLE_TRANSFORM, SPELL_SHAPE_OF_THE_EAGLE, SPELL_SHAPE_OF_THE_BEAR},
    {SAY_LYNX_TRANSFORM, SPELL_SHAPE_OF_THE_LYNX, SPELL_SHAPE_OF_THE_EAGLE},
    {SAY_DRAGONHAWK_TRANSFORM, SPELL_SHAPE_OF_THE_DRAGONHAWK, SPELL_SHAPE_OF_THE_LYNX}
};
/* ############################
   ############################ */
struct MANGOS_DLL_DECL boss_zuljinAI : public ScriptedAI
{
    boss_zuljinAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());

		SpellEntry *TempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_CLAW_RAGE_DAMAGE);
        if(TempSpell)
        {
            if(TempSpell->DmgClass != SPELL_DAMAGE_CLASS_MELEE)
                TempSpell->DmgClass = SPELL_DAMAGE_CLASS_MELEE;
            if(TempSpell->EffectApplyAuraName[2] != SPELL_AURA_MOD_STUN)
                TempSpell->EffectApplyAuraName[2] = SPELL_AURA_MOD_STUN;
        }

        Reset();
    }

    ScriptedInstance* pInstance;


	uint64 SpiritGUID[4];
    uint64 ClawTargetGUID;
    uint64 TankGUID;

    uint32 Phase;
    uint32 health_20;

	bool isIntro;
	bool InCombat;

    uint32 Intro_Timer;
    uint32 Berserk_Timer;

    uint32 Whirlwind_Timer;
    uint32 Grievous_Throw_Timer;

    uint32 Creeping_Paralysis_Timer;
    uint32 Overpower_Timer;

    uint32 Claw_Rage_Timer;
    uint32 Lynx_Rush_Timer;
    uint32 Claw_Counter;
    uint32 Claw_Loop_Timer;

    uint32 Flame_Whirl_Timer;
    uint32 Flame_Breath_Timer;
    uint32 Pillar_Of_Fire_Timer;


    void Reset()
    {
		if (pInstance)
            pInstance->SetData(TYPE_ZULJIN, NOT_STARTED);

		Phase = 0;

        health_20 = m_creature->GetMaxHealth()*0.2;
		
		isIntro	= false;
		InCombat = false;

        Intro_Timer = 32000;
        Berserk_Timer = 600000;

        Whirlwind_Timer = 7000;
        Grievous_Throw_Timer = 8000;

        Creeping_Paralysis_Timer = 7000;
        Overpower_Timer = 0;

        Claw_Rage_Timer = 5000;
        Lynx_Rush_Timer = 25000;
        Claw_Loop_Timer = 0;
        Claw_Counter = 0;

        Flame_Whirl_Timer = 5000;
        Flame_Breath_Timer = 6000;
        Pillar_Of_Fire_Timer = 7000;

        ClawTargetGUID = 0;
        TankGUID = 0;        

        DespawnSummons(CREATURE_FEATHER_VORTEX);
        DespawnSummons(CREATURE_COLUMN_OF_FIRE);

        m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 47174);
        m_creature->SetByteValue(UNIT_FIELD_BYTES_2, 0, SHEATH_STATE_MELEE);
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void Aggro(Unit* pWho)
    {
        if(pInstance)
            pInstance->SetData(TYPE_ZULJIN, IN_PROGRESS);

		if(!isIntro && Intro_Timer != 0)
		{
				DoStopAttack();
				isIntro = true;
        DoScriptText(SAY_AGGRO, m_creature);
		}else{
				m_creature->SetInCombatWithZone();
    }
	}

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_KILL1 : SAY_KILL2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

		DespawnSummons(CREATURE_COLUMN_OF_FIRE);
				
		if (!pInstance)
            return;

        pInstance->SetData(TYPE_ZULJIN, DONE);
    }


	void AttackStart(Unit* who)
    {
        if (!who)
            return;
		
        if (m_creature->Attack(who, true))
        {
            m_creature->AddThreat(who, 0.0f);
            m_creature->SetInCombatWith(who);
            who->SetInCombatWith(m_creature);

            if (!InCombat)
            {
                InCombat = true;
                Aggro(who);
            }

            if(Phase == 2)
                DoStartNoMovement(who);
			else if(isIntro && Intro_Timer !=0)
				DoStartNoMovement(who);
            else
                DoStartMovement(who);
        }
		
    }

	void DoMeleeAttackIfReady()
    {
        if( !m_creature->IsNonMeleeSpellCasted(false))
        {
            if(m_creature->isAttackReady() && m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
            {
                if(Phase == 1 && !Overpower_Timer)
                {
                    uint32 health = m_creature->getVictim()->GetHealth();
                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    if(m_creature->getVictim() && health == m_creature->getVictim()->GetHealth())
                    {
                        m_creature->CastSpell(m_creature->getVictim(), SPELL_OVERPOWER, false);
                        Overpower_Timer = 5000;
                    }
                }else m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();
            }
        }
    }

    void SpawnAdds()
    {
       Creature *pCreature = NULL;
        for(uint8 i = 0; i < 4; i++)
        {
            if(pCreature)
            {
                pCreature->CastSpell(pCreature, SPELL_SPIRIT_AURA, true);
                pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                SpiritGUID[i] = pCreature->GetGUID();
            }
        }
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

    void EnterPhase(uint32 NextPhase)
   {
        switch(NextPhase)
        {
        case 0:
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            m_creature->Relocate(CENTER_X, CENTER_Y, CENTER_Z,0);
			m_creature->SendMonsterMove(CENTER_X, CENTER_Y, CENTER_Z,0,MONSTER_MOVE_WALK,0,0);
            DoResetThreat();
            m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 0);
            m_creature->RemoveAurasDueToSpell(Transform[Phase].unaura);
            DoCast(m_creature, Transform[Phase].spell);
            DoScriptText(Transform[Phase].text, m_creature);
        
            if(NextPhase == 2)
            {
                m_creature->GetMotionMaster()->Clear();
                m_creature->CastSpell(m_creature, SPELL_ENERGY_STORM, true); // enemy aura
                for(uint8 i = 0; i < 4; i++)
                {
                    Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                    Creature* Vortex = DoSpawnCreature(CREATURE_FEATHER_VORTEX, 0, 0, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                    if(Vortex && target)
                    {
                        Vortex->CastSpell(Vortex, SPELL_CYCLONE_PASSIVE, true);
                        Vortex->CastSpell(Vortex, SPELL_CYCLONE_VISUAL, true);
                        Vortex->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        Vortex->SetSpeed(MOVE_RUN, 1.0f);
                        Vortex->AI()->AttackStart(target);
                        Vortex->SetInCombatWithZone();
                    }
                }
            }
            else
                m_creature->AI()->AttackStart(m_creature->getVictim());
            if(NextPhase == 3)
            {
                m_creature->RemoveAurasDueToSpell(SPELL_ENERGY_STORM);
                DespawnSummons(CREATURE_FEATHER_VORTEX);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            }
            break;
        default:
            break;
        }
        Phase = NextPhase;
    }

    void UpdateAI(const uint32 diff)
    {
		if(isIntro==true)
		{
			if(Intro_Timer < diff)
			{
				m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);     
                DoScriptText(SAY_AGGRO, m_creature);
                AttackStart(m_creature->getVictim());
                DoStartMovement(m_creature->getVictim());
                Intro_Timer = 0;
				isIntro = false;
				EnterPhase(0);
			}else
				Intro_Timer -= diff;
		}

		if(Intro_Timer==0 && !isIntro)
		{
		
	    if(!TankGUID)
        {
            if(!m_creature->SelectHostileTarget() && !m_creature->getVictim())
            return;

            if(m_creature->GetHealth() < health_20 * (4 - Phase))
                EnterPhase(Phase + 1);
        }

        if(Berserk_Timer < diff)
        {
            m_creature->CastSpell(m_creature, SPELL_BERSERK, true);
            DoScriptText(SAY_BERSERK, m_creature);
            Berserk_Timer = 60000;
        }else Berserk_Timer -= diff;

        switch (Phase)
        {
        case 0:
            if(Intro_Timer)
            {
                if(Intro_Timer <= diff)
                {
                    DoScriptText(SAY_AGGRO, m_creature);
                    Intro_Timer = 0;
                }else Intro_Timer -= diff;
            }

            if(Whirlwind_Timer < diff)
            {
                DoCast(m_creature, SPELL_WHIRLWIND);
                Whirlwind_Timer = 15000 + rand()%5000;
            }else Whirlwind_Timer -= diff;

            if(Grievous_Throw_Timer < diff)
            {
                if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    m_creature->CastSpell(target, SPELL_GRIEVOUS_THROW, false);
                Grievous_Throw_Timer = 10000;
            }else Grievous_Throw_Timer -= diff;
            break;

        case 1:
            if(Creeping_Paralysis_Timer < diff)
            {
                DoCast(m_creature, SPELL_CREEPING_PARALYSIS);
                Creeping_Paralysis_Timer = 20000;
            }else Creeping_Paralysis_Timer -= diff;

            if(Overpower_Timer < diff)
            {
                // implemented in DoMeleeAttackIfReady()
                Overpower_Timer = 0;
            }else Overpower_Timer -= diff;
            break;

        case 2:
            return;

        case 3:
            if(Claw_Rage_Timer <= diff)
            {
                if(!TankGUID)
                {
                    if(Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    {
                        TankGUID = m_creature->getVictim()->GetGUID();
                        m_creature->SetSpeed(MOVE_RUN, 5.0f);
                        AttackStart(target); // change victim
                        Claw_Rage_Timer = 0;
                        Claw_Loop_Timer = 500;
                        Claw_Counter = 0;
                    }
                }
                else if(!Claw_Rage_Timer) // do not do this when Lynx_Rush
                {
                    if(Claw_Loop_Timer < diff)
                    {
                        Unit* target = m_creature->getVictim();
                        if(!target || !target->isTargetableForAttack()) target = Unit::GetUnit(*m_creature, TankGUID);
                        if(!target || !target->isTargetableForAttack()) target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                        if(target)
                        {
                            AttackStart(target);
                            if(m_creature->IsWithinDistInMap(target, ATTACK_DISTANCE))
                            {
                                m_creature->CastSpell(target, SPELL_CLAW_RAGE_DAMAGE, true);
                                Claw_Counter++;
                                if(Claw_Counter == 12)
                                {
                                    Claw_Rage_Timer = 15000 + rand()%5000;
                                    m_creature->SetSpeed(MOVE_RUN, 1.2f);
                                    AttackStart(Unit::GetUnit(*m_creature, TankGUID));
                                    TankGUID = 0;
                                    return;
                                }
                               else
                                    Claw_Loop_Timer = 500;
                            }
                        }else EnterEvadeMode(); // if(target)
                    }else Claw_Loop_Timer -= diff;
                } //if(TankGUID)
            }else Claw_Rage_Timer -= diff;

            if(Lynx_Rush_Timer <= diff)
            {
                if(!TankGUID)
                {
                    if(Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    {
                        TankGUID = m_creature->getVictim()->GetGUID();
                        m_creature->SetSpeed(MOVE_RUN, 5.0f);
                        AttackStart(target); // change victim
                        Lynx_Rush_Timer = 0;
                        Claw_Counter = 0;
                    }
                }
                else if(!Lynx_Rush_Timer)
                {
                    Unit* target = m_creature->getVictim();
                    if(!target || !target->isTargetableForAttack())
                    {
                        target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                        if (target) AttackStart(target);
                    }
                    if(target)
                    {
                        if(m_creature->IsWithinDistInMap(target, ATTACK_DISTANCE))
                        {
                            m_creature->CastSpell(target, SPELL_LYNX_RUSH_DAMAGE, true);
                           Claw_Counter++;
                            if(Claw_Counter == 4)
                            {
                                Lynx_Rush_Timer = 35000 + rand()%5000;
                                m_creature->SetSpeed(MOVE_RUN, 1.2f);
                                AttackStart(Unit::GetUnit(*m_creature, TankGUID));
                                TankGUID = 0;
                            }
                            else
                            {
                                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                                    AttackStart(target);
                            }
                        }
                    }else EnterEvadeMode(); // if(target)
                } //if(TankGUID)
            }else Lynx_Rush_Timer -= diff;

            break;
        case 4:
           if(Flame_Whirl_Timer < diff)
            {
               DoCast(m_creature, SPELL_FLAME_WHIRL);
                Flame_Whirl_Timer = 12000;
            }Flame_Whirl_Timer -= diff;

            if(Pillar_Of_Fire_Timer < diff)
           {
                if(Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                {
                    float x, y, z;
                    target->GetPosition(x, y, z);
                    Creature* Pillar = m_creature->SummonCreature(CREATURE_COLUMN_OF_FIRE, x, y, z, 0, TEMPSUMMON_TIMED_DESPAWN, 30000);
                }
                Pillar_Of_Fire_Timer = 10000;
            }else Pillar_Of_Fire_Timer -= diff;

            if(Flame_Breath_Timer < diff)
            {
                if(Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    m_creature->CastSpell(m_creature, SPELL_FLAME_BREATH, false);
                Flame_Breath_Timer = 10000;
            }else Flame_Breath_Timer -= diff;
            break;

        default:
            break;
        }

        if(!TankGUID)
        DoMeleeAttackIfReady();
		
    }
		}
    
};

CreatureAI* GetAI_boss_zuljin(Creature* pCreature)
{
    return new boss_zuljinAI(pCreature);
}

struct MANGOS_DLL_DECL do_nothingAI : public ScriptedAI
{
    do_nothingAI(Creature *c) : ScriptedAI(c) {}
	void Reset() {
		m_creature->CastSpell(m_creature, SPELL_PILLAR_TRIGGER, true);
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		m_creature->StopMoving();
		m_creature->AttackStop();}
    void Aggro(Unit* who) {}
    void AttackStart(Unit* who) {}
    void MoveInLineOfSight(Unit* who) {}
    void UpdateAI(const uint32 diff) {}
};

CreatureAI* GetAI_do_nothing(Creature *_Creature)
{
    return new do_nothingAI (_Creature);
}

struct MANGOS_DLL_DECL feather_vortexAI : public ScriptedAI
{
    feather_vortexAI(Creature *c) : ScriptedAI(c) {}

    void Reset() {}

    void Aggro(Unit* target) {}

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if(spell->Id == SPELL_ZAP_INFORM)
            m_creature->CastSpell(caster, SPELL_ZAP_DAMAGE, true);           
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->getVictim())
            return;

        //if the vortex reach the target, it change his target to another player
        if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
            if (Unit* nextVictim = SelectUnit(SELECT_TARGET_RANDOM, 0))
                AttackStart(nextVictim);
    }
};

CreatureAI* GetAI_feather_vortexAI(Creature *_Creature)
{
    return new feather_vortexAI (_Creature);
}

void AddSC_boss_zuljin()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_zuljin";
    newscript->GetAI = &GetAI_boss_zuljin;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "do_nothing";
    newscript->GetAI = &GetAI_do_nothing;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_zuljin_vortex";
    newscript->GetAI = &GetAI_feather_vortexAI;
    newscript->RegisterSelf();
}
