//---------------------------------------------------------------------------
#ifndef TCorteH
#define TCorteH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTCorte.h"

//---------------------------------------------------------------------------
class TCorte : public VTCorte
   {
   public:
                 __fastcall TCorte(void);
                 __fastcall ~TCorte(void);
      //bool       __fastcall ConectaMRede(VTMRede *mrede);
      bool       __fastcall ExisteBarra(VTBarra *pbarra);
      bool       __fastcall ExisteBarra(TList *lisEQP);
      //bool       __fastcall ExisteMRede(VTMRede *mrede);
		bool       __fastcall ExisteReducao(void);
      void       __fastcall InsereBarra(VTBarra *pbarra);
      void       __fastcall InsereLisBarra(TList *lisEQP);
      void       __fastcall InsereLisRede(TList *lisEQP);
      //void       __fastcall InsereMRede(VTMRede *mrede);
      void       __fastcall InsereRede(VTRede *rede);
      TList*     __fastcall LisBarra(void);
      void       __fastcall LisBarra(TList *lisEQP);
		void       __fastcall LisEqbar(TList *lisEQP, int tipo);
		//TList*     __fastcall LisMRede(void);
      void       __fastcall LisMRede(TList *lisEQP);
      void       __fastcall LisMRedeCarregada(TList *lisEQP);
      void       __fastcall LisMRedeVizinha(VTMRede *mrede, TList *lisEQP);
      void       __fastcall LisMRedeVizinhaCarregada(VTMRede *mrede, TList *lisEQP);
      TList*     __fastcall LisRede(void);
      void       __fastcall LisRede(TList *lisEQP);
      //void       __fastcall LisRedeSimples(TList *lisEQP);
      void       __fastcall LisRedeVizinha(VTRede *rede, TList *lisEQP);
      //void       __fastcall LisRedeVizinhaAtiva(VTRede *rede, TList *lisEQP);
      //void       __fastcall LisRedeVizinhaCarregada(VTRede *rede, TList *lisEQP);
      void       __fastcall LisReducao(TList *lisEQP);
      void       __fastcall LisReducaoVizinha(VTMRede *mrede, TList *lisEQP);
      //void       __fastcall LisReducaoVizinha(VTRede *rede, TList *lisEQP);
      int        __fastcall NumeroBarras(void);
      int        __fastcall NumeroMRedes(void);
      int        __fastcall NumeroMRedesCarregadas(void);
      //void       __fastcall ReiniciaLisRede(VTRedes *redes);
      //void       __fastcall ReiniciaLisBarra(void);
      //void       __fastcall ReiniciaLisMRede(void);
      void       __fastcall ReiniciaLisRede(void);
      //void       __fastcall RemoveMRede(VTMRede *rede);
      void       __fastcall RemoveRede(TList *lisEQP);
      int        __fastcall Tipo(void);
      AnsiString __fastcall TipoAsString(void);

   protected: //métodos acessados via property
		AnsiString __fastcall PM_GetCodigo(void);
		int        __fastcall PM_NumeroBarra(void);
      strCOORD   __fastcall PM_GetPtoMedio(void);
      void       __fastcall PM_SetPtoMedio(strCOORD coord);

   private: //locais
      strCOORD pto_medio;
      TList *lisBAR;    //lista de objetos VTBarra que fazem parte do Corte
      //TList *lisMR;     //lista de objetos VTMRede que se ligam ao Corte
      TList *lisREDE;   //lista de objetos VTRede que se ligam ao Corte
   };

//---------------------------------------------------------------------------
#endif
//eof

