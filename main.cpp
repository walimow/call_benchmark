
#include <iostream>
#include <functional>


/*
struct IiFnc
{
	virtual ~IiFnc(){}
	virtual int operator()(int)=0;
};


typedef  int (*FT)(int);

struct FiFunc: public IiFnc
{
	//typedef decltype ( [](int)->{} ) FT;
	FT _ft;
	FiFunc(FT ft):_ft(ft){}
	int operator()(int v)override{
		return _ft(v);
	}
};



int main(int argc, char **argv) {
    
    FiFunc fminus([](int i)->int{ return -i;} )	;
    
    int res=fminus(9);
    
    std::cout<<"\nMinus: "<<res<<"\n";
	
    return 0;
}

*/

//


#include <boost/date_time/posix_time/posix_time.hpp>


using namespace std::placeholders;
// Get current date/time in milliseconds.





unsigned long long  addf(unsigned long long  add, unsigned char i) {
	return add + i;
}



template<typename R, typename F, typename L=R, typename X=L>
struct I_FLX2R
{
	R operator()(const F& f, const X&x) { return F(f,x); }
};


template<typename R,  typename A = R, typename X = A>
struct I_LX2R
{
	virtual R operator()(const A& f, const X&x) = 0;
};


template< typename R, typename F, typename L = R, typename X = L>
struct TLX2R
{

	typedef  R (*FT)(const F& f, const L& l);
	FT _ft;
	TLX2R(FT ft):_ft(ft){}
	R operator()(const F& f, const X&x) { return _ft(f, x); }
	
};


//typedef  int (*FT)(int);





struct _f_add
{
	
	unsigned long long  operator()(unsigned long long  add, unsigned char i)const  {
		return add + i;
	}
	
};



struct I_f221
{
	
	virtual unsigned long long  operator()(unsigned long long  add, unsigned char i)const  = 0;
};

struct _f_add22: public I_LX2R<unsigned long long, unsigned long long, unsigned char>
{
	
	unsigned long long  operator()(unsigned long long  add, unsigned char i) const {
		return add + i ;
	}
	
};

struct _f_add2 : public I_f221
{
	
	unsigned long long  operator()(unsigned long long  add, unsigned char i) const override {
		return add + i;
	}
	
};


unsigned long long sz = 140000000;

int main()
{
	{
		boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();
		unsigned long long ul = 0;
		
		for (unsigned long long i = 0; i < sz; i++)
			ul= addf(ul, i);
		
		
		boost::posix_time::ptime t2 = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_duration diff = t2 - t1;
		std::cout<< "\nTest1: " << diff.total_milliseconds() << " / "<< ul << std::endl;
	}
	
	{
		typedef std::function<long long(unsigned long long, unsigned char)> FT;
		FT fadd = std::bind(addf, _1, _2);
		unsigned long long ul = 0;
		boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();
		
		for (unsigned long long i = 0; i < sz; i++)
			ul = _f_add() (ul, i);
		
		boost::posix_time::ptime t2 = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_duration diff = t2 - t1;
		std::cout << "\nTest2+: " << diff.total_milliseconds() << " / " << ul << std::endl;
	}
	
	{
		//typedef std::function<long long(unsigned long long, unsigned char)> FT;
		const I_f221& fref  = _f_add2();
		unsigned long long ul = 0;
		boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();
		
		for (unsigned long long i = 0; i < sz; i++)
			ul = fref(ul, i);
		
		boost::posix_time::ptime t2 = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_duration diff = t2 - t1;
		std::cout << "\nTest2/2: " << diff.total_milliseconds() << " / " << ul << std::endl;
	}
	
	{
		
		TLX2R<unsigned long long,unsigned long long, unsigned char>	
			ff( [](const unsigned long long& a, const unsigned char& b)->unsigned long long{ return a+b;});
			
			unsigned long long ul = 0;
			boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();
			
			for (unsigned long long i = 0; i < sz; i++)
				ul = ff(ul, i);
			
			boost::posix_time::ptime t2 = boost::posix_time::second_clock::local_time();
			boost::posix_time::time_duration diff = t2 - t1;
			std::cout << "\nTest2.b: " << diff.total_milliseconds() << " / " << ul << std::endl;
		
	}
	
	{
		typedef std::function<long long(unsigned long long, unsigned char)> FT;
		FT fadd = std::bind(addf, _1,_2 );
		unsigned long long ul = 0;
		boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();
		
		for (unsigned long long i = 0; i < sz; i++)
			ul = fadd(ul, i);
		
		boost::posix_time::ptime t2 = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_duration diff = t2 - t1;
		std::cout << "\nTest2x: " << diff.total_milliseconds() << " / " << ul << std::endl;
	}
	
	
	return 0;
}

