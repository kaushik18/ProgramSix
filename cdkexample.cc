/*
 * Kaushik Nadimpalli
 * kxn160430
 * Program 6
 * kxn160430@utdallas.edu
 */


/* Most of the methods for this program are in this .cc file. 
   This includes the main method, which is where we read and outputted
   the binary file records and put them in the CDK Matrix. */

//Necessary directories we include for the functions/processes we use in our program
#include <iostream>
#include "cdk.h"
#include <stdint.h>
#include <fstream>
#include <stdio.h>


//We define these hear. One can define these in header file too
#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 25
#define MATRIX_NAME_STRING "Binary File Contents"
//The matrix is called Binary File Contents and it is a 3x5 matrix
//If there are more than 4 records, it only prints the first 4
//If there are less than 4 records, it prints however  many records there might exit.


using namespace std;

//Class for Binary Header Record(let us assume there is one header)
//This class is used later on to read in our Header Record later in main.
class BinaryFileHeader
{

 public:

  uint32_t magicNumber;
  uint32_t versionNumber;
  uint64_t numRecords;
};
/* Should be 0xFEEDFACE */



// Records in the file have a fixed length buffer * that will hold a C-Style string. This is the * size of the fixed length buffer.
const int maxRecordStringLength = 25;

//Class for Binary File Records
//Class is used later when we are to read in and output the file records from binary file into the matrix.
class BinaryFileRecord
 { public:
   uint8_t strLength;
   char stringBuffer[maxRecordStringLength];
 };


// In this file we do most of the necessary and pivotal operation required by this program
// We create the CDK Matrix
// We read in the Header record
// We put the header record into first row of matrix
// We read in the other file records
// We put those other file records into each subsequent row/columns in the matrix
// We close the binary file
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

  //This ensures that our matrix is not empty, other we exit gracefully.
  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  char buffr[1024];
  // Let's set our buffer to a suitable large size so we don't run into any compatability issues.

  // Here, create a variable to that will read the header record from the binary file
  BinaryFileHeader *headerRecord = new BinaryFileHeader();
  ifstream binFileInput ("cs3377.bin", ios::in | ios::binary); 

  // Reading the header record from the binary file
  binFileInput.read((char *) headerRecord, sizeof(BinaryFileHeader));  
   
  // Printing row 1, column 1 to matrix
  sprintf(buffr, "Magic: 0x%X", headerRecord->magicNumber);
  setCDKMatrixCell(myMatrix, 1, 1, buffr); 

  // Printing row 1, column 2 to matrix
  sprintf(buffr, "Version: %d", headerRecord->versionNumber);
  setCDKMatrixCell(myMatrix, 1, 2, buffr);
  
  // Printing row 1, column 3 to matrix
  sprintf(buffr, "NumRecords: %lu", headerRecord->numRecords);
  setCDKMatrixCell(myMatrix, 1, 3, buffr);

  // Here, we create a variable that will read file records from binary file
  BinaryFileRecord *fileRecord = new BinaryFileRecord();
  
  int xx  = 2;
  // increment it everytime iterator adds +1

  // The for loop will test to ensure that the first 4 records will print no matter what.
  // However, assuming there's less than 4 records it will only print however many records exit in binary file.
  // If there are more than 4 file records, it still only prints up to first 4 and 5 total records including header.
 
  for(int i = 0; i < 4; i++)
    {
      binFileInput.read((char *) fileRecord, sizeof(BinaryFileRecord));
      sprintf(buffr, "strlen: %lu", strlen(fileRecord->stringBuffer));
      setCDKMatrixCell(myMatrix, xx, 1, buffr);

      sprintf(buffr, "%s", fileRecord->stringBuffer);
      setCDKMatrixCell(myMatrix, xx, 2, buffr); 
      xx++;
    }
  

  // We must ensure to close our binary file reading.
  binFileInput.close();

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);


  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;

  // Cleanup screen
  endCDK();
}
