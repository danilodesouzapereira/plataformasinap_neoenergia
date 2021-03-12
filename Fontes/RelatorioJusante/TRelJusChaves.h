// ---------------------------------------------------------------------------
#ifndef TRelJusChavesH
#define TRelJusChavesH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTRelJusChaves.h"

// ---------------------------------------------------------------------------
class VTApl;
class VTArvore;
class VTBlocos;
class VTNode;
class TInfoRJChave;
// ---------------------------------------------------------------------------
class TRelJusChaves : public VTRelJusChaves
{

public:
			__fastcall TRelJusChaves(VTApl *apl);
			__fastcall ~TRelJusChaves(void);
bool		__fastcall Inicia(void);
void		__fastcall AgregaValores(int n_dias, int patamar);

public:
	__property VTArvore* ArvoreBlocos = {read=arvoreBlocos};
	__property VTArvore* ArvoreChave = {read=arvoreChaves};

private:
bool	__fastcall AgregaValores(void);
void 	__fastcall AgregaValoresCarga(TList *lisCARGA, TInfoRJChave *str);
void 	__fastcall AgregaValoresTrafo(TList *lisTRAFO, TInfoRJChave *str);
void	__fastcall AgregaValoresJusante(TInfoRJChave *str);
void	__fastcall ClearArvoreChaves(void);
VTNode* __fastcall ExisteNodeBloco(VTBloco *bloco);
int 	__fastcall IniciaArvoreBlocos(void);
void 	__fastcall IniciaArvoreChaves(void);
void 	__fastcall InsereNodesChaveFilhas(VTBloco *pai, VTNode *nodeChavePai);
void    __fastcall LimpaArvoreChaves(void);
void	__fastcall ValoresProprios(TInfoRJChave *str);
bool	__fastcall OrdenaChaves(void);


private: //dados externos
	VTApl *apl;


private:
	VTBlocos *blocos;
	int indPatamar;
	int nDias;
	bool executado;
protected:
	VTArvore *arvoreBlocos;
	VTArvore *arvoreChaves;

};

#endif
// ---------------------------------------------------------------------------
// eof
