//---------------------------------------------------------------------------
#ifndef TDlin_H
#define TDlin_H

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class TDbar;

//---------------------------------------------------------------------------
class TDlin : TObject
   {
   public:
           __fastcall TDlin(void);
           __fastcall ~TDlin(void);
      bool __fastcall Excluida(void);
      bool __fastcall TipoCapserie(void);
      bool __fastcall TipoRegulador(void);
      bool __fastcall TipoTrafo(void);
      bool __fastcall TipoTrecho(void);

   public:  //dados oriundos do Anarede
		int        da_barra;                   // 00
		AnsiString abertura_da_barra;          // 01
		AnsiString operacao;                   // 02
		AnsiString abertura_para_barra;        // 03
		int        para_barra;                 // 04
		int        circuito;                   // 05
		AnsiString estado;                     // 06
		AnsiString proprietario;               // 07
		double     resistencia_pu;             // 08
		double     reatancia_pu;               // 09
		double     susceptancia_shunt_mvar;    // 10
		double     tap_pu;                     // 11
		double     tap_min_pu;                 // 12
		double     tap_max_pu;                 // 13
		double     defasagem;                  // 14
		int        barra_controlada;           // 15
		double     capacidade_normal;          // 16
		double     capacidade_emergencia;      // 17
		int        numero_steps;               // 18

   public:  //dados adicionais
      int   id;
      TDbar *dbar1, *dbar2;

   private:  //dados
      struct strTIPO {
                     bool capserie;
                     bool regulador;
                     bool trafo;
                     bool trecho;
                     };

   private:  //métodos
      void __fastcall VerificaTipo(strTIPO &tipo);
   };

#endif
//-----------------------------------------------------------------------------
// eof
