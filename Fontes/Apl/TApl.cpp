//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TApl.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTApl* __fastcall NewObjApl(TForm *form_owner, VTApl *apl_owner)
   {
   try{//cria objeto
      return(new TApl(form_owner, apl_owner));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
   
//---------------------------------------------------------------------------
__fastcall TApl::TApl(TForm *form_owner, VTApl *apl_owner)
   {
   //salva dados recebidos
   this->form_owner = form_owner;
   this->apl_owner  = apl_owner;
   //cria lista de objetos
   lisOBJ = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TApl::~TApl(void)
   {
   //variáveis locais
   TObject *obj;

   try{//proteção
      if (lisOBJ == NULL) return;
      //destrói lista e seus objetos
      for (int n = lisOBJ->Count-1; n >= 0; n--)
         {
         try{//retira objeto da lista e destrói
            obj = (TObject*)lisOBJ->Items[n];
            lisOBJ->Delete(n);
            delete obj;
            }catch(Exception &e)
               {//nada a fazer
               }
         }
      //destrói a lista
      delete lisOBJ;
      lisOBJ = NULL;
      }catch(Exception &e)
         {
         #ifdef _DEBUG
         Aviso("TApl::~TApl() - Erro na destruição de objetos");
         #endif
         }
   }

//---------------------------------------------------------------------------
void __fastcall TApl::Add(TObject *obj)
   {
   //proteção
   if (obj != NULL) lisOBJ->Add(obj);
   }

//---------------------------------------------------------------------------
TForm* __fastcall TApl::GetForm(void)
   {
   return(form_owner);
   }

//---------------------------------------------------------------------------
TObject* __fastcall TApl::GetObject(AnsiString class_name)
   {
   //variáveis locais
   AnsiString obj_name;
   TObject    *obj;

   //bloqueia acesso a objetos proibidos
   if (ObjetoProibido(class_name)) return(NULL);
   //retorna objeto da classe indicada
   for (int n = 0; n < lisOBJ->Count; n++)
      {
      obj = (TObject*)lisOBJ->Items[n];
      obj_name = obj->ClassName();
	  if (obj_name.AnsiCompareIC(class_name) == 0)
	  	{return(obj);}
      }
   //não encontrou o objeto: solicita o objeto ao apl_owner
   if (apl_owner) return(apl_owner->GetObject(class_name));
   //não existe o objeto
   return(NULL);
   }

//---------------------------------------------------------------------------
TObject* __fastcall TApl::GetObject(TMetaClass* meta_class)
   {
   //variáveis locais
   TObject    *obj;
   TMetaClass *obj_class;
   AnsiString obj_class_name;
   AnsiString meta_class_name = meta_class->ClassName();

   //bloqueia acesso a objetos proibidos
   if (ObjetoProibido(meta_class)) return(NULL);
   //retorna objeto da classe indicada
   for (int n = 0; n < lisOBJ->Count; n++)
      {
      obj       = (TObject*)lisOBJ->Items[n];
      obj_class = obj->ClassType();
      while(obj_class)
         {
         obj_class_name = obj_class->ClassName();
		 if (obj_class_name.AnsiCompareIC(meta_class_name) == 0)
			{return(obj);}
         obj_class = obj_class->ClassParent();
         }
      }
   //não encontrou o objeto: solicita o objeto ao apl_owner
   if (apl_owner) return(apl_owner->GetObject(meta_class));
   //não existe o objeto
   return(NULL);
   }
   
//---------------------------------------------------------------------------
void __fastcall TApl::Remove(TObject *obj)
   {
   if (obj != NULL) lisOBJ->Remove(obj);
   }

//---------------------------------------------------------------------------
bool __fastcall TApl::ObjetoProibido(AnsiString class_name)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TApl::ObjetoProibido(TMetaClass* meta_class)
   {
   return(false);
   }

//---------------------------------------------------------------------------
//eof