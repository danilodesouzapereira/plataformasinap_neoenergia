//---------------------------------------------------------------------------
#ifndef TDestaqueH
#define TDestaqueH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16082236
class TDestaque : public TObject
   {
   public:		// User declarations
           __fastcall TDestaque(void);
           __fastcall ~TDestaque(void);

   public:
	  TList *lisEQP;
	  TColor color;
	  int    dim;
   };

//---------------------------------------------------------------------------
#endif
//eof
