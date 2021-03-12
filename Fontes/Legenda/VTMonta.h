//---------------------------------------------------------------------------
#ifndef VTMontaH
#define VTMontaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;
class VTGrupo;
class VTCanal;

//---------------------------------------------------------------------------
class VTMonta : public TObject
   {
   public:
                         __fastcall VTMonta(void) {};
      virtual            __fastcall ~VTMonta(void) {};
      virtual VTGrupo*   __fastcall IniciaGrupo(VTEqpto *eqpto) = 0;
      virtual bool       __fastcall LisAtributos(VTEqpto *eqpto, TList *lisATR) = 0;
      virtual bool       __fastcall LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR) = 0;
      virtual bool       __fastcall LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR) = 0;
      virtual bool       __fastcall LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat) = 0;
      virtual bool       __fastcall LisValoresCurva(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat) = 0;
      virtual bool       __fastcall LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat) = 0;
      virtual bool       __fastcall LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat) = 0;
      virtual bool       __fastcall LisVetValEqpto(VTEqpto *eqpto, TList *lisEXT, int ind_atrib) = 0;
      virtual bool       __fastcall LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano=0) = 0;
      //virtual bool       __fastcall LisVetValCanal(VTCanal *canal, TList *lisEXT, int ind_atrib, int ano=0) {return false;};
      virtual AnsiString __fastcall ValorAtributo(VTEqpto *eqpto, int ind_atrib) = 0;
     };

//---------------------------------------------------------------------------
//função global p/ criar objetos VTMonta
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18686973
VTMonta* __fastcall NewObjMontaArranjo(VTApl *apl);
VTMonta* __fastcall NewObjMontaBarra(VTApl *apl);
VTMonta* __fastcall NewObjMontaBateria(VTApl *apl);
VTMonta* __fastcall NewObjMontaCapacitor(VTApl *apl);
VTMonta* __fastcall NewObjMontaCapserie(VTApl *apl);
VTMonta* __fastcall NewObjMontaCarga(VTApl *apl);
VTMonta* __fastcall NewObjMontaChave(VTApl *apl);
VTMonta* __fastcall NewObjMontaGerador(VTApl *apl);
//VTMonta* __fastcall NewObjMontaCanal(VTApl *apl);
VTMonta* __fastcall NewObjMontaMedidor(VTApl *apl);
VTMonta* __fastcall NewObjMontaMutua(VTApl *apl);
VTMonta* __fastcall NewObjMontaReator(VTApl *apl);
VTMonta* __fastcall NewObjMontaRegulador(VTApl *apl);
VTMonta* __fastcall NewObjMontaReducao(VTApl *apl);
VTMonta* __fastcall NewObjMontaSuprimento(VTApl *apl);
VTMonta* __fastcall NewObjMontaTrafo(VTApl *apl);
VTMonta* __fastcall NewObjMontaTrafoZZ(VTApl *apl);
VTMonta* __fastcall NewObjMontaTrafo3E(VTApl *apl);
VTMonta* __fastcall NewObjMontaTrecho(VTApl *apl);
VTMonta* __fastcall NewObjMontaYref(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof
