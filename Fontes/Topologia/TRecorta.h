//---------------------------------------------------------------------------
#ifndef TRecortaH
#define TRecortaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTRecorta.h"

//---------------------------------------------------------------------------
class VTArea;
class VTBarra;
class VTCorte;
class VTRede;
class VTReducao;
class VTCronometro;

//---------------------------------------------------------------------------
class TRecorta : public VTRecorta
   {
   public:
           __fastcall TRecorta(VTApl *apl);
           __fastcall ~TRecorta(void);
      bool __fastcall Executa(VTRedes *redes);
      void __fastcall VerificaExclusaoDeCortes(void);
      void __fastcall VerificaFusaoDeCortes(TList *lisMR);

   private: //métodos
      void     __fastcall EliminaReducaoMRedeCarregada(void);
      bool     __fastcall ExisteBarraInterface(TList *lisBAR1, TList *lisBAR2, VTArea *area2, TList *lisBAR12);
      void     __fastcall MontaConjuntoBarrasCorteDaReducao(VTReducao *reducao);
      void     __fastcall MontaNovoConjuntoCortes(void);
      void     __fastcall ReiniciaCortes(void);

   private: //dados externos
      VTApl        *apl;
      VTRedes      *redes;
      VTCronometro *cronometro;

   private: //dados locais
      struct   {
               TList *lisBR1, *lisBR2;
               TList *lisCORTE;
               TList *lisEQP;
               TList *lisMR1;
               TList *lisMR2;
               }local;
   };

//---------------------------------------------------------------------------
#endif
//eof

