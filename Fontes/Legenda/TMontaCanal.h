//---------------------------------------------------------------------------
#ifndef TMontaCanalH
#define TMontaCanalH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TAue.h"
#include "TMontaEqbar.h"

//---------------------------------------------------------------------------
class VTCanal;
class VTFases;

//---------------------------------------------------------------------------
class TMontaCanal : public TMontaEqbar
   {
   public:
                 __fastcall TMontaCanal(VTApl *apl);
                 __fastcall ~TMontaCanal(void);
      VTGrupo*   __fastcall IniciaGrupo(VTEqpto *eqpto) {return NULL;};
      bool       __fastcall LisAtributos(VTEqpto *eqpto, TList *lisATR) {return false;};
      bool       __fastcall LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR) {return false;};
      bool       __fastcall LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR) {return false;};
      bool       __fastcall LisValores(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat) {return false;};
      bool       __fastcall LisValoresCurva(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat) {return false;};
      bool       __fastcall LisValoresEqpto(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat) {return false;};
      bool       __fastcall LisValoresFluxo(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat) {return false;};
      bool       __fastcall LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano) {return false;};
      bool       __fastcall LisVetValCanal(VTCanal *canal, TList *lisEXT, int ind_atrib, int ano);
      //AnsiString __fastcall ValorAtributo(VTEqpto *eqpto, int ind_atrib);

   private:
      void __fastcall VetValorPotAparente(int ind_atrib);
      void __fastcall VetValorPotAtiva(int ind_atrib);
      void __fastcall VetValorPotReativa(int ind_atrib);
      void __fastcall VetValorTensao(int ind_atrib);
      void __fastcall VetValorCurvaPQ_PV_VF(int ind_atrib);

   private: //objetos externos
      VTApl   *apl;
      VTCanal *canal;
      union unionVALOR  {
                     double vet[4];
                     struct {double p, q, v, t;} var;
                     };

   };

//---------------------------------------------------------------------------
#endif
//eof