//---------------------------------------------------------------------------
#ifndef TMalhaH
#define TMalhaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMalha.h"

//---------------------------------------------------------------------------
class VTCarga;
class VTConsulta;
class VTCorte;
class VTMRede;
class VTRede;

//---------------------------------------------------------------------------
class TMalha : public VTMalha
   {
   public:
               __fastcall TMalha(void);
               __fastcall ~TMalha(void);
      VTMRede* __fastcall Executa(TList *lisMR);

   private: //métodos
      bool     __fastcall ExisteMalha(TList *lisMR, TList *lisMALHA);
      //VTMRede* __fastcall ExisteMRede(VTRede *rede, TList *lisMR);
      //void     __fastcall FundeMRedeMalha(TList *lisMALHA);
      //void     __fastcall LimpaListas(void);
      //void     __fastcall MontaListaCorte(TList *lisREDE, TList *lisCORTE);
      void     __fastcall MontaListaMRedeEmMalha(VTMRede *mrede1, VTMRede *mrede2, TList *lisMALHA);
      //void     __fastcall NotificaMalhaCritica(TList *lisREDE);

   private: //objetos externos

   private: //dados locais
      struct {
             TList  *lisCORTE; //lista de Cortes
             TList  *lisORD;   //lista de Redes ordenadas
             TList  *lisVIZ;   //lista de Redes vizinhas
             TList  *lisMRM;   //lista de objetos MRede em malha
             TList  *lisMR;    //lista de objetos MRede
             }tmp;
   };

//---------------------------------------------------------------------------
#endif
//eof

