//---------------------------------------------------------------------------
#ifndef VTPenFinder_h
#define VTPenFinder_h

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTPendrive;

//---------------------------------------------------------------------------
class VTPenFinder : public TObject
   {
   public:
                          __fastcall VTPenFinder(void) {};
      virtual             __fastcall ~VTPenFinder(void) {};
      virtual TList*      __fastcall Executa(void) = 0;
      virtual VTPendrive* __fastcall ExistePendrive(AnsiString pen_id) = 0;
   };

//---------------------------------------------------------------------------
//método p/ criar objeto da classe
//---------------------------------------------------------------------------
VTPenFinder* __fastcall NewObjPenFinder(void);

//---------------------------------------------------------------------------
#endif
//eof

