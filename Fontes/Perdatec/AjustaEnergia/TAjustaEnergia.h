//---------------------------------------------------------------------------
#ifndef TAjustaEnergiaH
#define TAjustaEnergiaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAjustaEnergia.h"

//---------------------------------------------------------------------------
class TMedicao;
class VTMedicoes;
class VTNode;

//---------------------------------------------------------------------------
class TAjustaEnergia : public VTAjustaEnergia
   {
   public:
             __fastcall  TAjustaEnergia(VTApl *apl_owner);
             __fastcall ~TAjustaEnergia(void);
      double __fastcall  ExecutaAjusteComPerda(int dias_mes);
      bool   __fastcall  ExecutaAjusteSemPerda(int dias_mes);
      bool   __fastcall  IniciaAllMedicao(int dias_mes, bool divide_perda_comercial);
      bool   __fastcall  IniciaDivisaoPerdaComercial(TMedicao *medicao);
      bool   __fastcall  IniciaUmaMedicao(int dias_mes, TMedicao *medicao);
      void   __fastcall  SalvaCargaFinal(void);
      bool   __fastcall  TESTE_CriaMedicoes(int dia_mes);

   private: //métodos
	  double __fastcall EnergiaCarga_mwhmes(TList *lisCARGA);
	  double __fastcall EnergiaReducoesFilhas_mwhmes(TList *lisRedes);
//      bool   __fastcall IniciaDivisaoPerdaComercial(TMedicao *medicao);
	  void   __fastcall IniciaLisCargaAjustaveisFixas(TMedicao *medicao, bool pos_fluxo);
      void   __fastcall IniciaLisCargaNivelTensao(TMedicao *medicao);
      bool   __fastcall IniciaLisRedeAjustavelLisRedeFixa(TMedicao *medicao, VTNode *node_pai);
	  bool   __fastcall RedefineEnergiaCarga(TList *lisCARGA, double fatmult);
	  void   __fastcall RemoveCargasBarraInicial(TMedicao *medicao);
	  void 	 __fastcall RemoveMedicaoInvalida(void);
	  void   __fastcall ValidaDistribuicaoPerdaComercial(TMedicao *medicao);
      bool   __fastcall ValidaMedicao(TMedicao *medicao);

private: //objetos externos
      VTApl      *apl;
      VTMedicoes *medicoes;

	private: //dados locais
      int    num_dias;
      TList *lisEQP;
	};

//---------------------------------------------------------------------------
#endif
//eof

 