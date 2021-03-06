
#ifndef DAPPCLI_TIME_SAVE_HPP
#define DAPPCLI_TIME_SAVE_HPP

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp> 
#include <string>
 
#include <iostream>
#include <fstream>

class timeSave {
public:
    timeSave(boost::asio::io_context& io,std::string file) :strand_(io),
    save_file(file),
    timer1_(io,boost::asio::chrono::seconds(1)),
	timer2_(io,boost::asio::chrono::seconds(1)),
	count_(0)
	{
        timer1_.async_wait(boost::asio::bind_executor(strand_,boost::bind(&timeSave::onSave1,this)));
        timer2_.async_wait(boost::asio::bind_executor(strand_,boost::bind(&timeSave::onSave2,this)));
    }

    ~timeSave() {
        std::cout << "Final count is " << count_ << std::endl;
    }

    void onSave1() {
        
        //std::cout << "Timer 1: " << count_ << std::endl;
        ++count_;
		saveData();	
		timer1_.expires_at(timer1_.expiry() + boost::asio::chrono::seconds(5));
		timer1_.async_wait(boost::asio::bind_executor(strand_,boost::bind(&timeSave::onSave1,this)));        
    }

    void onSave2() {
        if (count_ < 10) {
            //std::cout << "Timer 2: " << count_ << std::endl;
            ++count_;
            timer2_.expires_at(timer2_.expiry() + boost::asio::chrono::seconds(6));
            timer2_.async_wait(boost::asio::bind_executor(strand_,boost::bind(&timeSave::onSave2,this)));
        }
    }
	
	void updateData(std::string  hostdata)
	{
		int pos = hostdata.find("\n");
		std::string keystr = hostdata.substr(0,pos);
		uosdata[keystr] = hostdata;
	}
	
private:

	//save disk data
	void  saveData()
	{		
	
	    std::string strFile = save_file;
	    std::string strHtml = strFile;
        strHtml.replace(strHtml.end()-3,strHtml.end(),"html"); 

	    std::ofstream outfile(strFile.c_str());   
	    std::ofstream outhtml(strHtml.c_str());   
		outhtml<< "<body><p>" << std::endl;
        int count=0; 
		for(auto iter = uosdata.begin(); iter != uosdata.end(); iter++)  			
		{	
            count++;
			outfile<< iter->second << std::endl;
            std::string outputstr = iter->second;
            boost::algorithm::replace_all(outputstr,"\n","<br/>" );
		    outhtml<< "<br/>"<< outputstr  << std::endl;
			//cout<<iter->first<<' '<<iter->second<<endl;   
		}	
        outfile<<"total    "<<count <<std::endl; 		
        outhtml<<"total    "<<count <<std::endl; 		
		outhtml<< "<br/> </body></p>" << std::endl;

		outfile.close();
		outhtml.close();

	    if(count_>24*60*10)	
		{  
           uosdata.erase( uosdata.begin(), uosdata.end() );  
           count_=0;//如果有程序退出.保持一天的状态 
        }
	}
    std::string save_file;	
	std::map<std::string,std::string> uosdata;
	
    boost::asio::steady_timer timer1_;
    boost::asio::steady_timer timer2_;
    int count_;
    boost::asio::io_context::strand strand_;
};
 
timeSave  * g_uosSave;

#endif //DAPPCLI_TIME_SAVE_HPP

