//-----------------------------------------------------------------------------
#ifndef VTMalhaH
#define VTMalhaH

//arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTBarra;
class VTLigacao;
class VTRede;

//-----------------------------------------------------------------------------
class VTMalha  : public TObject
   {
	public:
							 __fastcall  VTMalha(void)  {};
		virtual         __fastcall ~VTMalha(void) {};
		virtual bool    __fastcall  ExisteLigacaoComum(VTMalha *malha) = 0;
		virtual VTRede* __fastcall  FirstRede(void) = 0;
		virtual void    __fastcall  IncorporaMalha(VTMalha *malha) = 0;
		virtual bool    __fastcall  InsereLigacao(TList *lisEXT) = 0;
		virtual TList*  __fastcall  LisBarra(void) = 0;
		virtual TList*  __fastcall  LisLigacao(void) = 0;
		virtual TList*  __fastcall  LisRede(void) = 0;
		virtual int     __fastcall  NumeroDeLigacoes(VTRede *rede=NULL, int tipo=-1) = 0;
		virtual int     __fastcall  NumeroDeRedes(void) = 0;
    };

//-----------------------------------------------------------------------------
//função global
VTMalha* __fastcall NewObjMalha(void);

#endif
//---------------------------------------------------------------------------
//eof
