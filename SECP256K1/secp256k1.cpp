#include "pch.h"
#include "secp256k1.h"

secp256k1_my::secp256k1_my() {};
secp256k1_my::~secp256k1_my() {};

INT256_T pCurve("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");
INT256_T n("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");

template<typename T>
static T secp256k1_my::mod(T a, T base) {	
	return (a < 0 ? ((a % base) + base) % base : a % base);
}

template <typename T>
T secp256k1_my::ModInv(T a, T n)
{
	T lm = 1;
	T hm = 0;
	T low = mod<T>(a, n);
	T high = n;
	T ratio;
	T nm;
	T news;
	while (low > 1) {
		ratio = high / low;
		nm = hm - lm * ratio;
		news = high - low * ratio;

		T lmT = lm;
		T lowT = low;
		T hmT = hm;
		T highT = high;

		lmT = nm;
		lowT = news;
		hmT = lm;
		highT = low;

		lm = lmT;
		low = lowT;
		hm = hmT;
		high = highT;

	}
	
	return mod<T>(lm, n);
}

point secp256k1_my::ECdouble(point a)
{
	// slope = (3x ^ 2 + a) / 2y
	INT1024_T multTemp = (3 * (INT1024_T)a.pointX * (INT1024_T)a.pointX + 0) * ModInv<INT1024_T>((2 * (INT1024_T)a.pointY), (INT1024_T)pCurve);
	INT1024_T Lam = mod<INT1024_T>(multTemp, pCurve);
	// x = slope ^ 2 - 2x
	INT1024_T xTemp = Lam * Lam - (2 * (INT1024_T)a.pointX);
	INT256_T x = (INT256_T)mod<INT1024_T>(xTemp, pCurve);

	// y = slope * (x - nx)
	INT1024_T yTemp = Lam * ((INT1024_T)a.pointX - x) - (INT1024_T)a.pointY;
	INT256_T y = (INT256_T)mod<INT1024_T>(yTemp, pCurve);
	point ret;
	ret.pointX = x;
	ret.pointY = y;
	return ret;
}

point secp256k1_my::ECadd(point a, point b)
{
	INT1024_T LamAddTemp = ((INT1024_T)b.pointY - (INT1024_T)a.pointY) * ModInv<INT1024_T>((INT1024_T)b.pointX - (INT1024_T)a.pointX, (INT1024_T)pCurve);
	// slope = (y1 - y2) / (x1 - x2)
	INT1024_T LamAdd = mod<INT1024_T>(LamAddTemp, pCurve);
	// x = slope ^ 2 - x1 - x2
	INT1024_T xTemp = (LamAdd * LamAdd - a.pointX - b.pointX);
	INT256_T x = (INT256_T)mod<INT1024_T>(xTemp, pCurve);
	// y = slope * (x1 - nx) - y1
	INT1024_T yTemp = (LamAdd * (a.pointX - x) - a.pointY);
	INT256_T y = (INT256_T)mod<INT1024_T>(yTemp, pCurve);
	// Return the new point
	point ret;
	ret.pointX = x;
	ret.pointY = y;
	return ret;
}

point secp256k1_my::ECmult(const point& g, const INT256_T& k)
{
	if ((k == 0) || (k >= n)) throw std::invalid_argument("Invalid Scalar/Private Key");
	unsigned char hex[32];
	memcpy(hex, &k, sizeof(hex));
	std::string privBin = ReturnArrBits(hex, sizeof(hex));
	
	point Q = g;
	for (int i = 1; i < privBin.length(); i++) { 
		Q = ECdouble(Q); //std::cout << "DUB: " << Q.pointX << std::endl;
		if (privBin[i] == 49) { // 49 - char symbol of "1"
			Q = ECadd(Q, g);// std::cout << "ADD: " << Q.pointX << std::endl;
		}
	}
	return Q;
}

std::string secp256k1_my::ReturnArrBits(unsigned char* inArr, int len) // make string array of bits from hex array 
{
	int g = 0;
	std::string out;
	for (int i = 0; i < len; i++)
	{
		std::bitset<8> gg(inArr[i]);
		std::string outTemp = gg.to_string();
		std::reverse(outTemp.begin(), outTemp.end());
		out += outTemp;
		if (g >= len * 8) break;
		g++;	
	}
	std::reverse(out.begin(), out.end());
	
	while(true)
	{
		if (out[0] == 48)
		{
			out.erase(0, 1);
		}
		else break;
	}	
	return out;
}

void* secp256k1_my::Swapbytes(void* inp, size_t len)
{
	unsigned int i;
	unsigned char* in = (unsigned char*)inp, tmp;

	for (i = 0; i < len / 2; i++) {
		tmp = *(in + i);
		*(in + i) = *(in + len - i - 1);
		*(in + len - i - 1) = tmp;
	}

	return inp;
}

std::string secp256k1_my::convertToString(char* a, int size)
{
	int i;
	std::string s = "";
	char constZero = 48;
	char constOne = 49;
	for (i = 0; i < size; i++) {
		if (a[i] == 0) 
		s += constZero;
		else if(a[i] == 1)
			s += constOne;
	}
	return s;
}