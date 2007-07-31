
/*
 * DO NOT EDIT THIS FILE!
 * It is generated automatically by ctpdb
 */
#ifndef __CitySize_RECORD_H__
#define __CitySize_RECORD_H__

/*
 * CitySize description
 */
#include "CTPRecord.h"
//
// Range definitions

/*
 * DO NOT EDIT THIS FILE!
 * It is generated automatically by ctpdb
 */

class CitySizeRecord : public CTPRecord
{
public:
    typedef sint32 (CitySizeRecord::*IntAccessor)() const;
    typedef bool (CitySizeRecord::*BoolAccessor)() const;
    typedef double (CitySizeRecord::*FloatAccessor)() const ;
    typedef bool (CitySizeRecord::*BitIntAccessor)(sint32 &val) const;
    typedef bool (CitySizeRecord::*BitFloatAccessor)(double &val) const;
private:
    sint32            m_Population;
    sint32            m_SquaredRadius;
    sint32            m_IntRadius;
    double            m_VisionRadius;
    sint32            m_MaxWorkers;
    sint32            m_GrowthRate;
    sint32            m_MinGrowthRate;
    sint32            m_MaxSurplusFood;
    sint32            m_BaseOvercrowding;
    sint32            m_BaseMaxPop;

public:
    CitySizeRecord() { Init(); };
    ~CitySizeRecord();
    Init();
    // These methods are needed for records to conform to
    // 'Orthodox Cannonical Form' and work with resizing STL vectors. 
    CitySizeRecord(const CitySizeRecord &rval) { Init(); *this = rval; }
    void operator=(const CitySizeRecord &rval);

    void CheckRequiredFields(DBLexer *lex);
    sint32 Parse(DBLexer *lex);

    void ResolveDBReferences();
    void Merge(const CitySizeRecord & rval);
    //
    // Accessors
    //
    sint32           GetPopulation() const { return m_Population; }
    sint32           GetSquaredRadius() const { return m_SquaredRadius; }
    sint32           GetIntRadius() const { return m_IntRadius; }
    double           GetVisionRadius() const { return m_VisionRadius; }
    sint32           GetMaxWorkers() const { return m_MaxWorkers; }
    sint32           GetGrowthRate() const { return m_GrowthRate; }
    sint32           GetMinGrowthRate() const { return m_MinGrowthRate; }
    sint32           GetMaxSurplusFood() const { return m_MaxSurplusFood; }
    sint32           GetBaseOvercrowding() const { return m_BaseOvercrowding; }
    sint32           GetBaseMaxPop() const { return m_BaseMaxPop; }
}; /* CitySizeRecord */

struct CitySizeRecordAccessorInfo {
    CitySizeRecord::IntAccessor m_intAccessor;
    CitySizeRecord::BoolAccessor m_boolAccessor;
    CitySizeRecord::FloatAccessor m_floatAccessor;
    CitySizeRecord::BitIntAccessor m_bitIntAccessor;
    CitySizeRecord::BitFloatAccessor m_bitFloatAccessor;
};
extern CitySizeRecordAccessorInfo g_CitySizeRecord_Accessors[];
extern char *g_CitySize_Tokens[];
#define k_Num_CitySizeRecord_Tokens 10

template <class T> class CTPDatabase;
extern CTPDatabase<CitySizeRecord> *g_theCitySizeDB;

#endif /* ifndef __CitySize_RECORD_H__ */
