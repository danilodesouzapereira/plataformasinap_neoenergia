//---------------------------------------------------------------------------
#ifndef TFasorAjusteH
#define TFasorAjusteH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTFasor.h"

//---------------------------------------------------------------------------
class TFasorAjuste : public VTFasor
   {
   public:
           __fastcall  TFasorAjuste(VTFasor *fasorI, VTFasor *fasorV, char classe);
           __fastcall ~TFasorAjuste(void);
      void __fastcall  DefinePatamar(int ind_pat);
      void __fastcall  RedefineBarraReferencia(VTBarra *bar_ref);
      int  __fastcall  Tipo(void);
      bool __fastcall  Valor(complex<double> &valor, bool pu=true);
      bool __fastcall  ValorAngulo(double &ang_rad);

   public: //métodos exclusivos desta classe derivada
      char __fastcall  Classe(void);
      void __fastcall  DefineAnguloAjuste_rad(double ang_rad);
      bool __fastcall  ValorAnguloIni_rad(double &ang_rad);
      bool __fastcall  ValorAnguloFim_rad(double &ang_rad);

   protected:  //métodos acessados via property

   private: //métodos
      void __fastcall IniciaFasor(void);

   private: //objetos externos
      VTFasor *fasorI, *fasorV;
      double  ang_ajuste_rad;

   private: //dados locais
      int  ind_pat;
      char classe;
   };

//---------------------------------------------------------------------------
#endif
//eof
