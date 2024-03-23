
#include "SDT.h"
/*
const char *topMenus[] = { "Bearing", "CW Options", "RF Set", "VFO Select",
                           "EEPROM", "AGC", "Spectrum Options",
                           "Noise Floor", "Mic Gain", "Mic Comp",
                           "EQ Rec Set", "EQ Xmt Set", "Calibrate" };

int (*functionPtr[])() = { &BearingMaps, &CWOptions, &RFOptions, &VFOSelect,
                           &EEPROMOptions, &AGCOptions, &SpectrumOptions,
                           &ButtonSetNoiseFloor, &MicGainSet, &MicOptions,
                           &EqualizerRecOptions, &EqualizerXmtOptions, &IQOptions

};
*/
/*****
  Purpose: void ShowMenu()

  Parameter list:
    char *menuItem          pointers to the menu
    int where               0 is a primary menu, 1 is a secondary menu

  Return value;
    void
*****/
void ShowMenu(const char *menu[], int where)
{
  tft.setFontScale( (enum RA8875tsize) 1);  
     NoActiveMenu();  // Does this erase only?
     
  if (where == PRIMARY_MENU) {                                              // Should print on left edge of top line
    tft.fillRect(PRIMARY_MENU_X, MENUS_Y, 300, CHAR_HEIGHT, RA8875_BLUE);   // Top-left of display
    tft.setCursor(5, 0);
    tft.setTextColor(RA8875_WHITE);
    tft.print(*menu);                                                       // Primary Menu
  } else {
    tft.fillRect(SECONDARY_MENU_X, MENUS_Y, 300, CHAR_HEIGHT, RA8875_GREEN);// Right of primary display
    tft.setCursor(35, 0);
    tft.setTextColor(RA8875_WHITE);
    tft.print(*menu);                                                       // Secondary Menu
  }
  return;
}
/*****
  Purpose: int WarningWindow(const char *msg)
    select, menu up + menu down

  Parameter list:
    char *msg               pointers to the message

  Return value:
    int                    -1 error, 0 exit, 1 continue
*****/
int WarningWindow(const char lines[]) {
  int16_t win_w = 600;
  int16_t win_h = 280;

  int16_t win_x = XPIXELS/2 - win_w/2;
  int16_t win_y = YPIXELS/2 - win_h/2;

  constexpr uint16_t background = RA8875_WHITE;
  tft.fillRect(win_x, win_y,win_w,win_h, background);

  tft.setFont(&FreeSans12pt7b);
  tft.setFontScale(0);
  uint8_t line_char_h=tft.getFontHeight();
  tft.setTextColor(RA8875_BLACK);

  {
    int16_t line_pos_y=win_y + 16;
    size_t lines_last_ind=strlen(lines)-1;
    size_t i =0;
    size_t line_cnt =0;
    String line;
    while (lines[i] != '\0' && line_cnt<7 ) {
      if (lines[i] == '\n' || i==lines_last_ind) {
        int16_t x1, y1;
        uint16_t w, h;
        tft.getTextBounds(line, win_x + win_w/2, line_pos_y, &x1, &y1, &w, &h);
        tft.setCursor(win_x + win_w/2 -w/2, line_pos_y);
        tft.print(line);
        line_pos_y+=line_char_h+line_char_h/2;
        line="";
        line_cnt++;
      } else {
        line+=lines[i];
      }
      i++;
    }
  }

  {
    int16_t guide_pos_y=win_y + 16 +(line_char_h+line_char_h/2)*8;
    constexpr char guide[] = "Use MenuUp, MenuDown and Select";
    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(guide, win_x + win_w/2, guide_pos_y, &x1, &y1, &w, &h);
    tft.setCursor(win_x + win_w/2 -w/2, guide_pos_y);
    tft.print(guide);
  }

  const char *options[] = { "Exit", "Continue"};
  constexpr int options_size = sizeof(options) / sizeof(char *);
  int option_selected = 0;

  bool w_continue = true;
  bool refresh = true;
  while (w_continue) {
    int val = ReadSelectedPushButton();
    delay(150L);
    if (val != -1 && val < (EEPROMData.switchValues[0] + WIGGLE_ROOM)) {
      val = ProcessButtonPress(val);
      if (val > -1) {
        switch (val) {
        case MENU_OPTION_SELECT:
          w_continue=false;
          break;
        case MAIN_MENU_UP:
          if (option_selected < (options_size-1)) {
            option_selected++;
            refresh=true;
          }
          break;
        case MAIN_MENU_DN:
              if (option_selected > 0) {
                option_selected--;
                refresh=true;
              }
          break;
        default:
          break;
        }
      }
    }
    if (refresh && option_selected >= 0 && option_selected < options_size) {
      tft.setFont(&FreeSans18pt7b);
      tft.setFontScale(0);

      int16_t char_h =26; // from getTextBounds
      int16_t option_off_y = win_h - (char_h+char_h/2);
      tft.setTextColor(RA8875_RED);
      int16_t c_x=win_x + win_w/2;
      int16_t c_y=win_y + option_off_y;
      int16_t x1, y1;
      uint16_t w, h;
      tft.getTextBounds(options[option_selected], c_x, c_y, &x1, &y1, &w, &h);
      tft.setCursor(c_x-w/2,c_y);
      tft.fillRect(win_x+100, y1-4 ,win_w-200,h+8, RA8875_LIGHT_GREY);
      tft.print(options[option_selected]);

      refresh = false;
    }
  }

  tft.fillRect(win_x, win_y,win_w,win_y, RA8875_BLACK);

  return option_selected;
}
