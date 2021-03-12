//---------------------------------------------------------------------------

#ifndef TTaggerDecoratorH
#define TTaggerDecoratorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTTagger.h"
#include <map>
//---------------------------------------------------------------------------
class VTApl;
class VTAcao;
class VTAutoTag;
class VTFiguraComentario;
class VTTagFormatter;
class VTEqpto;
//---------------------------------------------------------------------------
class TTaggerDecorator : public VTTagger
{
	public:  //métodos
				__fastcall  TTaggerDecorator(VTApl *apl, VTTagger *tagger, VTTagFormatter *formatter);
				__fastcall ~TTaggerDecorator(void);
		bool    __fastcall  InsereTags(VTAcao *acao, bool undo);
		void    __fastcall  InicializaComentariosAutomaticos(void);

	protected:
		bool    __fastcall DeveDeletarComentario(VTFiguraComentario * comentario, AnsiString msgTag);
		VTFiguraComentario* __fastcall ExisteComentario(VTEqpto *eqpto);
		VTFiguraComentario* __fastcall InsereComentario(VTEqpto *eqpto);
		bool    __fastcall  IsAcaoTipoEqpto(VTAcao *acao, int tipo_eqpto);
		bool    __fastcall  IsComentarioAutomatico(VTFiguraComentario *comentario);
		bool 	__fastcall  RemoveTag(VTFiguraComentario *comentario, VTEqpto *eqpto);

	protected:  //dados externos
		VTApl *apl_;
		VTTagger *tagger_;
		VTTagFormatter *formatter_;
        std::map<VTEqpto*,VTFiguraComentario*> mapEQPTO_COMETARIO_;


};
//---------------------------------------------------------------------------
#endif
