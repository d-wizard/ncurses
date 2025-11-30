/* Copyright 2025 Dan Williams. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#pragma once
#include <ncurses.h>
#include <string>


class NcursesProgressBar
{
public:
   NcursesProgressBar(int maxProgress)
      : maxProgress(maxProgress)
   {
      initscr();          // Start ncurses
      curs_set(0);        // Hide cursor
      noecho();
      progress = 0;
   }

   ~NcursesProgressBar()
   {
      endwin();           // End ncurses
   }

   void update(int value, const std::string& statusText = "")
   {
      // Clamp input
      if (value < 0) value = 0;
      if (value > maxProgress) value = maxProgress;
      progress = value;

      // -----------------------------------------
      // Draw status text line
      // -----------------------------------------
      int clearWidth = maxProgress + STATUS_EXTRA_CLEAR_WIDTH;

      // Clear status line
      mvprintw(STATUS_ROW, 0, "%s", std::string(clearWidth, ' ').c_str());

      // Print status text
      mvprintw(STATUS_ROW, 0, "%s", statusText.c_str());

      // -----------------------------------------
      // Draw progress bar line
      // -----------------------------------------
      int barClearWidth = maxProgress + PROGRESS_EXTRA_CLEAR_WIDTH;

      // Clear bar line
      mvprintw(BAR_ROW, 0, "%s", std::string(barClearWidth, ' ').c_str());

      // Draw the filled portion
      for (int j = 0; j < progress; ++j)
      {
         mvaddch(BAR_ROW, j, FILL_CHAR);
      }

      // Percentage label
      int percentColumn = maxProgress + PERCENT_COLUMN_OFFSET;
      mvprintw(BAR_ROW, percentColumn, "%3d%%", (progress * 100) / maxProgress);

      refresh();
   }

private:
   // ---- Layout constants ----
   static constexpr int STATUS_ROW = 0;
   static constexpr int BAR_ROW = 1;

   // Extra width added to clearing range
   static constexpr int STATUS_EXTRA_CLEAR_WIDTH = 20;
   static constexpr int PROGRESS_EXTRA_CLEAR_WIDTH = 7;

   // Position offset for percentage text after the bar
   static constexpr int PERCENT_COLUMN_OFFSET = 2;

   // Progress bar character
   static constexpr char FILL_CHAR = '#';

private:
   int maxProgress;
   int progress;
};
