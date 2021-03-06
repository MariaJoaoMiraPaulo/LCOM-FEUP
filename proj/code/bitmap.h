
#ifndef __BITMAP__H
#define __BITMAP__H

#include <minix/syslib.h>

//typedef enum {
//	ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT
//} Alignment;

typedef struct {
	unsigned short type; // specifies the file type
	unsigned int size; // specifies the size in bytes of the bitmap file
	unsigned int reserved; // reserved; must be 0
	unsigned int offset; // specifies the offset in bytes from the bitmapfileheader to the bitmap bits
} Bitmap_File_Header;

typedef struct {
	unsigned int size; // specifies the number of bytes required by the struct
	int width; // specifies width in pixels
	int height; // specifies height in pixels
	unsigned short planes; // specifies the number of color planes, must be 1
	unsigned short bits; // specifies the number of bit per pixel
	unsigned int compression; // specifies the type of compression
	unsigned int imageSize; // size of image in bytes
	int xResolution; // number of pixels per meter in x axis
	int yResolution; // number of pixels per meter in y axis
	unsigned int nColors; // number of colors used by the bitmap
	unsigned int importantColors; // number of colors that are important
} BitmapInfoHeader;

/// Represents a Bitmap
typedef struct {
	BitmapInfoHeader bitmapInfoHeader;
	unsigned char* bitmapData;
} Bitmap;

/**
 * @brief Loads a bmp image
 *
 * @param filename Path of the image to load
 * @return Non NULL pointer to the image buffer
 */
Bitmap* loadBitmap(const char* filename);

/**
 * @brief Draws an unscaled, unrotated bitmap at the given position
 *
 * @param bitmap bitmap to be drawn
 * @param x destiny x coord
 * @param y destiny y coord
 * @param alignment image alignment
 */
void drawBitmap(Bitmap* bitmap, int x, int y); //, Alignment alignment

/**
 * @brief Destroys the given bitmap, freeing all resources used by it.
 *
 * @param bitmap bitmap to be destroyed
 */
void deleteBitmap(Bitmap* bmp);
/**
 * @brief Draws images without background
 *
 * @param bitmap bitmap to be drawn
 * @param x destiny x coord
 * @param y destiny y coord
 */

void drawBitmapWithoutBackground (Bitmap* bmp, int x, int y);
/**
 * @brief Draws images without background
 *
 * @param bitmap bitmap to be drawn
 * @param x destiny x coord
 * @param y destiny y coord
 * @param number to be draw
 */
void drawNumbers(Bitmap* bmp,int x,int y, char number);
/**
 * @brief writes distance of copter
 *
 * @param bitmap bitmap to be drawn
 * @param distance distance of the opter
 */
void draw_distance(int distance,Bitmap* bmp);

/**
 * @brief draws real time on the game
 *
 * @param char* time array of char with real time
 */
void drawTime(char* time);
/**
 * @brief load clock image
 */
void loadClock();
/**
 * @brief delete clock image
 */
void deleteClock();

/*
 * @brief draw copter image
 *
 * @param bmp pointer to copter image
 * @param x x coordinate left corner of copter image
 * @param y y coordinate left corner of copter image
 */
void drawCopters(Bitmap* bmp,int x,int y, char number);

/**
 * @brief draws best score
 */
void drawBest(int distance, Bitmap* bmp);

#endif /* __BITMAP_H */
