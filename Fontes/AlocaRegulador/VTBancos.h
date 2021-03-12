//---------------------------------------------------------------------------
#ifndef VTBancosH
#define VTBancosH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBanco;
class VTLigacao;
class VTRede;
class VTRegulador;

//---------------------------------------------------------------------------
class VTBancos : public TObject
   {
   public:
							  __fastcall  VTBancos(void) {};
      virtual          __fastcall ~VTBancos(void) {};
		virtual VTBanco* __fastcall  ExisteBanco(VTLigacao *ligacao) = 0;
		virtual VTBanco* __fastcall  InsereBanco(void) = 0;
		virtual TList*   __fastcall  LisBanco(void) = 0;
      virtual TList*   __fastcall  LisBanco(VTRede *rede) = 0;   //DVK 2014.04.03
		virtual int      __fastcall  LisRegulador(TList *lisEXT) = 0;
		virtual int      __fastcall  LisCloneRegulador(TList *lisEXT) = 0;
		virtual int      __fastcall  NumBanco(void) = 0;
		virtual void     __fastcall  Reinicia(void) = 0;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTBancos* __fastcall NewObjBancos(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof

