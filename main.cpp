#include <ncurses.h>
#include <string>
#include <thread>
#include <chrono>

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
      //   getch();            // Wait for a key
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

int main()
{
   { // Make scope for NcursesProgressBar
      int progBar_maxProgress = 64;
      NcursesProgressBar bar(progBar_maxProgress);

      for (int i = 0; i <= progBar_maxProgress; ++i)
      {
         bar.update(i, "Status Text - " + std::to_string(i));
         std::this_thread::sleep_for(std::chrono::milliseconds(250));
      }
   }

   std::this_thread::sleep_for(std::chrono::milliseconds(5000));

   return 0;   // Destructor handles cleanup
}