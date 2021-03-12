//-----------------------------------------------------------------------------
#ifndef VTMalhasH
#define VTMalhasH

//arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;
class VTMalha;

//-----------------------------------------------------------------------------
class VTMalhas  : public TObject
   {
   public:
                     __fastcall  VTMalhas(void)  {};
      virtual        __fastcall ~VTMalhas(void) {};
		virtual bool   __fastcall  Inicia(void) = 0;
		virtual bool   __fastcall  InsereMalha(VTMalha *malha) = 0;
		virtual bool   __fastcall  InsereMalhaGeracao(VTMalha *malha) = 0;
		virtual TList* __fastcall  LisMalhaExterna(void) = 0;
		virtual TList* __fastcall  LisMalhaGeracao(void) = 0;
		virtual TList* __fastcall  LisMalhaInterna(void) = 0;
		virtual bool   __fastcall  RemoveMalha(VTMalha *malha) = 0;
	 };

//-----------------------------------------------------------------------------
//função global
VTMalhas* __fastcall NewObjMalhas(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
