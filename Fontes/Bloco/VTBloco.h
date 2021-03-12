//---------------------------------------------------------------------------
#ifndef VTBlocoH
#define VTBlocoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTBarra;
class VTLigacao;
class VTRede;

//---------------------------------------------------------------------------
class VTBloco : public TObject
   {
	public:	//property
		__property TColor  Color = {read=PM_GetColor, write=PM_SetColor};
		__property VTRede* Rede  = {read=PM_GetRede};
		__property TObject* Obj  = {read=PM_GetObj, write=PM_SetObj};

   public:
							__fastcall VTBloco(void) {};
      virtual        __fastcall ~VTBloco(void) {};
		virtual bool   __fastcall ExisteBarra(VTBarra *barra) = 0;
      virtual bool   __fastcall ExisteGeracao(void) = 0;
      virtual bool   __fastcall ExisteLigacao(VTLigacao *ligacao) = 0;
		virtual bool   __fastcall ExisteLigacaoDaRede(VTRede *rede) = 0;
      virtual void   __fastcall InsereBarra(VTBarra *barra) = 0;
      virtual void   __fastcall InsereLigacao(VTLigacao *ligacao) = 0;
      virtual TList* __fastcall LisBarra(void) = 0;
		virtual void   __fastcall LisEqbar(TList *lisEXT, int tipo=-1) = 0;
		virtual TList* __fastcall LisLigacao(void) = 0;
		virtual void   __fastcall LisLigacao(TList *lisEXT, int tipo=-1) = 0;
		virtual TList* __fastcall LisRede(void) = 0;

	protected:	//métodos acessados via property
      virtual TColor  __fastcall PM_GetColor(void) = 0;
		virtual VTRede* __fastcall PM_GetRede(void) = 0;
	  virtual void    __fastcall PM_SetColor(TColor color) = 0;
	  virtual TObject*   __fastcall PM_GetObj(void) = 0;
	  virtual void       __fastcall PM_SetObj(TObject* obj) = 0;
	public:
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTBloco* __fastcall NewObjBloco(void);

#endif
//---------------------------------------------------------------------------
//eof

