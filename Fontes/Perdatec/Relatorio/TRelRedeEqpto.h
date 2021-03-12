//---------------------------------------------------------------------------

#ifndef TRelRedeEqptoH
#define TRelRedeEqptoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTAgregada;

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16127109
class TRelRedeEqpto : public TObject
   {
   public:
			   __fastcall TRelRedeEqpto(VTApl *apl);
			   __fastcall ~TRelRedeEqpto(void);
      void     __fastcall DefineFatMult(double fat_mult);
	  bool     __fastcall Executa(AnsiString arq_rede, AnsiString title);
	  bool     __fastcall InsereLisResumo(TList *lisRESUMO, VTAgregada *agregada);

   private: //métodos
	  AnsiString __fastcall FormataEnergia(double energia_mwh);
	  AnsiString __fastcall FormataPorcentagem(double dividendo, double divisor);

   private: //objetos externos
	  VTApl       *apl;

   private: //obj interno
	  TStringList *linhas;
	  double fat_mult;

   };

//---------------------------------------------------------------------------
#endif
