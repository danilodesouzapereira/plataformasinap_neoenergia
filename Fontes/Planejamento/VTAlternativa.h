//---------------------------------------------------------------------------
#ifndef VTAlternativaH
#define VTAlternativaH

//----------------------------------------------------------------------------
#include <Classes.hpp>

//----------------------------------------------------------------------------
class VTApl;
class VTObra;
class VTObras;

//----------------------------------------------------------------------------
class VTAlternativa : public TObject
   {
   public:
      __property int         Id          = {read=PD.id,          write=PD.id          };
      __property AnsiString  Codigo      = {read=PD.codigo,      write=PD.codigo      };
      __property VTApl*      Apl         = {read=PD.apl};
      __property VTObras*    Obras       = {read=PD.obras};
      __property bool        Escolhida   = {read=PD.escolhida,   write=PD.escolhida   };
      __property TObject*    Object      = {read=PD.object,      write=PD.object      };
      __property double      CustoTotal  = {read=PD.custo_total, write=PD.custo_total };
      __property bool        Valida      = {read=PD.valida,      write=PD.valida      };
	  __property bool        Fixa        = {read=PM_GetFixa,     write=PM_SetFixa     };
      __property bool        Alterado    = {read=PM_GetAlterado, write=PM_SetAlterado};
	  __property AnsiString  Justificativa = {read=PD.justificativa, write=PD.justificativa };

   public:
                   __fastcall  VTAlternativa(void) {};
      virtual      __fastcall ~VTAlternativa(void) {};
      virtual void __fastcall  CopiaObras(VTAlternativa *alternativa) = 0;
      virtual void __fastcall  ReconfiguraRede(int ano) = 0;
      virtual void __fastcall  ReconfiguraRedeAteObra(VTObra *obra) = 0;
      virtual void __fastcall  ReconfiguraRedeSemObra(void) = 0;
      virtual void __fastcall  ReconfiguraRedeTodaObra(VTObra *obra=NULL) = 0;
      virtual void __fastcall  RetiraObraSemAcao(void) = 0;

   protected:    //métodos acessados via property
       virtual bool __fastcall PM_GetFixa(void) = 0;
	   virtual void __fastcall PM_SetFixa(bool fixa) = 0;
	   virtual bool __fastcall PM_GetAlterado(void)=0;
       virtual void __fastcall PM_SetAlterado(bool alterado) = 0;

   protected:    //dados acessados via property
      struct   {
			   int         id;
			   AnsiString  codigo;
			   AnsiString  justificativa;
               VTApl      *apl;
               VTObras    *obras;
               TObject    *object;
               bool        valida;
               bool        escolhida;
               bool        fixa;
               double      custo_total;
               } PD;
   };

//---------------------------------------------------------------------------
//função p/ criar método da classe
//---------------------------------------------------------------------------
VTAlternativa* __fastcall NewObjAlternativa(VTApl *apl);
//---------------------------------------------------------------------------
#endif
//eof
