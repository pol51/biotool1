#ifndef __VCIL_H__
#define __VCIL_H__

#include "cellItem.h"

class VCil : public CellItem
{
  public:
    VCil() : CellItem() {}
    VCil(const CellItem& cellItem) : CellItem(cellItem) {}
    virtual ~VCil() { }

};

#endif
