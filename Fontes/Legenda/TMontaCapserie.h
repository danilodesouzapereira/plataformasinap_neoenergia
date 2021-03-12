//---------------------------------------------------------------------------
#ifndef TMontaCapserieH
#define TMontaCapserieH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TMontaLigacao.h"

//---------------------------------------------------------------------------
class VTCapserie;

//---------------------------------------------------------------------------
class TMontaCapserie : public TMontaLigacao
   {
   public:
                 __fastcall TMontaCapserie(VTApl *apl);
                 __fastcall ~TMontaCapserie(void);
      VTGrupo*   __fastcall IniciaGrupo(VTEqpto *eqpto);
      bool       __fastcall LisAtributos(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      bool       __fastcall LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      //bool       __fastcall LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      //bool       __fastcall LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano);
      AnsiString __fastcall ValorAtributo(VTEqpto *eqpto, int ind_atrib);
                
   private:     
      void __fastcall ValorCapacidade(void);
      void __fastcall ValorReatancia(void);

   private: //objetos externos
      VTCapserie *capserie;

   private: //dados locais
      VTMonta *monta_arranjo;
   };

//---------------------------------------------------------------------------
#endif
//eof
