//---------------------------------------------------------------------------
#ifndef VTSegmentoH
#define VTSegmentoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\VTArea.h"
//#include "..\Rede\VTRede.h"

//---------------------------------------------------------------------------
enum SEG_MERCADO {smDEFAULT=1, smTIPOREDE, smREDE, smAREA, smCARGA, smREGIAO, smINDEF=0};

//---------------------------------------------------------------------------
class VTClasse;
class VTCrescimento;
class VTRegiao;

//---------------------------------------------------------------------------
class VTSegmento : public TObject
   {
   public:  //property
		__property int        Tipo           = {read=PM_GetTipo,           write=PM_SetTipo};
		__property AnsiString Codigo         = {read=PM_GetCodigo,         write=PM_SetCodigo};
      __property bool       Alterada       = {read=PM_GetAlterada,       write=PM_SetAlterada};
		__property VTArea*    Area           = {read=PM_GetArea,           write=PM_SetArea};
		__property VTRegiao*  Regiao         = {read=PM_GetRegiao,         write=PM_SetRegiao};
		__property int        Id             = {read=PM_GetId,             write=PM_SetId};
      __property bool       Nova           = {read=PM_GetNova,           write=PM_SetNova};
		__property bool       Enabled        = {read=PM_GetEnabled,        write=PM_SetEnabled};

   public:  //métodos
                             __fastcall  VTSegmento(void) {};
      virtual                __fastcall ~VTSegmento(void) {};
	  virtual VTCrescimento* __fastcall  ExisteCrescimento(VTClasse *classe) = 0;
      virtual void           __fastcall  IniciaCrescimentoNulo(void) = 0;
      virtual bool           __fastcall  InsereCrescimento(VTCrescimento *crescimento) = 0;
      virtual TList*         __fastcall  LisCrescimento(void) = 0;
      virtual void           __fastcall  RedefinePeriodo(int ano_ini, int ano_fim) = 0;

   protected:  //métodos acessados via properpy
        virtual VTArea*    __fastcall PM_GetArea(void) = 0;
		virtual bool       __fastcall PM_GetAlterada(void) = 0;
		virtual AnsiString __fastcall PM_GetCodigo(void) = 0;
		virtual bool       __fastcall PM_GetEnabled(void) = 0;
		virtual int        __fastcall PM_GetId(void) = 0;
		virtual bool       __fastcall PM_GetNova(void) = 0;
		virtual VTRegiao*  __fastcall PM_GetRegiao(void) = 0;
		virtual int        __fastcall PM_GetTipo(void) = 0;
		virtual void       __fastcall PM_SetAlterada(bool alterada) = 0;
      virtual void       __fastcall PM_SetArea(VTArea *area) = 0;
      virtual void       __fastcall PM_SetCodigo(AnsiString codigo) = 0;
		virtual void       __fastcall PM_SetEnabled(bool enabled) = 0;
		virtual void       __fastcall PM_SetId(int Segmento_id) = 0;
		virtual void       __fastcall PM_SetNova(bool nova) = 0;
		virtual void       __fastcall PM_SetRegiao(VTRegiao *regiao) = 0;
		virtual void       __fastcall PM_SetTipo(int tipo) = 0;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTSegmento
//---------------------------------------------------------------------------
VTSegmento* __fastcall NewObjSegmento(void);

//---------------------------------------------------------------------------
#endif
//eof
