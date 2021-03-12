//---------------------------------------------------------------------------

#ifndef TTaggerChaveManobradaH
#define TTaggerChaveManobradaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TTaggerDecorator.h"
//---------------------------------------------------------------------------
class VTChave;
class VTEstilo;
//---------------------------------------------------------------------------
class TTaggerChaveManobrada : public TTaggerDecorator
{
	public:  //métodos
				__fastcall  TTaggerChaveManobrada(VTApl *apl, VTTagger *tagger, VTTagFormatter *formatter);
				__fastcall ~TTaggerChaveManobrada(void);
		bool    __fastcall  InsereTags(VTAcao *acao, bool undo);

	private:  //metodos
		//teste
        void    __fastcall AvisoEstadoChave(VTChave *chave, bool antes);
		bool    __fastcall DoInsere(VTAcao *acao, VTChave *chave , int estado_final);
		bool    __fastcall DoRetira(VTAcao *acao, VTChave *chave , int estado_final);
		bool    __fastcall UndoInsere(VTAcao *acao, VTChave *chave , int estado_final);
		bool    __fastcall UndoRetira(VTAcao *acao, VTChave *chave , int estado_final);
		bool    __fastcall InsereTags(VTAcao *acao, VTChave *chave, bool undo);
		bool    __fastcall IsAcaoTrocaEstado(VTAcao *acao, int &estado_final, bool &cria_tag, bool undo);
		void    __fastcall TrocaEstado(int &estado_final);
        AnsiString __fastcall TextoTag(VTChave *chave);

	private: //dados proprios

	private:  //dados externos

};
//---------------------------------------------------------------------------
#endif
