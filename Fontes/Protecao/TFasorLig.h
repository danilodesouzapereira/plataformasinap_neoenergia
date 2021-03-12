//---------------------------------------------------------------------------
#ifndef TFasorLigH
#define TFasorLigH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTFasor.h"

//---------------------------------------------------------------------------
class TFasorLig : public VTFasor
   {
   public:
           __fastcall  TFasorLig(VTLigacao *ligacao, VTBarra *bar_ref);
           __fastcall ~TFasorLig(void);
      void __fastcall  AtualizaAngulo(double ang_novo);
      void __fastcall  DefinePatamar(int ind_pat);
      void __fastcall  RedefineBarraReferencia(VTBarra *bar_ref);
      int  __fastcall  Tipo(void);
      bool __fastcall  Valor(complex<double> &valor, bool pu=false);

   protected:  //métodos acessados via property

   private: //métodos
      double __fastcall Ibase_amp(void);
      void   __fastcall IniciaFasorIfas(void);
      void   __fastcall IniciaFasorIseq(void);

   private: //objetos externos
      VTLigacao *ligacao;

   private: //dados locais
      int             ind_bar, ind_pat;
      complex<double> corrente_amp;
   };

//---------------------------------------------------------------------------
#endif
//eof
