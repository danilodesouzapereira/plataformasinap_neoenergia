//-----------------------------------------------------------------------------
#ifndef VTResumosH
#define VTResumosH

//arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;
class VTNode;
class VTResumo;
class VTEqpto;

//-----------------------------------------------------------------------------
class VTResumos : public TObject
   {
   public:
                        __fastcall  VTResumos(void) {};
                        __fastcall ~VTResumos(void) {};
      virtual void      __fastcall  AtualizaTodosResumos(void) = 0;
      virtual void      __fastcall  Clear(void) = 0;
      virtual VTResumo* __fastcall  ExisteResumo(VTEqpto *eqpto) = 0;
      virtual bool      __fastcall  IniciaArvoreComResumo(void) = 0;
      virtual bool      __fastcall  InsereResumo(int nodepai_id, int node_id, VTResumo *resumo) = 0;
      virtual TList*    __fastcall  LisResumo(void) = 0;
      virtual void      __fastcall  LisResumo(TList *lisEXT) = 0;
      virtual void      __fastcall  LisResumo(int tiporede_id, TList *lisEXT) = 0;
      virtual void      __fastcall  LisResumoJusante(VTNode *node, TList *lisEXT) = 0;
      virtual void      __fastcall  LisResumoJusante(VTNode *node, int tiporede_id, TList *lisEXT) = 0;
      virtual void      __fastcall  LisResumoMontante(VTNode *node, TList *lisEXT) = 0;
      virtual void      __fastcall  LisResumoNode(VTNode *node, TList *lisEXT) = 0;
	  virtual void      __fastcall  LisResumoNode(VTNode *node, int tiporede_id, TList *lisEXT) = 0;
	  virtual int       __fastcall  SelecionaResumo(TList *lisORIG, TList *lisDEST, int tiporede_id) = 0;
	  virtual void      __fastcall  TrataResumoPaiFilho(VTResumo *resumo, TList *lisRESUMO) = 0;
   };

//-----------------------------------------------------------------------------
//função global
VTResumos* __fastcall NewObjResumos(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
