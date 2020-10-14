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

#pragma once

#include "libopenui.h"

class MainViewTrim : public Window
{
  public:
    MainViewTrim(Window * parent, const rect_t & rect, std::function<int16_t()> getValue):
      Window(parent, rect),
      getValue(std::move(getValue))
    {
    }

    void checkEvents() override
    {
      Window::checkEvents();
      int8_t newValue = getValue();
      if (value != newValue) {
        value = newValue;
        invalidate();
      }
    }

  protected:
    std::function<int16_t()> getValue;
    int8_t value = 0;
};

class MainViewHorizontalTrim : public MainViewTrim
{
  public:
    using MainViewTrim::MainViewTrim;

    void paint(BitmapBuffer * dc) override
    {
      drawHorizontalSlider(dc, 0, 0, width(), getValue(), -RESX, RESX, 0, OPTION_SLIDER_EMPTY_BAR|OPTION_SLIDER_TRIM_BUTTON);
    }
};

class MainViewVerticalTrim : public MainViewTrim
{
  public:
    using MainViewTrim::MainViewTrim;

    void paint(BitmapBuffer * dc) override
    {
      if (g_model.extendedTrims == 1) {
        drawVerticalSlider(dc, 0, 0, height(), getValue(), TRIM_EXTENDED_MIN, TRIM_EXTENDED_MAX, 0, OPTION_SLIDER_EMPTY_BAR | OPTION_SLIDER_TRIM_BUTTON);
      }
      else {
        drawVerticalSlider(dc, 0, 0, height(), getValue(), TRIM_MIN, TRIM_MAX, 0, OPTION_SLIDER_EMPTY_BAR | OPTION_SLIDER_TRIM_BUTTON);
      }
    }
};