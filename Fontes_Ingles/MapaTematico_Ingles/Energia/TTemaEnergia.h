//---------------------------------------------------------------------------
#ifndef TTemaEnergiaH
#define TTemaEnergiaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Basico\TTema.h"

//---------------------------------------------------------------------------
class VTCelula;

//---------------------------------------------------------------------------
class TTemaEnergia : public TTema
   {
   public:  //m�todos
           __fastcall  TTemaEnergia(VTApl *apl);
           __fastcall ~TTemaEnergia(void);

   protected://m�todos a serem implementados nas classes derivadas
      void __fastcall CalculaValorProprioCelula(void);
      void __fastcall CalculaValorTotalCelula(void);
      void __fastcall DefineCorCelula(void);
      void __fastcall NormalizaValorTotalCelula(void);

   private: //m�todos locais
      double __fastcall ConsumoEnergia(VTCelula *celula);
      TColor __fastcall Degrade(double valor);
      TColor __fastcall DegradeCinza(double valor);

   private: //dados locais
      TList *lisEQP;
   };

//---------------------------------------------------------------------------
#endif
//eof
