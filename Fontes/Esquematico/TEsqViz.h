//---------------------------------------------------------------------------
#ifndef TEsqVizH
#define TEsqVizH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEsqViz.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTCorte;
class VTLigacao;

//---------------------------------------------------------------------------
class TEsqViz : public VTEsqViz
   {
   public:
           __fastcall TEsqViz(VTApl *apl);
           __fastcall ~TEsqViz(void);
      void __fastcall Executa(VTRede *rede);

   private: //métodos
      VTBarra* __fastcall ExisteBarraComum(VTLigacao *ligacao, TList *lisBAR);
      void     __fastcall IniciaLisBarraComum(VTRede *rede1, VTRede *rede2, TList *lisBAR);
      void     __fastcall IniciaLisLigacao(VTRede *rede, TList *lisBAR, TList *lisLIG);
      void     __fastcall MontaEsquematicoRede(VTRede *rede, TList *lisBAR, TList *lisLIG);
      void     __fastcall ReiniciaEsquematicos(void);

   private: //dados externos
      VTApl   *apl;

   private: //dados locais
      TList *lisBAR;
      TList *lisLIG;
      TList *lisVIZ;
      TList *lisTMP;
      TList *lisESQ; //lista p/ salvar Esquemáticos já criados
   };

//---------------------------------------------------------------------------
#endif
//eof

