
#include <stdio.h>
#include "rb_vdp.h"
#include "rb_vtext.h"

int main(void) {
    // Function from toolchain
    volatile SYSVAR *sv; 
	sv = vdp_vdu_init(); 

    vdp_cls();
    vdp_clear_graphics();
    vdp_set_bgcolor(BLACK);
    vdp_set_fgcolor(BRIGHT_WHITE);

	for (int m = 0; m < 4; m++) {
		vdp_set_mode(m);
		vdp_set_logical_coords(false);

        // Function from toolchain
		vdp_get_scr_dims(true);   

		printf("\n\n");

		printf("Mode:        %d\n", m);
		printf("Screen size: %d x %d pixels\n", sv->scrWidth, sv->scrHeight);
		printf("Text size:   %d rows and %d columns\n", sv->scrRows, sv->scrCols);
		printf("Colors:      %d\n", sv->scrColours);

		int blocks = sv->scrCols / 10;
		for (int b = 0; b < blocks; b++ ) {
			printf(".........%1d", b+1);
		}

		printf("\n");
		for (int b = 0; b < blocks; b++ ) {
			printf("1234567890");
		}
		
        printf("\n");
		for (int r = 3; r <= sv->scrRows-10; r++)
            printf("%2d\n", r);

        for (int c = 0; c < sv->scrColours; c++) {
            vdp_set_plot_color(c);

    		vdp_plot_moveto(20+c*10, 80);
		    vdp_plot_lineto(20+c*10, sv->scrHeight-20);
        }

        // Vector text
        vdp_set_plot_color(BRIGHT_WHITE);
        vtext_draw_string(100, sv->scrHeight/2, "AGON LIGHT TEST", 3);

		printf("Press any key to continue...\n");
		getchar();
	}		

    vdp_cls();
    vdp_clear_graphics();

	return 0;
}

// Callback for vtext

void platform_draw_line(int x1, int y1, int x2, int y2) {
    vdp_plot_moveto(x1, y1);
    vdp_plot_lineto(x2, y2);     
}
