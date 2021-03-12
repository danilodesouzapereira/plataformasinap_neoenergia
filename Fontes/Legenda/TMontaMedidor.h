//---------------------------------------------------------------------------
#ifndef TMontaMedidorH
#define TMontaMedidorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TAue.h"
#include "TMontaEqbar.h"

//---------------------------------------------------------------------------
class VTMedidor;
class VTCanal;
class VTFases;

//---------------------------------------------------------------------------
class TMontaMedidor : public TMontaEqbar
   {
   public:
                 __fastcall TMontaMedidor(VTApl *apl);
                 __fastcall ~TMontaMedidor(void);
      VTGrupo*   __fastcall IniciaGrupo(VTEqpto *eqpto);
		bool       __fastcall LisAtributos(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisValores(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      bool       __fastcall LisValoresEqpto(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      //bool       __fastcall LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      //bool       __fastcall LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano);
      AnsiString __fastcall ValorAtributo(VTEqpto *eqpto, int ind_atrib);

   private:
      VTGrupo* __fastcall IniciaGrupoCanal(VTCanal *canal);
      bool     __fastcall LisValoresCanal(VTCanal *canal, TList *lisEXT, int ano, int num_pat);
      bool     __fastcall LisVetValCanal(VTCanal *canal, TList *lisEXT);
      //bool     __fastcall LisValoresCurva(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat) {return false;};
      void     __fastcall ValorEnabledCanal(VTCanal *canal);
      void     __fastcall ValorEqptoCanal(VTCanal *canal);
      void     __fastcall ValorFasesCanal(VTCanal *canal);
      void     __fastcall ValorNumeroCanal(void);
      void     __fastcall ValorToleranciaCanal(VTCanal *canal);
      void     __fastcall ValorTipoCanal(VTCanal *canal);
		void     __fastcall VetValorCorrenteModulo(VTCanal *canal, bool medido);
      void     __fastcall VetValorPotAparente(VTCanal *canal, bool medido);
      void     __fastcall VetValorPotAtiva(VTCanal *canal, bool medido);
      void     __fastcall VetValorPotReativa(VTCanal *canal, bool medido);
      void     __fastcall VetValorTensaoModulo(VTCanal *canal, bool medido);

   private: //objetos externos
      VTApl     *apl;
      VTFases   *fases;
      VTMedidor *medidor;
      /*
      union unionVALOR  {
							double vet[5];
							struct {double p, q, v, i, t;} var;
                     };
      */
   };

//---------------------------------------------------------------------------
#endif
//eof