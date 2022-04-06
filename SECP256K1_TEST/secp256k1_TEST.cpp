#include "pch.h"
#include "CppUnitTest.h"
#include "boost\multiprecision\cpp_int.hpp"
#include <secp256k1.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CryptoKeyPairTEST2
{
	

	TEST_CLASS(CryptoKeyPairTEST2)
	{
	public:

		TEST_METHOD(ReturnArrBitsTEST)
		{		
			secp256k1_my secp;
			INT256_T in("28695618543805844332113829720373285210420739438570883203839696518176414791234");
			unsigned char inArr[32];
			memcpy(inArr, &in, sizeof(inArr));
			std::string arrStrRef = "111111011100010010010100000000000000010011000010000101101100011011000010000"
				"0101111111110111111110011000100001111000100000001101100111011111101110010010111110011010101101000001001"
				"0111011111001111100001100001001101010110111011011001011010100111011001000010";				
			//std::reverse(arrStrRef.begin(), arrStrRef.end());
			std::string arrBitsOut = secp.ReturnArrBits(inArr, sizeof(inArr));		
			//std::string arrStrOut = secp.convertToString(arrBitsOut, sizeof(arrBitsOut));
			Assert::AreEqual(arrStrRef, arrBitsOut);
		}
		TEST_METHOD(ECmultTEST)
		{

			INT256_T refX("0xBE686ED7F0539AFFBAF634F3BCC2B235E8E220E7BE57E9397AB1C14C39137EB4");
			INT256_T refY("0x3705125AAC75A865268EF33C53897C141BD092CF4D1A306B2A57E37E1386826D");

			INT256_T gX("0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
			INT256_T gY("0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8");
			INT256_T privKey("0xA665A45920422F9D417E4867EFDC4FB8A04A1F3FFF1FA07E998E86F7F7A27AE3");
			
			point ref;
			ref.pointX = refX;
			ref.pointY = refY;
			point in;
			in.pointX = gX;
			in.pointY = gY;
			point out;
			secp256k1_my secp;

			out = secp.ECmult(in, privKey);
			bool ya = ref == out;
			//Assert::AreEqual(ref, out);
			Assert::IsTrue(ya);
		}
	};
}
