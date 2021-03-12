#ifndef VTMontaArvoreH
#define VTMontaArvoreH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
struct strNODE {
               int        node_id;
               int        node_pai_id;
               int        obj_id;
               AnsiString obj_codigo;
               };

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTMontaArvore : public TObject
   {
	public:
						 __fastcall  VTMontaArvore(void) {};
						 __fastcall ~VTMontaArvore(void) {};
		virtual bool __fastcall  InsereNode(strNODE &str) = 0;
	};

//-----------------------------------------------------------------------------
VTMontaArvore* NewObjMontaArvore(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
