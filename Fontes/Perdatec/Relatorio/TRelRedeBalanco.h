//---------------------------------------------------------------------------

#ifndef TRelRedeBalancoH
#define TRelRedeBalancoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTAgregada;

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16127109
class TRelRedeBalanco : public TObject
   {
   public:
			   __fastcall TRelRedeBalanco(VTApl *apl);
			   __fastcall ~TRelRedeBalanco(void);
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
	  double fat_mult;

   };

//---------------------------------------------------------------------------
#endif
