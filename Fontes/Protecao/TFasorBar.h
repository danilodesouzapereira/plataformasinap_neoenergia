//---------------------------------------------------------------------------
#ifndef TFasorBarH
#define TFasorBarH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTFasor.h"

//---------------------------------------------------------------------------
class VTEqpto;

//---------------------------------------------------------------------------
class TFasorBar : public VTFasor
   {
   public:
           __fastcall  TFasorBar(VTBarra *barra);
           __fastcall ~TFasorBar(void);
      void __fastcall  AtualizaAngulo(double ang_novo);
      void __fastcall  DefinePatamar(int ind_pat);
      void __fastcall  RedefineBarraReferencia(VTBarra *barra);
      int  __fastcall  Tipo(void);
      bool __fastcall  Valor(complex<double> &valor, bool pu=true);

   protected:  //métodos acessados via property

   private: //métodos
      void __fastcall IniciaFasorVfas(void);
      void __fastcall IniciaFasorVSeq(void);

   private: //objetos externos
      VTBarra *barra;

   private: //dados locais
      int  ind_pat;
      complex<double> tensao_kv, tensao_pu;
   };

//---------------------------------------------------------------------------
#endif
//eof
