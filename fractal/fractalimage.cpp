#include "fractalimage.h"
#include <QImage>
#include <QPainter>
#include <vector>
#include <complex>
#include <iostream>
#include <thread>

using namespace std;

pair<double,double> FractalImage::pixToReal(int x,int y)
{
    // linear interpolation with x
    double ax = (3.0*d_)/((double)w);
    double bx = xc_-1.5*d_;
    double rx = ax*x + bx;

    // linear interpolation with y
    double ay = (2.0*d_)/((double)h);
    double by = yc_-d_;
    double ry = ay*y + by;

    return std::make_pair(rx, ry);
}

QRgb FractalImage::calc_color(const int i)
{
    return color[i];
}


FractalImage::FractalImage(int width,int height,double xc,double yc,double d): QImage(width,height,QImage::Format_RGB32),w(width),h(height),xc_(xc),yc_(yc),d_(d)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), Qt::black);

    vector<double> xs{ 0., 0.16, 0.42, 0.6425, 0.8575};
    vector<double> yr{ 0., 32. , 237. , 215. , 0. };
    vector<double> yg{ 7., 107. , 255. , 170. , 10. };
    vector<double> yb{ 100., 183. , 235. , 40. , 15. };

    Linear red_color (xs,yr);
    Linear green_color (xs,yg);
    Linear blue_color (xs,yb);

    for (int i = 0; i < 2048; i++) {
      color[i] = qRgb(static_cast<int>(red_color(i/2047.)),
                      static_cast<int>(green_color(i/2047.)),
                      static_cast<int>(blue_color(i/2047.)));
    }
}

void FractalImage::process_sub_image(int i, int m)
{
    for(int py=0;py< h;++py) {
          for(int px=(w/m)*i;px< (w/m)*(i+1);++px) {
            calc_pix(px,py);
          }
     }
}

void FractalImage::calc_pix(int px,int py)
{
    auto [rx, ry] = pixToReal(px,py);
    auto [is_outside, rgb_color] = calc_in_out(rx,ry);
    if (is_outside) {
      setPixel(px, py, rgb_color);
    }
}
