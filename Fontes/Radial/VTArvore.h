//-----------------------------------------------------------------------------
#ifndef VTArvoreH
#define VTArvoreH

//arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>
#include <ComCtrls.hpp>

//-----------------------------------------------------------------------------
enum tipoARVORE {arvoreINDEFINIDA=0, arvorePERDATEC=1, arvoreDENSIDADE=2, arvoreARSESP=4,
				 arvoreREDE=5, arvoreLIGA=6};

//-----------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTBlocos;
class VTRedes;
class VTNode;

//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14217067
class VTArvore  : public TObject
   {
   public:  //property
	  __property int        Id          = {read=PD.id,    write=PD.id   };
	  __property int        TipoArvore  = {read=PD.tipo,  write=PD.tipo };
	  __property TDateTime  Data        = {read=PD.data,  write=PD.data };
	  __property bool       SalvaNaBase = {read=PD.salva, write=PD.salva};

   public:
					  __fastcall  VTArvore(void)  {};
	  virtual         __fastcall ~VTArvore(void) {};
	  virtual void    __fastcall  Clear(void) = 0;
	  virtual void    __fastcall  ClearObjects(void) = 0;
	  virtual void    __fastcall  DefineNodeId(void) = 0;
	  virtual VTNode* __fastcall  ExisteNode(AnsiString codigo) = 0;
	  virtual VTNode* __fastcall  ExisteNode(int node_id) = 0;
	  virtual VTNode* __fastcall  ExisteNode(TObject *object) = 0;
	  virtual bool    __fastcall  Inicia(VTBlocos *blocos) = 0;
	  virtual bool    __fastcall  Inicia(VTRedes *redes, bool incluir_rede_isolada=true) = 0;
	  virtual bool    __fastcall  Inicia(TList *lisMREDE) = 0;
	  virtual bool    __fastcall  Inicia(TList *lisLIG, VTBarra *barra_ini) = 0;
	  virtual VTNode* __fastcall  InsereNode(VTNode *node_pai=NULL) = 0;
	  virtual void    __fastcall  InsereNode(VTNode *node_pai, VTNode *node_filho) = 0;
	  virtual void    __fastcall  LisAllNode(TList *lisEXT) = 0;
	  virtual void    __fastcall  LisAllObject(TList *lisEXT) = 0;
	  virtual void    __fastcall  LisNode(TList *lisEXT, int level) = 0;
	  virtual TList*  __fastcall  LisNodeRaiz(void) = 0;
	  virtual bool    __fastcall  RetiraNode(VTNode *node) = 0;
	  virtual void    __fastcall  TreeViewInicia(TTreeView *TView, bool sort=true) = 0;

   protected:  //dados acessados via property
	  struct{
			int       id;
			int       tipo;
			TDateTime data;
			bool      salva;
			}PD;
    };

//-----------------------------------------------------------------------------
//função global
VTArvore* __fastcall NewObjArvore(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
