#include <minix/drivers.h>
#include <sys/video.h>
#include <sys/mman.h>

#include <assert.h>

#include "vt_info.h"

#include "video_txt.h"

/* Private global variables */

static char *video_mem;		/* Address to which VRAM is mapped */

static unsigned scr_width;	/* Width of screen in columns */
static unsigned scr_lines;	/* Height of screen in lines */


void vt_fill(char ch, char attr)
{
  char *vptr;
  
  vptr=video_mem;  //vptr is pointing to the first pixel of the screen
  
  int i;
 for (i=0; i<(scr_width*scr_lines);i++)   // scr_width*scr_lines represents all the pixels
 {
	  *vptr=ch;    //modify the first byte of the pixel
	  vptr++;
	  *vptr=attr;   //modify the second byte of the pixel
	  vptr++;
 }
}

void vt_blank()
{
	vt_fill(0x00, 0x00);     //Fills the screen with blank
}

int vt_print_char(char ch, char attr, int r, int c)
{
	 char *vptr;

	 vptr=video_mem;     //vptr is pointing to the first pixel of the screen

	 vptr= 2*c+vptr;           //goes to the right column
	 vptr= 2*r*scr_width+vptr;   //goes to the right line, now is in the right position

	 *vptr=ch;      //lets change the char
	 vptr++;
	 *vptr=attr;    //and the color of the char and the color of the background
	 vptr++;

	  return 1;
}

int vt_print_string(char *str, char attr, int r, int c) {

  /* To complete ... */

}

int vt_print_int(int num, char attr, int r, int c) {

  /* To complete ... */

}


int vt_draw_frame(int width, int height, char attr, int r, int c) {

  /* To complete ... */

}

/*
 * THIS FUNCTION IS FINALIZED, do NOT touch it
 */

char *vt_init(vt_info_t *vi_p) {

  int r;
  struct mem_range mr;

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes)(vi_p->vram_base);
  mr.mr_limit = mr.mr_base + vi_p->vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	  panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vi_p->vram_size);

  if(video_mem == MAP_FAILED)
	  panic("video_txt couldn't map video memory");

  /* Save text mode resolution */

  scr_lines = vi_p->scr_lines;
  scr_width = vi_p->scr_width;

  return video_mem;
}
