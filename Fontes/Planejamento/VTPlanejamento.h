//---------------------------------------------------------------------------
#ifndef VTPlanejamentoH
#define VTPlanejamentoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTIpe.h"

//---------------------------------------------------------------------------
class VTApl;
class VTEstudo;
//class VTIpe;

//----------------------------------------------------------------------------
//enum tipoINTEGRACAO {sistemaNENHUM=0, sistemaVIP, sistemaGRP, reclamacaoBT};

//---------------------------------------------------------------------------
class VTPlanejamento : public TObject
   {
   public:  //property
      __property bool       AnaliseEnabled = {read=PD.analise_enabled, write=PD.analise_enabled};
//      __property int        Integracao     = {read=PD.integracao,      write=PD.integracao};
      __property VTIpe*     Ipe            = {read=PD.ipe,             write=PD.ipe};
	  __property TAction*   ActionEdtObra  = {read=PD.ActionEdtObra,   write=PD.ActionEdtObra};
      __property TAction*   ActionRelObra  = {read=PD.ActionRelObra,   write=PD.ActionRelObra};


   public:
                        __fastcall  VTPlanejamento(void) {};
      virtual           __fastcall ~VTPlanejamento(void) {};
      virtual VTEstudo* __fastcall  ExisteEstudo(AnsiString codigo) = 0;
      virtual VTEstudo* __fastcall  ExisteEstudo(int id=-1) = 0;
      virtual bool      __fastcall  ExisteEstudoAlterado(void) = 0;
      virtual bool      __fastcall  ExisteEstudoNovo(void) = 0;
      virtual void      __fastcall  InsereActionAnalise(TAction *ActionExterna) = 0;
      virtual void      __fastcall  InsereActionRelatorio(TAction *ActionExterna) = 0;
      virtual VTEstudo* __fastcall  InsereEstudo(AnsiString codigo, int tipo, int estudo_id) = 0;
      virtual VTEstudo* __fastcall  InsereEstudoRedeOriginal(void) = 0;
      virtual TList*    __fastcall  LisActionAnalise(void) = 0;
      virtual TList*    __fastcall  LisActionRelatorio(void) = 0;
      virtual TList*    __fastcall  LisEstudo(void) = 0;
      virtual void      __fastcall  Reinicia(VTEstudo *estudo=NULL) = 0;
      virtual void      __fastcall  RetiraActionAnalise(TAction *ActionExterna) = 0;
      virtual void      __fastcall  RetiraActionRelatorio(TAction *ActionExterna) = 0;
      virtual bool      __fastcall  RetiraEstudo(VTEstudo *estudo) = 0;

   protected:  //dados acessados via property
      struct{
            bool      analise_enabled;
            //int       integracao;
			TAction   *ActionEdtObra;
			TAction   *ActionRelObra;
			VTIpe     *ipe;
            }PD;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTPlanejamento
//---------------------------------------------------------------------------
VTPlanejamento* __fastcall NewObjPlanejamento(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 