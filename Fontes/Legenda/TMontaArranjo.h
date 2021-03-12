//---------------------------------------------------------------------------
#ifndef TMontaArranjoH
#define TMontaArranjoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TMontaEqpto.h"

//---------------------------------------------------------------------------
class VTArranjo;
class VTFases;

//---------------------------------------------------------------------------
class TMontaArranjo : public TMontaEqpto
   {
   public:
               __fastcall TMontaArranjo(VTApl *apl);
               __fastcall ~TMontaArranjo(void);
      VTGrupo* __fastcall IniciaGrupo(VTEqpto *eqpto);
      bool     __fastcall LisAtributos(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR);
      bool     __fastcall LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      //bool     __fastcall LisValoresCurva(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      bool     __fastcall LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      //bool     __fastcall LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      //bool     __fastcall LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano);
      //AnsiString __fastcall ValorAtributo(VTEqpto *eqpto, int ind_atrib);

   private:
      void __fastcall ValorDadosConfiguracao(void);
      void __fastcall ValorTipoArranjoCABOxFASE(void);
      void __fastcall ValorTipoArranjoCABO_Z0Z1(void);
      void __fastcall ValorTipoArranjoMAT_Z(void);
      void __fastcall ValorTipoArranjoPU_SB100(void);
      void __fastcall ValorTipoArranjoZ0Z1(void);

   private: //objetos externos
      VTApl     *apl;
      VTArranjo *arranjo;
      VTFases   *fases;
   };

//---------------------------------------------------------------------------
#endif
//eof
