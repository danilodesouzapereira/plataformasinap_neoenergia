//---------------------------------------------------------------------------
#ifndef TDbar_H
#define TDbar_H

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class TDgbt;

//---------------------------------------------------------------------------
class TDbar : TObject
   {
   public:
           __fastcall TDbar(void);
           __fastcall ~TDbar(void);
      bool __fastcall Excluida(void);
      bool __fastcall ExisteCapacitor(void);
      bool __fastcall ExisteCarga(void);
      bool __fastcall ExisteGeracao(void);
      bool __fastcall ExisteReator(void);
      bool __fastcall TipoPV(void);
      bool __fastcall TipoVF(void);

   public:  //dados oriundos do Anarede
      int        numero;                 // 00
      AnsiString operacao;               // 01
		AnsiString estado;                 // 02
		int        tipo;                   // 03
		AnsiString grupo_base_tensao;      // 04
		AnsiString nome;                   // 05
		AnsiString grupo_limite_tensao;    // 06
      double     tensao_pu;              // 07
		double     angulo_graus;           // 08
		double     geracao_mw;             // 09
		double     geracao_mvar;           // 10
		double     geracao_min_mvar;       // 11
		double     geracao_max_mvar;       // 12
      int        barra_controlada;       // 13
		double     carga_mw;               // 14
		double     carga_mvar;             // 15
		double     capac_reator_mvar;      // 16
		int        area;                   // 17
		double     tensao_carga_pu;        // 18

   public:  //dados adicionais
      int   id;
      int   coord_x, coord_y;
      bool  conectada;
      TDgbt *dgbt;
   };

#endif
//-----------------------------------------------------------------------------
// eof
