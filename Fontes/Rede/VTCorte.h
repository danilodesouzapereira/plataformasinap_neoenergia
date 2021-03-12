//---------------------------------------------------------------------------
#ifndef VTCorteH
#define VTCorteH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "Estrutura.h"
#include "VTEqpto.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTMRede;
class VTRede;
class VTRedes;

//---------------------------------------------------------------------------
class VTCorte : public VTEqpto
   {
   public:  //property
		__property strCOORD PtoMedio    = {read=PM_GetPtoMedio, write=PM_SetPtoMedio};

   public:
                         __fastcall VTCorte(void) {};
      virtual            __fastcall ~VTCorte(void) {};
      virtual VTCorte*   __fastcall Clone(void) {return NULL;};
      //virtual bool       __fastcall ConectaMRede(VTMRede *mrede) = 0;
      virtual bool       __fastcall ExisteBarra(VTBarra *pbarra) = 0;
      virtual bool       __fastcall ExisteBarra(TList *lisEQP) = 0;
		//virtual bool       __fastcall ExisteMRede(VTMRede *mrede) = 0;
      virtual bool       __fastcall ExisteReducao(void) = 0;
      virtual void       __fastcall InsereBarra(VTBarra *pbarra) = 0;
      virtual void       __fastcall InsereLisBarra(TList *lisEQP) = 0;
      virtual void       __fastcall InsereLisRede(TList *lisEQP) = 0;
      virtual void       __fastcall InsereRede(VTRede *rede) = 0;
      //virtual void       __fastcall InsereMRede(VTMRede *mrede) = 0;
      virtual TList*     __fastcall LisBarra(void) = 0;
      virtual void       __fastcall LisBarra(TList *lisEQP) = 0;
      virtual void       __fastcall LisEqbar(TList *lisEQP, int tipo) = 0;
		//virtual TList*     __fastcall LisMRede(void) = 0;
      virtual void       __fastcall LisMRede(TList *lisEQP) = 0;
      virtual void       __fastcall LisMRedeCarregada(TList *lisEQP) = 0;
      virtual void       __fastcall LisMRedeVizinha(VTMRede *mrede, TList *lisEQP) = 0;
      virtual void       __fastcall LisMRedeVizinhaCarregada(VTMRede *mrede, TList *lisEQP) = 0;
      virtual TList*     __fastcall LisRede(void) = 0;
      virtual void       __fastcall LisRede(TList *lisEQP) = 0;
		//virtual void       __fastcall LisRedeSimples(TList *lisEQP) = 0;
      virtual void       __fastcall LisRedeVizinha(VTRede *rede, TList *lisEQP) = 0;
		//virtual void       __fastcall LisRedeVizinhaAtiva(VTRede *rede, TList *lisEQP) = 0;
      //virtual void       __fastcall LisRedeVizinhaCarregada(VTMRede *rede, TList *lisEQP) = 0;
      virtual void       __fastcall LisReducao(TList *lisEQP)= 0;
      virtual void       __fastcall LisReducaoVizinha(VTMRede *mrede, TList *lisEQP)= 0;
      //virtual void       __fastcall LisReducaoVizinha(VTRede *rede, TList *lisEQP)= 0;
      virtual int        __fastcall NumeroBarras(void) = 0;
      virtual int        __fastcall NumeroMRedes(void) = 0;
      virtual int        __fastcall NumeroMRedesCarregadas(void) = 0;
      //virtual void       __fastcall ReiniciaLisBarra(void) = 0;
      //virtual void       __fastcall ReiniciaLisMRede(void) = 0;
      virtual void       __fastcall ReiniciaLisRede(void) = 0;
      //virtual void       __fastcall RemoveMRede(VTMRede *mrede) = 0;
      virtual void       __fastcall RemoveRede(TList *lisEQP) = 0;

	protected:  //métodos acessados via property
		virtual strCOORD __fastcall PM_GetPtoMedio(void) = 0;
		virtual void     __fastcall PM_SetPtoMedio(strCOORD coord) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCorte
//---------------------------------------------------------------------------
VTCorte* __fastcall NewObjCorte(void);

//---------------------------------------------------------------------------
#endif
//eof



 