//---------------------------------------------------------------------------
#ifndef VTObrasH
#define VTObrasH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;
class VTObra;
class VTAcao;

//---------------------------------------------------------------------------
class VTObras : public TObject
   {
   public:
		__property TDateTime DataInicial   = {read=PM_GetDataInicial                            };
		__property TDateTime DataFinal     = {read=PM_GetDataFinal                              };
		__property bool      Modificada    = {read=PM_GetModificada , write=PM_SetModificada    };
		__property bool      Analisada     = {read=PM_GetAnalisada  , write=PM_SetAnalisada     };
		__property TColor    Color         = {                        write=PM_SetColor         };
		__property bool      ColorEnabled  = {                        write=PM_SetColorEnabled  };

   public:
                       __fastcall  VTObras(void) {};
      virtual          __fastcall ~VTObras(void) {};
      virtual VTObras* __fastcall  Clone(void) = 0;
	  virtual void     __fastcall  CopiaAtributosDe(VTObras &obras) = 0;
      virtual void     __fastcall DefineRedesVisiveis(VTObra *obra) = 0;
//      virtual bool     __fastcall  EfetivaObra(VTObra *obra) = 0;
      virtual VTObra*  __fastcall  ExisteObra(int obra_id) = 0;
	  virtual void     __fastcall  FundeObras(void) = 0;
      virtual void     __fastcall  IniciaListaRedesPorObra(void) = 0;
      virtual VTObra*  __fastcall  InsereObra(void) = 0;
      virtual VTObra*  __fastcall  InsereObra(TDateTime data) = 0;
      virtual bool     __fastcall  InsereObra(VTObra *obra) = 0;
      virtual VTAcao*  __fastcall  LastAcao(int tipo, VTEqpto *eqpto)=0;
      virtual TList*   __fastcall  LisAcao(void) = 0;
      virtual TList*   __fastcall  LisObra(void) = 0;
      virtual void     __fastcall  LisObra(TList *lisEXT, int ano) = 0;
	  virtual void 	   __fastcall  LisObraContinuidade(TList *lisEXT, int ano) = 0;
      virtual TList*   __fastcall  LisObraExcluida(void) = 0;
      virtual VTObra*  __fastcall  ObraFinal(void) = 0;
      virtual VTObra*  __fastcall  ObraInicial(void) = 0;
      virtual void     __fastcall  ReconfiguraRede(int ano) = 0;
      virtual void     __fastcall  ReconfiguraRede(VTObra *obra_ant, VTObra *obra_ref) = 0;
      virtual void     __fastcall  ReconfiguraRedeAteObra(VTObra *obra_ref) = 0;
      virtual void     __fastcall  ReconfiguraRedeSemObra(void) = 0;
      virtual void     __fastcall  ReconfiguraRedeTodaObra(VTObra *obra_ref) = 0;
      virtual void     __fastcall  Reinicia(void) = 0;
      virtual void     __fastcall  RetiraObra(VTObra *obra) = 0;
      virtual bool     __fastcall  VerificaEqptoBloqueado(VTEqpto *eqpto) = 0;
      virtual bool     __fastcall  VerificaPossibilidadeExclusao(VTObra *obra) = 0;

   protected:  //métodos acessados via property
      virtual bool      __fastcall PM_GetAnalisada(void) = 0;
      virtual TDateTime __fastcall PM_GetDataFinal(void) = 0;
      virtual TDateTime __fastcall PM_GetDataInicial(void) = 0;
		virtual bool      __fastcall PM_GetModificada(void) = 0;
		//
      virtual void      __fastcall PM_SetAnalisada(bool analisada) = 0;
		virtual void      __fastcall PM_SetColor(TColor color) = 0;
		virtual void      __fastcall PM_SetColorEnabled(bool enabled) = 0;
		virtual void      __fastcall PM_SetModificada(bool modificada) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTObras* __fastcall NewObjObras(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
