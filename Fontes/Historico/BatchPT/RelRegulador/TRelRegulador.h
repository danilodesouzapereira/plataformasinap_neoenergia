//---------------------------------------------------------------------------

#ifndef TRelReguladorH
#define TRelReguladorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTRegulador;

//---------------------------------------------------------------------------
class TRelRegulador : public TObject
{
   public:  //métodos
		   __fastcall  TRelRegulador(VTApl *apl);
		   __fastcall ~TRelRegulador(void);
   void    __fastcall Executa(TDateTime data);
   void 	   __fastcall SalvaArquivo(AnsiString filename);
   AnsiString  __fastcall MesStr(TDateTime data);

   private:
    AnsiString __fastcall Bypass(VTRegulador *regulador);
	AnsiString __fastcall Configuracao(VTRegulador *regulador);
	AnsiString __fastcall EnergiaMensalporFase(VTRegulador *regulador, int indFase);
	bool       __fastcall ExistePmwNegativo(VTRegulador *regulador, int indFase);
	AnsiString __fastcall Fases(VTRegulador *regulador);
	AnsiString __fastcall Header(void);
	int 	   __fastcall IndiceBarraMontante(VTRegulador *regulador);
	bool 	   __fastcall LicencaOk(void);
	AnsiString __fastcall Ligacao(VTRegulador *regulador);
	AnsiString __fastcall LinhaRegulador(VTRegulador *regulador);



   private: //dados externos
	 VTApl *mApl;

   private: //dados locais
      TList *lisLIG;
	  TList *lisREDE;
	  TStringList *linhas;
	  int nDiasMes;
	  TDateTime mDate;

};

//---------------------------------------------------------------------------
#endif
//eof

