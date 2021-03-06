//---------------------------------------------------------------------------

#ifndef TRelEqptoAgregadoH
#define TRelEqptoAgregadoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTAgregada;
class VTResumo;

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16127109
class TRelEqptoAgregado : public TObject
   {
   public:
			   __fastcall TRelEqptoAgregado(VTApl *apl);
			   __fastcall ~TRelEqptoAgregado(void);
   	  void     __fastcall AgregaResumo(VTResumo *resumo);
      void     __fastcall DefineFatMult(double fat_mult);
	  bool     __fastcall Executa(AnsiString arq_rede, AnsiString title);
	  bool     __fastcall InsereLisResumo(TList *lisRESUMO, VTAgregada *agregada);

   private: //m?todos
	  AnsiString __fastcall FormataEnergia(double energia_mwh);
	  AnsiString __fastcall FormataPorcentagem(double dividendo, double divisor);

   private: //objetos externos
	  VTApl       *apl;

   private: //obj interno
	  TStringList *linhas;
	  TList *lisTMP;
	  double fat_mult;
	  VTResumo *resumo_agregado;

   };

//---------------------------------------------------------------------------
#endif
