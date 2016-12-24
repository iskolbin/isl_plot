#ifndef ISL_INCLUDE_ISL_PLOT_H_
#define ISL_INCLUDE_ISL_PLOT_H_

#include <stddef.h>

#ifdef ISLP_STATIC
	#define ISLP_DEF static
#else
	#define ISLP_DEF extern
#endif

#ifndef ISLP_COORD
typedef int islp_coord;
#else
typedef ISLP_COORD islp_coord;
#endif

#ifndef ISLP_VALUE
typedef float islp_value;
#else
typedef ISLP_VALUE islp_value;
#endif

#ifndef ISLP_USERDATA
typedef void * islp_userdata;
#else
typedef ISLP_USERDATA islp_userdata;
#endif

#if !defined(ISLP_MALLOC)&&!defined(ISLP_REALLOC)&&!defined(ISLP_FREE)
	#include <stdlib.h>
	#define ISLP_MALLOC(n) malloc(n)
	#define ISLP_REALLOC(p,n) realloc((p),(n))
	#define ISLP_FREE(p) free(p)
#elif !defined(ISLP_MALLOC)||!defined(ISLP_REALLOC)||!defined(ISLP_FREE)
	#error "You must to define ISLP_MALLOC, ISLP_REALLOC, ISLP_FREE to remove stdlib dependency"
#endif

typedef int (*islp_plot)( islp_coord x, islp_coord y, islp_value v, islp_userdata data );

#ifdef __cplusplus
extern "C" {
#endif

ISLP_DEF void islp_rect( islp_plot f, islp_coord x, islp_coord y, islp_coord w, islp_coord h, islp_userdata data );
ISLP_DEF void islp_line( islp_plot f, islp_coord x0, islp_coord y0, islp_coord x1, islp_coord y1, islp_userdata data );
ISLP_DEF void islp_circle( islp_plot f, islp_coord x, islp_coord y, islp_coord r, islp_userdata data );

#ifdef __cplusplus
}
#endif

#define islp_each(f,data) (islp_rect((f)0,0,(grid)->width,(grid)->height,(data)))
#define islp_point(f,x,y,data) (islp_rect((f),(x),(y),1,1,(data)))

#endif // ISL_INCLUDE_ISL_PLOT_H_

#ifdef ISL_PLOT_IMPLEMENTATION

void islp_rect( islp_plot f, islp_coord x, islp_coord y, islp_coord w, islp_coord h, islp_userdata data ) {
	islp_coord x0 = x;
	for ( ; y < h; y++ ) {
		for ( x = x0; x < h; x++ ) {
			if ( f( x, y, 1.0, data )) {
				return;
			}
		}
	}
}

#define ISLP_ABS(x) ((x)>0?(x):-(x))

void islp_line( islp_plot f, islp_coord x0, islp_coord y0, islp_coord x1, islp_coord y1, islp_userdata data ) {
	islp_coord dx = ISLP_ABS( x0 - x1 );
	islp_coord dy = ISLP_ABS( y0 - y1 );
	islp_coord sx = x0 < x1 ? 1 : -1;
	islp_coord sy = y0 < y1 ? 1 : -1;
	islp_coord err = dx - dy;

	while ( !f( x0, y0, 1.0, data ) && !( x0 == x1 && y0 == y1 )) {
		islp_coord e2 = 2*err;
		if ( e2 > -dy ) {
			err = err - dy;
			x0 += sx;
		}
		if ( e2 < dx ) {
			err = err + dx;
			y0 += sy;
		}
	}
}

void islp_circle( islp_plot f, islp_coord x0, islp_coord y0, islp_coord r, islp_userdata data ) {
	islp_coord x = -r, y = 0, err = 2 - 2*r;
	do {
		if ( f( x0 - x, y0 + y, 1.0, data ) ||
			f( x0 - y, y0 - x, 1.0, data ) ||
			f( x0 + x, y0 - y, 1.0, data ) ||
			f( x0 + y, y0 + x, 1.0, data )) {
			break;
		}
		r = err;
		if ( r < y ) {
			y++;
			err = err + 2*y + 1; 
		}
		if ( r > x || err > y ) {
			x++;
			err = err + 2*x + 1;
		}
	} while ( x <= 0 );
}	

#endif // ISL_PLOT_IMPLEMENTATION
