//---------------------------------------------------------------------------
#ifndef VTAvaliaH
#define VTAvaliaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTIndividuo;
//---------------------------------------------------------------------------
class VTAvalia: public TObject
   {
   public:
                   __fastcall VTAvalia(void) {};
                   __fastcall ~VTAvalia(void) {};
			  virtual bool         __fastcall Executa(VTIndividuo* indiv, TList* lisALL = NULL) = 0;
			  virtual void         __fastcall ImprimeInd(VTIndividuo* indiv, AnsiString descricao, bool flag_cab = false) = 0;
			  virtual VTIndividuo* __fastcall Individuo(void) = 0;
			  virtual VTIndividuo* __fastcall IndivInicial(void) = 0;
			  virtual void         __fastcall Inicia(void) = 0;
			  virtual VTIndividuo* __fastcall NovoIndividuo(VTIndividuo* ind1, bool flag_orig = false) = 0;

   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTDespacho
//---------------------------------------------------------------------------
VTAvalia* __fastcall NewObjAvaliaGD(VTApl* apl);
VTAvalia* __fastcall NewObjAvaliaDespacho(VTApl* apl);
VTAvalia* __fastcall NewObjAvaliaReconf(VTApl* apl);
VTAvalia* __fastcall NewObjAvaliaRecRede(VTApl* apl);
VTAvalia* __fastcall NewObjAvaliaRecRedeTopo(VTApl* apl);
VTAvalia* __fastcall NewObjAvaliaTurmas(VTApl* apl);
VTAvalia* __fastcall NewObjAvaliaTurmasEscala(VTApl* apl);
VTAvalia* __fastcall NewObjAvaliaPosIF(VTApl* apl);
VTAvalia* __fastcall NewObjAvaliaDistribIF(VTApl* apl);
//---------------------------------------------------------------------------
#endif
//eof

           //virtual TList*       __fastcall RelSaida(void) = 0;
           //virtual AnsiString   __fastcall NomeVar(TLocus* ploc_aval) = 0;



