//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAplEditor.h"
#include "VTEditor.h"
#include "VTEdicao.h"
#include "..\Busca\VTBusca.h"
#include "..\..\DLL_Inc\Busca.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TAplEditor::TAplEditor(TForm *form_owner, VTApl *apl_owner)
                      :TApl(form_owner, apl_owner)
   {
   //vari�veis locais
   TObject *obj;

   //cria objetos necess�rios ao Editor
   if ((obj = DLL_NewObjBusca(this)) != NULL) lisOBJ->Add(obj);
   if ((obj = NewObjEditor(this))    != NULL) lisOBJ->Add(obj);
   if ((obj = NewObjEdicao(this))    != NULL) lisOBJ->Add(obj);
  }

//---------------------------------------------------------------------------
__fastcall TAplEditor::~TAplEditor(void)
   {
   //OBS: a classe base destr�i a lista e seus objetos
   }

//---------------------------------------------------------------------------
//eof