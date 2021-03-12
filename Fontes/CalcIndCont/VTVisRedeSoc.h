//---------------------------------------------------------------------------
#ifndef VTVisRedeSocH
#define VTVisRedeSocH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTVisRede;
class VTRede;
class VTBlocoRad;
//---------------------------------------------------------------------------
class VTVisRedeSoc: public TObject
	{
	public:  //property
//		__property VTBlocoRad*   BlocoRadMon         = {read=PM_GetBlocoRadMon, write=PM_SetBlocoRadMon};


	public:
									__fastcall VTVisRedeSoc(void) {};
									__fastcall ~VTVisRedeSoc(void) {};

	protected: //métodos acessados via property
//	  virtual VTBlocoRad*  __fastcall PM_GetBlocoRadMon(void) = 0;
//	  virtual void         __fastcall PM_SetBlocoRadMon(VTBlocoRad* blocoRadMon) = 0;

	public:
	 //patamar da manobra
	 int npat; 
	 //Etapas de isolamento a justante rápido e lento
	 VTVisRede  *visRede;      //manobras
	 VTBlocoRad *blRadRapida;
	 VTBlocoRad *blRadLenta;

	 TList      *lisChvIsola;  //lista de chaves de isolamento/ corte de carga
	 TList      *lisBlRadIsola;//lista de blocosRad a jusante da chave de isolamento

	 TList      *lisBlocosSoc; //lista de blocos socorrido por visRede
	 TList      *lisBlocosSocRapida;
	 TList      *lisBlocosSocLenta;

	 double    cargaTransf;   //carga transferida em npat por visRede
	 VTRede    *redeTransf;   //rede de transferência da carga
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto
//---------------------------------------------------------------------------
VTVisRedeSoc* __fastcall NewObjVisRedeSoc(int npat);

//---------------------------------------------------------------------------
#endif

