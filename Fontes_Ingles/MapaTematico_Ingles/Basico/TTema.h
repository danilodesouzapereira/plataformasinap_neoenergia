//---------------------------------------------------------------------------
#ifndef TTemaH
#define TTemaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTTema.h"

//---------------------------------------------------------------------------
class TTema : public VTTema
   {
   public:  //m�todos
           __fastcall  TTema(VTApl *apl);
           __fastcall ~TTema(void);
      void __fastcall  Reinicia(int ind_pat);

   protected://m�todos a serem implementados nas classes derivadas
      virtual void __fastcall CalculaValorProprioCelula(void) = 0;
      virtual void __fastcall CalculaValorTotalCelula(void) = 0;
      virtual void __fastcall NormalizaValorTotalCelula(void) = 0;

   private:  //m�todos
      //TColor __fastcall  Degrade(double valor);
      //TColor __fastcall  DegradeCinza(double valor);

   protected:  //objetos externos
      VTApl *apl;

   protected:  //dados locais
      int ind_pat;
   };

//---------------------------------------------------------------------------
#endif
//eof
