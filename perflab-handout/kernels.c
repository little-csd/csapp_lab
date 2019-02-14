/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "Little_csd",              /* Team name */

    "Cai",     /* First member full name */
    "Sidi",  /* First member email address */

    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j, k, l;
    int div = 32;
    for (i = 0; i < dim; i += div)
    for (j = 0; j < dim; j += div) {
        for(k = i; k < i + div; k++) {
            int _dst = RIDX(k,j,dim);
            int _src = RIDX(j, dim - 1 - k, dim);
            for(l = j; l < j + div; l++) {
                dst[_dst] = src[_src];
                _dst++;
                _src+=dim;
            }
        }
    }
//     for (k = i; k < i + div; k++)
//     for (l = j; l < j + div; l++)
//         dst[RIDX(k, l, dim)] = src[RIDX(l, dim - 1 - k, dim)];
}
/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and repo4rt the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);   
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
inline void _sml(int x, int y, pixel* src, pixel* dst, int dim)
{
    int pos = RIDX(x,y,dim);
    dst[pos].red = (src[pos].red + src[pos+1].red + src[pos-dim].red 
        + src[pos+dim].red + src[pos+1+dim].red + src[pos+1-dim].red) / 6;
    dst[pos].green = (src[pos].green + src[pos+1].green + src[pos-dim].green 
        + src[pos+dim].green + src[pos+1+dim].green + src[pos+1-dim].green) / 6;
    dst[pos].blue = (src[pos].blue + src[pos+1].blue + src[pos-dim].blue 
        + src[pos+dim].blue + src[pos+1+dim].blue + src[pos+1-dim].blue) / 6;
}

inline void _smr(int x, int y, pixel* src, pixel* dst, int dim)
{
    int pos = RIDX(x,y,dim);
    dst[pos].red = (src[pos].red  + src[pos-1].red + src[pos+dim].red 
        + src[pos+dim-1].red + src[pos-dim].red + src[pos-dim-1].red) / 6;
    dst[pos].green = (src[pos].green  + src[pos-1].green + src[pos+dim].green 
        + src[pos+dim-1].green + src[pos-dim].green + src[pos-dim-1].green) / 6;
    dst[pos].blue = (src[pos].blue  + src[pos-1].blue + src[pos+dim].blue 
        + src[pos+dim-1].blue + src[pos-dim].blue + src[pos-dim-1].blue) / 6;
}

inline void _smt(int x, int y, pixel* src, pixel* dst, int dim)
{
    int pos = RIDX(x,y,dim);
    dst[pos].red = (src[pos].red  + src[pos-1].red + src[pos+1].red 
        + src[pos+dim-1].red + src[pos+dim].red + src[pos+dim+1].red) / 6;
    dst[pos].green = (src[pos].green  + src[pos-1].green + src[pos+1].green 
        + src[pos+dim-1].green + src[pos+dim].green + src[pos+dim+1].green) / 6;
    dst[pos].blue = (src[pos].blue  + src[pos-1].blue + src[pos+1].blue 
        + src[pos+dim-1].blue + src[pos+dim].blue + src[pos+dim+1].blue) / 6;
}

inline void _smb(int x, int y, pixel* src, pixel* dst, int dim)
{
    int pos = RIDX(x,y,dim);
    dst[pos].red = (src[pos].red  + src[pos-1].red + src[pos+1].red 
        + src[pos-dim-1].red + src[pos-dim].red + src[pos-dim+1].red) / 6;
    dst[pos].green = (src[pos].green  + src[pos-1].green + src[pos+1].green 
        + src[pos-dim-1].green + src[pos-dim].green + src[pos-dim+1].green) / 6;
    dst[pos].blue = (src[pos].blue  + src[pos-1].blue + src[pos+1].blue 
        + src[pos-dim-1].blue + src[pos-dim].blue + src[pos-dim+1].blue) / 6;
}

char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
    dst[0].red = (src[0].red + src[1].red + src[dim].red + src[dim+1].red) / 4;
    dst[0].green = (src[0].green + src[1].green + src[dim].green + src[dim+1].green) / 4;
    dst[0].blue = (src[0].blue + src[1].blue + src[dim].blue + src[dim+1].blue) / 4;
    
    int _dim = 2 * dim -1;
    dst[dim-1].red = (src[dim-1].red + src[dim-2].red + src[_dim].red + src[_dim-1].red)/ 4;
    dst[dim-1].green = (src[dim-1].green + src[dim-2].green + src[_dim].green + src[_dim-1].green)/ 4;
    dst[dim-1].blue = (src[dim-1].blue + src[dim-2].blue + src[_dim].blue + src[_dim-1].blue)/ 4;
    
    int pos = dim * (dim - 1);
    dst[pos].red = (src[pos].red + src[pos+1].red + src[pos-dim].red + src[pos-dim+1].red)/ 4;
    dst[pos].green = (src[pos].green + src[pos+1].green + src[pos-dim].green + src[pos-dim+1].green)/ 4;
    dst[pos].blue = (src[pos].blue + src[pos+1].blue + src[pos-dim].blue + src[pos-dim+1].blue)/ 4;
    
    pos = dim * dim - 1;
    dst[pos].red = (src[pos].red + src[pos-1].red + src[pos-dim].red + src[pos-dim-1].red)/ 4;
    dst[pos].green = (src[pos].green + src[pos-1].green + src[pos-dim].green + src[pos-dim-1].green)/ 4;
    dst[pos].blue = (src[pos].blue + src[pos-1].blue + src[pos-dim].blue + src[pos-dim-1].blue)/ 4;
    
    for(int j = 1; j < dim - 1;j++) {
        _smt(0,j,src,dst,dim);
        _smb(dim - 1,j,src,dst,dim);
        _sml(j,0,src,dst,dim);
        _smr(j,dim - 1,src,dst,dim);
    }
    
    for(int i = 1; i < dim - 1; i++) {
        for(int j = 1; j < dim - 1; j++) {
            pos = RIDX(i,j,dim);
            dst[pos].red = (src[pos].red + src[pos+1].red + src[pos-1].red
                + src[pos+dim].red + src[pos+dim+1].red + src[pos+dim-1].red
                + src[pos-dim].red + src[pos-dim-1].red + src[pos-dim+1].red) / 9;
            dst[pos].green = (src[pos].green + src[pos+1].green + src[pos-1].green
                + src[pos+dim].green + src[pos+dim+1].green + src[pos+dim-1].green
                + src[pos-dim].green + src[pos-dim-1].green + src[pos-dim+1].green) / 9;
            dst[pos].blue = (src[pos].blue + src[pos+1].blue + src[pos-1].blue
                + src[pos+dim].blue + src[pos+dim+1].blue + src[pos+dim-1].blue
                + src[pos-dim].blue + src[pos-dim-1].blue + src[pos-dim+1].blue) / 9;
        }
    }
}


/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}

