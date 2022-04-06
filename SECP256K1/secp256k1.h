#pragma once

#include "pch.h"



#define COUTd(x) std::cout << x << std::endl << std::endl
#define COUT(x) std::cout << x << std::endl
#define COUTh(x) std::cout << std::hex << x  << std::endl
#define ENDL std::cout << std::endl

#ifdef _DEBUG //measure code execution
#define _st uint32_t start_time = clock(); // start point
#define _en uint32_t end_time = clock(); uint32_t search_time = end_time - start_time;//end point
#define _re  std::cout << std::endl << "Time:" << search_time/1000.0 <<" sec" << std::endl;
#endif

#define INT256_T boost::multiprecision::int256_t
#define INT1024_T boost::multiprecision::int1024_t
#define CHR_ZERO 48;
#define CHR_ONE 49;

struct point
{
	INT256_T pointX;
	INT256_T pointY;

	bool operator==(const point& a) const
	{
		return (pointX == a.pointX && pointY == a.pointY);
	}
};

class secp256k1_my
{
public:
	secp256k1_my();
	~secp256k1_my();

	template<typename T>
	static T mod(T a, T base);

	template <typename T>
	T ModInv(T a, T n);
	std::string convertToString(char* a, int size);
	point ECdouble(point a);
	point ECadd(point a, point b);
	point ECmult(const point& g, const INT256_T& k);
	std::string ReturnArrBits(unsigned char* inArr, int len);
	void* Swapbytes(void* inp, size_t len);

};

