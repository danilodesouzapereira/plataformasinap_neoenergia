//---------------------------------------------------------------------------
#ifndef TMontaBateriaH
#define TMontaBateriaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TAue.h"
#include "TMontaEqbar.h"

//---------------------------------------------------------------------------
class VTBateria;
class VTFases;

//---------------------------------------------------------------------------
class TMontaBateria : public TMontaEqbar
   {
   public:
				 __fastcall TMontaBateria(VTApl *apl);
				 __fastcall ~TMontaBateria(void);
      VTGrupo*   __fastcall IniciaGrupo(VTEqpto *eqpto);
      bool       __fastcall LisAtributos(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisValores(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      bool       __fastcall LisValoresCurva(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      bool       __fastcall LisValoresEqpto(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      bool       __fastcall LisValoresFluxo(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      bool       __fastcall LisVetValEqpto(VTEqpto *eqpto, TList *lisEXT, int ind_atrib);
      bool       __fastcall LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano);
      AnsiString __fastcall ValorAtributo(VTEqpto *eqpto, int ind_atrib);
                
   private:
      void __fastcall ValorClasse(void);
      void __fastcall ValorDRP_DRC(void);
      void __fastcall ValorEnergia(void);
      void __fastcall ValorFases(void);
      void __fastcall ValorModelo(void);
      void __fastcall ValorNumConsumidores(void);
      void __fastcall ValorPotAtiva(void);
      void __fastcall ValorPotReativa(void);
      void __fastcall ValorPotAparente(void);
      void __fastcall VetValorPotAparente(int ind_atrib);
      void __fastcall VetValorPotAtiva(int ind_atrib);
      void __fastcall VetValorPotReativa(int ind_atrib);

   private: //objetos externos
      VTApl   *apl;
      VTFases *fases;
	  VTBateria *bateria;
   };

//---------------------------------------------------------------------------
#endif
//eof
