//---------------------------------------------------------------------------
#ifndef TCalculaSupH
#define TCalculaSupH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCalculaSup.h"

//---------------------------------------------------------------------------
class VTPatamares;
class VTCurva;

//---------------------------------------------------------------------------
class TCalculaSup : public VTCalculaSup
   {
   public:
           __fastcall TCalculaSup(VTApl *apl);
           __fastcall ~TCalculaSup(void);
      bool __fastcall GetValorGerador(VTGerador *gerador, int np, strSUP *str_sup);
      bool __fastcall GetValorGeradorReducao(VTGerador *gerador, int np, strSUP vet_sup[MAX_FASE]);
      bool __fastcall GetValorSuprimento(VTSuprimento *suprimento, int np, strSUP *str_sup);
      bool __fastcall GetValorSuprimentoReducao(VTSuprimento *suprimento, int np, strSUP vet_sup[MAX_FASE]);
      bool __fastcall SetValorGerador(VTGerador *gerador, int np, strSUP *str_sup);
      bool __fastcall SetValorGeradorReducao(VTGerador *gerador, int np, strSUP vet_sup[MAX_FASE]);
      bool __fastcall SetValorSuprimento(VTSuprimento *suprimento, int np, strSUP *str_sup);
      bool __fastcall SetValorSuprimentoReducao(VTSuprimento *suprimento, int np, strSUP vet_sup[MAX_FASE]);

   private: //métodos
      bool __fastcall GetValorCurvaPQVT(VTCurva *curva, int np, strSUP *str_sup);
      bool __fastcall GetValorCurvaPQVTPorFase(VTCurva *curva, int np, strSUP vet_sup[MAX_FASE]);
      bool __fastcall SetValorCurvaPQVT(VTCurva *curva, int np, strSUP *str_sup);
      bool __fastcall SetValorCurvaPQVTPorFase(VTCurva *curva, int np, strSUP vet_sup[MAX_FASE]);

   private: //objetos externos
      VTApl       *apl;
      VTPatamares *patamares;
   };

#endif
//---------------------------------------------------------------------------
//eof
