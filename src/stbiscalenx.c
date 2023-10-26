#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include "scalenx.h"

void scalenx_usage(char* prog, int scale)
{
    printf("Stb Image Scale Near X version %s.\n", SCALENX_VERSION);
    printf("usage: %s [options] image_in out.png\n", prog);
    printf("options:\n");
    printf("  -k NUM    scale coef (default %d)\n", scale);
    printf("  -h        show this help message and exit\n");
}

int main(int argc, char **argv)
{
    int height, width, channels, y, x, d;
    int resize_height = 0, resize_width = 0;
    int scale = 2;
    int fhelp = 0;
    int opt;
    size_t szorig, szdest, ki, kd;
    unsigned char *data = NULL, *resize_data = NULL;
    stbi_uc *img = NULL;

    while ((opt = getopt(argc, argv, ":k:h")) != -1)
    {
        switch(opt)
        {
        case 'k':
            scale = atoi(optarg);
            if ((scale < 2) || (scale > 3))
            {
                fprintf(stderr, "ERROR: scale coef not {2,3}: %d\n", scale);
                return 1;
            }
            break;
        case 'h':
            fhelp = 1;
            break;
        case ':':
            fprintf(stderr, "ERROR: option needs a value\n");
            return 2;
            break;
        case '?':
            fprintf(stderr, "ERROR: unknown option: %c\n", optopt);
            return 3;
            break;
        }
    }
    if(optind + 2 > argc || fhelp)
    {
        scalenx_usage(argv[0], scale);
        return 0;
    }
    const char *src_name = argv[optind];
    const char *dst_name = argv[optind + 1];


    printf("Load: %s\n", src_name);
    if (!(img = stbi_load(src_name, &width, &height, &channels, STBI_rgb_alpha)))
    {
        fprintf(stderr, "ERROR: not read image: %s\n", src_name);
        return 1;
    }
    printf("image: %dx%d:%d\n", width, height, channels);
    if (!(data = (unsigned char*)malloc(height * width * channels * sizeof(unsigned char))))
    {
        fprintf(stderr, "ERROR: not use memmory\n");
        return 1;
    }
    ki = 0;
    kd = 0;
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            for (d = 0; d < channels; d++)
            {
                data[kd + d] = (unsigned char)img[ki + d];
            }
            ki += STBI_rgb_alpha;
            kd += channels;
        }
    }
    stbi_image_free(img);
    szorig = (size_t)height * width;

    resize_height = height * scale;
    resize_width = width * scale;
    if ((resize_height == 0) || (resize_width == 0))
    {
        fprintf(stderr, "ERROR: bad target size %dx%d:%d\n", resize_width, resize_height, channels);
        return 1;
    }
    szdest = (size_t)resize_height * resize_width;

    printf("resize: %dx%d:%d\n", resize_width, resize_height, channels);
    if (!(resize_data = (unsigned char*)malloc(resize_height * resize_width * channels * sizeof(unsigned char))))
    {
        fprintf(stderr, "ERROR: not use memmory\n");
        return 2;
    }

    printf("ScaleNX: %d\n", scale);
    ScaleNX(data, height, width, channels, scale, resize_data);

    printf("Save png: %s\n", dst_name);
    if (!(stbi_write_png(dst_name, resize_width, resize_height, channels, resize_data, resize_width * channels)))
    {
        fprintf(stderr, "ERROR: not write image: %s\n", dst_name);
        return 1;
    }

    free(resize_data);
    free(data);
    return 0;
}
