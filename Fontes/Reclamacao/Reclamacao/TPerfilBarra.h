//---------------------------------------------------------------------------
#ifndef TPerfilBarraH
#define TPerfilBarraH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\Constante\Fases.h>
#include "VTPerfilBarra.h"

//---------------------------------------------------------------------------
class VTTensao;

//---------------------------------------------------------------------------
class TPerfilBarra  : public VTPerfilBarra
   {
   public:
             __fastcall  TPerfilBarra(VTBarra *barra);
             __fastcall ~TPerfilBarra(void);
      bool   __fastcall  CalculaDRA_DRP_DRC(VTRes505 *res505, int limite_amostra);
      bool   __fastcall  CalculaIndicesQualidadeUmDia(VTRes505 *res505, int ind_dia);
      int    __fastcall  DRA(int fases);
      double __fastcall  DRA_perc(int fases);
      int    __fastcall  DRC(int fases);
      double __fastcall  DRC_perc(int fases);
      int    __fastcall  DRP(int fases);
      double __fastcall  DRP_perc(int fases);
      int    __fastcall  NumeroAmostragem(void);
		bool   __fastcall  SalvaTensao(TDateTime data, VTMedicao *medicao, bool padrao);
      bool   __fastcall  Vfn_kv(int ind_dia, int ind_pat, double vfn_kv[MAX_FASE]);
      bool   __fastcall  Vfn_kv(int ind_dia, int ind_pat, complex<double> vfn_kv[MAX_FASE]);

   protected:  //métodos acessados vias property
      bool __fastcall  PM_GetFaseEnabled(int ind_fase);
		void __fastcall  PM_SetFaseEnabled(int ind_fase, bool enabled);

	private: //métodos
		VTTensao* __fastcall ExisteTensao(TDateTime data);

   private: //objetos externos

   private: //dados
      TList *lisTENSAO;
   };

//---------------------------------------------------------------------------
#endif
//eof


