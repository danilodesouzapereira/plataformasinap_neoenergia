//---------------------------------------------------------------------------
#ifndef VTDensidadeH
#define VTDensidadeH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTArvore;

//---------------------------------------------------------------------------
class VTDensidade : public TObject
   {
   //public: //property
   //   __property VTArvore* Arvore = {read=PM_GetArvore};

   public:
                     __fastcall  VTDensidade(void) {};
      virtual        __fastcall ~VTDensidade(void) {};
      virtual bool   __fastcall  Executa(TDateTime date)=0;

   protected:  //métodos acessados via property
      //virtual VTArvore* __fastcall PM_GetArvore(void) = 0;
   };

//---------------------------------------------------------------------------
//função p/ criar objeto da classe
//---------------------------------------------------------------------------
VTDensidade* __fastcall NewObjDensidade(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 