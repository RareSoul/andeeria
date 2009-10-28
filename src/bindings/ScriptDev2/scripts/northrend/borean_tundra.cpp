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
SDName: Borean_Tundra
SD%Complete: 100
SDComment: Quest support: 11708, 11692, 11961. Taxi vendors.
SDCategory: Borean Tundra
EndScriptData */

/* ContentData
mob_elder
npc_fizzcrank_fullthrottle
npc_iruk
npc_kara_thricestar
npc_surristrasz
npc_tiare
EndContentData */

#include "precompiled.h"
#include "ObjectMgr.h"

/*######
## mob_elder
######*/
enum
{
    SPELL_ANCESTOR_RITUAL               = 45536,
    SPELL_GREEN_BEAM                    = 39165,

    NPC_ELDER_KESUK                     = 25397,
    NPC_ELDER_SAGANI                    = 25398,
    NPC_ELDER_TAKRET                    = 25399,
    
    GO_ELDER_KESUK                      = 191088,
    GO_ELDER_SAGANI                     = 191089,
    GO_ELDER_TAKRET                     = 191090,

    POINT_ID                            = 1
};

struct MANGOS_DLL_DECL mob_elderAI : public ScriptedAI
{
    mob_elderAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    bool bEventInProgress;
    uint64 uiPlayerGUID;
    uint32 m_uiResetTimer;

    void Reset()
    {
        uiPlayerGUID = 0;
        m_uiResetTimer = 10000;

        m_creature->InterruptSpell(CURRENT_CHANNELED_SPELL,false);
        m_creature->RemoveAllAuras();
        m_creature->SetDisplayId(m_creature->GetNativeDisplayId());
        m_creature->GetMotionMaster()->MoveTargetedHome();

        bEventInProgress = false;
    }

    void SpellHit(Unit* pCaster, SpellEntry const* pSpell)
    {
        if (!pCaster || bEventInProgress)
            return;

        if (pCaster->GetTypeId() == TYPEID_PLAYER && pSpell->Id == SPELL_ANCESTOR_RITUAL)
        {
            bEventInProgress = true;
            uiPlayerGUID = pCaster->GetGUID();
            if (CreatureInfo const* pTemp = GetCreatureTemplateStore(m_creature->GetEntry()))
                m_creature->SetDisplayId(((Player*)pCaster)->GetTeam() == HORDE ? pTemp->DisplayID_H[0] : pTemp->DisplayID_A[0]);

            m_creature->GetMotionMaster()->MoveIdle();
            m_creature->StopMoving();

            uint32 go_entry;
            switch (m_creature->GetEntry())
            {
                case NPC_ELDER_SAGANI:  go_entry = GO_ELDER_SAGANI; break;
                case NPC_ELDER_KESUK:   go_entry = GO_ELDER_KESUK; break;
                case NPC_ELDER_TAKRET:  go_entry = GO_ELDER_TAKRET; break;
                default: break;
            }
            
            float x,y,z;
            if (GameObject* pGo = GetClosestGameObjectWithEntry(pCaster, go_entry, DEFAULT_VISIBILITY_DISTANCE))
                pGo->GetNearPoint(pGo,x,y,z,2.0f,2.0f,pGo->GetOrientation());
            else 
            {
                error_log("SD2: mob_elder (%s entry %u) couldn't find gameobject entry %u. Please check your database",m_creature->GetName(),m_creature->GetEntry(),go_entry);
                Reset();
            }

            m_creature->GetMotionMaster()->MovePoint(POINT_ID,x,y,z);
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE)
            return;

        Player* pPlayer = (Player*)Unit::GetUnit((*m_creature),uiPlayerGUID);

        if (!pPlayer)
            return;

        if (id == POINT_ID)
        {
            DoCast(m_creature,SPELL_GREEN_BEAM,false);
            // Because spell focus is m_creature not static gameobject or something we have to check if player actualy is near m_creature
            if (pPlayer->GetDistance(m_creature) < 5.0f)
                pPlayer->KilledMonsterCredit(m_creature->GetEntry(),m_creature->GetGUID()); 
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_creature->HasAura(SPELL_GREEN_BEAM))
        {
            if (m_uiResetTimer <= uiDiff)
                Reset();
            else m_uiResetTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_mob_elder(Creature* pCreature)
{
    return new mob_elderAI(pCreature);
}


/*######
## npc_fizzcrank_fullthrottle
######*/

#define GOSSIP_ITEM_GO_ON   "Go on."
#define GOSSIP_ITEM_TELL_ME "Tell me what's going on out here, Fizzcrank."

enum
{
    GOSSIP_TEXTID_FIZZCRANK1    = 12456,
    GOSSIP_TEXTID_FIZZCRANK2    = 12457,
    GOSSIP_TEXTID_FIZZCRANK3    = 12458,
    GOSSIP_TEXTID_FIZZCRANK4    = 12459,
    GOSSIP_TEXTID_FIZZCRANK5    = 12460,
    GOSSIP_TEXTID_FIZZCRANK6    = 12461,
    GOSSIP_TEXTID_FIZZCRANK7    = 12462,
    GOSSIP_TEXTID_FIZZCRANK8    = 12463,
    GOSSIP_TEXTID_FIZZCRANK9    = 12464,

    QUEST_THE_MECHAGNOMES       = 11708
};

bool GossipHello_npc_fizzcrank_fullthrottle(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(QUEST_THE_MECHAGNOMES) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TELL_ME, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_fizzcrank_fullthrottle(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_GO_ON, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_FIZZCRANK1, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_GO_ON, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_FIZZCRANK2, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_GO_ON, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_FIZZCRANK3, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_GO_ON, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_FIZZCRANK4, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_GO_ON, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_FIZZCRANK5, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+6:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_GO_ON, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_FIZZCRANK6, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+7:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_GO_ON, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_FIZZCRANK7, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+8:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_GO_ON, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_FIZZCRANK8, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+9:
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_FIZZCRANK9, pCreature->GetGUID());
            pPlayer->AreaExploredOrEventHappens(QUEST_THE_MECHAGNOMES);
            break;
    }
    return true;
}

/*######
## npc_iruk
######*/

#define GOSSIP_ITEM_IRUK        "<Search corpse for Issliruk's Totem.>"

enum
{
    QUEST_SPIRITS_WATCH_OVER_US     = 11961,
    SPELL_CREATE_TOTEM              = 46816
};

bool GossipHello_npc_iruk(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(QUEST_SPIRITS_WATCH_OVER_US) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_IRUK, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_iruk(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF + 1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->CastSpell(pPlayer,SPELL_CREATE_TOTEM,true);
    }

    return true;
}

/*######
## npc_kara_thricestar
######*/

#define GOSSIP_ITEM_THRICESTAR1      "Do you think I could take a ride on one of those flying machines?"
#define GOSSIP_ITEM_THRICESTAR2      "Kara, I need to be flown out the Dens of Dying to find Bixie."

enum
{
    QUEST_CHECK_IN_WITH_BIXIE       = 11692,
    SPELL_FIZZCRANK_AIRSTRIP        = 51446
};

bool GossipHello_npc_kara_thricestar(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isTaxi())
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, GOSSIP_ITEM_THRICESTAR1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

    if (pPlayer->GetQuestStatus(QUEST_CHECK_IN_WITH_BIXIE) == QUEST_STATUS_COMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_THRICESTAR2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_kara_thricestar(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:
            pPlayer->GetSession()->SendTaxiMenu(pCreature);
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer, SPELL_FIZZCRANK_AIRSTRIP, false);
            break;
    }

    return true;
}

/*######
## npc_surristrasz
######*/

#define GOSSIP_ITEM_FREE_FLIGHT "I'd like passage to the Transitus Shield."
#define GOSSIP_ITEM_FLIGHT      "May I use a drake to fly elsewhere?"

enum
{
    SPELL_ABMER_TO_COLDARRA     = 46064
};

bool GossipHello_npc_surristrasz(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pCreature->isTaxi())
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FREE_FLIGHT, GOSSIP_SENDER_MAIN, GOSSIP_OPTION_GOSSIP);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, GOSSIP_ITEM_FLIGHT, GOSSIP_SENDER_MAIN, GOSSIP_OPTION_TAXIVENDOR);
    }

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_surristrasz(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_OPTION_GOSSIP)
    {
        pPlayer->CLOSE_GOSSIP_MENU();

        //TaxiPath 795 (amber to coldarra)
        pPlayer->CastSpell(pPlayer, SPELL_ABMER_TO_COLDARRA, true);
    }

    if (uiAction == GOSSIP_OPTION_TAXIVENDOR)
        pPlayer->GetSession()->SendTaxiMenu(pCreature);

    return true;
}

/*######
## npc_tiare
######*/

#define GOSSIP_ITEM_TELEPORT    "Teleport me to Amber Ledge, please."

enum
{
    SPELL_TELEPORT_COLDARRA     = 50135
};

bool GossipHello_npc_tiare(Player* pPlayer, Creature* pCreature)
{
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TELEPORT, GOSSIP_SENDER_MAIN, GOSSIP_OPTION_GOSSIP);
    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_tiare(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_OPTION_GOSSIP)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_COLDARRA, true);
    }
    return true;
}

void AddSC_borean_tundra()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "mob_elder";
    newscript->GetAI = &GetAI_mob_elder;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_fizzcrank_fullthrottle";
    newscript->pGossipHello = &GossipHello_npc_fizzcrank_fullthrottle;
    newscript->pGossipSelect = &GossipSelect_npc_fizzcrank_fullthrottle;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_iruk";
    newscript->pGossipHello = &GossipHello_npc_iruk;
    newscript->pGossipSelect = &GossipSelect_npc_iruk;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_kara_thricestar";
    newscript->pGossipHello = &GossipHello_npc_kara_thricestar;
    newscript->pGossipSelect = &GossipSelect_npc_kara_thricestar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_surristrasz";
    newscript->pGossipHello = &GossipHello_npc_surristrasz;
    newscript->pGossipSelect = &GossipSelect_npc_surristrasz;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_tiare";
    newscript->pGossipHello = &GossipHello_npc_tiare;
    newscript->pGossipSelect = &GossipSelect_npc_tiare;
    newscript->RegisterSelf();
}
