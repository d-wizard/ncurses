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
      if (value < 0) value = 0;
      if (value > maxProgress) value = maxProgress;
      progress = value;

        // ----- Draw status line -----
        // Clear previous status text by overwriting with spaces
        int clearWidth = maxProgress + 20;
        mvprintw(0, 0, "%s", std::string(clearWidth, ' ').c_str());
        mvprintw(0, 0, "%s", statusText.c_str());

      // ----- Draw progress bar -----
      // Clear progress bar line
      mvprintw(1, 0, "%s", std::string(maxProgress + 7, ' ').c_str());

      // Draw the filled portion
      for (int j = 0; j < progress; ++j)
      {
         mvaddch(1, j, '#');
      }

      // Percentage display
      mvprintw(1, maxProgress + 2, "%3d%%", (progress * 100) / maxProgress);

      refresh();
   }

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