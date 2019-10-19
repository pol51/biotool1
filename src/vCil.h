#pragma once

#include "cellItem.h"

class Settings;
class SettingsView;

class VCil : public CellItem
{
  public:
    VCil() : CellItem() {}
    VCil(const CellItem& cellItem) : CellItem(cellItem) {}

    bool averageArrow() const override { return _showAverageArrow; }

  protected:
    friend class Settings;
    friend class SettingsView;
    static bool _showAverageArrow;
};
