#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <png.h>

int main(int argc, char *argv[])
{
    char buffer[1024];
    
    if (argv < 2)
    {
        fprintf(stderr, "usage: %s [pngfile]\n", argv[0]);
    }
    
    FILE *fp = fopen(argv[1], "r");
    if (!fp)
    {
        fprintf(stderr, "cannot open file %s", argv[1]);
        exit(1);
    }

    fread(buffer, 1, 8, fp);
    char is_png = !png_sig_cmp(buffer, 0, 8);
    if (!is_png)
    {
        fprintf(stderr, "not a png file\n");
        exit(1);
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fprintf(stderr, "creating png structure...\n");
        exit(1);
    }
    
    png_info info_ptr, end_info;
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        fprintf(stderr, "creating info structure...\n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        exit(1);
    }

    end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        fprintf(stderr, "creating end info structure...\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        exit(1);
    }

    
    return 0;
}
