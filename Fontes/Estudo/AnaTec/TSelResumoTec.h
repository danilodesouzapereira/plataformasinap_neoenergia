//---------------------------------------------------------------------------

#ifndef TSelResumoTecH
#define TSelResumoTecH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <set>
#include <map>

//---------------------------------------------------------------------------
class VTApl;
class VTAlternativa;

//---------------------------------------------------------------------------
class TSelResumoTec : public TObject
{
   public:      // User declarations
			   __fastcall  TSelResumoTec(VTApl *apl_owner);
			   __fastcall ~TSelResumoTec(void);
	  bool     __fastcall AlimentadorSelecionado(AnsiString cod_alim, AnsiString cod_sub);
	  bool     __fastcall AlternativaSelecionada(VTAlternativa *alternativa);
	  bool     __fastcall AnoSelecionado(int ano_sel);
      bool     __fastcall Exporta(AnsiString nome_arq);
	  bool     __fastcall InsereAlimentador(AnsiString cod_alim, AnsiString cod_sub);
	  bool     __fastcall InsereAlternativa(VTAlternativa *alternativa);
	  bool     __fastcall InsereAno(int ano_sel);

   private:
	  double __fastcall ComprimentoTotal(TList *lisTRECHO);
	  double __fastcall PerdaOriginal(AnsiString nome_alimentador, AnsiString nome_subestacao, int ano_sel);
	  void   __fastcall SalvaPerdaOriginal(AnsiString nome_alimentador, AnsiString nome_subestacao, int ano_sel, double perda);

   private:  //obj locais
	  std::set<AnsiString> alimentador;
	  std::map<AnsiString,double> perda_orig;
	  TList *lisALT_SEL;
      DynamicArray<int> ano;

   private:    //obj externo
      VTApl *apl;

};
//---------------------------------------------------------------------------
#endif
