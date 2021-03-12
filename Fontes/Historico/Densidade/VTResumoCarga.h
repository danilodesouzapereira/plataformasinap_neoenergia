//---------------------------------------------------------------------------
#ifndef VTResumoCargaH
#define VTResumoCargaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTCarga;
class VTNode;
class VTRede;

//---------------------------------------------------------------------------
class VTResumoCarga : public TObject
   {
   public:  //property
      __property VTNode* Node = {read=PD.node, write=PD.node};
      __property VTRede* Rede = {read=PD.rede, write=PD.rede};

   public:
                     __fastcall  VTResumoCarga(void) {};
      virtual        __fastcall ~VTResumoCarga(void) {};
      virtual void   __fastcall  InsereCarga(VTCarga *carga) = 0;
      virtual TList* __fastcall  LisCarga(void) = 0;

   protected:  //dados
      struct{
            VTNode    *node;
            VTRede    *rede;
            }PD;

   };

//---------------------------------------------------------------------------
VTResumoCarga* __fastcall NewObjResumoCarga(void);

//---------------------------------------------------------------------------
#endif
//eof

