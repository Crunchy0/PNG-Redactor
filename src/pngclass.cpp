#include "pngclass.h"
#include <iostream>
#include <cmath>

PngClass::PngClass()
{
    valid = false;
}

PngClass::~PngClass(){
}

void PngClass::read(const char *file_name){
    valid = false;
    char sig[SIG_SIZE];
    FILE *sauce = fopen(file_name, "rb");
    if(!sauce){
        fclose(sauce);
        std::cout<<"File reading error"<<std::endl;
        return;
    }
    fread(sig, 1, SIG_SIZE, sauce);
    if(png_sig_cmp((png_const_bytep)sig, 0, SIG_SIZE)){
        fclose(sauce);
        std::cout<<"Not a PNG file"<<std::endl;
        return;
    }
    png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png){
        fclose(sauce);
        std::cout<<"PNG image structure initialization failed"<<std::endl;
        return;
    }
    info = png_create_info_struct(png);
    if(!info){
        fclose(sauce);
        png_destroy_read_struct(&png, &info, NULL);
        std::cout<<"PNG info structure initialization failed"<<std::endl;
        return;
    }
    if(setjmp(png_jmpbuf(png))){
        fclose(sauce);
        png_destroy_read_struct(&png, &info, NULL);
        std::cout<<"Initialization error"<<std::endl;
        return;
    }
    png_init_io(png, sauce);
    png_set_sig_bytes(png, SIG_SIZE);
    png_read_info(png, info);

    width = png_get_image_width(png, info);
    height = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth = png_get_bit_depth(png, info);
    interlace = png_set_interlace_handling(png);
    png_read_update_info(png, info);

    if(setjmp(png_jmpbuf(png))){
        fclose(sauce);
        png_destroy_read_struct(&png, &info, NULL);
        std::cout<<"PNG info reading error"<<std::endl;
        return;
    }

    row = (png_bytep*)malloc(height * sizeof(png_bytep));
    for(int i = 0; i < height; i++){
        row[i] = (png_bytep)malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, row);
    fclose(sauce);
    valid = true;
}

bool PngClass::write(const char *filename){
    if(!valid){
        std::cout<<"Image is damaged or absent"<<std::endl;
        return false;
    }
    FILE *sauce = fopen(filename, "wb");
    if(!sauce){
        std::cout<<"Error while creating file"<<std::endl;
        return false;
    }
    png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png){
        fclose(sauce);
        std::cout<<"PNG image structure initialization failed"<<std::endl;
        return false;
    }
    info = png_create_info_struct(png);
    if(!info){
        fclose(sauce);
        png_destroy_write_struct(&png, &info);
        std::cout<<"PNG info structure initialization failed"<<std::endl;
        return false;
    }
    if(setjmp(png_jmpbuf(png))){
        fclose(sauce);
        png_destroy_write_struct(&png, &info);
        std::cout<<"Initialization error"<<std::endl;
        return false;
    }

    png_init_io(png, sauce);

    if(setjmp(png_jmpbuf(png))){
        fclose(sauce);
        png_destroy_write_struct(&png, &info);
        std::cout<<"PNG info writing error"<<std::endl;
        return false;
    }

    png_set_IHDR(png, info, width, height, bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png, info);

    if(setjmp(png_jmpbuf(png))){
        fclose(sauce);
        png_destroy_write_struct(&png, &info);
        std::cout<<"PNG image writing error"<<std::endl;
        return false;
    }

    png_write_image(png, row);

    if(setjmp(png_jmpbuf(png))){
        fclose(sauce);
        png_destroy_write_struct(&png, &info);
        std::cout<<"PNG IEND writing error"<<std::endl;
        return false;
    }

    png_write_end(png, NULL);
    fclose(sauce);
    return true;
}

void PngClass::cut(int hb1, int hb2, int wb1, int wb2){
    if(wb1 > wb2){
        int v = wb1;
        wb1 = wb2;
        wb2 = v;
    }
    if(hb1 > hb2){
        int v = hb1;
        hb1 = hb2;
        hb2 = v;
    }
    int bytes = 0;
    if(color_type == PNG_COLOR_TYPE_RGB_ALPHA){
        bytes = 4;
    }
    else if(color_type == PNG_COLOR_TYPE_RGB){
        bytes = 3;
    }
    else if(color_type == PNG_COLOR_TYPE_GRAY){
        bytes = 1;
    }
    int new_height = hb2 + 1 - hb1, new_width = wb2 + 1 - wb1;
    for(int i = hb2 + 1; i < height; i++){
        for(int j = 0; j < bytes*(width); j++){
            row[i][j] = 0;
        }
        free(row[i]);
    }
    for(int i = 0; i < hb1; i++){
        free(row[0]);
        for(int j = 0; j < hb2 - i; j++){
            row[j] = row[j+1];
        }
        row[hb2 - i] = NULL;
    }
    for(int i = 0; i < new_height; i++){
        for(int j = 0; j < bytes*(new_width); j++){
            row[i][j] = row[i][j + bytes*wb1];
        }
    }

    height = new_height;
    width = new_width;
}

void PngClass::gray(int hb1, int hb2, int wb1, int wb2){
    if(color_type == PNG_COLOR_TYPE_GRAY){
        return;
    }
    if(wb1 > wb2){
        int v = wb1;
        wb1 = wb2;
        wb2 = v;
    }
    if(hb1 > hb2){
        int v = hb1;
        hb1 = hb2;
        hb2 = v;
    }
    int bytes = 0;
    if(color_type == PNG_COLOR_TYPE_RGB_ALPHA){
        bytes = 4;
    }
    else if(color_type == PNG_COLOR_TYPE_RGB){
        bytes = 3;
    }
    for(int i = hb1; i < hb2 + 1; i++){
        for(int j = wb1; j < wb2 + 1; j++){
            png_bytep cur = &row[i][j*bytes];
            int gray = rintf(((float)(cur[0] + cur[1] + cur[2])/3));
            cur[0] = gray;
            cur[1] = gray;
            cur[2] = gray;
        }
    }
}

void PngClass::invert(int y, int x, float r){
    int bytes = 0;
    if(color_type == PNG_COLOR_TYPE_RGB_ALPHA){
        bytes = 4;
    }
    else if(color_type == PNG_COLOR_TYPE_RGB){
        bytes = 3;
    }
    else if(color_type == PNG_COLOR_TYPE_GRAY){
        bytes = 1;
    }
    if(pow(r, 2) > pow(width, 2) + pow(height, 2)){
        r = sqrt(pow(width, 2) + pow(height, 2));
    }
    int y1 = (int)rintf(y - r);
    int y2 = (int)rintf(y + r);
    int x1 = (int)rintf(x - r);
    int x2 = (int)rintf(x + r);
    for(int i = y1; i <= y2; i++){
        for(int j = x1; j <= x2; j++){
            if(i >= 0 && j >= 0 && i < height && j < width){
                if((r < 4 && pow(y - i, 2) + pow(x - j, 2) <= pow(r, 2)) || r >= 4 && pow(y - i, 2) + pow(x - j, 2) < pow(r, 2)){
                    png_bytep cur = &row[i][j*bytes];
                    cur[0] ^= 0xff;
                    if(bytes > 1){
                        cur[1] ^= 0xff;
                        cur[2] ^= 0xff;
                    }
                }
            }
        }
    }
}

void PngClass::invert(int y1, int y2, int x1, int x2){
    if(x1 > x2){
        int v = x1;
        x1 = x2;
        x2 = v;
    }
    if(y1 > y2){
        int v = y1;
        y1 = y2;
        y2 = v;
    }
    int bytes = 0;
    if(color_type == PNG_COLOR_TYPE_RGB_ALPHA){
        bytes = 4;
    }
    else if(color_type == PNG_COLOR_TYPE_RGB){
        bytes = 3;
    }
    else if(color_type == PNG_COLOR_TYPE_GRAY){
        bytes = 1;
    }
    float r = (x2 - x1)/2;
    int c1 = (x2 + x1)/2;
    int c2 = (y2 + y1)/2;
    for(int i = y1; i <= y2; i++){
        for(int j = x1; j <= x2; j++){
            if(i >= 0 && j >= 0 && i < height && j < width){
                if((r < 4 && pow(c2 - i, 2) + pow(c1 - j, 2) <= pow(r, 2)) || r >= 4 && pow(c2 - i, 2) + pow(c1 - j, 2) < pow(r, 2)){
                    png_bytep cur = &row[i][j*bytes];
                    cur[0] ^= 0xff;
                    if(bytes > 1){
                        cur[1] ^= 0xff;
                        cur[2] ^= 0xff;
                    }
                }
            }
        }
    }
}


void PngClass::otrezok(int x1, int x2, int y1, int y2, int thickness, int color){
    if(x1 > x2){
        int v = x1;
        x1 = x2;
        x2 = v;
        v = y1;
        y1 = y2;
        y2 = v;
    }
    int bytes = 0;
    if(color_type == PNG_COLOR_TYPE_RGB_ALPHA){
        bytes = 4;
    }
    else if(color_type == PNG_COLOR_TYPE_RGB){
        bytes = 3;
    }
    else if(color_type == PNG_COLOR_TYPE_GRAY){
        bytes = 1;
    }
    else{
        return;
    }
    float x = 0, y = 0; // Здесь будем прибавлять дифференциал, накапливая смещение
    float derrivative;

    // Случай dy <= dx
    if(abs(y2 - y1) <= abs(x2 - x1)){
        derrivative = (float)(y2 - y1)/(float)(x2 - x1);
        for(int i = x1; i <= x2; i++){
            for(int j = 0; j < thickness; j++){
                // Индексы смещения
                int index1 = y1 + (int)rintf(y) - thickness/2 + j;
                int index2 = i - (j - thickness/2)*derrivative;
                if(index1 >= -1 && index1 < height && index2 >= 0 && index2 < width){
                    if(index1 > -1){
                        row[index1][index2*bytes] = color%256;
                        if(bytes > 1){
                            row[index1][index2*bytes+1] = (color/256)%256;
                            row[index1][index2*bytes+2] = (color/256/256)%256;
                            if(bytes == 4){
                                row[index1][index2*bytes+3] = 0xff;
                            }
                        }
                    }
                    else if(index1 < height - 1){
                        row[index1 + 1][index2*bytes] = color%256;
                        if(bytes > 1){
                            row[index1 + 1][index2*bytes+1] = (color/256)%256;
                            row[index1 + 1][index2*bytes+2] = (color/256/256)%256;
                            if(bytes == 4){
                                row[index1 + 1][index2*bytes+3] = 0xff;
                            }
                        }
                    }
                    if(thickness > 1 && index1 < height - 1 && j < thickness){
                        row[index1 + 1][index2*bytes] = color%256;
                        if(bytes > 1){
                            row[index1 + 1][index2*bytes+1] = (color/256)%256;
                            row[index1 + 1][index2*bytes+2] = (color/256/256)%256;
                            if(bytes == 4){
                                row[index1 + 1][index2*bytes+3] = 0xff;
                            }
                        }
                    }
                }
            }
            y += derrivative;
        }
    }

    // Случай dy > dx
    else{
        derrivative = (float)(x2 - x1)/(float)(y2 - y1);
            for(int i = ((y1 <= y2)?y1:y2); i <= ((y1 <= y2)?y2:y1); i++){
                for(int j = 0; j < thickness; j++){
                    int index1 = i - (j - thickness/2)*derrivative;
                    int index2 = ((y1 <= y2)?x1:x2) +(int)rintf(x) - thickness/2 + j;
                    if(index1 >= 0 && index1 < height && index2 >= -1 && index2 < width){
                        if(index2 > -1){
                            row[index1][index2*bytes] = color%256;
                            if(bytes > 1){
                                row[index1][index2*bytes+1] = (color/256)%256;
                                row[index1][index2*bytes+2] = (color/256/256)%256;
                                if(bytes == 4){
                                    row[index1][index2*bytes+3] = 0xff;
                                }
                            }

                        }
                        else if(index2 < width - 1){
                            row[index1][(index2 + 1)*bytes] = color%256;
                            if(bytes > 1){
                                row[index1][(index2 + 1)*bytes+1] = (color/256)%256;
                                row[index1][(index2 + 1)*bytes+2] = (color/256/256)%256;
                                if(bytes == 4){
                                    row[index1][(index2 + 1)*bytes+3] = 0xff;
                                }
                            }
                        }
                        if(thickness > 1 && index2 < width - 1 && j < thickness){
                            row[index1][(index2 + 1)*bytes] = color%256;
                            if(bytes > 1){
                                row[index1][(index2 + 1)*bytes+1] = (color/256)%256;
                                row[index1][(index2 + 1)*bytes+2] = (color/256/256)%256;
                                if(bytes == 4){
                                    row[index1][(index2 + 1)*bytes+3] = 0xff;
                                }
                            }
                        }
                    }
                }
                x += derrivative;
            }

    }
}

bool PngClass::is_valid(){
    if(valid) return true;
    else return false;
}

int PngClass::get_width(){
    return width;
}

int PngClass::get_height(){
    return height;
}

int PngClass::get_color_type(){
    return color_type;
}

void PngClass::free_self(){
    free(png);
    free(info);
    for(int i = 0; i < height; i++){
        free(row[i]);
    }
    free(row);
}
