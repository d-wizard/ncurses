#include <iostream>
#include <ncurses.h>
#include <thread> // For usleep

int main()
{
   initscr(); // Initialize ncurses
   curs_set(0); // Hide the cursor

   int max_progress = 50;
   for (int i = 0; i <= max_progress; ++i)
   {
      // Clear the line for redraw
      mvprintw(0, 0, "%s", std::string(max_progress + 7, ' ').c_str());

      // Draw the filled part of the bar
      for (int j = 0; j < i; ++j)
      {
         mvaddch(0, j, '#');
      }

      // Display percentage
      mvprintw(0, max_progress + 2, "%3d%%", (i * 100) / max_progress);

      refresh(); // Update the screen
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
   }

   getch(); // Wait for a key press before exiting
   endwin(); // End ncurses

   return 0;
}