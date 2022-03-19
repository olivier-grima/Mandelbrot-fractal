#ifndef JULIAIMAGE_H
#define JULIAIMAGE_H

#include "fractalimage.h"
#include <QImage>
#include <QLabel>

//A class to represent a julia set fractal image
class JuliaImage : public FractalImage
{
public:
    JuliaImage(int width,int height,double xc,double yc,double d):FractalImage(width,height,xc,yc,d){}
    void process_threaded(int i) override;
    void process_simple() override;
protected:
    std::pair<bool, QRgb> calc_in_out(double rx, double ry) override;
};

#endif // JULIAIMAGE_H
