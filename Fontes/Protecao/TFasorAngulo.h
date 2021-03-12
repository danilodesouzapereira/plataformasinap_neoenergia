//---------------------------------------------------------------------------
#ifndef TFasorAnguloH
#define TFasorAnguloH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTFasor.h"

//---------------------------------------------------------------------------
class TFasorAngulo : public VTFasor
   {
   public:
           __fastcall  TFasorAngulo(VTFasor *fasor1, VTFasor *fasor2);
           __fastcall ~TFasorAngulo(void);
      void __fastcall  AtualizaAngulo(double ang_novo);
      void __fastcall  DefinePatamar(int ind_pat);
      void __fastcall  RedefineBarraReferencia(VTBarra *bar_ref);
      int  __fastcall  Tipo(void);
      bool __fastcall  Valor(complex<double> &valor, bool pu=true);
      bool __fastcall  ValorAngulo(double &ang_rad);

   protected:  //métodos acessados via property

   private: //métodos
      void __fastcall IniciaFasor(void);

   private: //objetos externos
      VTFasor *fasor1, *fasor2;

   private: //dados locais
      int  ind_pat;
   };

//---------------------------------------------------------------------------
#endif
//eof
