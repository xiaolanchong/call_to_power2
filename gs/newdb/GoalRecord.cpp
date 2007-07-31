
/*
 * DO NOT EDIT THIS FILE!
 * It is generated automatically by ctpdb
 */
#include "c3.h"
#include "c3errors.h"
#include "GoalRecord.h"
#include "DBLexer.h"
#include "DBTokens.h"
#include "CTPDatabase.h"
#include "StrDB.h"
#include "BitArray.h"

#include "OrderRecord.h"
#include "WonderRecord.h"

CTPDatabase<GoalRecord> *g_theGoalDB = NULL;

GoalRecord::Init()
{
    m_flags0 = 0;
    m_SquadClass = 0;
    m_TargetType = 0;
    m_TargetOwner = 0;
    m_Execute = 0x7fffffff;
    m_ThreatBonus = 0;
    m_EnemyValueBonus = 0;
    m_AlliedValueBonus = 0;
    m_PowerBonus = 0;
    m_DistanceToHomeBonus = 0;
    m_DistanceToEnemyBonus = 0;
    m_ChokePointBonus = 0;
    m_UnexploredBonus = 0;
    m_ObsoleteArmyBonus = 0;
    m_TreaspassingArmyBonus = 0;
    m_ThreatenType = 0;
    m_ThreatenBonus = 0;
    m_ForceMatch = 0;
    m_TargetProtectionWonderValue = 0;
}

GoalRecord::~GoalRecord()
{
	int index = 0;
}

void GoalRecord::operator=(const GoalRecord & rval)
{
	int index = 0;
	m_index = rval.m_index;
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_SquadClass = rval.m_SquadClass; 

	m_TargetType = rval.m_TargetType; 

	m_TargetOwner = rval.m_TargetOwner; 

	m_Execute = rval.m_Execute; 

	m_ThreatBonus = rval.m_ThreatBonus; 

	m_EnemyValueBonus = rval.m_EnemyValueBonus; 

	m_AlliedValueBonus = rval.m_AlliedValueBonus; 

	m_PowerBonus = rval.m_PowerBonus; 

	m_DistanceToHomeBonus = rval.m_DistanceToHomeBonus; 

	m_DistanceToEnemyBonus = rval.m_DistanceToEnemyBonus; 

	m_ChokePointBonus = rval.m_ChokePointBonus; 

	m_UnexploredBonus = rval.m_UnexploredBonus; 

	m_ObsoleteArmyBonus = rval.m_ObsoleteArmyBonus; 

	m_TreaspassingArmyBonus = rval.m_TreaspassingArmyBonus; 

	m_ThreatenType = rval.m_ThreatenType; 

	m_ThreatenBonus = rval.m_ThreatenBonus; 

	m_ForceMatch = rval.m_ForceMatch; 

	m_flags0 = rval.m_flags0; 
		m_TargetProtectionWonderValue = rval.m_TargetProtectionWonderValue; 

}

char *g_Goal_Tokens[] =
{
    "NeedsEscort",
    "IsSally",
    "AvoidWatchfulCity",
    "ExecuteIncrementally",
    "NeverSatisfied",
    "RemoveWhenComplete",
    "RallyFirst",
    "NoTransport",
    "SquadClass",
    "TargetType",
    "TargetOwner",
    "Execute",
    "ThreatBonus",
    "EnemyValueBonus",
    "AlliedValueBonus",
    "PowerBonus",
    "DistanceToHomeBonus",
    "DistanceToEnemyBonus",
    "ChokePointBonus",
    "UnexploredBonus",
    "ObsoleteArmyBonus",
    "TreaspassingArmyBonus",
    "ThreatenType",
    "ThreatenBonus",
    "ForceMatch",
    "TargetProtectionWonder",
    "TargetProtectionWonderValue",
};

GoalRecordAccessorInfo g_GoalRecord_Accessors[] = 
{
    { NULL, GoalRecord::GetNeedsEscort, NULL, NULL, NULL },
    { NULL, GoalRecord::GetIsSally, NULL, NULL, NULL },
    { NULL, GoalRecord::GetAvoidWatchfulCity, NULL, NULL, NULL },
    { NULL, GoalRecord::GetExecuteIncrementally, NULL, NULL, NULL },
    { NULL, GoalRecord::GetNeverSatisfied, NULL, NULL, NULL },
    { NULL, GoalRecord::GetRemoveWhenComplete, NULL, NULL, NULL },
    { NULL, GoalRecord::GetRallyFirst, NULL, NULL, NULL },
    { NULL, GoalRecord::GetNoTransport, NULL, NULL, NULL },
    { NULL, NULL, NULL, NULL, NULL}, /* SquadClass */
    { NULL, NULL, NULL, NULL, NULL}, /* TargetType */
    { NULL, NULL, NULL, NULL, NULL}, /* TargetOwner */
    {GoalRecord::GetExecuteIndex, NULL, NULL, NULL, NULL },
    { GoalRecord::GetThreatBonus, NULL, NULL, NULL, NULL },
    { GoalRecord::GetEnemyValueBonus, NULL, NULL, NULL, NULL },
    { GoalRecord::GetAlliedValueBonus, NULL, NULL, NULL, NULL },
    { GoalRecord::GetPowerBonus, NULL, NULL, NULL, NULL },
    { GoalRecord::GetDistanceToHomeBonus, NULL, NULL, NULL, NULL },
    { GoalRecord::GetDistanceToEnemyBonus, NULL, NULL, NULL, NULL },
    { GoalRecord::GetChokePointBonus, NULL, NULL, NULL, NULL },
    { GoalRecord::GetUnexploredBonus, NULL, NULL, NULL, NULL },
    { GoalRecord::GetObsoleteArmyBonus, NULL, NULL, NULL, NULL },
    { GoalRecord::GetTreaspassingArmyBonus, NULL, NULL, NULL, NULL },
    { NULL, NULL, NULL, NULL, NULL}, /* ThreatenType */
    { GoalRecord::GetThreatenBonus, NULL, NULL, NULL, NULL },
    { NULL, NULL, NULL, NULL, NULL}, /* ForceMatch */
    { NULL, NULL, NULL, NULL, NULL}, /* TargetProtectionWonder */
    { NULL, NULL, NULL, NULL, NULL}, /* TargetProtectionWonder */
};

#define k_Token_Goal_NeedsEscort                 ((k_Token_Custom_Base) + 0)
#define k_Token_Goal_IsSally                     ((k_Token_Custom_Base) + 1)
#define k_Token_Goal_AvoidWatchfulCity           ((k_Token_Custom_Base) + 2)
#define k_Token_Goal_ExecuteIncrementally        ((k_Token_Custom_Base) + 3)
#define k_Token_Goal_NeverSatisfied              ((k_Token_Custom_Base) + 4)
#define k_Token_Goal_RemoveWhenComplete          ((k_Token_Custom_Base) + 5)
#define k_Token_Goal_RallyFirst                  ((k_Token_Custom_Base) + 6)
#define k_Token_Goal_NoTransport                 ((k_Token_Custom_Base) + 7)
#define k_Token_Goal_SquadClass                  ((k_Token_Custom_Base) + 8)
#define k_Token_Goal_TargetType                  ((k_Token_Custom_Base) + 9)
#define k_Token_Goal_TargetOwner                 ((k_Token_Custom_Base) + 10)
#define k_Token_Goal_Execute                     ((k_Token_Custom_Base) + 11)
#define k_Token_Goal_ThreatBonus                 ((k_Token_Custom_Base) + 12)
#define k_Token_Goal_EnemyValueBonus             ((k_Token_Custom_Base) + 13)
#define k_Token_Goal_AlliedValueBonus            ((k_Token_Custom_Base) + 14)
#define k_Token_Goal_PowerBonus                  ((k_Token_Custom_Base) + 15)
#define k_Token_Goal_DistanceToHomeBonus         ((k_Token_Custom_Base) + 16)
#define k_Token_Goal_DistanceToEnemyBonus        ((k_Token_Custom_Base) + 17)
#define k_Token_Goal_ChokePointBonus             ((k_Token_Custom_Base) + 18)
#define k_Token_Goal_UnexploredBonus             ((k_Token_Custom_Base) + 19)
#define k_Token_Goal_ObsoleteArmyBonus           ((k_Token_Custom_Base) + 20)
#define k_Token_Goal_TreaspassingArmyBonus       ((k_Token_Custom_Base) + 21)
#define k_Token_Goal_ThreatenType                ((k_Token_Custom_Base) + 22)
#define k_Token_Goal_ThreatenBonus               ((k_Token_Custom_Base) + 23)
#define k_Token_Goal_ForceMatch                  ((k_Token_Custom_Base) + 24)
#define k_Token_Goal_TargetProtectionWonder      ((k_Token_Custom_Base) + 25)
#define k_Token_Goal_TargetProtectionWonder_Value ((k_Token_Custom_Base) + 26)
#define k_Token_Goal_Max                         ((k_Token_Custom_Base) + 27)


static BitArray s_ParsedTokens(27);
void GoalRecord::CheckRequiredFields(DBLexer *lex)
{
    // Ick.
    if(!s_ParsedTokens.Bit(k_Token_Goal_ThreatBonus - k_Token_Custom_Base)) {
        DBERROR(("Warning: required field ThreatBonus missing"));
    }
    if(!s_ParsedTokens.Bit(k_Token_Goal_EnemyValueBonus - k_Token_Custom_Base)) {
        DBERROR(("Warning: required field EnemyValueBonus missing"));
    }
    if(!s_ParsedTokens.Bit(k_Token_Goal_AlliedValueBonus - k_Token_Custom_Base)) {
        DBERROR(("Warning: required field AlliedValueBonus missing"));
    }
    if(!s_ParsedTokens.Bit(k_Token_Goal_PowerBonus - k_Token_Custom_Base)) {
        DBERROR(("Warning: required field PowerBonus missing"));
    }
    if(!s_ParsedTokens.Bit(k_Token_Goal_DistanceToHomeBonus - k_Token_Custom_Base)) {
        DBERROR(("Warning: required field DistanceToHomeBonus missing"));
    }
    if(!s_ParsedTokens.Bit(k_Token_Goal_DistanceToEnemyBonus - k_Token_Custom_Base)) {
        DBERROR(("Warning: required field DistanceToEnemyBonus missing"));
    }
    if(!s_ParsedTokens.Bit(k_Token_Goal_ChokePointBonus - k_Token_Custom_Base)) {
        DBERROR(("Warning: required field ChokePointBonus missing"));
    }
    if(!s_ParsedTokens.Bit(k_Token_Goal_UnexploredBonus - k_Token_Custom_Base)) {
        DBERROR(("Warning: required field UnexploredBonus missing"));
    }
    if(!s_ParsedTokens.Bit(k_Token_Goal_ObsoleteArmyBonus - k_Token_Custom_Base)) {
        DBERROR(("Warning: required field ObsoleteArmyBonus missing"));
    }
    if(!s_ParsedTokens.Bit(k_Token_Goal_TreaspassingArmyBonus - k_Token_Custom_Base)) {
        DBERROR(("Warning: required field TreaspassingArmyBonus missing"));
    }
    if(!s_ParsedTokens.Bit(k_Token_Goal_ThreatenBonus - k_Token_Custom_Base)) {
        DBERROR(("Warning: required field ThreatenBonus missing"));
    }
}
    
sint32 GoalRecord::Parse(DBLexer *lex)
{
    bool done = false;
    sint32 result = 0;
    sint32 tok;
    DBPARSE_ERROR err = DBPARSE_OK;
    s_ParsedTokens.Clear();
    lex->SetTokens(g_Goal_Tokens, k_Token_Goal_Max);
    tok = lex->GetToken();
    if(tok != k_Token_Name) {
        DBERROR(("Record does not start with name"));
        return 0;
    }
    if(!g_theStringDB->GetStringID(lex->GetTokenText(), m_name)) {
        g_theStringDB->InsertStr(lex->GetTokenText(), lex->GetTokenText());
        if(!g_theStringDB->GetStringID(lex->GetTokenText(), m_name))
            SetTextName(lex->GetTokenText());
    }

    tok = lex->GetToken();
    if(tok != k_Token_OpenBrace) {
        DBERROR(("Missing open brace"));
        return 0;
    }

    while(!done) {
        tok = lex->GetToken();
        if(tok >= k_Token_Custom_Base && tok < k_Token_Goal_Max) { 
            s_ParsedTokens.SetBit(tok - k_Token_Custom_Base);
        }
        switch(tok) {
            case k_Token_Goal_NeedsEscort:
                m_flags0 |= k_Goal_NeedsEscort_Bit;
                break;
            case k_Token_Goal_IsSally:
                m_flags0 |= k_Goal_IsSally_Bit;
                break;
            case k_Token_Goal_AvoidWatchfulCity:
                m_flags0 |= k_Goal_AvoidWatchfulCity_Bit;
                break;
            case k_Token_Goal_ExecuteIncrementally:
                m_flags0 |= k_Goal_ExecuteIncrementally_Bit;
                break;
            case k_Token_Goal_NeverSatisfied:
                m_flags0 |= k_Goal_NeverSatisfied_Bit;
                break;
            case k_Token_Goal_RemoveWhenComplete:
                m_flags0 |= k_Goal_RemoveWhenComplete_Bit;
                break;
            case k_Token_Goal_RallyFirst:
                m_flags0 |= k_Goal_RallyFirst_Bit;
                break;
            case k_Token_Goal_NoTransport:
                m_flags0 |= k_Goal_NoTransport_Bit;
                break;
            case k_Token_Goal_SquadClass:
                if(!ParseSquadClassBit(lex)) {
                    done = true; break;
                }
                break;
            case k_Token_Goal_TargetType:
                if(!ParseTargetTypeBit(lex)) {
                    done = true; break;
                }
                break;
            case k_Token_Goal_TargetOwner:
                if(!ParseTargetOwnerBit(lex)) {
                    done = true; break;
                }
                break;
            case k_Token_Goal_Execute:
                if(!g_theOrderDB->GetRecordFromLexer(lex, m_Execute, err)) {
                    done = true; break;
                }
                break;
            case k_Token_Goal_ThreatBonus:
                if(!lex->GetIntAssignment(m_ThreatBonus)) {
                    DBERROR(("Expected integer"));
                    done = true; break;
                }
                break;
            case k_Token_Goal_EnemyValueBonus:
                if(!lex->GetIntAssignment(m_EnemyValueBonus)) {
                    DBERROR(("Expected integer"));
                    done = true; break;
                }
                break;
            case k_Token_Goal_AlliedValueBonus:
                if(!lex->GetIntAssignment(m_AlliedValueBonus)) {
                    DBERROR(("Expected integer"));
                    done = true; break;
                }
                break;
            case k_Token_Goal_PowerBonus:
                if(!lex->GetIntAssignment(m_PowerBonus)) {
                    DBERROR(("Expected integer"));
                    done = true; break;
                }
                break;
            case k_Token_Goal_DistanceToHomeBonus:
                if(!lex->GetIntAssignment(m_DistanceToHomeBonus)) {
                    DBERROR(("Expected integer"));
                    done = true; break;
                }
                break;
            case k_Token_Goal_DistanceToEnemyBonus:
                if(!lex->GetIntAssignment(m_DistanceToEnemyBonus)) {
                    DBERROR(("Expected integer"));
                    done = true; break;
                }
                break;
            case k_Token_Goal_ChokePointBonus:
                if(!lex->GetIntAssignment(m_ChokePointBonus)) {
                    DBERROR(("Expected integer"));
                    done = true; break;
                }
                break;
            case k_Token_Goal_UnexploredBonus:
                if(!lex->GetIntAssignment(m_UnexploredBonus)) {
                    DBERROR(("Expected integer"));
                    done = true; break;
                }
                break;
            case k_Token_Goal_ObsoleteArmyBonus:
                if(!lex->GetIntAssignment(m_ObsoleteArmyBonus)) {
                    DBERROR(("Expected integer"));
                    done = true; break;
                }
                break;
            case k_Token_Goal_TreaspassingArmyBonus:
                if(!lex->GetIntAssignment(m_TreaspassingArmyBonus)) {
                    DBERROR(("Expected integer"));
                    done = true; break;
                }
                break;
            case k_Token_Goal_ThreatenType:
                if(!ParseThreatenTypeBit(lex)) {
                    done = true; break;
                }
                break;
            case k_Token_Goal_ThreatenBonus:
                if(!lex->GetIntAssignment(m_ThreatenBonus)) {
                    DBERROR(("Expected integer"));
                    done = true; break;
                }
                break;
            case k_Token_Goal_ForceMatch:
                if(!ParseForceMatchBit(lex)) {
                    done = true; break;
                }
                break;
            case k_Token_Goal_TargetProtectionWonder:
                m_flags0 |= k_Goal_TargetProtectionWonder_Bit;
                if(!g_theWonderDB->GetRecordFromLexer(lex, m_TargetProtectionWonderValue, err)) {
                    DBERROR(("Expected record"));
                    done = true; break;
                }
                break;
            case k_Token_CloseBrace:
                done = true;
                result = 1;
                break;
            default:
                DBERROR(("Unknown token"));
                done = true;
                break;
        }
    }
    CheckRequiredFields(lex);
    lex->RestoreTokens();
    return result;
}

void GoalRecord::ResolveDBReferences()
{
    if(m_Execute & 0x80000000) {
        sint32 id = m_Execute & 0x7fffffff;
        if(!g_theOrderDB->GetNamedItem(id, m_Execute)) {
            c3errors_ErrorDialog("DB", "%s not found in Order database", g_theStringDB->GetNameStr(id));
        }
    } else if(m_Execute == 0x7fffffff) {
        m_Execute = -1;
    }
    if(m_TargetProtectionWonderValue & 0x80000000) {
        sint32 id = m_TargetProtectionWonderValue & 0x7fffffff;
        if(!g_theWonderDB->GetNamedItem(id, m_TargetProtectionWonderValue)) {
            c3errors_ErrorDialog("DB", "%s not found in Wonder database", g_theStringDB->GetNameStr(id));
        }
    } else if(m_TargetProtectionWonderValue == 0x7fffffff) {
        m_TargetProtectionWonderValue = -1;
    }
}
/*
 * Member class Parsers
 * (DO NOT EDIT!  Automatically generated file)
 */
/*
 * Data Parsers
 * (DO NOT EDIT!  Automatically generated file)
 */
static char *s_Goal_SquadClass_BitNames[] = {
    "CanExplore",
    "CanAttack",
    "CanDefend",
    "HasZoc",
    "CanCaptureCity",
    "CanBombard",
    "Special",
    "CanTransport",
};
#define k_Goal_SquadClass_Num_BitNames 8

sint32 GoalRecord::ParseSquadClassBit(DBLexer *lex)
{
    sint32 bitindex;
    if(!lex->GetBitIndex((const char **)s_Goal_SquadClass_BitNames, k_Goal_SquadClass_Num_BitNames, bitindex)) {
        DBERROR(("%s is not a member of SquadClass", lex->GetTokenText()));
        return 0;
    }
    m_SquadClass |= (1 << bitindex);
    return 1;
}

static char *s_Goal_TargetType_BitNames[] = {
    "AttackUnit",
    "SpecialUnit",
    "SettleLand",
    "SettleSea",
    "City",
    "ChokePoint",
    "GoodyHut",
    "TradeRoute",
    "Improvement",
    "Unexplored",
    "Border",
    "Endgame",
    "Random",
};
#define k_Goal_TargetType_Num_BitNames 13

sint32 GoalRecord::ParseTargetTypeBit(DBLexer *lex)
{
    sint32 bitindex;
    if(!lex->GetBitIndex((const char **)s_Goal_TargetType_BitNames, k_Goal_TargetType_Num_BitNames, bitindex)) {
        DBERROR(("%s is not a member of TargetType", lex->GetTokenText()));
        return 0;
    }
    m_TargetType |= (1 << bitindex);
    return 1;
}

static char *s_Goal_TargetOwner_BitNames[] = {
    "Self",
    "Ally",
    "Neutral",
    "ColdEnemy",
    "HotEnemy",
    "NoContact",
};
#define k_Goal_TargetOwner_Num_BitNames 6

sint32 GoalRecord::ParseTargetOwnerBit(DBLexer *lex)
{
    sint32 bitindex;
    if(!lex->GetBitIndex((const char **)s_Goal_TargetOwner_BitNames, k_Goal_TargetOwner_Num_BitNames, bitindex)) {
        DBERROR(("%s is not a member of TargetOwner", lex->GetTokenText()));
        return 0;
    }
    m_TargetOwner |= (1 << bitindex);
    return 1;
}

static char *s_Goal_ThreatenType_BitNames[] = {
    "AttackCity",
    "Pirate",
    "SpecialAttackCity",
    "DestroyCity",
    "None",
};
#define k_Goal_ThreatenType_Num_BitNames 5

sint32 GoalRecord::ParseThreatenTypeBit(DBLexer *lex)
{
    sint32 bitindex;
    if(!lex->GetBitIndex((const char **)s_Goal_ThreatenType_BitNames, k_Goal_ThreatenType_Num_BitNames, bitindex)) {
        DBERROR(("%s is not a member of ThreatenType", lex->GetTokenText()));
        return 0;
    }
    m_ThreatenType |= (1 << bitindex);
    return 1;
}

static char *s_Goal_ForceMatch_BitNames[] = {
    "Offensive",
    "Defensive",
    "StealthAttack",
    "Bombard",
    "Special",
    "Harass",
};
#define k_Goal_ForceMatch_Num_BitNames 6

sint32 GoalRecord::ParseForceMatchBit(DBLexer *lex)
{
    sint32 bitindex;
    if(!lex->GetBitIndex((const char **)s_Goal_ForceMatch_BitNames, k_Goal_ForceMatch_Num_BitNames, bitindex)) {
        DBERROR(("%s is not a member of ForceMatch", lex->GetTokenText()));
        return 0;
    }
    m_ForceMatch |= (1 << bitindex);
    return 1;
}

void GoalRecord::Merge(const GoalRecord & rval){
	int index = 0, obj_index = 0;
	// only replace values that have been set
	if(rval.m_SquadClass != 0x0) 
		m_SquadClass = rval.m_SquadClass; 

	// only replace values that have been set
	if(rval.m_TargetType != 0x0) 
		m_TargetType = rval.m_TargetType; 

	// only replace values that have been set
	if(rval.m_TargetOwner != 0x0) 
		m_TargetOwner = rval.m_TargetOwner; 

	// only replace values that have been set
	if(rval.m_ThreatenType != 0x0) 
		m_ThreatenType = rval.m_ThreatenType; 

	// only replace values that have been set
	if(rval.m_ForceMatch != 0x0) 
		m_ForceMatch = rval.m_ForceMatch; 

	// only replace values that have been set
	if((rval.m_flags0 & k_Goal_TargetProtectionWonder_Bit) != 0) 
		m_TargetProtectionWonderValue = rval.m_TargetProtectionWonderValue; 

} 

/*
 * Data accessors
 * (DO NOT EDIT!  Automatically generated file)
 */
const OrderRecord *GoalRecord::GetExecute() const
{
    return g_theOrderDB->Get(m_Execute);
}

const WonderRecord *GoalRecord::GetTargetProtectionWonderPtr() const
{
    return g_theWonderDB->Get(m_TargetProtectionWonderValue);
}

