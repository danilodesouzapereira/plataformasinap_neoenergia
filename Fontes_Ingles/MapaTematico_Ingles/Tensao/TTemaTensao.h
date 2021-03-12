//---------------------------------------------------------------------------
#ifndef TTemaTensaoH
#define TTemaTensaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Basico\TTema.h"

//---------------------------------------------------------------------------
class VTCelula;

//---------------------------------------------------------------------------
class TTemaTensao : public TTema
   {
   public:  //métodos
           __fastcall  TTemaTensao(VTApl *apl);
           __fastcall ~TTemaTensao(void);

   protected://métodos a serem implementados nas classes derivadas
      void __fastcall CalculaValorProprioCelula(void);
      void __fastcall CalculaValorTotalCelula(void);
      void __fastcall DefineCorCelula(void);
      void __fastcall NormalizaValorTotalCelula(void);

   private: //métodos locais
      TColor __fastcall CorClara(TColor color);
      TColor __fastcall CorEscura(TColor color);
      TColor __fastcall Degrade(TColor beginColor, TColor endColor, double percent);
      void   __fastcall IniciaEstruturas(void);
      float  __fastcall Porcentagem(double valor_inferior, double valor_superior, double valor);
      double __fastcall TensaoMinima(VTCelula *celula);

   private: //dados locais
      #define NUM_FAIXA 8
      struct strFAIXA{
                     double valor_inferior;
                     double valor_superior;
                     TColor cor_inferior;
                     TColor cor_superior;
                     }faixa[NUM_FAIXA];
      TList *lisEQP;
   };

//---------------------------------------------------------------------------
#endif
//eof
