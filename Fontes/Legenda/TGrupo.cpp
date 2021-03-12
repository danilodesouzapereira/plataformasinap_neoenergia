//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TGrupo.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTGrupo* __fastcall NewObjGrupo(VTEqpto *eqpto, AnsiString descricao)
   {
   try{
      return (new TGrupo(eqpto, descricao));
      }catch (Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TGrupo::TGrupo(VTEqpto *eqpto, AnsiString descricao)
   {
   //salva descrição e ponteiro p/ Eqpto
   PD.descricao = descricao;
   PD.eqpto     = eqpto;
   try{//cria listas
      PD.lisGRUPO  = new TList();
      PD.lisDADO   = new TList();
      PD.lisVETVAL = new TList();
      } catch (Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TGrupo::~TGrupo(void)
   {
   //destrói listas e seus objetos
   if (PD.lisDADO)   {LimpaTList(PD.lisDADO);   delete PD.lisDADO;   PD.lisDADO   = NULL;}
   if (PD.lisGRUPO)  {LimpaTList(PD.lisGRUPO);  delete PD.lisGRUPO;  PD.lisGRUPO  = NULL;}
   if (PD.lisVETVAL) {LimpaTList(PD.lisVETVAL); delete PD.lisVETVAL; PD.lisVETVAL = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TGrupo::Add(VTDado *dado)
   {
   //proteção
   if (dado == NULL) return;
   //verifica se a lista LisDado já foi criada
   if (LisDado == NULL)
   try{//cria lista
      PD.lisDADO = new TList();
      }catch(Exception &e)
         {
         }
   //insere Dado em LisDado
   LisDado->Add(dado);
   }

//---------------------------------------------------------------------------
void __fastcall TGrupo::Add(VTGrupo *grupo)
   {
   //proteção
   if (grupo == NULL) return;
   //verifica se a lista LisGrupo já foi criada
   if (LisGrupo == NULL)
   try{//cria lista
      PD.lisGRUPO = new TList();
      }catch(Exception &e)
         {
         }
   //insere Dado em LisDado
   LisGrupo->Add(grupo);
   }

//---------------------------------------------------------------------------
void __fastcall TGrupo::Add(VTVetval *vetval)
   {
   //proteção
   if (vetval == NULL) return;
   //verifica se a lista LisVetval já foi criada
   if (LisVetval == NULL)
   try{//cria lista
      PD.lisVETVAL = new TList();
      }catch(Exception &e)
         {
         }
   //insere Dado em LisDado
   LisVetval->Add(vetval);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TGrupo::PM_GetDescricao(void)
   {
   return(PD.descricao);
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TGrupo::PM_GetEqpto(void)
   {
   return(PD.eqpto);
   }

//---------------------------------------------------------------------------
TList* __fastcall TGrupo::PM_GetLisDado(void)
   {
   return(PD.lisDADO);
   }

//---------------------------------------------------------------------------
TList* __fastcall TGrupo::PM_GetLisGrupo(void)
   {
   return(PD.lisGRUPO);
   }

//---------------------------------------------------------------------------
TList* __fastcall TGrupo::PM_GetLisVetval(void)
   {
   return(PD.lisVETVAL);
   }

//---------------------------------------------------------------------------
//eof
