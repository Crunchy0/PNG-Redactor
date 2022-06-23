#ifndef PNGCLASS_H
#define PNGCLASS_H
#define SIG_SIZE 8
#include "png.h"


class PngClass
{
public:
    PngClass();
    ~PngClass();
    void read(const char *file_name);
    bool write(const char *filename);
    void cut(int hb1, int hb2, int wb1, int wb2);
    void gray(int hb1, int hb2, int wb1, int wb2);
    void invert(int y, int x, float r);
    void invert(int y1, int y2, int x1, int x2);
    void otrezok(int x1, int x2, int y1, int y2, int thickness, int color);
    bool is_valid();
    int get_width();
    int get_height();
    int get_color_type();
    void free_self();
private:
    int width, height, interlace;
    bool valid;
    png_structp png;
    png_infop info;
    png_bytepp row;
    png_byte color_type, bit_depth;
};



#endif // PNGCLASS_H
