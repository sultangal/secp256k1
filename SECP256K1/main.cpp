#include "pch.h"
#include "secp256k1.h"

int main()
{
	INT256_T n("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");
	INT256_T gX("0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
	INT256_T gY("0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8");
	INT256_T privKey("0xA665A45920422F9D417E4867EFDC4FB8A04A1F3FFF1FA07E998E86F7F7A27AE3");

	INT256_T randNum("0x3F712500013085B1B082FFBFCC43C406CEFDC97CD5A0977CF861356ED96A7642");
	INT256_T HashOfThingToSign("0xBE347331B4D87273E674B30384985C639069F852246E8C128417DBB1CA8BA812");

	secp256k1_my secp;
	point gen;
	point pubKey;
	gen.pointX = gX;
	gen.pointY = gY;
	COUT("Private key: ");
	pubKey = secp.ECmult(gen, privKey);
	COUTh(privKey); ENDL;

	COUT("Uncompressed public key: ");
	COUTh("04 "<< pubKey.pointX <<"\n   "<< pubKey.pointY); ENDL;

	COUT("Signature generation: ");
	point randSignPoint = secp.ECmult(gen, randNum);
	INT1024_T r = secp.mod<INT1024_T>((INT1024_T)randSignPoint.pointX, (INT1024_T)n);
	COUTh("r = " << r);
	INT1024_T s = secp.mod<INT1024_T>(((HashOfThingToSign + ((INT1024_T)r * (INT1024_T)privKey)) * (secp.ModInv<INT1024_T>((INT1024_T)randNum, (INT1024_T)n))), (INT1024_T)n);
	COUTh("s = " << s); ENDL;

	COUT("Signature generation: ");	
	INT1024_T w = secp.ModInv<INT1024_T>(s, (INT1024_T)n);	
	INT256_T u1Temp = (INT256_T)secp.mod<INT1024_T>((HashOfThingToSign * (INT1024_T)w), (INT1024_T)n);	
	point u1 = secp.ECmult(gen, u1Temp);	
	INT256_T u2Temp = (INT256_T)secp.mod<INT1024_T>(r*w,n);
	point u2 = secp.ECmult(pubKey, u2Temp);
	point xy = secp.ECadd(u1, u2);
	if (r == xy.pointX) COUT("true");		

	system("pause");
	return 0;
}





