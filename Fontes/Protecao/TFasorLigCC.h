//---------------------------------------------------------------------------
#ifndef TFasorLigCCH
#define TFasorLigCCH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTFasor.h"

class VTLigCC;
//---------------------------------------------------------------------------
class TFasorLigCC : public VTFasor
   {
   public:
           __fastcall  TFasorLigCC(VTLigacao *ligacao, VTBarra *bar_ref);
           __fastcall ~TFasorLigCC(void);
      void __fastcall  AtualizaAngulo(double ang_novo);
      void __fastcall  DefinePatamar(int ind_pat);
      void __fastcall  RedefineBarraReferencia(VTBarra *bar_ref);
      int  __fastcall  Tipo(void);
      bool __fastcall  Valor(complex<double> &valor, bool pu=true);

   protected:  //métodos acessados via property

   private: //métodos
      double     __fastcall Ibase_amp(void);
      void       __fastcall IniciaFasorIfas(void);
      void       __fastcall IniciaFasorIseq(void);
      AnsiString __fastcall NomeDefeito(int ind_def);

   private: //objetos externos
      VTLigacao *ligacao;

   private: //dados locais
      int             ind_bar, ind_pat;
      complex<double> corrente_amp;
   };

//---------------------------------------------------------------------------
#endif
//eof
