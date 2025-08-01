#include "SDT.h"

extern unsigned long transmitDitLength; // JJP 8/19/23
extern unsigned long transmitDitUnshapedBlocks;
extern unsigned long transmitDahUnshapedBlocks;

void SelectCWFilter();        // AFP 10-18-22
void SelectCWOffset();        // KF5N December 13, 2023
void DoCWReceiveProcessing(); // AFP 09-19-22
void SetDitLength(int wpm);
void SetTransmitDitLength(int wpm); // JJP 8/19/23
void SetKeyType();
void SetKeyPowerUp();
void SetSideToneVolume(bool speaker); // This function uses encoder to set sidetone volume.  KF5N August 29, 2023
void MorseCharacterDisplay(char currentLetter);
void ResetHistograms();
void DoCWDecoding(int audioValue);
void DoGapHistogram(long val);
void DoSignalHistogram(long val);
