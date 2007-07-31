
/*
 * DO NOT EDIT THIS FILE!
 * It is generated automatically by ctpdb
 */
#include "c3.h"
#include "c3errors.h"
#include "DiplomacyProposalRecord.h"
#include "DBLexer.h"
#include "DBTokens.h"
#include "CTPDatabase.h"
#include "StrDB.h"
#include "BitArray.h"

#include "DiplomacyProposalRecord.h"

CTPDatabase<DiplomacyProposalRecord> *g_theDiplomacyProposalDB = NULL;

DiplomacyProposalRecord::Init()
{
    m_flags0 = 0;
    m_Title = 0;
    m_Category = 0;
    m_Type = NULL;
    m_Arg1 = 0;
    memset(&m_Details0Value, 0, sizeof(m_Details0Value));
    memset(&m_Details1Value, 0, sizeof(m_Details1Value));
    memset(&m_Details2Value, 0, sizeof(m_Details2Value));
    memset(&m_Details3Value, 0, sizeof(m_Details3Value));
    memset(&m_Details4Value, 0, sizeof(m_Details4Value));
    memset(&m_DetailsEx0Value, 0, sizeof(m_DetailsEx0Value));
    memset(&m_DetailsEx1Value, 0, sizeof(m_DetailsEx1Value));
    memset(&m_DetailsEx2Value, 0, sizeof(m_DetailsEx2Value));
    memset(&m_DetailsEx3Value, 0, sizeof(m_DetailsEx3Value));
    memset(&m_DetailsEx4Value, 0, sizeof(m_DetailsEx4Value));
    m_Class = 0;
    m_Excludes = 0;
    m_ReciprocalValue = 0;
    m_Image = NULL;
    m_ImageSlotValue = 0;
}

DiplomacyProposalRecord::~DiplomacyProposalRecord()
{
	int index = 0;
	// free string attribute Type
	if (m_Type) 
		delete m_Type; 
	m_Type = NULL; 

	// free string attribute Image
	if (m_Image) 
		delete m_Image; 
	m_Image = NULL; 

}

void DiplomacyProposalRecord::operator=(const DiplomacyProposalRecord & rval)
{
	int index = 0;
	m_index = rval.m_index;
	m_Title = rval.m_Title; 

	m_Category = rval.m_Category; 

	if (m_Type) 
	{ 
	delete m_Type; 

	}

	if (rval.m_Type) 
	{ 
		m_Type = new char [strlen(rval.m_Type)+1]; 
		strcpy(m_Type, rval.m_Type);
	}

	m_Arg1 = rval.m_Arg1; 

	m_flags0 = rval.m_flags0; 
		m_Details0Value = rval.m_Details0Value; 

	m_flags0 = rval.m_flags0; 
		m_Details1Value = rval.m_Details1Value; 

	m_flags0 = rval.m_flags0; 
		m_Details2Value = rval.m_Details2Value; 

	m_flags0 = rval.m_flags0; 
		m_Details3Value = rval.m_Details3Value; 

	m_flags0 = rval.m_flags0; 
		m_Details4Value = rval.m_Details4Value; 

	m_flags0 = rval.m_flags0; 
		m_DetailsEx0Value = rval.m_DetailsEx0Value; 

	m_flags0 = rval.m_flags0; 
		m_DetailsEx1Value = rval.m_DetailsEx1Value; 

	m_flags0 = rval.m_flags0; 
		m_DetailsEx2Value = rval.m_DetailsEx2Value; 

	m_flags0 = rval.m_flags0; 
		m_DetailsEx3Value = rval.m_DetailsEx3Value; 

	m_flags0 = rval.m_flags0; 
		m_DetailsEx4Value = rval.m_DetailsEx4Value; 

	m_Class = rval.m_Class; 

	m_Excludes = rval.m_Excludes; 

	m_flags0 = rval.m_flags0; 
		m_ReciprocalValue = rval.m_ReciprocalValue; 

	if (m_Image) 
	{ 
	delete m_Image; 

	}

	if (rval.m_Image) 
	{ 
		m_Image = new char [strlen(rval.m_Image)+1]; 
		strcpy(m_Image, rval.m_Image);
	}

	m_flags0 = rval.m_flags0; 
		m_ImageSlotValue = rval.m_ImageSlotValue; 

	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
	m_flags0 = rval.m_flags0; 
}

char *g_DiplomacyProposal_Tokens[] =
{
    "Title",
    "Category",
    "Type",
    "Arg1",
    "Details0",
    "Details0Value",
    "Details1",
    "Details1Value",
    "Details2",
    "Details2Value",
    "Details3",
    "Details3Value",
    "Details4",
    "Details4Value",
    "DetailsEx0",
    "DetailsEx0Value",
    "DetailsEx1",
    "DetailsEx1Value",
    "DetailsEx2",
    "DetailsEx2Value",
    "DetailsEx3",
    "DetailsEx3Value",
    "DetailsEx4",
    "DetailsEx4Value",
    "Class",
    "Excludes",
    "Reciprocal",
    "ReciprocalValue",
    "Image",
    "ImageSlot",
    "ImageSlotValue",
    "Disabled",
    "HasAlly",
    "HasBorderIncursion",
    "IsPirating",
    "HasEmbassy",
    "HasHotwar",
    "NoHotwar",
    "HasPeaceTreaty",
    "HasAlliance",
    "HasNuclearWeapons",
    "HasBioWeapons",
    "HasNanoWeapons",
    "HasEmbargo",
    "HasMilitaryAgreement",
    "HasPollutionAgreement",
    "HasPollution",
    "TrustViolationGlobal",
};

DiplomacyProposalRecordAccessorInfo g_DiplomacyProposalRecord_Accessors[] = 
{
    { NULL, NULL, NULL, NULL, NULL}, /* Title */
    { NULL, NULL, NULL, NULL, NULL}, /* Category */
    { NULL, NULL, NULL, NULL, NULL}, /* Type */
    { NULL, NULL, NULL, NULL, NULL}, /* Arg1 */
    { NULL, NULL, NULL, NULL, NULL}, /* Details0 */
    { NULL, NULL, NULL, NULL, NULL}, /* Details0 */
    { NULL, NULL, NULL, NULL, NULL}, /* Details1 */
    { NULL, NULL, NULL, NULL, NULL}, /* Details1 */
    { NULL, NULL, NULL, NULL, NULL}, /* Details2 */
    { NULL, NULL, NULL, NULL, NULL}, /* Details2 */
    { NULL, NULL, NULL, NULL, NULL}, /* Details3 */
    { NULL, NULL, NULL, NULL, NULL}, /* Details3 */
    { NULL, NULL, NULL, NULL, NULL}, /* Details4 */
    { NULL, NULL, NULL, NULL, NULL}, /* Details4 */
    { NULL, NULL, NULL, NULL, NULL}, /* DetailsEx0 */
    { NULL, NULL, NULL, NULL, NULL}, /* DetailsEx0 */
    { NULL, NULL, NULL, NULL, NULL}, /* DetailsEx1 */
    { NULL, NULL, NULL, NULL, NULL}, /* DetailsEx1 */
    { NULL, NULL, NULL, NULL, NULL}, /* DetailsEx2 */
    { NULL, NULL, NULL, NULL, NULL}, /* DetailsEx2 */
    { NULL, NULL, NULL, NULL, NULL}, /* DetailsEx3 */
    { NULL, NULL, NULL, NULL, NULL}, /* DetailsEx3 */
    { NULL, NULL, NULL, NULL, NULL}, /* DetailsEx4 */
    { NULL, NULL, NULL, NULL, NULL}, /* DetailsEx4 */
    { NULL, NULL, NULL, NULL, NULL}, /* Class */
    { NULL, NULL, NULL, NULL, NULL}, /* Excludes */
    { NULL, NULL, NULL, NULL, NULL}, /* Reciprocal */
    { NULL, NULL, NULL, NULL, NULL}, /* Reciprocal */
    { NULL, NULL, NULL, NULL, NULL}, /* Image */
    { NULL, NULL, NULL, DiplomacyProposalRecord::GetImageSlot, NULL },
    { NULL, NULL, NULL, NULL, NULL}, /* ImageSlot */
    { NULL, DiplomacyProposalRecord::GetDisabled, NULL, NULL, NULL },
    { NULL, DiplomacyProposalRecord::GetHasAlly, NULL, NULL, NULL },
    { NULL, DiplomacyProposalRecord::GetHasBorderIncursion, NULL, NULL, NULL },
    { NULL, DiplomacyProposalRecord::GetIsPirating, NULL, NULL, NULL },
    { NULL, DiplomacyProposalRecord::GetHasEmbassy, NULL, NULL, NULL },
    { NULL, DiplomacyProposalRecord::GetHasHotwar, NULL, NULL, NULL },
    { NULL, DiplomacyProposalRecord::GetNoHotwar, NULL, NULL, NULL },
    { NULL, DiplomacyProposalRecord::GetHasPeaceTreaty, NULL, NULL, NULL },
    { NULL, DiplomacyProposalRecord::GetHasAlliance, NULL, NULL, NULL },
    { NULL, DiplomacyProposalRecord::GetHasNuclearWeapons, NULL, NULL, NULL },
    { NULL, DiplomacyProposalRecord::GetHasBioWeapons, NULL, NULL, NULL },
    { NULL, DiplomacyProposalRecord::GetHasNanoWeapons, NULL, NULL, NULL },
    { NULL, DiplomacyProposalRecord::GetHasEmbargo, NULL, NULL, NULL },
    { NULL, DiplomacyProposalRecord::GetHasMilitaryAgreement, NULL, NULL, NULL },
    { NULL, DiplomacyProposalRecord::GetHasPollutionAgreement, NULL, NULL, NULL },
    { NULL, DiplomacyProposalRecord::GetHasPollution, NULL, NULL, NULL },
    { NULL, DiplomacyProposalRecord::GetTrustViolationGlobal, NULL, NULL, NULL },
};

#define k_Token_DiplomacyProposal_Title          ((k_Token_Custom_Base) + 0)
#define k_Token_DiplomacyProposal_Category       ((k_Token_Custom_Base) + 1)
#define k_Token_DiplomacyProposal_Type           ((k_Token_Custom_Base) + 2)
#define k_Token_DiplomacyProposal_Arg1           ((k_Token_Custom_Base) + 3)
#define k_Token_DiplomacyProposal_Details0       ((k_Token_Custom_Base) + 4)
#define k_Token_DiplomacyProposal_Details0_Value ((k_Token_Custom_Base) + 5)
#define k_Token_DiplomacyProposal_Details1       ((k_Token_Custom_Base) + 6)
#define k_Token_DiplomacyProposal_Details1_Value ((k_Token_Custom_Base) + 7)
#define k_Token_DiplomacyProposal_Details2       ((k_Token_Custom_Base) + 8)
#define k_Token_DiplomacyProposal_Details2_Value ((k_Token_Custom_Base) + 9)
#define k_Token_DiplomacyProposal_Details3       ((k_Token_Custom_Base) + 10)
#define k_Token_DiplomacyProposal_Details3_Value ((k_Token_Custom_Base) + 11)
#define k_Token_DiplomacyProposal_Details4       ((k_Token_Custom_Base) + 12)
#define k_Token_DiplomacyProposal_Details4_Value ((k_Token_Custom_Base) + 13)
#define k_Token_DiplomacyProposal_DetailsEx0     ((k_Token_Custom_Base) + 14)
#define k_Token_DiplomacyProposal_DetailsEx0_Value ((k_Token_Custom_Base) + 15)
#define k_Token_DiplomacyProposal_DetailsEx1     ((k_Token_Custom_Base) + 16)
#define k_Token_DiplomacyProposal_DetailsEx1_Value ((k_Token_Custom_Base) + 17)
#define k_Token_DiplomacyProposal_DetailsEx2     ((k_Token_Custom_Base) + 18)
#define k_Token_DiplomacyProposal_DetailsEx2_Value ((k_Token_Custom_Base) + 19)
#define k_Token_DiplomacyProposal_DetailsEx3     ((k_Token_Custom_Base) + 20)
#define k_Token_DiplomacyProposal_DetailsEx3_Value ((k_Token_Custom_Base) + 21)
#define k_Token_DiplomacyProposal_DetailsEx4     ((k_Token_Custom_Base) + 22)
#define k_Token_DiplomacyProposal_DetailsEx4_Value ((k_Token_Custom_Base) + 23)
#define k_Token_DiplomacyProposal_Class          ((k_Token_Custom_Base) + 24)
#define k_Token_DiplomacyProposal_Excludes       ((k_Token_Custom_Base) + 25)
#define k_Token_DiplomacyProposal_Reciprocal     ((k_Token_Custom_Base) + 26)
#define k_Token_DiplomacyProposal_Reciprocal_Value ((k_Token_Custom_Base) + 27)
#define k_Token_DiplomacyProposal_Image          ((k_Token_Custom_Base) + 28)
#define k_Token_DiplomacyProposal_ImageSlot      ((k_Token_Custom_Base) + 29)
#define k_Token_DiplomacyProposal_ImageSlot_Value ((k_Token_Custom_Base) + 30)
#define k_Token_DiplomacyProposal_Disabled       ((k_Token_Custom_Base) + 31)
#define k_Token_DiplomacyProposal_HasAlly        ((k_Token_Custom_Base) + 32)
#define k_Token_DiplomacyProposal_HasBorderIncursion ((k_Token_Custom_Base) + 33)
#define k_Token_DiplomacyProposal_IsPirating     ((k_Token_Custom_Base) + 34)
#define k_Token_DiplomacyProposal_HasEmbassy     ((k_Token_Custom_Base) + 35)
#define k_Token_DiplomacyProposal_HasHotwar      ((k_Token_Custom_Base) + 36)
#define k_Token_DiplomacyProposal_NoHotwar       ((k_Token_Custom_Base) + 37)
#define k_Token_DiplomacyProposal_HasPeaceTreaty ((k_Token_Custom_Base) + 38)
#define k_Token_DiplomacyProposal_HasAlliance    ((k_Token_Custom_Base) + 39)
#define k_Token_DiplomacyProposal_HasNuclearWeapons ((k_Token_Custom_Base) + 40)
#define k_Token_DiplomacyProposal_HasBioWeapons  ((k_Token_Custom_Base) + 41)
#define k_Token_DiplomacyProposal_HasNanoWeapons ((k_Token_Custom_Base) + 42)
#define k_Token_DiplomacyProposal_HasEmbargo     ((k_Token_Custom_Base) + 43)
#define k_Token_DiplomacyProposal_HasMilitaryAgreement ((k_Token_Custom_Base) + 44)
#define k_Token_DiplomacyProposal_HasPollutionAgreement ((k_Token_Custom_Base) + 45)
#define k_Token_DiplomacyProposal_HasPollution   ((k_Token_Custom_Base) + 46)
#define k_Token_DiplomacyProposal_TrustViolationGlobal ((k_Token_Custom_Base) + 47)
#define k_Token_DiplomacyProposal_Max            ((k_Token_Custom_Base) + 48)


static BitArray s_ParsedTokens(48);
void DiplomacyProposalRecord::CheckRequiredFields(DBLexer *lex)
{
    // Ick.
    if(!s_ParsedTokens.Bit(k_Token_DiplomacyProposal_Title - k_Token_Custom_Base)) {
        DBERROR(("Warning: required field Title missing"));
    }
    if(!s_ParsedTokens.Bit(k_Token_DiplomacyProposal_Type - k_Token_Custom_Base)) {
        DBERROR(("Warning: required field Type missing"));
    }
    if(!s_ParsedTokens.Bit(k_Token_DiplomacyProposal_Image - k_Token_Custom_Base)) {
        DBERROR(("Warning: required field Image missing"));
    }
}
    
sint32 DiplomacyProposalRecord::Parse(DBLexer *lex)
{
    bool done = false;
    sint32 result = 0;
    sint32 tok;
    DBPARSE_ERROR err = DBPARSE_OK;
    s_ParsedTokens.Clear();
    lex->SetTokens(g_DiplomacyProposal_Tokens, k_Token_DiplomacyProposal_Max);
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
        if(tok >= k_Token_Custom_Base && tok < k_Token_DiplomacyProposal_Max) { 
            s_ParsedTokens.SetBit(tok - k_Token_Custom_Base);
        }
        switch(tok) {
            case k_Token_DiplomacyProposal_Title:
                if(!lex->GetStringIdAssignment(m_Title)) {
                    DBERROR(("Expected string ID"));
                    done = true; break;
                }
                break;
            case k_Token_DiplomacyProposal_Category:
                if(!ParseCategoryBit(lex)) {
                    done = true; break;
                }
                break;
            case k_Token_DiplomacyProposal_Type:
                if(!lex->GetFileAssignment(m_Type)) {
                    DBERROR(("Expected string"));
                    done = true; break;
                }
                break;
            case k_Token_DiplomacyProposal_Arg1:
                if(!ParseArg1Bit(lex)) {
                    done = true; break;
                }
                break;
            case k_Token_DiplomacyProposal_Details0:
                m_flags0 |= k_DiplomacyProposal_Details0_Bit;
                if(!m_Details0Value.Parse(lex)) {
                    DBERROR(("Expected struct"));
                    done = true; break;
                }
                break;
            case k_Token_DiplomacyProposal_Details1:
                m_flags0 |= k_DiplomacyProposal_Details1_Bit;
                if(!m_Details1Value.Parse(lex)) {
                    DBERROR(("Expected struct"));
                    done = true; break;
                }
                break;
            case k_Token_DiplomacyProposal_Details2:
                m_flags0 |= k_DiplomacyProposal_Details2_Bit;
                if(!m_Details2Value.Parse(lex)) {
                    DBERROR(("Expected struct"));
                    done = true; break;
                }
                break;
            case k_Token_DiplomacyProposal_Details3:
                m_flags0 |= k_DiplomacyProposal_Details3_Bit;
                if(!m_Details3Value.Parse(lex)) {
                    DBERROR(("Expected struct"));
                    done = true; break;
                }
                break;
            case k_Token_DiplomacyProposal_Details4:
                m_flags0 |= k_DiplomacyProposal_Details4_Bit;
                if(!m_Details4Value.Parse(lex)) {
                    DBERROR(("Expected struct"));
                    done = true; break;
                }
                break;
            case k_Token_DiplomacyProposal_DetailsEx0:
                m_flags0 |= k_DiplomacyProposal_DetailsEx0_Bit;
                if(!m_DetailsEx0Value.Parse(lex)) {
                    DBERROR(("Expected struct"));
                    done = true; break;
                }
                break;
            case k_Token_DiplomacyProposal_DetailsEx1:
                m_flags0 |= k_DiplomacyProposal_DetailsEx1_Bit;
                if(!m_DetailsEx1Value.Parse(lex)) {
                    DBERROR(("Expected struct"));
                    done = true; break;
                }
                break;
            case k_Token_DiplomacyProposal_DetailsEx2:
                m_flags0 |= k_DiplomacyProposal_DetailsEx2_Bit;
                if(!m_DetailsEx2Value.Parse(lex)) {
                    DBERROR(("Expected struct"));
                    done = true; break;
                }
                break;
            case k_Token_DiplomacyProposal_DetailsEx3:
                m_flags0 |= k_DiplomacyProposal_DetailsEx3_Bit;
                if(!m_DetailsEx3Value.Parse(lex)) {
                    DBERROR(("Expected struct"));
                    done = true; break;
                }
                break;
            case k_Token_DiplomacyProposal_DetailsEx4:
                m_flags0 |= k_DiplomacyProposal_DetailsEx4_Bit;
                if(!m_DetailsEx4Value.Parse(lex)) {
                    DBERROR(("Expected struct"));
                    done = true; break;
                }
                break;
            case k_Token_DiplomacyProposal_Class:
                if(!ParseClassBit(lex)) {
                    done = true; break;
                }
                break;
            case k_Token_DiplomacyProposal_Excludes:
                if(!ParseExcludesBit(lex)) {
                    done = true; break;
                }
                break;
            case k_Token_DiplomacyProposal_Reciprocal:
                m_flags0 |= k_DiplomacyProposal_Reciprocal_Bit;
                if(!g_theDiplomacyProposalDB->GetRecordFromLexer(lex, m_ReciprocalValue, err)) {
                    DBERROR(("Expected record"));
                    done = true; break;
                }
                break;
            case k_Token_DiplomacyProposal_Image:
                if(!lex->GetFileAssignment(m_Image)) {
                    DBERROR(("Expected string"));
                    done = true; break;
                }
                break;
            case k_Token_DiplomacyProposal_ImageSlot:
                m_flags0 |= k_DiplomacyProposal_ImageSlot_Bit;
                if(!lex->GetIntAssignment(m_ImageSlotValue)) {
                    DBERROR(("Expected integer"));
                    done = true; break;
                }
                break;
            case k_Token_DiplomacyProposal_Disabled:
                m_flags0 |= k_DiplomacyProposal_Disabled_Bit;
                break;
            case k_Token_DiplomacyProposal_HasAlly:
                m_flags0 |= k_DiplomacyProposal_HasAlly_Bit;
                break;
            case k_Token_DiplomacyProposal_HasBorderIncursion:
                m_flags0 |= k_DiplomacyProposal_HasBorderIncursion_Bit;
                break;
            case k_Token_DiplomacyProposal_IsPirating:
                m_flags0 |= k_DiplomacyProposal_IsPirating_Bit;
                break;
            case k_Token_DiplomacyProposal_HasEmbassy:
                m_flags0 |= k_DiplomacyProposal_HasEmbassy_Bit;
                break;
            case k_Token_DiplomacyProposal_HasHotwar:
                m_flags0 |= k_DiplomacyProposal_HasHotwar_Bit;
                break;
            case k_Token_DiplomacyProposal_NoHotwar:
                m_flags0 |= k_DiplomacyProposal_NoHotwar_Bit;
                break;
            case k_Token_DiplomacyProposal_HasPeaceTreaty:
                m_flags0 |= k_DiplomacyProposal_HasPeaceTreaty_Bit;
                break;
            case k_Token_DiplomacyProposal_HasAlliance:
                m_flags0 |= k_DiplomacyProposal_HasAlliance_Bit;
                break;
            case k_Token_DiplomacyProposal_HasNuclearWeapons:
                m_flags0 |= k_DiplomacyProposal_HasNuclearWeapons_Bit;
                break;
            case k_Token_DiplomacyProposal_HasBioWeapons:
                m_flags0 |= k_DiplomacyProposal_HasBioWeapons_Bit;
                break;
            case k_Token_DiplomacyProposal_HasNanoWeapons:
                m_flags0 |= k_DiplomacyProposal_HasNanoWeapons_Bit;
                break;
            case k_Token_DiplomacyProposal_HasEmbargo:
                m_flags0 |= k_DiplomacyProposal_HasEmbargo_Bit;
                break;
            case k_Token_DiplomacyProposal_HasMilitaryAgreement:
                m_flags0 |= k_DiplomacyProposal_HasMilitaryAgreement_Bit;
                break;
            case k_Token_DiplomacyProposal_HasPollutionAgreement:
                m_flags0 |= k_DiplomacyProposal_HasPollutionAgreement_Bit;
                break;
            case k_Token_DiplomacyProposal_HasPollution:
                m_flags0 |= k_DiplomacyProposal_HasPollution_Bit;
                break;
            case k_Token_DiplomacyProposal_TrustViolationGlobal:
                m_flags0 |= k_DiplomacyProposal_TrustViolationGlobal_Bit;
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

void DiplomacyProposalRecord::ResolveDBReferences()
{
    m_Details0Value.ResolveDBReferences();
    m_Details1Value.ResolveDBReferences();
    m_Details2Value.ResolveDBReferences();
    m_Details3Value.ResolveDBReferences();
    m_Details4Value.ResolveDBReferences();
    m_DetailsEx0Value.ResolveDBReferences();
    m_DetailsEx1Value.ResolveDBReferences();
    m_DetailsEx2Value.ResolveDBReferences();
    m_DetailsEx3Value.ResolveDBReferences();
    m_DetailsEx4Value.ResolveDBReferences();
    if(m_ReciprocalValue & 0x80000000) {
        sint32 id = m_ReciprocalValue & 0x7fffffff;
        if(!g_theDiplomacyProposalDB->GetNamedItem(id, m_ReciprocalValue)) {
            c3errors_ErrorDialog("DB", "%s not found in DiplomacyProposal database", g_theStringDB->GetNameStr(id));
        }
    } else if(m_ReciprocalValue == 0x7fffffff) {
        m_ReciprocalValue = -1;
    }
}
/*
 * Member class Parsers
 * (DO NOT EDIT!  Automatically generated file)
 */
DiplomacyProposalRecord::StrengthStrings::StrengthStrings()
{
    m_flags0 = 0;
    m_DetailsWeakValue = 0;
    m_DetailsEvenValue = 0;
    m_DetailsStrongValue = 0;
}
DiplomacyProposalRecord::StrengthStrings::~StrengthStrings()
{
}
void DiplomacyProposalRecord::StrengthStrings::operator=(const StrengthStrings & rval)
{
	m_flags0 = rval.m_flags0; 
		m_DetailsWeakValue = rval.m_DetailsWeakValue; 

	m_flags0 = rval.m_flags0; 
		m_DetailsEvenValue = rval.m_DetailsEvenValue; 

	m_flags0 = rval.m_flags0; 
		m_DetailsStrongValue = rval.m_DetailsStrongValue; 

}
static char *s_DiplomacyProposal_StrengthStrings_Tokens[] = {
    "DetailsWeak",
    "DetailsEven",
    "DetailsStrong",
};
#define k_Token_DiplomacyProposal_StrengthStrings_DetailsWeak ((k_Token_Custom_Base) + 0)
#define k_Token_DiplomacyProposal_StrengthStrings_DetailsEven ((k_Token_Custom_Base) + 1)
#define k_Token_DiplomacyProposal_StrengthStrings_DetailsStrong ((k_Token_Custom_Base) + 2)
#define k_Token_DiplomacyProposal_StrengthStrings_Max ((k_Token_Custom_Base) + 3)
sint32 DiplomacyProposalRecord::StrengthStrings::Parse(DBLexer *lex)
{
    bool done = false;
    sint32 tok;
    sint32 result = 0;
    DBPARSE_ERROR err = DBPARSE_OK;
    tok = lex->PeekAhead();
    if(tok != k_Token_OpenBrace) {
        DBERROR(("Expected open brace for StrengthStrings"));
        return 0;
    }
    tok = lex->GetToken();
    lex->SetTokens(s_DiplomacyProposal_StrengthStrings_Tokens, k_Token_DiplomacyProposal_StrengthStrings_Max);
    while(!done) {
        tok = lex->GetToken();
        switch(tok) {
            case k_Token_DiplomacyProposal_StrengthStrings_DetailsWeak:
                m_flags0 |= k_StrengthStrings_DetailsWeak_Bit;
                if(!lex->GetStringIdAssignment(m_DetailsWeakValue)) {
                    DBERROR(("Expected string ID"));
                    done = true; break;
                }
                break;
            case k_Token_DiplomacyProposal_StrengthStrings_DetailsEven:
                m_flags0 |= k_StrengthStrings_DetailsEven_Bit;
                if(!lex->GetStringIdAssignment(m_DetailsEvenValue)) {
                    DBERROR(("Expected string ID"));
                    done = true; break;
                }
                break;
            case k_Token_DiplomacyProposal_StrengthStrings_DetailsStrong:
                m_flags0 |= k_StrengthStrings_DetailsStrong_Bit;
                if(!lex->GetStringIdAssignment(m_DetailsStrongValue)) {
                    DBERROR(("Expected string ID"));
                    done = true; break;
                }
                break;
            case k_Token_CloseBrace:
                done = true;
                result = 1;
                break;
            default:
                Assert(FALSE);
                done = true;
                break;
        }
    }
    lex->RestoreTokens();
    return result;
}

sint32 DiplomacyProposalRecord::StrengthStrings::ParseInArray(DBLexer *lex, StrengthStrings **array, sint32 *numElements)
{
    if(*numElements > 0) {
        StrengthStrings *oldArray = *array;
        *array = new StrengthStrings[(*numElements) + 1];
        for (int i=0; i < (*numElements); i++) 
             (*array)[i] = oldArray[i];
        delete [] oldArray;
    } else {
        *array = new StrengthStrings[1];
    }
    (*array)[*numElements].Parse(lex);
    *numElements += 1;
    return 1;
}
sint32 DiplomacyProposalRecord::StrengthStrings::ParseInArray(DBLexer *lex, StrengthStrings *array, sint32 *numElements, sint32 maxSize)
{
    if(*numElements >= maxSize) {
        return 0;
    }
    array[*numElements].Parse(lex);
    *numElements += 1;
    return 1;
}
void DiplomacyProposalRecord::StrengthStrings::ResolveDBReferences()
{
}
/*
 * Data Parsers
 * (DO NOT EDIT!  Automatically generated file)
 */
static char *s_DiplomacyProposal_Category_BitNames[] = {
    "Request",
    "Treaty",
    "Gift",
};
#define k_DiplomacyProposal_Category_Num_BitNames 3

sint32 DiplomacyProposalRecord::ParseCategoryBit(DBLexer *lex)
{
    sint32 bitindex;
    if(!lex->GetBitIndex((const char **)s_DiplomacyProposal_Category_BitNames, k_DiplomacyProposal_Category_Num_BitNames, bitindex)) {
        DBERROR(("%s is not a member of Category", lex->GetTokenText()));
        return 0;
    }
    m_Category |= (1 << bitindex);
    return 1;
}

static char *s_DiplomacyProposal_Arg1_BitNames[] = {
    "OwnCity",
    "HisCity",
    "OwnArmy",
    "HisArmy",
    "HisAgreement",
    "OwnAdvance",
    "HisAdvance",
    "OwnStopResearch",
    "HisStopResearch",
    "OwnUnitType",
    "OwnGood",
    "HisGood",
    "ThirdParty",
    "OwnGold",
    "HisGold",
    "OwnPollution",
    "HisPollution",
    "Percent",
};
#define k_DiplomacyProposal_Arg1_Num_BitNames 18

sint32 DiplomacyProposalRecord::ParseArg1Bit(DBLexer *lex)
{
    sint32 bitindex;
    if(!lex->GetBitIndex((const char **)s_DiplomacyProposal_Arg1_BitNames, k_DiplomacyProposal_Arg1_Num_BitNames, bitindex)) {
        DBERROR(("%s is not a member of Arg1", lex->GetTokenText()));
        return 0;
    }
    m_Arg1 |= (1 << bitindex);
    return 1;
}

static char *s_DiplomacyProposal_Class_BitNames[] = {
    "Request",
    "Treaty",
    "Offer",
};
#define k_DiplomacyProposal_Class_Num_BitNames 3

sint32 DiplomacyProposalRecord::ParseClassBit(DBLexer *lex)
{
    sint32 bitindex;
    if(!lex->GetBitIndex((const char **)s_DiplomacyProposal_Class_BitNames, k_DiplomacyProposal_Class_Num_BitNames, bitindex)) {
        DBERROR(("%s is not a member of Class", lex->GetTokenText()));
        return 0;
    }
    m_Class |= (1 << bitindex);
    return 1;
}

static char *s_DiplomacyProposal_Excludes_BitNames[] = {
    "Request",
    "Treaty",
    "Offer",
};
#define k_DiplomacyProposal_Excludes_Num_BitNames 3

sint32 DiplomacyProposalRecord::ParseExcludesBit(DBLexer *lex)
{
    sint32 bitindex;
    if(!lex->GetBitIndex((const char **)s_DiplomacyProposal_Excludes_BitNames, k_DiplomacyProposal_Excludes_Num_BitNames, bitindex)) {
        DBERROR(("%s is not a member of Excludes", lex->GetTokenText()));
        return 0;
    }
    m_Excludes |= (1 << bitindex);
    return 1;
}

void DiplomacyProposalRecord::Merge(const DiplomacyProposalRecord & rval){
	int index = 0, obj_index = 0;
	// only replace values that have been set
	if(rval.m_Category != 0x0) 
		m_Category = rval.m_Category; 

	// only replace values that have been set
	if(rval.m_Arg1 != 0x0) 
		m_Arg1 = rval.m_Arg1; 

	// only replace values that have been set
	if((rval.m_flags0 & k_DiplomacyProposal_Details0_Bit) != 0) 
		m_Details0Value = rval.m_Details0Value; 

	// only replace values that have been set
	if((rval.m_flags0 & k_DiplomacyProposal_Details1_Bit) != 0) 
		m_Details1Value = rval.m_Details1Value; 

	// only replace values that have been set
	if((rval.m_flags0 & k_DiplomacyProposal_Details2_Bit) != 0) 
		m_Details2Value = rval.m_Details2Value; 

	// only replace values that have been set
	if((rval.m_flags0 & k_DiplomacyProposal_Details3_Bit) != 0) 
		m_Details3Value = rval.m_Details3Value; 

	// only replace values that have been set
	if((rval.m_flags0 & k_DiplomacyProposal_Details4_Bit) != 0) 
		m_Details4Value = rval.m_Details4Value; 

	// only replace values that have been set
	if((rval.m_flags0 & k_DiplomacyProposal_DetailsEx0_Bit) != 0) 
		m_DetailsEx0Value = rval.m_DetailsEx0Value; 

	// only replace values that have been set
	if((rval.m_flags0 & k_DiplomacyProposal_DetailsEx1_Bit) != 0) 
		m_DetailsEx1Value = rval.m_DetailsEx1Value; 

	// only replace values that have been set
	if((rval.m_flags0 & k_DiplomacyProposal_DetailsEx2_Bit) != 0) 
		m_DetailsEx2Value = rval.m_DetailsEx2Value; 

	// only replace values that have been set
	if((rval.m_flags0 & k_DiplomacyProposal_DetailsEx3_Bit) != 0) 
		m_DetailsEx3Value = rval.m_DetailsEx3Value; 

	// only replace values that have been set
	if((rval.m_flags0 & k_DiplomacyProposal_DetailsEx4_Bit) != 0) 
		m_DetailsEx4Value = rval.m_DetailsEx4Value; 

	// only replace values that have been set
	if(rval.m_Class != 0x0) 
		m_Class = rval.m_Class; 

	// only replace values that have been set
	if(rval.m_Excludes != 0x0) 
		m_Excludes = rval.m_Excludes; 

	// only replace values that have been set
	if((rval.m_flags0 & k_DiplomacyProposal_Reciprocal_Bit) != 0) 
		m_ReciprocalValue = rval.m_ReciprocalValue; 

	// only replace values that have been set
	if((rval.m_flags0 & k_DiplomacyProposal_ImageSlot_Bit) != 0) 
		m_ImageSlotValue = rval.m_ImageSlotValue; 

} 

/*
 * Data accessors
 * (DO NOT EDIT!  Automatically generated file)
 */
const DiplomacyProposalRecord *DiplomacyProposalRecord::GetReciprocalPtr() const
{
    return g_theDiplomacyProposalDB->Get(m_ReciprocalValue);
}

