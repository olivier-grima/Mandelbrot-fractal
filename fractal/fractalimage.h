#ifndef FRACTALIMAGE_H
#define FRACTALIMAGE_H

#include <QImage>
#include <QLabel>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>

// A class to calculate linear interpolation from points in vectors
class Linear{
 private:
    std::vector<double> x_;
    std::vector<double> a_;
    std::vector<double> b_;

 public:
    Linear(const std::vector<double> &xs,
           const std::vector<double> &ys): x_(xs) {
        for (unsigned int i = 0; i < x_.size()-1; ++i) {
            a_.push_back((ys[i+1] - ys[i]) / (x_[i+1] - x_[i]));
            b_.push_back(ys[i]);
        }
        a_.push_back((ys.front() - ys.back()) / (1 - x_.back()));
        b_.push_back(ys.back());
    }
    double operator() (const double &x) {
        if (x < 0)
            return a_[0] * (-x_[0]) + b_[0];
        auto i = x_.size()-1;
        if (x > 1)
            return a_[i] * (1 - x_[i]) + b_[i];
        while (i > 0  && x < x_[i]) --i;
        return a_[i] * (x - x_[i]) + b_[i];
    }
};

// A class to cadd comas in a number
class Commify {
 private:
    uint32_t val_;
 public:
    explicit Commify(uint32_t value):val_{value} {}

    friend std::ostream &operator << (std::ostream &f, const Commify &c){
        std::ostringstream ss;
        ss << c.val_;
        std::string s = ss.str();
        int insertPosition = s.length() - 3;
        while (insertPosition > 0) {
            s.insert(insertPosition, ",");
            insertPosition -= 3;
        }
        f << s;
        return f;
    }
};

//An abstract class to represent a fractal image
class FractalImage : public QImage
{
public:
    FractalImage(int width,int height,double xc,double yc,double d);
    virtual void process_sub_image(int i, int m);
    virtual void process_threaded(int i)=0;
    virtual void process_simple()=0;
protected:
    std::pair<double,double> pixToReal(int x,int y);
    virtual std::pair<bool, QRgb> calc_in_out(double rx, double ry)=0;
    void calc_pix(int px,int py);
    QRgb calc_color(int i);

    QRgb color[2048];
    int w,h;
    double xc_,yc_,d_;
};

#endif // FRACTALIMAGE_H
