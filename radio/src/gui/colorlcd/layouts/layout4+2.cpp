/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "opentx.h"
#include "sliders.h"

#define HAS_TOPBAR()      (persistentData->options[0].value.boolValue == true)
#define HAS_FM()          (persistentData->options[1].value.boolValue == true)
#define HAS_SLIDERS()     (persistentData->options[2].value.boolValue == true)
#define HAS_TRIMS()       (persistentData->options[3].value.boolValue == true)
#define IS_MIRRORED()     (persistentData->options[4].value.boolValue == true)

const uint8_t LBM_LAYOUT_4P2[] = {
#include "mask_layout4+2.lbm"
};

const ZoneOption OPTIONS_LAYOUT_4P2[] = {
  { STR_TOP_BAR, ZoneOption::Bool },
  { STR_FLIGHT_MODE, ZoneOption::Bool },
  { STR_SLIDERS, ZoneOption::Bool },
  { STR_TRIMS, ZoneOption::Bool },
  { STR_MIRROR, ZoneOption::Bool },
  { NULL, ZoneOption::Bool }
};

constexpr coord_t HMARGIN = 5;
constexpr coord_t HTRIM_W = 200;
constexpr coord_t HTRIM_H = 30;
constexpr coord_t HSLIDER_W = 180;
constexpr coord_t HSLIDER_H = 20;
constexpr coord_t VSLIDER_W = 25;
constexpr coord_t VSLIDER_H = 201;
constexpr coord_t MULTIPOS_H = 20;
constexpr coord_t MULTIPOS_W = 60;

class Layout4P2: public Layout
{
  public:
    Layout4P2(const LayoutFactory * factory, Layout::PersistentData * persistentData):
      Layout(factory, persistentData)
    {
      decorate();
    }

    void create() override
    {
      Layout::create();
      persistentData->options[0].value.boolValue = true;
      persistentData->options[1].value.boolValue = true;
      persistentData->options[2].value.boolValue = true;
      persistentData->options[3].value.boolValue = true;
      persistentData->options[4].value.boolValue = false;
      persistentData->options[5].value.boolValue = false;

      decorate();
    }

    unsigned int getZonesCount() const override
    {
      return 6;
    }

    void decorate()
    {
      if (HAS_SLIDERS()) {
        new MainViewHorizontalSlider(this, {HMARGIN, LCD_H - HSLIDER_H, HSLIDER_W + 1, HSLIDER_H},
                                     [=] { return calibratedAnalogs[CALIBRATED_POT1]; },OPTION_SLIDER_TICKS | OPTION_SLIDER_BIG_TICKS | OPTION_SLIDER_SQUARE_BUTTON);

        if (IS_POT_MULTIPOS(POT2)) {
          new MainView6POS(this, {LCD_W / 2 - MULTIPOS_W / 2, LCD_H - MULTIPOS_H, MULTIPOS_W + 1, MULTIPOS_H},
                                       [=] { return 1 + (potsPos[1] & 0x0f); },OPTION_SLIDER_TICKS | OPTION_SLIDER_BIG_TICKS | OPTION_SLIDER_SQUARE_BUTTON);
        }

        new MainViewHorizontalSlider(this, {LCD_W - HSLIDER_W - HMARGIN, LCD_H - HSLIDER_H, HSLIDER_W + 1, HSLIDER_H},
                                     [=] { return calibratedAnalogs[CALIBRATED_POT3]; }, OPTION_SLIDER_TICKS | OPTION_SLIDER_BIG_TICKS | OPTION_SLIDER_SQUARE_BUTTON);

        new MainViewVerticalSlider(this, {HMARGIN, LCD_H /2 - VSLIDER_H / 2, VSLIDER_W + 1, VSLIDER_H},
                                   [=] { return calibratedAnalogs[CALIBRATED_SLIDER_REAR_LEFT]; },OPTION_SLIDER_TICKS | OPTION_SLIDER_BIG_TICKS | OPTION_SLIDER_SQUARE_BUTTON);

        new MainViewVerticalSlider(this, {LCD_W - VSLIDER_W, LCD_H /2 - VSLIDER_H / 2, VSLIDER_W + 1, VSLIDER_H},
                                   [=] { return calibratedAnalogs[CALIBRATED_SLIDER_REAR_RIGHT]; },OPTION_SLIDER_TICKS | OPTION_SLIDER_BIG_TICKS | OPTION_SLIDER_SQUARE_BUTTON);
      }

      if (HAS_FM()) {
    /*    new DynamicText(this, LCD_W / 2 - getTextWidth(g_model.flightModeData[mixerCurrentFlightMode].name,
                                                  sizeof(g_model.flightModeData[mixerCurrentFlightMode].name),
                                                  ZCHAR | SMLSIZE) / 2,
                         232,
                         g_model.flightModeData[mixerCurrentFlightMode].name,
                         sizeof(g_model.flightModeData[mixerCurrentFlightMode].name), ZCHAR | SMLSIZE);*/
      }
    }

    rect_t getZone(unsigned int index) const override
    {
      coord_t areaw = LCD_W - (HAS_SLIDERS() ? 55 : 8) - (HAS_TRIMS() ? 55 : 8);
      coord_t areah = LCD_H - 4 - (HAS_TOPBAR() ? 55 : 8) - (HAS_SLIDERS() ? HSLIDER_H + 4 : 4) - (HAS_TRIMS() ? HTRIM_H + 4 : 4);

      rect_t zone;
      zone.x = IS_MIRRORED() ? ((index >= 4) ? (LCD_W - areaw) / 2 - 4 : 245) : ((index >= 4) ? 245 : (LCD_W - areaw) / 2 - 4);
      zone.h = (index >= 4) ?  (areah / 2) :  (areah / 4) - 2;
      zone.y = (index >= 4) ? (HAS_TOPBAR() ? 52 : 6) + (index == 5 ? zone.h + ((HAS_TRIMS() + HAS_SLIDERS() == 1) ? 8 : 10): 0): (HAS_TOPBAR() ? 52 : 6) + (index % 4) * (zone.h + 6);
      zone.w = areaw / 2;

      return zone;
    }

    //virtual void refresh();
};

//void Layout4P2::refresh()
//{
  //theme->drawBackground();

  //if (HAS_TOPBAR()) {
  //  drawTopBar();
  //}
  //
  //if (HAS_FM()) {
  //  // Flight mode
  //  lcdDrawSizedText(LCD_W / 2 - getTextWidth(g_model.flightModeData[mixerCurrentFlightMode].name, sizeof(g_model.flightModeData[mixerCurrentFlightMode].name),
  //                                            ZCHAR | SMLSIZE) / 2, 232, g_model.flightModeData[mixerCurrentFlightMode].name,
  //                   sizeof(g_model.flightModeData[mixerCurrentFlightMode].name), ZCHAR | SMLSIZE);
  //}
  //
  //if (HAS_SLIDERS()) {
  //  // Pots and rear sliders positions
  //  drawMainPots();
  //}
  //
  //if (HAS_TRIMS()) {
  //  // Trims
  //  drawTrims(mixerCurrentFlightMode, HAS_SLIDERS());
  //}

  //Layout::refresh();
//}

BaseLayoutFactory<Layout4P2> layout4P2("Layout4P2", "4 + 2", LBM_LAYOUT_4P2, OPTIONS_LAYOUT_4P2);