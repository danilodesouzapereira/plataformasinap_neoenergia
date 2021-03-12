//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormLogo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormLogo::TFormLogo(TComponent* Owner, TWinControl *parent)
   : TForm(Owner)
   {
   //assume o Parent indicado
   if (parent != NULL)
      {//redimensaion parent
      parent->ClientWidth  = Width;
      parent->ClientHeight = Height + 22;
      //insere este Form no pParent
      Parent = parent;
      Align = alClient;
      }
   }

//---------------------------------------------------------------------------
__fastcall TFormLogo::~TFormLogo(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
//eof
