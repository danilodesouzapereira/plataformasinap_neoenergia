//---------------------------------------------------------------------------
#ifndef TFasorBarCCH
#define TFasorBarCCH

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTFasor.h"

//---------------------------------------------------------------------------
class TFasorBarCC : public VTFasor
   {
   public:
           __fastcall  TFasorBarCC(VTBarra *barra);
           __fastcall ~TFasorBarCC(void);
      void __fastcall  AtualizaAngulo(double ang_novo);
      void __fastcall  DefinePatamar(int ind_pat);
      void __fastcall  RedefineBarraReferencia(VTBarra *barra);
      int  __fastcall  Tipo(void);
      bool __fastcall  Valor(complex<double> &valor, bool pu=true);

   private: //m�todos
      void       __fastcall IniciaFasorVfas(void);
      void       __fastcall IniciaFasorVseq(void);
      AnsiString __fastcall NomeDefeito(int ind_def);

   private: //objetos externos
      VTBarra *barra;

   private: //dados locais
      int             ind_pat;
      complex<double> tensao_kv, tensao_pu;
   };

//---------------------------------------------------------------------------
#endif
//eof
