//-----------------------------------------------------------------------------
#ifndef VTValMalhasH
#define VTValMalhasH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTValMalhas : public TObject
   {
   public:
					 __fastcall  VTValMalhas(void) {};
		virtual      __fastcall ~VTValMalhas(void) {};
		virtual bool __fastcall  Executa(void)=0;

   public:  //property
		//malha externa
		__property bool	   AbreChaveExt  = {write=PD.abre_chave_ext};
		__property bool	   DeslocaTrechoExt  = {write=PD.desloca_trecho_ext};
		//malha interna
		__property bool	   AbreChaveInt  = {write=PD.abre_chave_int};
		__property bool	   DeslocaTrechoInt  = {write=PD.desloca_trecho_int};

   protected: //dados acessados via property
		struct  {
					bool abre_chave_ext;
					bool abre_chave_int;
					bool desloca_trecho_ext;
					bool desloca_trecho_int;
				} PD;

   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTMonta
//-----------------------------------------------------------------------------
VTValMalhas* __fastcall NewObjValMalhas(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
