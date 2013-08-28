#ifndef CUnitsManager_H
#define CUnitsManager_H

#include "SUnit.h"

#include <stdlib.h>
#include <time.h>

class CUnitsManager
{
public:
    CUnitsManager();
    virtual ~CUnitsManager();

    void addUnit(SUnit unit);
    void remUnit(u32   ID);

    u32     getUnitsCount();
    SUnit*  getUnitsList();
    SUnit*  getNextUnit();
    void    shake_units();

    SUnit* get_unit_by_id(u32 id);
    SUnit* get_unit_by_pos(s32 x,s32 y);

    void update();

    u32 request_new_unit_id();
protected:
private:
    //Units data
    u32 last_unit_id;
    u32 last_get_unit;
    std::vector<SUnit> Units;
};

#endif // CUnitsManager_H
