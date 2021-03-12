//---------------------------------------------------------------------------
#ifndef VTNodeDetalheH
#define VTNodeDetalheH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <PlataformaSinap\Fontes\Radial\TNode.h>
//#include "TNode.h"
//---------------------------------------------------------------------------
class VTItemObra;
//---------------------------------------------------------------------------
enum estadoNodeDetalhe
{
		eND_INVALIDO = -1,
		eND_CONFIGURADO = 0,
		eND_PENDENTE,
        eND_NAOCONTABILIZADO
};
//---------------------------------------------------------------------------
class VTNodeDetalhe : public TNode
{
	public:  //métodos
			__fastcall  VTNodeDetalhe(void){};
	virtual __fastcall ~VTNodeDetalhe(void){};

		__property TList* 		LisAcao    = {read=lisAcao};
		__property bool	  		Habilitado = {read=habilitado, write=habilitado};
		__property int          Estado     ={read=estado, write=estado};
		__property bool	  		Expandido = {read=expandido, write=expandido};
		__property int	  		TipoEqpto = {read=tipoEqpto, write=tipoEqpto};
		__property int	  		TipoAcao = {read=tipoAcao, write=tipoAcao};
		__property VTItemObra* 	ItemObra = {read=itemObra, write=itemObra};

//	protected:  //métodos acessados via property
//		//getters
//		virtual int __fastcall PM_GetEstado(void) =0;

	protected:  //dados locais
		int tipoEqpto;
		int tipoAcao;
		int estado;
		TList *lisAcao;
		bool  habilitado;
		bool  expandido;
		VTItemObra *itemObra;
};

//---------------------------------------------------------------------------
#endif
//eof
