//---------------------------------------------------------------------------
#ifndef VTCalculaSupH
#define VTCalculaSupH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Constante\Fases.h"
#include "..\Rede\Estrutura.h"

//---------------------------------------------------------------------------
class VTApl;
class VTGerador;
class VTSuprimento;

//---------------------------------------------------------------------------
class VTCalculaSup : public TObject
   {
   public:
                   __fastcall VTCalculaSup(void) {};
      virtual      __fastcall ~VTCalculaSup(void) {};
      virtual bool __fastcall GetValorGerador(VTGerador *gerador, int np, strSUP *str_sup) = 0;
      virtual bool __fastcall GetValorGeradorReducao(VTGerador *gerador, int np, strSUP vetor[MAX_FASE]) = 0;
      virtual bool __fastcall GetValorSuprimento(VTSuprimento *suprimento, int np, strSUP *str_sup) = 0;
      virtual bool __fastcall GetValorSuprimentoReducao(VTSuprimento *suprimento, int np, strSUP vetor[MAX_FASE]) = 0;
      virtual bool __fastcall SetValorGerador(VTGerador *gerador, int np, strSUP *str_sup) = 0;
      virtual bool __fastcall SetValorGeradorReducao(VTGerador *gerador, int np, strSUP vetor[MAX_FASE]) = 0;
      virtual bool __fastcall SetValorSuprimento(VTSuprimento *suprimento, int np, strSUP *str_sup) = 0;
      virtual bool __fastcall SetValorSuprimentoReducao(VTSuprimento *suprimento, int np, strSUP vetor[MAX_FASE]) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTCalculaSup* __fastcall NewObjCalculaSup(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
