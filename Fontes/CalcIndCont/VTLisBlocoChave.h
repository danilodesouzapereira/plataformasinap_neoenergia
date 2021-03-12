//---------------------------------------------------------------------------
#ifndef VTLisBlocoChaveH
#define VTLisBlocoChaveH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTChave;
class VTBlocoRad;
//---------------------------------------------------------------------------
class VTLisBlocoChave: public TObject
	{
	public:  //property


	public:
									__fastcall VTLisBlocoChave(void) {};
									__fastcall ~VTLisBlocoChave(void) {};

	protected: //métodos acessados via property

	public:
		TList     *lisBlJ;
		TList     *lisBlReliga;
		VTChave   *pchv;
		VTBlocoRad *blJus;
		VTBlocoRad *blMon;
		VTBlocoRad *blSup;
		VTBlocoRad *blMon1;//para o caso de chave NA entre alimentadores
		VTBlocoRad *blSup1;//para o caso de chave NA entre alimentadores
		//bool      flagBlocos;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto
//---------------------------------------------------------------------------
VTLisBlocoChave* __fastcall NewObjLisBlocoChave(VTChave* pchave);

//---------------------------------------------------------------------------
#endif

