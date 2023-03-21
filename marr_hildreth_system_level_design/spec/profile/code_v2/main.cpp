#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <iomanip>
#include <stdlib.h>
#include <sstream>

#include </usr/include/opencv4/opencv2/opencv.hpp>
#include </usr/include/opencv4/opencv2/core/core.hpp>
#include </usr/include/opencv4/opencv2/imgproc/imgproc.hpp>
#include </usr/include/opencv4/opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

int inp[833][833];
int newinp[833][833];
//int im[833][833];
unsigned char im[833][833];
int out_array[833][833];
typedef unsigned char BYTE;

struct PIC
{
    unsigned int nChannel;
    bool InterLeaved;
    unsigned int Width, Height, Maxval;
    BYTE *img;
};
bool LoadP5Header(ifstream &infile, PIC &pic)
{
    bool rtv = true;
    char buf[16];
    int bufIndex;
    int width, height, maxval;
    infile.read(buf, 2); 
    buf[2]='\0';

    if(buf[0] == 'P' && buf[1] == '5')
    {
        infile.read(buf, 1);
        while(isspace(buf[0])) 
        {
            infile.read(buf,1);
        }

        
        bufIndex = 0;
        while(bufIndex < 15 && !isspace(buf[bufIndex]))
        {
            bufIndex++;
            infile.read(buf+bufIndex, 1);
        }
        buf[bufIndex] = NULL;  
        width = atoi(buf);

       
        infile.read(buf,1);
        while(isspace(buf[0])) 
        {
            infile.read(buf,1);
        }
        bufIndex = 0;
        while(bufIndex < 15 && !isspace(buf[bufIndex]))
        {
            bufIndex++;
            infile.read(buf+bufIndex, 1);
        }
        buf[bufIndex] = NULL;  
        height = atoi(buf);

    
        infile.read(buf,1);
        while(isspace(buf[0])) 
        {
            infile.read(buf,1);
        }
        bufIndex = 0;
        while(bufIndex < 15 && !isspace(buf[bufIndex]))
        {
            bufIndex++;
            infile.read(buf+bufIndex, 1);
        }
        buf[bufIndex] = NULL;  
        maxval = atoi(buf);

        
        infile.read(buf,1);

        
        pic.InterLeaved = false;
        pic.Width = width;
        pic.Height = height;
        pic.Maxval = maxval;

    }
    else rtv = false;

    return rtv;
}; 

void LoadImage(ifstream &infile, PIC &pic)
{
    infile.read(reinterpret_cast<char *>(pic.img), pic.Width*pic.Height);
}

void LaplacianHW(int newinp[][833], int i, int h, int w, int k, int j)
{
        int logmask[5][5]={ {0, 0, -1, 0, 0},
                            {0, -1, -2, -1, 0},
                            {-1, -2, 16, -2, -1},
                            { 0, -1, -2, -1, 0},
                            {0, 0, -1, 0, 0} };
      
        int l1,l2=3; //3
        int temp;l1=5; //5

        j=0;
        int l=0;
        int border1=(l1/2);
        int border2=(l2/2);

      
        int m;
        int spix;
        //
        for (i=border1;i<(h-border1);i++)
        {
            for (j=border1;j<(w-border1);j++)
            {
                spix=0;
                for (k=0;k<l1;k++)
                {           
                    for (m=0;m<l1;m++)
                    { 
                        spix = spix+(logmask[k][m]*(inp[i-border1+k][j-border1+m]));
                    }

                }   
               
                   newinp[i][j]=spix;  
            }
        }
       
       

        int max=0, th;
        for (k=0;k<h;k++)
        {
            for (i=0;i<w;i++)
            {
                if (newinp[k][i]>max)
                {
                    max= newinp[k][i];
                }

            }

        } 
       th=int(.075*max);
       
         i=0;j=0;

   
        
        for (i=border2;i<(h-border2);i++)
        {    //j=0;
            for (j=border2;j<(w-border2);j++)
            {
                if ( newinp[i][j]!=0)
                {
                    if ((newinp[i][j+1]>=0 && newinp[i][j-1]<0) || (newinp[i][j+1]<0 && newinp[i][j-1]>=0))
                          {    
                              if ((newinp[i][j]>=th))
                                   { 
                                       im[i][j]=255;
                                    } 
                          }
                    
                    else if ((newinp[i+1][j]>=0 && newinp[i-1][j]<0) || (newinp[i+1][j]<0 && newinp[i-1][j]>=0))
                            { if (newinp[i][j]>=th)
                                 {  
                                    im[i][j]=255;
                                 }
                             }

                    else if ((newinp[i+1][j+1]>=0 && newinp[i-1][j-1]<0) || (newinp[i+1][j+1]<0 && newinp[i-1][j-1]>=0))
                             { if (newinp[i][j]>=th)
                                   {
                                    im[i][j]=255;
                                   }
                             }
                    
                    else if ((newinp[i-1][j+1]>=0 && newinp[i+1][j-1]<0) || (newinp[i-1][j+1]<0 && newinp[i+1][j-1]>=0))
                            { if (newinp[i][j]>=th)
                                 {
                                    im[i][j]=255;
                                 }
                            }
                }
            }
        }
           
 }


 
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
    int dx1 = pt1.x - pt0.x;
    int dy1 = pt1.y - pt0.y;
    int dx2 = pt2.x - pt0.x;
    int dy2 = pt2.y - pt0.y;

    return(dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
    
    
}

void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
{
    int fontface = cv::FONT_HERSHEY_SIMPLEX;
    double scale = 0.4;
    int thickness = 1;
    int baseline = 0;
    
    cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
    cv::Rect r = cv::boundingRect(contour);

    cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
    cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255,255,255), cv::FILLED);
    cv::putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
}

int main(int argc, char* argv[])
{
	ifstream infile_Pin(argv[1], std::ios::in);                
    
    int scale;
    
    int Npixels, i,l;
    
    
    PIC Pin;            

    

    if (LoadP5Header(infile_Pin, Pin)) // load pgm (Pin) image header information
    {
 
        Pin.img = new BYTE[Pin.Width*Pin.Height];

        LoadImage(infile_Pin, Pin);
    
    
        int w,h;
        w=(Pin.Width);
        h=(Pin.Height);
       
        
        int j=0,k;
    
       for (k=0;k<h;k++)
       {
          for (i=0;i<w;i++)
           {
            inp[k][i]=Pin.img[j];

            newinp[k][i]=0;
            im[k][i]=0;
                j++;                 
           } 
       }
      
     LaplacianHW(newinp ,i,h, w, k, j);
     }
     cv::Mat src = cv::imread(argv[1]); 
        if (src.empty())
            return -1;

     cv::Mat Laplacian = cv::Mat(833, 833, CV_8UC1, im);
     Laplacian.data = im[0];

     im[0][0] = uchar(0);

     cv::imshow("Laplacian", Laplacian);

    std::vector<std::vector<cv::Point> > contours;
    

    cv::findContours(Laplacian.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);


    std::vector<cv::Point> approx;
    cv::Mat dst = src.clone();

    for (int i = 0; i < contours.size(); i++)
    {
        
        cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);


        
        int vtc = approx.size();

        if (std::fabs(cv::contourArea(contours[i])) > 100 && cv::isContourConvex(approx)) {

            if (approx.size() == 3)
            {
                setLabel(dst, "TRIANGLE", contours[i]);    // Triangles
            }
            else if (approx.size() >= 4 && approx.size() <= 12)
            {

              
                std::vector<double> cos;
                for (int j = 2; j < vtc + 1; j++)
                    cos.push_back(angle(approx[j%vtc], approx[j - 2], approx[j - 1]));

              
                std::sort(cos.begin(), cos.end());

               
                double mincos = cos.front();
                double maxcos = cos.back();

                


                
                if (vtc == 4 && mincos >= -0.15 && maxcos <= 0.15 &&
                    (cv::minAreaRect(contours[i]).size.width / cv::minAreaRect(contours[i]).size.height > 0.8 &&
                        cv::minAreaRect(contours[i]).size.width / cv::minAreaRect(contours[i]).size.height < 1.2)) {
                    setLabel(dst, "SQUARE", contours[i]);
                }

                
                else if (vtc == 4 && mincos >= -0.15 && maxcos <= 0.15)
                    setLabel(dst, "RECTANGLE", contours[i]);

               
                else if (vtc == 4 && mincos <= -0.2)
                    setLabel(dst, "TRAPEZOID", contours[i]);

                
                else if (vtc == 5 && mincos >= -0.5 && maxcos <= -0.05)
                    setLabel(dst, "PENTAGON", contours[i]);

                
                else if(vtc == 5 && mincos >= -0.85 && maxcos <= 0.6)
                    setLabel(dst, "SEMI-CIRCLE", contours[i]);

                else if (vtc == 5 && mincos >= -1 && maxcos <= 0.6)
                    setLabel(dst, "QUARTER-CIRCLE", contours[i]);

                
                else if (vtc == 6 && mincos >= -0.7 && maxcos <= -0.3)
                    setLabel(dst, "HEXA", contours[i]);

                
                else if (vtc == 6 && mincos >= -0.85 && maxcos <= 0.6)
                    setLabel(dst, "SEMI-CIRCLE", contours[i]);

                
                else if (vtc == 6 && mincos >= -1 && maxcos <= 0.6)
                    setLabel(dst, "QUARTER-CIRCLE", contours[i]);

                
                else if (vtc == 7)
                    setLabel(dst, "HEPTA", contours[i]);

               
                else if (vtc == 8) {
                    
                    double area = cv::contourArea(contours[i]);
                    cv::Rect r = cv::boundingRect(contours[i]);
                    int radius = r.width / 2;
                    if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
                        std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2) {
                        setLabel(dst, "CIRCLE", contours[i]);
                    }
                    
                   
                    else
                        setLabel(dst, "ELLIPSE", contours[i]);
                }

            }
        }

       
        else if (std::fabs(cv::contourArea(contours[i])) > 100) {
            if (vtc == 4)
                setLabel(dst, "QUADRANGULAR", contours[i]);
            if (vtc == 5)
                setLabel(dst, "PENTAGON", contours[i]);
            if (vtc == 6)
                setLabel(dst, "HEXAGON", contours[i]);
            if (vtc == 7)
                setLabel(dst, "HEPTAGON", contours[i]);
            if (vtc == 10)
                setLabel(dst, "STAR", contours[i]);
            else if (vtc == 12)
                setLabel(dst, "CROSS", contours[i]);
            else
                setLabel(dst, "UNKNOWN", contours[i]);
        }
        
        
    }
    cv::imshow("INPUT", src);
    cv::imshow("OUTPUT", dst);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}
