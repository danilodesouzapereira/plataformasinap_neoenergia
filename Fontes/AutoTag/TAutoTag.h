//---------------------------------------------------------------------------

#ifndef TAutoTagH
#define TAutoTagH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAutoTag.h"
//---------------------------------------------------------------------------
class VTApl;
class VTTagger;
class VTTagFormatter;
class VTEqpto;
class VTFiguraComentario;
//---------------------------------------------------------------------------
class TAutoTag : public VTAutoTag
{
	public:  //métodos
			__fastcall  TAutoTag(VTApl *apl);
			__fastcall ~TAutoTag(void);
	bool    __fastcall Inicializa(void);
	bool 	__fastcall InsereTagAcao(VTAcao *acao, bool undo);

	private:  //metodos
		VTTagFormatter* __fastcall CriaFormatter(void);
		VTTagger* 		__fastcall CriaTagger(void);

	protected:
		void __fastcall PM_SetChaveManobra(bool enabled);



	private:  //dados externos
		VTApl *apl_;

    private: //dados proprios
		VTTagFormatter 	*tag_formatter_;
		VTTagger 		*tagger_;

};
//---------------------------------------------------------------------------
#endif
