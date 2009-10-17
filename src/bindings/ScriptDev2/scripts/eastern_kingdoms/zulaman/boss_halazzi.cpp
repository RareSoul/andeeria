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
SDName: Boss_Halazzi
SD%Complete: 90
SDComment: Place Holder
SDCategory: Zul'Aman
EndScriptData */

#include "precompiled.h"
#include "zulaman.h"

enum
{
    SAY_AGGRO                       = -1568034,
    SAY_SPLIT                       = -1568035,
    SAY_MERGE                       = -1568036,
    SAY_SABERLASH1                  = -1568037,
    SAY_SABERLASH2                  = -1568038,
    SAY_BERSERK                     = -1568039,
    SAY_KILL1                       = -1568040,
    SAY_KILL2                       = -1568041,
    SAY_DEATH                       = -1568042,
    SAY_EVENT1                      = -1568043,
    SAY_EVENT2                      = -1568044,

    SPELL_DUAL_WIELD        = 29651,
    SPELL_SABER_LASH                = 43267,
    SPELL_FRENZY                    = 43139,
    SPELL_FLAMESHOCK                = 43303,
    SPELL_EARTHSHOCK                = 43305,
    SPELL_TRANSFORM_SPLIT   = 43142,
    SPELL_TRANSFORM_SPLIT2  = 43573,
    SPELL_TRANSFORM_MERGE   = 43271,
    SPELL_SUMMON_LYNX       = 43143,
    SPELL_SUMMON_TOTEM      = 43302,
    SPELL_BERSERK                   = 45078,

    MOB_SPIRIT_LYNX         = 24143,
    SPELL_LYNX_FRENZY       = 43290,
    SPELL_SHRED_ARMOR       = 43243,

    MOB_TOTEM               = 24224,
    SPELL_LIGHTNING         = 43301,
};

enum PhaseHalazzi
{
    PHASE_NONE = 0,
    PHASE_LYNX = 1,
    PHASE_SPLIT = 2,
    PHASE_HUMAN = 3,
    PHASE_MERGE = 4,
    PHASE_ENRAGE = 5
};

struct MANGOS_DLL_DECL boss_halazziAI : public ScriptedAI
{
    boss_halazziAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();

		SpellEntry *TempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_SUMMON_TOTEM);
		if(TempSpell && TempSpell->EffectImplicitTargetA[0] != 1)
		   TempSpell->EffectImplicitTargetA[0] = 1;

		TempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_LIGHTNING);
        if(TempSpell && TempSpell->CastingTimeIndex != 5)
           TempSpell->CastingTimeIndex = 5;
    }

    ScriptedInstance* pInstance;

    uint32 m_uiFrenzyTimer;
	uint32 SaberlashTimer;
    uint32 m_uiShockTimer;
	uint32 TotemTimer;
	uint32 CheckTimer;
	uint32 BerserkTimer;

	uint32 TransformCount;

	PhaseHalazzi Phase;
	uint64 LynxGUID;
	
    void Reset()
    {
		TransformCount = 0;
        BerserkTimer = 600000;
        CheckTimer = 1000;

        DoCast(m_creature, SPELL_DUAL_WIELD, true);

        Phase = PHASE_NONE;
        EnterPhase(PHASE_LYNX);

		if (!pInstance)
            return;

        pInstance->SetData(TYPE_HALAZZI, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        m_creature->SetInCombatWithZone();

		EnterPhase(PHASE_LYNX);

		if (pInstance)
            pInstance->SetData(TYPE_HALAZZI, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        DoScriptText(urand(0, 1) ? SAY_KILL1 : SAY_KILL2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
		if (!pInstance)
            return;
		
        DoScriptText(SAY_DEATH, m_creature);

		pInstance->SetData(TYPE_HALAZZI, DONE);
    }

	void JustSummoned(Creature* summon)
    {
        summon->AI()->AttackStart(m_creature->getVictim());
        if(summon->GetEntry() == MOB_SPIRIT_LYNX)
            LynxGUID = summon->GetGUID();
    }

	void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if(damage >= m_creature->GetHealth() && Phase != PHASE_ENRAGE)
            damage = 0;
    }

    void SpellHit(Unit*, const SpellEntry *spell)
        {
        if(spell->Id == SPELL_TRANSFORM_SPLIT2)
            EnterPhase(PHASE_HUMAN);
        }

    void AttackStart(Unit *who)
    {
        if(Phase != PHASE_MERGE) ScriptedAI::AttackStart(who);
    }

	void EnterPhase(PhaseHalazzi NextPhase)
    {
        switch(NextPhase)
        {
        case PHASE_LYNX:
        case PHASE_ENRAGE:
            if(Phase == PHASE_MERGE)
            {
                m_creature->CastSpell(m_creature, SPELL_TRANSFORM_MERGE, true);
                m_creature->Attack(m_creature->getVictim(), true);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                }
            if(Unit *Lynx = Unit::GetUnit(*m_creature, LynxGUID))
                {
                Lynx->SetVisibility(VISIBILITY_OFF);
                Lynx->setDeathState(JUST_DIED);
                }
            m_creature->SetMaxHealth(600000);
            m_creature->SetHealth(600000 - 150000 * TransformCount);
            m_uiFrenzyTimer = 16000;
            SaberlashTimer = 20000;
            m_uiShockTimer = 10000;
            TotemTimer = 12000;
            break;
        case PHASE_SPLIT:
                    DoScriptText(SAY_SPLIT, m_creature);
            m_creature->CastSpell(m_creature, SPELL_TRANSFORM_SPLIT, true);
            break;
        case PHASE_HUMAN:
            //DoCast(m_creature, SPELL_SUMMON_LYNX, true);
            DoSpawnCreature(MOB_SPIRIT_LYNX, 0,0,0,0, TEMPSUMMON_CORPSE_DESPAWN, 0);
            m_creature->SetMaxHealth(400000);
            m_creature->SetHealth(400000);
            m_uiShockTimer = 10000;
            TotemTimer = 12000;
            break;
        case PHASE_MERGE:
            if(Unit *Lynx = Unit::GetUnit(*m_creature, LynxGUID))
        {
                DoScriptText(SAY_MERGE, m_creature);
                Lynx->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                Lynx->GetMotionMaster()->Clear();
                Lynx->GetMotionMaster()->MoveFollow(m_creature, 0, 0);
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveFollow(Lynx, 0, 0);
                TransformCount++;
            }break;
        default:
            break;
                }
        Phase = NextPhase;
            }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
		/***********************************/
		if(BerserkTimer < diff)
        {
                DoScriptText(SAY_BERSERK, m_creature);
                DoCast(m_creature, SPELL_BERSERK,true);
            BerserkTimer = 60000;
        }else BerserkTimer -= diff;
		/***********************************/
		if(Phase == PHASE_LYNX || Phase == PHASE_ENRAGE)
        {
            if(SaberlashTimer < diff)
            {
                 m_creature->CastSpell(m_creature->getVictim(), SPELL_SABER_LASH, true);
                //m_creature->RemoveAurasDueToSpell(41296);
                SaberlashTimer = 30000;
                DoScriptText(urand(0, 1) ? SAY_SABERLASH1 : SAY_SABERLASH2, m_creature);
            }else SaberlashTimer -= diff;
			/***********************************/
			if(m_uiFrenzyTimer < diff)
        {
                DoCast(m_creature, SPELL_FRENZY);
                m_uiFrenzyTimer = (10+rand()%5)*1000;
           }else m_uiFrenzyTimer -= diff;
			/***********************************/
			if(Phase == PHASE_LYNX)
                if(CheckTimer < diff)
            {
                    if(m_creature->GetHealth() * 4 < m_creature->GetMaxHealth() * (3 - TransformCount))
                        EnterPhase(PHASE_SPLIT);
                    CheckTimer = 1000;
                }else CheckTimer -= diff;
            }
		/***********************************/
		if(Phase == PHASE_HUMAN || Phase == PHASE_ENRAGE)
        {
			/***********************************/
            if(TotemTimer < diff)
            {
                DoCast(m_creature, SPELL_SUMMON_TOTEM);
                TotemTimer = 20000;
            }else TotemTimer -= diff;
			/***********************************/
            if(m_uiShockTimer < diff)
            {
                if(Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                {
                    if(target->IsNonMeleeSpellCasted(false))
                        DoCast(target,SPELL_EARTHSHOCK);
                    else
                        DoCast(target, SPELL_FLAMESHOCK);

                    m_uiShockTimer = urand(10000, 14000);
                }
            }else m_uiShockTimer -= diff;
			/***********************************/
            if(Phase == PHASE_HUMAN)
                if(CheckTimer < diff)
                {
                    if(m_creature->GetHealth() * 10 < m_creature->GetMaxHealth())
                        EnterPhase(PHASE_MERGE);
                    else
                    {
                        Unit *Lynx = Unit::GetUnit(*m_creature, LynxGUID);
                        if(Lynx && Lynx->GetHealth() * 10 < Lynx->GetMaxHealth())
                            EnterPhase(PHASE_MERGE);
            }
                    CheckTimer = 1000;
                }else CheckTimer -= diff;
        }
		/***********************************/
		if(Phase == PHASE_MERGE)
        {
            if(CheckTimer < diff)
            {
                Unit *Lynx = Unit::GetUnit(*m_creature, LynxGUID);
                if(Lynx && m_creature->IsWithinDistInMap(Lynx, 6.0f))
                {
                    if(TransformCount < 3)
                        EnterPhase(PHASE_LYNX);
            else
                        EnterPhase(PHASE_ENRAGE);
        }
                CheckTimer = 1000;
            }else CheckTimer -= diff;
        }
		/***********************************/
        DoMeleeAttackIfReady();
    }
};

/******************************************************/
/******************************************************/
/******************************************************/

struct MANGOS_DLL_DECL boss_spiritlynxAI : public ScriptedAI
{
    boss_spiritlynxAI(Creature *c) : ScriptedAI(c) { Reset(); }

    uint32 m_uiFrenzyTimer;
    uint32 m_uiShredArmorTimer;

    void Reset()
    {
        m_uiFrenzyTimer = urand(10000, 20000);              //first frenzy after 10-20 seconds
        m_uiShredArmorTimer = 4000;
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if(damage >= m_creature->GetHealth())
            damage = 0;
    }

    void AttackStart(Unit *who)
    {
        if(!m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            ScriptedAI::AttackStart(who);
    }

    void Aggro(Unit *who) {
		m_creature->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFrenzyTimer < diff)
        {
            DoCast(m_creature, SPELL_LYNX_FRENZY);
            m_uiFrenzyTimer = urand(20000, 30000);          //subsequent frenzys casted every 20-30 seconds
        }
        else
            m_uiFrenzyTimer -= diff;

        if (m_uiShredArmorTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_SHRED_ARMOR);
            m_uiShredArmorTimer = 4000;
        }else  m_uiShredArmorTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

/******************************************************/
/******************************************************/
/******************************************************/

CreatureAI* GetAI_boss_halazzi(Creature* pCreature)
{
    return new boss_halazziAI(pCreature);
}

CreatureAI* GetAI_boss_spiritlynxAI(Creature *_Creature)
{
    return new boss_spiritlynxAI (_Creature);
}

void AddSC_boss_halazzi()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "boss_halazzi";
    newscript->GetAI = &GetAI_boss_halazzi;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_halazzi_lynx";
    newscript->GetAI = &GetAI_boss_spiritlynxAI;
    newscript->RegisterSelf();
}
