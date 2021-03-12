//---------------------------------------------------------------------------
#ifndef TMontaEqptoH
#define TMontaEqptoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TAue.h"
#include "VTMonta.h"

//---------------------------------------------------------------------------
class VTEqpto;

//---------------------------------------------------------------------------
class TMontaEqpto : public VTMonta
   {
   public:
                         __fastcall TMontaEqpto(void);
      virtual            __fastcall ~TMontaEqpto(void);
//      virtual VTGrupo* __fastcall IniciaGrupo(VTEqpto *eqpto);
//      virtual bool       __fastcall LisAtributos(VTEqpto *eqpto, TList *lisATR);
      virtual bool       __fastcall LisAtributosBasico(VTEqpto *eqpto, TList *lisATR);
      virtual bool       __fastcall LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR);
      virtual bool       __fastcall LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR);
      virtual bool       __fastcall LisValoresCurva(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      virtual bool       __fastcall LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      virtual bool       __fastcall LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      virtual bool       __fastcall LisVetValEqpto(VTEqpto *eqpto, TList *lisEXT, int ind_atrib);
      virtual bool       __fastcall LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano=0);
      virtual AnsiString __fastcall ValorAtributo(VTEqpto *eqpto, int ind_atrib);

   protected:
      void __fastcall ValorCodigo(void);
      void __fastcall ValorId(void);
      
   protected: //objetos externos
      VTEqpto *eqpto;
      TList   *lisEXT;

   protected: //dados locais
      TAue aue;
      int  ano;
      int  num_pat;
   };

//---------------------------------------------------------------------------
#endif
//eof
