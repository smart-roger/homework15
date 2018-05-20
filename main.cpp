#include <iostream>
#include <vector>
#include <string>

#include <dlib/clustering.h>
#include <dlib/rand.h>
#include <dlib/image_io.h>

using namespace std;
using namespace dlib;

typedef matrix<double,2,1> sample_type;
typedef radial_basis_kernel<sample_type> kernel_type;

int main(int argc, char** argv)
{
    try{
        if(argc<2){
            std::cout << "usage kkmeans <num_clusters> <draw>" << std::endl;
            return -1;
        }

        unsigned long num_clusters = std::stoul(argv[1]);
        if(num_clusters<=1){
            std::cout << "Wrong num_clusters! num_clusters should be more than 1!" << std::endl;
            return -2;
        }

        bool drawing (false);
        double minX, minY, maxX, maxY;
        if( (argc>2) && ("draw" == std::string(argv[2])) )
            drawing = true;

        kcentroid<kernel_type> kc(kernel_type(0.1),0.01, num_clusters);
        kkmeans<kernel_type> test(kc);

        sample_type m;
        std::vector<sample_type> samples;
        std::vector<sample_type> initial_centers;

        std::string line;
        while (std::getline(std::cin, line)){
            auto pos = line.find(";");
            if(pos == std::string::npos)
                std::cout << "Wrong line: " << line << std::endl;
            else{
                try{
                    double x,y;
                    x = std::stod(line.substr(0,pos));
                    y = std::stod(line.substr(pos+1));

                    if (drawing){
                        if(samples.empty()){
                            minX = maxX = x;
                            minY = maxY = y;
                        } else{
                            if(x < minX) minX = x;
                            if(y < minY) minY = y;
                            if(x > maxX) maxX = x;
                            if(y > maxY) maxY = y;
                        }
                    }

                    m(0) = x;
                    m(1) = y;
                    samples.push_back(m);
                } catch(std::exception& e){
                    std::cout << "Wrong line: " << line << std::endl;
                }
            }
        }

        test.set_number_of_centers(num_clusters);
        pick_initial_centers(num_clusters, initial_centers, samples, test.get_kernel());
        test.train(samples,initial_centers);

        std::vector<unsigned long> assignments = spectral_cluster(kernel_type(0.1), samples, num_clusters);

        for(size_t i=0;i<samples.size();++i){
            unsigned long cluster (assignments.at(i));
            m = samples.at(i);
            std::cout << m(0) <<";"<<m(1)<< ";"<<"cluster" << cluster << std::endl;
        }

        if(drawing){
            std::vector<rgb_pixel> colors;
                colors.push_back(rgb_pixel(0x00, 0xFF, 0xFF));
                colors.push_back(rgb_pixel(0x00, 0x00, 0xFF));
                colors.push_back(rgb_pixel(0xFF, 0x00, 0xFF));
                colors.push_back(rgb_pixel(0x80, 0x80, 0x80));
                colors.push_back(rgb_pixel(0x00, 0xFF, 0x00));
                colors.push_back(rgb_pixel(0x80, 0x00, 0x00));
                colors.push_back(rgb_pixel(0x00, 0x00, 0x80));
                colors.push_back(rgb_pixel(0x80, 0x80, 0x00));
                colors.push_back(rgb_pixel(0x80, 0x00, 0x80));
                colors.push_back(rgb_pixel(0xFF, 0x00, 0x00));
                colors.push_back(rgb_pixel(0xC0, 0xC0, 0xC0));
                colors.push_back(rgb_pixel(0x00, 0x80, 0x80));
                colors.push_back(rgb_pixel(0xFF, 0xFF, 0x00));

            array2d<rgb_pixel> img;
            size_t img_size=250, step=5;

            img.set_size(img_size, img_size);

            for(size_t x=0; x<img_size;++x)
                for(size_t y=0; y<img_size;++y)
                    img[x][y] = rgb_pixel(-1,-1,-1);

            for(size_t x=0; x<img_size;++x){
                img[x][0] = rgb_pixel(0,0,0);
                img[x][img_size-1] = rgb_pixel(0,0,0);
            }
            for(size_t y=0; y<img_size;++y){
                img[0][y] = rgb_pixel(0,0,0);
                img[img_size-1][y] = rgb_pixel(0,0,0);
            }

            assert (assignments.size() == samples.size());
            double k(img_size-2*step);

            if ((maxX-minX) > (maxY-minY))
                k /= (maxX-minX);
            else k /= (maxY-minY);

            minX -= step/k;
            minY -= step/k;

            for(size_t i=0;i<samples.size();++i){
                unsigned long cluster (assignments.at(i));
                m = samples.at(i);

                double x = k*(m(0) - minX);
                double y = k*(m(1) - minY);

                img[x][y] = colors.at(cluster%colors.size());
            }
            std::string image_name(std::to_string(std::time(nullptr))+"_out.bmp");
            save_bmp(img, image_name.c_str());
        }
    }
    catch(std::exception& e){
        std::cout << "Unexpected exception! " << e.what() << std::endl;
    }
}
