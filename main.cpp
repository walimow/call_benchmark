// bench_fnc.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

//#include "stdafx.h"
//#include <inctest.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <functional>

#include "vartest.h"

using namespace std::placeholders;

unsigned long long  addf(const unsigned long long&  add, const unsigned char& i) {
	return add + i;
}

template<typename R,  typename A = R, typename X = A>
struct I_LX2R
{
	virtual R operator()(const A& f, const X&x)const  = 0;
};

template< typename R,  typename L = R, typename X = L>
struct TLX2R: public I_LX2R<R, L, X>
{

	typedef   R (*FT) (const L& f, const  X& l);
	FT _ft;
	TLX2R(FT ft) :_ft(ft) {}
	 R operator()(const L& f, const X&x)const  override { return _ft(f, x); }

};

template< typename R, typename L = R, typename X = L>
struct TLX2RDirect 
{
	typedef   R(*FT) (const  L& f, const  X& l);
	FT _ft;
	TLX2RDirect(FT ft) :_ft(ft) {}
	 R operator()(const L& f, const X&x) { return _ft(f, x); }

};

struct _f_add
{
	unsigned long long  operator()(const unsigned long long&  add, const unsigned char& i) {
		return add + i;
	}		
};


///fuctor with cargo
template <typename RES, typename LEFT= RES, typename RIGHT=LEFT>
struct Dual
{
	typedef RES(*OP)(LEFT, RIGHT);
	OP _op;
	Dual(OP op) :_op(op) {}
	
	RES operator() (LEFT l, RIGHT r) {
		return _op(l, r);
	}
};

template <typename RES, typename LEFT = RES, typename RIGHT = LEFT>
struct _Dual_1
{
	typedef RES(*OP)(RIGHT);
	OP _op;
	const LEFT _l;
	_Dual_1(OP op, const LEFT& l) :_op(op) {}

	RES operator() ( RIGHT r) {
		return _op(_l, r);
	}
};


struct _f_add2 : // public I_f221
	public I_LX2R<unsigned long long, unsigned long long, unsigned char>
{

	unsigned long long  operator()( const unsigned long long&  add, const unsigned char& i)const override {
		return add + i;
	}

};


unsigned long long sz = 400000000;

int main()
{
	{
	boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();
	unsigned long long ul = 0;
	
	for (unsigned long long i = 0; i < sz; i++)
		ul= addf(ul, (unsigned char)i);


	boost::posix_time::ptime t2 = boost::posix_time::second_clock::local_time();
	boost::posix_time::time_duration diff = t2 - t1;
	std::cout<< "\ndirect(stat. function): " << diff.total_milliseconds() << " / "<< ul << std::endl;
	}

	{
		typedef std::function<long long(unsigned long long, unsigned char)> FT;
		FT fadd = std::bind(addf, _1, _2);
		unsigned long long ul = 0;
		boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();

		for (unsigned long long i = 0; i < sz; i++)
			ul = _f_add() (ul, (unsigned char)i);

		boost::posix_time::ptime t2 = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_duration diff = t2 - t1;
		std::cout << "\nfunktor: " << diff.total_milliseconds() << " / " << ul << std::endl;
	}

	{
//		typedef std::function<long long(unsigned long long, unsigned char)> FT;
		const I_LX2R<unsigned long long, unsigned long long, unsigned char >&
		/*I_f221&*/  fref = _f_add2();
		unsigned long long ul = 0;
		boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();

		for (unsigned long long i = 0; i < sz; i++)
			ul = fref(ul, (unsigned char)i);

		boost::posix_time::ptime t2 = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_duration diff = t2 - t1;
		std::cout << "\nfunktor (abstract Basis, called by virtually on base): " << diff.total_milliseconds() << " / " << ul << std::endl;
	}

	{
//		typedef std::function<long long(unsigned long long, unsigned char)> FT;
		const auto& fref = _f_add2();
		unsigned long long ul = 0;
		boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();

		for (unsigned long long i = 0; i < sz; i++)
			ul = fref(ul, (unsigned char)i);

		boost::posix_time::ptime t2 = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_duration diff = t2 - t1;
		std::cout << "\nfunktor (abstract base, direktly called ): " << diff.total_milliseconds() << " / " << ul << std::endl;
	}

	{
		TLX2R<unsigned long long, unsigned long long, unsigned char>
			ff([](const unsigned long long& a, const unsigned char& b)->unsigned long long { return a + b; });

		unsigned long long ul = 0;
		boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();

		for (unsigned long long i = 0; i < sz; i++)
			ul = ff(ul, (unsigned char)i);

		boost::posix_time::ptime t2 = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_duration diff = t2 - t1;
		std::cout << "\nabstr. base + template impl: " << diff.total_milliseconds() << " / " << ul << std::endl;

	}

	{
		TLX2RDirect<unsigned long long, unsigned long long, unsigned char>
			ff([](const unsigned long long& a, const unsigned char& b)->unsigned long long { return a + b; });

		unsigned long long ul = 0;
		boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();

		for (unsigned long long i = 0; i < sz; i++)
			ul = ff(ul, (unsigned char)i);

		boost::posix_time::ptime t2 = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_duration diff = t2 - t1;
		std::cout << "\nabstr. basisklasse + template impl (direct ): " << diff.total_milliseconds() << " / " << ul << std::endl;

	}
	{
		typedef  unsigned long long(*FPlus)(unsigned long long , unsigned char);
		FPlus op = [](unsigned long long l, unsigned char r)->unsigned long long {return l + r; };

		unsigned long long ul = 0;
		boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();

		for (unsigned long long i = 0; i < sz; i++)
			ul = op(ul, (unsigned char)i);

		boost::posix_time::ptime t2 = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_duration diff = t2 - t1;
		std::cout << "\nfptr: " << diff.total_milliseconds() << " / " << ul << std::endl;
	
	}
	
	{
		typedef std::function<long long(unsigned long long, unsigned char)> FT;
		FT fadd = std::bind(addf, _1,_2 );
		unsigned long long ul = 0;
		boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();

		for (unsigned long long i = 0; i < sz; i++)
			ul = fadd(ul, (unsigned char)i);

		boost::posix_time::ptime t2 = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_duration diff = t2 - t1;
		std::cout << "\nstd::function(bind): " << diff.total_milliseconds() << " / " << ul << std::endl;
	}
	
	{
		
		OpBinType xop= op_add();

		unsigned long long ul = 0;
		boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();

		for (unsigned long long i = 0; i < sz; i++)
			ul = op_bin(xop, ul, (unsigned char)i);
			

		boost::posix_time::ptime t2 = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_duration diff = t2 - t1;
		std::cout << "\nstd::variant: " << diff.total_milliseconds() << " / " << ul << std::endl;
	}



	{

		Dual<unsigned long long, unsigned long long, unsigned char>   fnc([](unsigned long long l, unsigned char r)->unsigned long long { return l + r; });

		unsigned long long ul = 0;
		boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();

		for (unsigned long long i = 0; i < sz; i++)
			ul = fnc(ul, (unsigned char)i);


		boost::posix_time::ptime t2 = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_duration diff = t2 - t1;
		std::cout << "\nstructures templeted fnccall: " << diff.total_milliseconds() << " / " << ul << std::endl;
	}

	{

		Dual<unsigned long long, const unsigned long long&, const unsigned char&>   fnc([](const unsigned long long& l, const unsigned char& r)->unsigned long long { return l + r; });

		unsigned long long ul = 0;
		boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();

		for (unsigned long long i = 0; i < sz; i++)
			ul = fnc(ul, (unsigned char)i);


		boost::posix_time::ptime t2 = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_duration diff = t2 - t1;
		std::cout << "\nstructures templeted fnccall(ref): " << diff.total_milliseconds() << " / " << ul << std::endl;
	}

/*	{

		_Dual_1<unsigned long long, const unsigned long long&, const unsigned char&>   fnc([](const unsigned long long& l, const unsigned char& r)->unsigned long long { return l + r; });

		unsigned long long ul = 0;
		boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();

		for (unsigned long long i = 0; i < sz; i++)
			ul = fnc(ul, (unsigned char)i);


		boost::posix_time::ptime t2 = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_duration diff = t2 - t1;
		std::cout << "\nstructures templeted fnccall: " << diff.total_milliseconds() << " / " << ul << std::endl;
	}*/


    return 0;
}

