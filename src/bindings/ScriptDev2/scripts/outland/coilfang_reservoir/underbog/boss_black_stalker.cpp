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
SDName: Black_Stalker
SD%Complete: 100
SDComment:
SDCategory: Underbog
EndScriptData */

#include "precompiled.h"
#include "def_underbog.h"

enum
{
    // Black Stalker Spells
    SPELL_STATIC_CHARGE		    = 31715,
    SPELL_CHAIN_LIGHTNING       = 31717,
    SPELL_LEVITATE				= 31704,

	// Spore Strider (add) spell
	SPELL_LIGHTNING_BOLT		= 20824,

	MOB_SPORE_STRIDER			= 22299
};

float SporeStriderLoc[6][4]=
{
    {119.0, 33.33, 25.66, 6.01},
    {124.78, 48.4, 25.66, 5.48},
    {149.26, 58.35, 25.66, 4.79},
	{137.25, -10.95, 25.66, 1.86},
	{160.77, -15.36, 25.66, 1.96},
	{175.66, -11, 25.66, 2.29}
};

struct MANGOS_DLL_DECL boss_black_stalkerAI : public ScriptedAI
{
    boss_black_stalkerAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		m_bIsHeroicMode = pCreature->GetMap()->IsHeroic();
		Reset();
	}

	ScriptedInstance* m_pInstance;

    uint32 m_uiChainLightning_Timer;
    uint32 m_uiLevitate_Timer;

    uint32 m_uiSummonSporeStrider_Timer;

    uint32 m_uiLightninBolt_Timer;

	bool m_bIsHeroicMode;

    void Reset()
    {
		uint32 m_uiChainLightning_Timer = 5000;
		uint32 m_uiLevitate_Timer = 10000;

		uint32 m_uiSummonSporeStrider_Timer = 10000;

		uint32 m_uiLightninBolt_Timer = 1000;
    }


    void Aggro(Unit* pWho)
	{
		if (m_pInstance && pWho)
			m_pInstance->SetData(TYPE_STALKER, IN_PROGRESS);
	}

	void JustSummoned(Creature *summoned)
    {
        if (summoned->GetEntry() == MOB_SPORE_STRIDER)
        {
            summoned->setFaction(m_creature->getFaction());
			
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0);
			if (!target)
				target = m_creature->getVictim();
         
			if (target)
			{
                summoned->AI()->AttackStart(target);
				summoned->GetMotionMaster()->MoveChase(target,20.0f);
			}
        }
    }

	void JustDied(Unit* pVictim)
	{
		if (m_pInstance)
			m_pInstance->SetData(TYPE_STALKER, DONE);
	}

	void JustReachedHome()
    {
		if (m_pInstance)
			m_pInstance->SetData(TYPE_STALKER, NOT_STARTED);
	}

    void UpdateAI(const uint32 uiDiff)
    {
		if (!m_creature->getVictim() && !m_creature->SelectHostileTarget())
            return;
		
		if (m_uiLevitate_Timer < uiDiff)
		{
			Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 1);

			if (target)
				m_creature->CastSpell(target, SPELL_LEVITATE,false);								

			m_uiLevitate_Timer = 30000;
		}else m_uiLevitate_Timer -= uiDiff;

		if (m_uiChainLightning_Timer < uiDiff)
		{
			m_creature->CastSpell(m_creature->getVictim(), SPELL_CHAIN_LIGHTNING,false);
			m_uiChainLightning_Timer = 5000 + rand()%10000;
		}else m_uiChainLightning_Timer -= uiDiff;

		if (m_uiSummonSporeStrider_Timer < uiDiff)
		{
			if (m_bIsHeroicMode)
				switch(rand()%2)
				{
				case 0:
					m_creature->SummonCreature(MOB_SPORE_STRIDER, SporeStriderLoc[0][0], SporeStriderLoc[0][1], SporeStriderLoc[0][2],SporeStriderLoc[0][3], TEMPSUMMON_TIMED_DESPAWN, 30000);
					m_creature->SummonCreature(MOB_SPORE_STRIDER, SporeStriderLoc[1][0], SporeStriderLoc[1][1], SporeStriderLoc[1][2],SporeStriderLoc[1][3], TEMPSUMMON_TIMED_DESPAWN, 30000);
					m_creature->SummonCreature(MOB_SPORE_STRIDER, SporeStriderLoc[2][0], SporeStriderLoc[2][1], SporeStriderLoc[2][2],SporeStriderLoc[2][3], TEMPSUMMON_TIMED_DESPAWN, 30000);
					break;
				case 1:	
					m_creature->SummonCreature(MOB_SPORE_STRIDER, SporeStriderLoc[3][0], SporeStriderLoc[3][1], SporeStriderLoc[3][2],SporeStriderLoc[3][3], TEMPSUMMON_TIMED_DESPAWN, 30000);
					m_creature->SummonCreature(MOB_SPORE_STRIDER, SporeStriderLoc[4][0], SporeStriderLoc[4][1], SporeStriderLoc[4][2],SporeStriderLoc[4][3], TEMPSUMMON_TIMED_DESPAWN, 30000);
					m_creature->SummonCreature(MOB_SPORE_STRIDER, SporeStriderLoc[5][0], SporeStriderLoc[5][1], SporeStriderLoc[5][2],SporeStriderLoc[5][3], TEMPSUMMON_TIMED_DESPAWN, 30000);
					break;
				}
	
			m_uiSummonSporeStrider_Timer = 20000;
		}else m_uiSummonSporeStrider_Timer -= uiDiff;


        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_spore_striderAI : public ScriptedAI
{
    mob_spore_striderAI(Creature* pCreature) : ScriptedAI(pCreature) 
	
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Reset();
	}

	ScriptedInstance* m_pInstance;

    uint32 m_uiLightningBolt_Timer;

    void Reset()
    {
        m_uiLightningBolt_Timer = 1000;
    }

	void JustDied(Unit* pVictim)
	{
		if (m_pInstance && m_pInstance->GetData(TYPE_STALKER) == IN_PROGRESS)
		{
			m_creature->SummonCreature(MOB_SPORE_STRIDER,(m_creature->GetPositionX()+2),(m_creature->GetPositionY()-2),m_creature->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,30000);
			m_creature->SummonCreature(MOB_SPORE_STRIDER,(m_creature->GetPositionX()-2),(m_creature->GetPositionY()+2),m_creature->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,30000);
			m_creature->SummonCreature(MOB_SPORE_STRIDER,(m_creature->GetPositionX()+2),m_creature->GetPositionY(),m_creature->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,30000);
		}
	}

    void UpdateAI(const uint32 uiDiff)
    {
		if (m_pInstance && m_pInstance->GetData(TYPE_STALKER) != IN_PROGRESS)
			m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiLightningBolt_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_LIGHTNING_BOLT);
            m_uiLightningBolt_Timer = 4000;
        } else m_uiLightningBolt_Timer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_spore_strider(Creature* pCreature)
{
    return new mob_spore_striderAI(pCreature);
}

CreatureAI* GetAI_boss_black_stalker(Creature* pCreature)
{
    return new boss_black_stalkerAI(pCreature);
}

void AddSC_boss_black_stalker()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_black_stalker";
    newscript->GetAI = &GetAI_boss_black_stalker;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_spore_strider";
    newscript->GetAI = &GetAI_mob_spore_strider;
    newscript->RegisterSelf();
}
