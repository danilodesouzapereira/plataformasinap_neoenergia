//---------------------------------------------------------------------------
#ifndef VTLegendaH
#define VTLegendaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;
class VTGrupo;
//class VTCanal;

//---------------------------------------------------------------------------
class VTLegenda : public TObject
   {
   public:
                         __fastcall VTLegenda(void) {};
                         __fastcall ~VTLegenda(void) {};
      virtual VTGrupo*   __fastcall IniciaGrupo(VTEqpto *eqpto) = 0;
      virtual bool       __fastcall LisAtributos(VTEqpto *eqpto, TList *lisATR) = 0;
      virtual bool       __fastcall LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR) = 0;
      virtual bool       __fastcall LisValores(VTEqpto *eqpto, TList *lisDADO, int ano=0, int num_pat=0) = 0;
      virtual bool       __fastcall LisValoresEqpto(VTEqpto *eqpto, TList *lisDADO, int ano=0, int num_pat=0) = 0;
      virtual bool       __fastcall LisValoresFluxo(VTEqpto *eqpto, TList *lisDADO, int ano=0, int num_pat=0) = 0;
      virtual bool       __fastcall LisVetValEqpto(VTEqpto *eqpto, TList *lisVET, int ind_atrib) = 0;
      virtual bool       __fastcall LisVetValFluxo(VTEqpto *eqpto, TList *lisVET, int ind_atrib, int ano=0) = 0;
      virtual AnsiString __fastcall ValorAtributo(VTEqpto *eqpto, int ind_atrib) = 0;
   };

//-----------------------------------------------------------------------------
// funções globais para criar objetos da classe VTLegenda
//-----------------------------------------------------------------------------
VTLegenda* NewObjLegenda(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
