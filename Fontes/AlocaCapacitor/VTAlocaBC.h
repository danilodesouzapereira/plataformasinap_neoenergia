//---------------------------------------------------------------------------
#ifndef VTAlocaBCH
#define VTAlocaBCH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTAlocaBC : public TObject
   {
   public:
                     __fastcall  VTAlocaBC(void) {};
      virtual        __fastcall ~VTAlocaBC(void){};
      virtual bool   __fastcall  Executa(TList *lisTabCap) = 0;
      virtual TList* __fastcall  ListaBancoAlocado(void) = 0;
      virtual int    __fastcall  ListaCapacitorAlocado(TList *lisEXT) = 0;
      virtual int    __fastcall  ListaCloneCapacitorAlocado(TList *lisEXT) = 0;
		virtual void   __fastcall  Reinicia(void) = 0;
		virtual bool   __fastcall  RetiraBancosAlocadosDaRede(TList *lisCap) = 0;
		virtual int    __fastcall  VerificaPrimariosIsolados(TList *lisEXT)  = 0;

	public:
      struct{
            bool        qmax_definido;
            double      qmax_mvar;
            bool        fatpot_definido;
            double      fatpot_min;
            bool        dist_definido;
            double      dist_m;
            double      custo_energia;  //$/MWh
            bool        tronco;         //DVK 2013.07.24
            bool        sempre_ligado;  //DVK 2014.11.11
            int         ind_patamar;       //DVK 2014.11.11
            }cfg;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTAjuste
//---------------------------------------------------------------------------
VTAlocaBC* __fastcall NewObjAlocaBC(VTApl *apl);
//---------------------------------------------------------------------------
#endif
//eof


