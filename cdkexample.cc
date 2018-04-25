/*
 * Usage of CDK Matrix
 *
 * File:   example1.cc
 * Author: Stephen Perkins
 * Email:  stephen.perkins@utdallas.edu
 */

#include <iostream>
#include "cdk.h"
#include <stdint.h>
#include <fstream>
#include <stdio.h>


#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 25
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;

class BinaryFileHeader
{

 public:

  uint32_t magicNumber;
  uint32_t versionNumber;
  uint64_t numRecords;
};

/* Should be 0xFEEDFACE */


/*
 * Records in the file have a fixed length buffer * that will hold a C-Style string. This is the * size of the fixed length buffer.
 */

const int maxRecordStringLength = 25;

class BinaryFileRecord
 { public:
   uint8_t strLength;
   char stringBuffer[maxRecordStringLength];
 };


int main()
{

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  // Remember that matrix starts out at 1,1.
  // Since arrays start out at 0, the first entries
  // below ("R0", and "C0") are just placeholders
  // 
  // Finally... make sure your arrays have enough entries given the
  // values you choose to set for MATRIX_WIDTH and MATRIX_HEIGHT
  // above.

  const char 		*rowTitles[] = {"R0", "a", "b", "c", "d", "e", "f"};
  const char 		*columnTitles[] = {"C0","a", "b", "c", "d", "e", "f"};
  int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  char buffr[1024];


  BinaryFileHeader *headerRecord = new BinaryFileHeader();
  ifstream binFileInput ("cs3377.bin", ios::in | ios::binary); 

  binFileInput.read((char *) headerRecord, sizeof(BinaryFileHeader));  
   
  sprintf(buffr, "Magic: 0x%X", headerRecord->magicNumber);
  setCDKMatrixCell(myMatrix, 1, 1, buffr); 

  sprintf(buffr, "Version: %d", headerRecord->versionNumber);
  setCDKMatrixCell(myMatrix, 1, 2, buffr);
  
  sprintf(buffr, "NumRecords: %lu", headerRecord->numRecords);
  setCDKMatrixCell(myMatrix, 1, 3, buffr);

   
  BinaryFileRecord *fileRecord = new BinaryFileRecord();
  
  int xx  = 2;
 
  for(int i = 0; i < 4; i++)
    {
      binFileInput.read((char *) fileRecord, sizeof(BinaryFileRecord));
      sprintf(buffr, "strlen: %lu", strlen(fileRecord->stringBuffer));
      setCDKMatrixCell(myMatrix, xx, 1, buffr);

      sprintf(buffr, "%s", fileRecord->stringBuffer);
      setCDKMatrixCell(myMatrix, xx, 2, buffr); 
      xx++;
    }
  


  binFileInput.close();

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);


  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;

  // Cleanup screen
  endCDK();
}
