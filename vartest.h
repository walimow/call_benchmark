#pragma once

#include <boost/variant.hpp>

struct op_add
{
	  unsigned long long  plus(unsigned long long v, unsigned char  x) const { return v + x; }
};

struct op_min
{
	  unsigned long long  minus(unsigned long long v, unsigned char  x) const { return v - x; }
};


class op_visitor : public boost::static_visitor<unsigned long long>
{
	const unsigned long long _ul;
	const unsigned char _uc;
public:
	op_visitor(unsigned long long ul, unsigned char uc) :_ul(ul), _uc(uc) {}
	inline  unsigned long long operator()( const op_add&op) const
	{
		return op.plus(_ul,_uc);
	}

	inline  unsigned long long operator()(const op_min&op) const
	{
		return op.minus(_ul, _uc);
	}
};


typedef boost::variant<op_add, op_min>  OpBinType;

unsigned long long op_bin(const OpBinType& op, unsigned long long  v, unsigned char s) {
	return boost::apply_visitor(op_visitor(v,s), op);
}

/*
class op_visitor2 : public boost::static_visitor<unsigned long long>
{
	const unsigned long long _ul;
//	const unsigned char _uc;
public:
	op_visitor2(unsigned long long ul) :_ul(ul) {}
	inline  unsigned long long operator()(const op_add&op, unsigned long long ul, unsigned char uc) const
	{
		return op.plus(_ul, uc);
	}

	inline  unsigned long long operator()(const op_min&op, unsigned long long ul, unsigned char uc) const
	{
		return op.minus(_ul, uc);
	}
};

unsigned long long op_bin(const OpBinType& op, unsigned long long  v, unsigned char s) {
	return boost::apply_visitor(op_visitor2(v), op, s);
}*/