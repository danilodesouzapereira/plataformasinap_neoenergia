//-----------------------------------------------------------------------------
#ifndef VTNode_H
#define VTNode_H

//arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTNode : public TObject
   {
   public:  //property
      __property int        Id     = {read=PD.id,     write=PD.id};
      __property int        Level  = {read=PD.level,  write=PD.level};
      __property AnsiString Codigo = {read=PD.codigo, write=PD.codigo};
      __property VTNode*    Pai    = {read=PD.pai,    write=PD.pai};
      __property TObject*   Object = {read=PD.obj,    write=PD.obj};

   public:
                        __fastcall  VTNode(void) {};
                        __fastcall ~VTNode(void) {};
      virtual VTNode*   __fastcall  ExisteNode(AnsiString codigo) = 0;
      virtual VTNode*   __fastcall  ExisteNode(int node_id) = 0;
      virtual VTNode*   __fastcall  ExisteNode(TObject *object) = 0;
      virtual void      __fastcall  Clear(void) = 0;
      virtual bool      __fastcall  InsereNode(VTNode *node) = 0;
      virtual bool      __fastcall  InsereObject(TObject *object) = 0;
      virtual TList*    __fastcall  LisNode(void) = 0;
      virtual void      __fastcall  LisNode(TList *lisEXT) = 0;
	  virtual void      __fastcall  LisNodeJusante(TList *lisEXT) = 0;
	  virtual void      __fastcall  LisNodeMontante(TList *lisEXT) = 0;
      virtual TList*    __fastcall  LisObjectNode(void) = 0;
      virtual void      __fastcall  LisObjectNode(TList *lisEXT) = 0;
      virtual void      __fastcall  LisObjectJusante(TList *lisEXT) = 0;
      virtual void      __fastcall  LisObjectMontante(TList *lisEXT) = 0;
      virtual void      __fastcall  ReiniciaLisObject(TList *lisEXT) = 0;
      virtual bool      __fastcall  RetiraNode(VTNode *node) = 0;
      virtual bool      __fastcall  RetiraObject(TObject *object) = 0;

   protected:  //dados  acessados via property
      struct   {
               int        id;
               int        level;
               AnsiString codigo;
               VTNode     *pai;
               TObject    *obj;
               }PD;
    };

//-----------------------------------------------------------------------------
//função global
VTNode* __fastcall NewObjNode(void);
VTNode* __fastcall NewObjNodeDetalhe(void);
#endif
//---------------------------------------------------------------------------
//eof
