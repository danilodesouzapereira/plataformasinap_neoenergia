//---------------------------------------------------------------------------
#ifndef VTCfgCorH
#define VTCfgCorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Graphics.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
struct strTENSAO {
                 TColor corV1;
                 TColor corV2;
                 TColor corV3;
                 TColor corV4;
                 TColor corV5;
                 };

struct strENERGIA {
                  TColor corPadrao;
                  };

//---------------------------------------------------------------------------
class VTCfgCor : public TObject
   {
   public: //property

   public:
               __fastcall  VTCfgCor(void) {};
      virtual  __fastcall ~VTCfgCor(void) {};

   public: //dados locais
      strENERGIA ENERGIA;
      strTENSAO  TENSAO;

   protected:  //métodos acessados via property

   };

//---------------------------------------------------------------------------
//função global
//---------------------------------------------------------------------------
VTCfgCor* __fastcall NewObjCfgCor(void);

//---------------------------------------------------------------------------
#endif


