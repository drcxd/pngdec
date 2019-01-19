#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <png.h>

#define MASK 0x00000001

int main(int argc, char *argv[])
{
    char buffer[1024];
    
    if (argc < 2)
    {
        fprintf(stderr, "usage: %s [pngfile]\n", argv[0]);
        exit(1);
    }
    
    FILE *fp = fopen(argv[1], "r");
    if (!fp)
    {
        fprintf(stderr, "cannot open file %s", argv[1]);
        exit(1);
    }

    /* fread(buffer, 1, 8, fp); */
    /* char is_png = !png_sig_cmp(buffer, 0, 8); */
    /* if (!is_png) */
    /* { */
    /*     fprintf(stderr, "not a png file\n"); */
    /*     exit(1); */
    /* } */

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fprintf(stderr, "creating png structure...\n");
        fclose(fp);
        exit(1);
    }
    
    png_infop info_ptr, end_info;
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        fprintf(stderr, "creating info structure...\n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        exit(1);
    }

    end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        fprintf(stderr, "creating end info structure...\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        fclose(fp);
        exit(1);
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        fprintf(stderr, "setting jump buffer");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        exit(1);
    }

    png_init_io(png_ptr, fp);

    png_read_info(png_ptr, info_ptr);

    unsigned width, height;
    int bit_depth, color_type, interlace_type, compression_type, filter_method;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type,
        &compression_type, &filter_method);
    printf("width\t%u\theight\t%u\n", width, height);
    printf("bit_depth\t\t%d\n", bit_depth);
    printf("color_type\t\t%d\n", color_type);
    printf("interlace_type\t\t%d\n", interlace_type);
    printf("compression_type\t\t%d\n", compression_type);
    printf("filter_method\t\t%d\n", filter_method);

    png_bytep row_pointers[height];
    for (unsigned row = 0; row < height; ++row)
        row_pointers[row] = NULL;
    for (unsigned row = 0; row < height; ++row)
        row_pointers[row] = png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
    printf("bytes per row %zu\n", png_get_rowbytes(png_ptr, info_ptr));

    for (unsigned y = 0; y < height; ++y)
        png_read_rows(png_ptr, &row_pointers[y], NULL, 1);
    printf("read finished\n");
    fclose(fp);

    /* do transform here */
    int count = 0;
    for (unsigned i = 0; i < height; ++i)
    {
        for (unsigned j = 0; j < width; ++j)
        {
            png_bytep pixelp = row_pointers[i] + j * 4;
            int *pixel = (int *)pixelp;
            if (*pixel & MASK)
            {
                *pixel = 0xffffffff;
            }
            else
            {
                *pixel = 0xff000000;
            }
            /* printf("0x%x\n", *pixel); */
            ++count;
        }
    }
    printf("total pixle %d\n", count);
    
    

    fp = fopen("decode.png", "w");
    if (!fp)
    {
        fprintf(stderr, "opening output file...\n");
        exit(1);
    }

    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!write_ptr)
    {
        fclose(fp);
        fprintf(stderr, "creating write structure...\n");
        exit(1);
    }

    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info)
    {
        fclose(fp);
        png_destroy_write_struct(&write_ptr, NULL);
        fprintf(stderr, "creating write info structure...\n");
        exit(1);
    }

    if (setjmp(png_jmpbuf(write_ptr)))
    {
        fclose(fp);
        png_destroy_write_struct(&write_ptr, &write_info);
        fprintf(stderr, "something is wrong...\n");
        exit(1);
    }

    png_init_io(write_ptr, fp);

    png_set_IHDR(write_ptr, write_info, width, height, bit_depth,
        color_type, interlace_type, compression_type, filter_method);

    png_write_info(write_ptr, write_info);

    png_write_image(write_ptr, row_pointers);

    png_write_end(write_ptr, write_info);

    png_destroy_write_struct(&write_ptr, &info_ptr);

    fclose(fp);
    
    return 0;
}
