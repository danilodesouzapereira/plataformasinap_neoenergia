//---------------------------------------------------------------------------

#ifndef TRelRedePerdaH
#define TRelRedePerdaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTAgregada;

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/7975908
class TRelRedePerda : public TObject
   {
   public:
			   __fastcall TRelRedePerda(VTApl *apl);
			   __fastcall ~TRelRedePerda(void);
      void     __fastcall DefineFatMult(double fat_mult);
	  bool     __fastcall Executa(AnsiString arq_rede, AnsiString title);
	  bool     __fastcall InsereLisResumo(TList *lisRESUMO, VTAgregada *agregada);
//      VTPerda* __fastcall ExistePerda(TObject *object);
//      TList*   __fastcall LisPerda(void);
//      void     __fastcall Reinicia(void);

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
