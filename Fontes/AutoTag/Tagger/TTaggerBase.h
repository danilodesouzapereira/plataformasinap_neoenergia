//---------------------------------------------------------------------------

#ifndef TTaggerBaseH
#define TTaggerBaseH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTTagger.h"

//---------------------------------------------------------------------------
class VTApl;
class VTAcao;
class VTAutoTag;
class VTFigura;
//---------------------------------------------------------------------------
class TTaggerBase : public VTTagger
{
	public:  //métodos
					__fastcall  TTaggerBase(VTApl *apl, VTAutoTag *autoTag);
					__fastcall ~TTaggerBase(void);
		bool	 	__fastcall  InsereTags(VTAcao *acao, bool undo);
        void 		__fastcall  InicializaComentariosAutomaticos(void);

	protected:  //dados externos
		VTApl *apl_;
		VTAutoTag *autoTag_ ;

};
//---------------------------------------------------------------------------
#endif
