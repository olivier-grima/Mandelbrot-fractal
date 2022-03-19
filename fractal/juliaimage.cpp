#include "juliaimage.h"
#include <complex>
#include <thread>
#include <iostream>

using namespace std;

std::pair<bool, QRgb> JuliaImage::calc_in_out(double rx, double ry)
{
    std::complex<double> c0;
    std::complex<double> zn;
    bool is_outside;
    int iteration=0;
    QRgb rgb_color;

    c0.real(-0.4); c0.imag(0.6);
    zn.real(rx);zn.imag(ry);

    for (int j = 0 ; j<512 ; j++){
        zn=zn*zn+c0;
        if (abs(zn) >= 256.)
        {
            iteration = j;
            break;
        }
    }

    if(abs(zn)<2.){
        is_outside=false;
        rgb_color = qRgb(0,0,0);
    }
    else{
        double v = log2(log2(abs(zn)*abs(zn)));

        int i = 1024 * sqrt(( iteration + 5 - v));
        i = i % 2048;

        rgb_color = calc_color(i);

        is_outside=true;
    }

    return std::make_pair(is_outside, rgb_color);
}

void JuliaImage::process_simple()
{
    auto start = std::chrono::high_resolution_clock::now();

    for(int py=0;py< h;++py) {
          for(int px=0;px< w;++px) {
            calc_pix(px,py);
          }
     }

    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed_time = end - start;
    cout << "Info: non-threaded image calculated in " << Commify(elapsed_time.count()) << " ns" << endl;
}

void JuliaImage::process_threaded(int i)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    int max_threads = i;    // number of threads
    for (int i = 0; i < max_threads; i++) {
        threads.emplace_back([=]() {
            process_sub_image(i, max_threads);
        });
    }
    for (auto &thread_elem : threads)   // joins threads
        thread_elem.join();

    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed_time = end - start;
    cout << "Info: threaded (" << max_threads << " threads)image calculated in " << Commify(elapsed_time.count()) << " ns" << endl;
}
