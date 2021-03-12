//---------------------------------------------------------------------------
#ifndef TLegendaH
#define TLegendaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTLegenda.h"

//---------------------------------------------------------------------------
class VTMonta;

//---------------------------------------------------------------------------
class TLegenda : public VTLegenda
   {
   public:
                 __fastcall TLegenda(VTApl *apl);
                 __fastcall ~TLegenda(void);
      VTGrupo*   __fastcall IniciaGrupo(VTEqpto *eqpto);
      bool       __fastcall LisAtributos(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisValores(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      bool       __fastcall LisValoresEqpto(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      bool       __fastcall LisValoresFluxo(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      bool       __fastcall LisVetValEqpto(VTEqpto *eqpto, TList *lisVET, int ind_atrib);
      bool       __fastcall LisVetValFluxo(VTEqpto *eqpto, TList *lisVET, int ind_atrib, int ano);
      //bool       __fastcall LisVetValCanal(VTCanal *canal, TList *lisVET, int ind_atrib, int ano);
      AnsiString __fastcall ValorAtributo(VTEqpto *eqpto, int ind_atrib);

   private: //métodos
      VTMonta* __fastcall SelecionaMonta(VTEqpto *eqpto);

   private: //dados locais
	  VTMonta *monta_barra;
	  VTMonta *monta_bateria;
      VTMonta *monta_capacitor;
      VTMonta *monta_capserie;
      VTMonta *monta_carga;
      VTMonta *monta_chave;
      VTMonta *monta_gerador;
      //VTMonta *monta_canal;
      VTMonta *monta_medidor;
      VTMonta *monta_mutua;
      VTMonta *monta_regulador;
      VTMonta *monta_reator;
      VTMonta *monta_reducao;
      VTMonta *monta_suprimento;
      VTMonta *monta_trafo;
      VTMonta *monta_trafoZZ;
      VTMonta *monta_trafo3E;
      VTMonta *monta_trecho;
      VTMonta *monta_yref;
      TList   *lisMONTA;
   };

//---------------------------------------------------------------------------
#endif
//eof
