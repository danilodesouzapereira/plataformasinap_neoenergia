//---------------------------------------------------------------------------
#ifndef VTSelecaoH
#define VTSelecaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
//class VTCorte;
class VTApl;
class VTBD;
class VTEstudo;
class VTRede;

//---------------------------------------------------------------------------
class VTSelecao : public TObject
   {
   public:
                     __fastcall  VTSelecao(void) {};
                     __fastcall ~VTSelecao(void) {};
	  virtual bool   __fastcall  CarregaRedeAdicional(VTBD *bd, VTEstudo *estudo, TList *lisREDE_SEL, int aviso_num_rede) = 0;
	  virtual bool   __fastcall  CarregaRedeAdicional(VTBD *bd, VTEstudo *estudo, TList *lisREDE_PRE, TList *lisREDE_SEL, int aviso_num_rede) = 0;
	  virtual bool   __fastcall  CarregaEstudo(VTBD *bd, VTEstudo *estudo) = 0;
	  virtual bool   __fastcall  CarregaRedeCompleta(VTBD *bd, VTEstudo *estudo, int aviso_num_rede) = 0;
      virtual bool   __fastcall  CarregaRedeSelecionada(VTBD *bd, VTEstudo *estudo, TList *lisREDE_SEL, int aviso_num_rede) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTSelecao
//---------------------------------------------------------------------------
VTSelecao* __fastcall NewObjSelecao(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 