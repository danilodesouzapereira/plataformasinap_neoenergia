//---------------------------------------------------------------------------
#pragma hdrstop
#include "TAplAjustaDem.h"
#include "TBackup.h"
#include "..\Editor\VTEditor.h"
#include "..\Flow\VTFlow.h"
#include "..\ImportaMedicao\VTImportaMedicao.h"
#include "..\ImportaMedicao\VTMedicoes.h"
#include "..\Obra\VTObra.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Radial\VTRadial.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Copia.h"
#include "..\..\DLL_Inc\Edita.h"
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Flow.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\ImportaMedicao.h"
#include "..\..\DLL_Inc\Medidor.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Radial.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TAplAjustaDem::TAplAjustaDem(TForm *form_owner, VTApl *apl_owner)
                         :TApl(form_owner, apl_owner)

   {
   //cria objetos
   Add(DLL_NewObjEditor(this));
   Add(DLL_NewObjFlow(this));
   Add(DLL_NewObjRadial(this));
   Add(DLL_NewObjMedicoes(this));
   Add(DLL_NewObjImportaMedicao(form_owner, this));
   Add(DLL_NewObjProgresso(form_owner, NULL));
   Add(new TBackup(this));
   Add(DLL_NewObjObra(this));
   }

//---------------------------------------------------------------------------
__fastcall TAplAjustaDem::~TAplAjustaDem(void)
   {
   //nada: a classe base TApl destrói todos os objetos criados
   }

//---------------------------------------------------------------------------
//eof