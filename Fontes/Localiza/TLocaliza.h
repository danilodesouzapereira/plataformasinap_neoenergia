//---------------------------------------------------------------------------
#ifndef TLocalizaH
#define TLocalizaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTLocaliza.h"

//---------------------------------------------------------------------------
class VTConsulta;
class VTLegenda;

//---------------------------------------------------------------------------
class TLocaliza : public VTLocaliza
   {
   public:
              __fastcall  TLocaliza(VTApl *apl);
              __fastcall ~TLocaliza(void);
      TList*  __fastcall  LisEqpto(VTRedes *redes, int tipo_eqpto, int operador,
                                   int ind_atrib, AnsiString val_alvo, bool area_visivel);
      TList*  __fastcall  LisEqptoErroGis(VTRedes *redes, int tipo_eqpto, bool area_visivel);

   private: //métodos
      bool   __fastcall ComparaAnsi(AnsiString nomeeqpto, AnsiString regra);
      bool   __fastcall ComparaChar(char *nome, char *regra);
	  TList* __fastcall LocalizaPorCodigo(VTRedes *redes, int tipo_eqpto, AnsiString filtro);
		bool   __fastcall ValorValido(int operador, AnsiString valor, AnsiString val_alvo);
	  bool   __fastcall ValorValido(int operador, double valor, double val_alvo);


   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      VTConsulta  *consulta;
      VTLegenda   *legenda;
	  TList       *lisEQP;
      TStrings    *lisSTRAUX;
   };

#endif
//---------------------------------------------------------------------------
//eof
