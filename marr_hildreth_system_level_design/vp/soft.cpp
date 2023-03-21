
#include "soft.hpp"
char** data_string;
int argc;
ofstream simTime;

unsigned char* pic_to_memory;
unsigned char* finish;
tlm_utils::tlm_quantumkeeper qk;
SC_HAS_PROCESS(Soft);

Soft::Soft(sc_core::sc_module_name name,char** strings, int argv) : sc_module(name), offset(sc_core::SC_ZERO_TIME)
{

    soft_target_dma.register_b_transport(this, &Soft::b_transport);

    SC_THREAD(software);
    SC_REPORT_INFO("Soft", "Constructed.");
	data_string = strings;
	argc = argv;
}

Soft::~Soft()
{
    SC_REPORT_INFO("Soft", "Destroyed.");
}

void Soft::software()
{
    simTime.open("simulationTime.txt");
	int ad = 0;
    cout << "SOFT::Welcome to software" << endl;
    ifstream infile_Pin(data_string[1], std::ios::in);    
    
    int scale;
    
    int Npixels,l;
    int i;    
    
    PIC Pin;
    pl_t pl;
    offset = SC_ZERO_TIME;
    simTime << "simTime start := " << offset << "\n";

    unsigned char* picture_laplacian;

    tlm_utils::tlm_quantumkeeper qk;
    qk.reset();

    if (Load5Header(infile_Pin, Pin)) // load pgm (Pin) image header information
    {
        Pin.img = new BYTE[Pin.Width*Pin.Height];
        LoadImage(infile_Pin, Pin);
        
        w=(Pin.Width);
        h=(Pin.Height);
      cout << "Software: Number of rows :" << h << endl;
       cout << "Software: Number of columns :" << w << endl;

       write_hard(VP_IP_ADDR_HEIGHT, h);
       qk.set_and_sync(offset);
       simTime << "simTime after comunications with IP(*WRITE,height) := " << offset <<"\n";
       
       write_hard(VP_IP_ADDR_WIDTH, w);
       qk.set_and_sync(offset);
       simTime << "simTime after comunications with IP(*WRITE,width) := " << offset <<"\n";
       
       int j=0,k;

       pic_to_memory = new unsigned char[w*h];

        for (k=0;k<h;k++)
       {
          for (i=0;i<w;i++)
           {
            pic_to_memory[j] = Pin.img[j];
            j++;                 
           } 
       }
        
        sc_uint<32> count = h*w;
        cout << "Count is : " << count << endl;

        pl.set_command(TLM_WRITE_COMMAND);
        pl.set_address(VP_DMA_COUNT_ADDR);
        pl.set_data_ptr((unsigned char*)&count);
        pl.set_data_length(6);
        cout<<"**SOFT: Sending the maximum amount of data! "<<endl;
        
        qk.inc(sc_time(10, SC_NS));
        //offset = qk.get_local_time();
        soft_ic_i->b_transport(pl, offset);
        qk.set_and_sync(offset);
        simTime << "simTime after comunications with DMA(*WRITE,count) := " << offset <<"\n";


        
        sc_uint<32> dest_addr = VP_IP_HARD;
        pl.set_command(TLM_WRITE_COMMAND);
        pl.set_address(VP_DMA_DESTINATION_ADDR);
        pl.set_data_ptr((unsigned char*)&dest_addr);
        pl.set_data_length(4);
        cout << "SOFT: Sending destination to DMA!" << endl;
        qk.inc(sc_time(10, SC_NS));
        //offset = qk.get_local_time();
        soft_ic_i->b_transport(pl, offset);
        qk.set_and_sync(offset);
        simTime << "simTime after comunications with DMA(*WRITE,destination) := " << offset <<"\n";
        simTime << "\n";

        sc_uint<32> dma_control = 1;
        pl.set_command(TLM_WRITE_COMMAND);
        pl.set_address(VP_DMA_CONTROL_ADDR);
        pl.set_data_ptr((unsigned char*)&dma_control);
        pl.set_data_length(1);
        cout << "SOFT: Sending a control bit" << endl;
        
        qk.inc(sc_time(10, SC_NS));
        //offset = qk.get_local_time();
        soft_ic_i->b_transport(pl, offset);
        qk.set_and_sync(offset);
        simTime << "simTime after comunications with DMA(*WRITE,dma_control).\n";
        simTime << "------>DMA Transfer time to IP (+1000ns)  := " << offset <<"\n";

        sc_uint<2> ip_hard_START = 1;
        unsigned int ip_hard_LEN = 1;

        pl.set_command(TLM_WRITE_COMMAND);
        pl.set_address(VP_IP_ADDR_START);
        pl.set_data_ptr((unsigned char*)&ip_hard_START);
        pl.set_data_length(ip_hard_LEN);

        qk.inc(sc_time(10, SC_NS));
            //offset = qk.get_local_time();
        cout << "**SOFT: Sending a start bit !" << endl;
        soft_ic_i->b_transport(pl, offset);
        qk.set_and_sync(offset);
        simTime << "\n";
        simTime << "simTime after comunications with DMA(*WRITE, ip_hard_START).\n";
        simTime << "----->IP hardware Laplacian time (+2000)  := " << offset <<"\n";
        simTime << "\n";

 
        sc_uint<2> ip_hard;
        //read_hard(VP_IP_ADDR_START, ip_hard);

        pl.set_address(VP_IP_ADDR_START); // 0x11880001
        pl.set_data_length(1); //
        pl.set_data_ptr((unsigned char*)&ip_hard);  // 392
        pl.set_command(tlm::TLM_READ_COMMAND); // upisi 

        pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
        soft_ic_i->b_transport(pl,offset); // posalji 
        qk.set_and_sync(offset);
        simTime << "simTime after comunications with IP(*READ,ip_hard) := " << offset <<"\n";
  
        if(ip_hard == 0x2)
        {
            sc_uint<32> dest_addr = VP_MEMORY;
            pl.set_command(TLM_WRITE_COMMAND);
            pl.set_address(VP_DMA_DESTINATION_ADDR);
            pl.set_data_ptr((unsigned char*)&dest_addr);
            pl.set_data_length(4);
            cout << "**SOFT: Sending destination to DMA!" << endl;
            qk.inc(sc_time(10, SC_NS));
                    //offset = qk.get_local_time();
            soft_ic_i->b_transport(pl, offset);
            qk.set_and_sync(offset);
            simTime << "simTime after comunications with DMA(*WRITE,destination) := " << offset <<"\n";
            simTime << "\n";

            dma_control = 1;
            pl.set_command(TLM_WRITE_COMMAND);
            pl.set_address(VP_DMA_CONTROL_ADDR);
            pl.set_data_ptr((unsigned char*)&dma_control);
            pl.set_data_length(1);
            cout << "**SOFT: Sending a control bit" << endl;
            qk.inc(sc_time(10, SC_NS));
                    //offset = qk.get_local_time();
            soft_ic_i->b_transport(pl, offset);
            qk.set_and_sync(offset);

            simTime << "simTime after comunications with DMA(*WRITE,dma_control).\n";
            simTime << "------>DMA Transfer time to SOFT (+1000ns)  := " << offset << "\n";
            simTime << "\n";
        }



        cout << "**SOFT: --GAME OVER--" << endl;
        
        simTime << "simTime GAME OVER := " << offset <<"\n";
        simTime.close();

        cv::Mat Laplacian = cv::Mat(392, 833, CV_8UC1, pic_to_memory);
        cv::Mat Laplacian1 = cv::Mat(392, 833, CV_8UC1, finish);
        

        std::vector<std::vector<cv::Point> > contours;

        cv::findContours(Laplacian1.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        std::vector<cv::Point> approx;
        
        cv::Mat dst = Laplacian.clone();

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
        cv::imshow("Input", Laplacian);
        cv::imshow("Laplacian", Laplacian1);
        cv::imshow("Output", dst);
        cv::waitKey(0);   
    }


}

bool Soft::Load5Header(ifstream &infile, PIC &pic)
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
        height = atoi(buf); //stao ovdje

    
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

void Soft::LoadImage(ifstream &infile, PIC &pic)
{
    infile.read(reinterpret_cast<char *>(pic.img), pic.Width*pic.Height);
};

void Soft::write_hard(sc_dt::uint64 addr,int val)
{
    pl_t pl;

    unsigned char buf[4];

    toUchar(buf,val); 

    pl.set_address(addr);
    pl.set_data_length(5);
    pl.set_data_ptr(buf);
    pl.set_command(tlm::TLM_WRITE_COMMAND);
    pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    
    qk.inc(sc_time(10,SC_NS));
    //offset = qk.get_local_time();
    soft_ic_i->b_transport(pl,offset);

};
void Soft::read_hard(sc_dt::uint64 addr,sc_uint<2> val)
{
    pl_t pl;
    //unsigned char buf[4];

    //toUchar(buf,val); 

    pl.set_address(addr); // 0x11880001
    pl.set_data_length(1); //
    pl.set_data_ptr((unsigned char*)&val);  // 392
    pl.set_command(tlm::TLM_READ_COMMAND); // upisi 
    pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    soft_ic_i->b_transport(pl,offset); // posalji 
};

void Soft::toUchar(unsigned char *buf,int val)
{
    buf[0] = (char) (val >> 24);
    buf[1] = (char) (val >> 16);
    buf[2] = (char) (val >> 8);
    buf[3] = (char) (val);
};
int Soft::toInt(unsigned char *buf)
{
    int val = 0;
 
    val += ((int)buf[0]) << 24;
    val += ((int)buf[1]) << 16;
    val += ((int)buf[2]) << 8;
    val += ((int)buf[3]);

    return val;
}

void Soft::b_transport(pl_t &pl, sc_time &offset)
{
    cout << "**SOFT: B_TRANSPORT !" << endl << endl;
    tlm_command cmd = pl.get_command(); //read
    sc_dt::uint64 addr = pl.get_address(); //VP_DMA
   unsigned char *data = pl.get_data_ptr(); // picture_laplacian
    unsigned int len = pl.get_data_length(); // 4

    switch(cmd)
    {
       case TLM_WRITE_COMMAND:
        finish = new unsigned char[h*w];
            for (int i = 0; i < h*w; ++i)
            {
                    finish[i] = data[i];
            }
        break;

        case TLM_READ_COMMAND:
            cout << "**SOFT: TLM_READ !" << endl << endl;
                for (int i = 0; i < h*w; ++i)
                {
                    data[i] = pic_to_memory[i];

                }
        break;

        default:
             pl.set_response_status( TLM_COMMAND_ERROR_RESPONSE );
      break;

   offset += sc_time(5, SC_NS);
    }

}

void Soft::setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
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

double Soft::angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
    int dx1 = pt1.x - pt0.x;
    int dy1 = pt1.y - pt0.y;
    int dx2 = pt2.x - pt0.x;
    int dy2 = pt2.y - pt0.y;

    return(dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
    
    
}


