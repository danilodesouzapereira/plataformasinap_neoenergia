//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TResCC.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Legenda\VTDado.h"
#include "..\Legenda\VTDado.h"
#include "..\Legenda\VTGrupo.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTTrafo.h"
#include "..\RedeCC\ResCurto.h"
#include "..\RedeCC\VTBarCC.h"
#include "..\RedeCC\VTLigCC.h"
#include "..\RedeCC\VTRedeCC.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
//função global p/ criar objeto da classe VTResCC
//-----------------------------------------------------------------------------
VTResCC* __fastcall NewObjResCC(VTApl *apl)
   {
   return(new TResCC(apl));
   }

//-----------------------------------------------------------------------------
__fastcall TResCC::TResCC(VTApl *apl)
   {
   //salva ponteiro dos objetos
   this->apl = apl;
   redeCC    = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
   fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   aue       = new TAue();
   }

//-----------------------------------------------------------------------------
__fastcall TResCC::~TResCC(void)
   {
   //destrói objetos
   if (aue) {delete aue; aue = NULL;}
   }

//-----------------------------------------------------------------------------
VTGrupo* __fastcall TResCC::IniciaGrupoBarra(VTBarCC *barCC)
   {
   //variáveis locais
   VTGrupo *grupo_pai, *grupo;

   try{//cria Grupo pai  p/ a Barra
      grupo_pai = NewObjGrupo(barCC->Barra, barCC->Barra->Codigo);
      //cria Grupos p/ tipo de defeito
      for (int nd = 0; nd < NUM_MAX_DEF; nd++)
         {//cria Grupo e insere no Grupo pai
         grupo_pai->Add(grupo = NewObjGrupo(NULL, NomeDefeito(nd)));
         //cria Grupos p/ tensão de fase e de seq
         MontaGrupoVfas_pu(barCC, grupo, nd);
         MontaGrupoVfas_kv(barCC, grupo, nd);
         MontaGrupoVseq_pu(barCC, grupo, nd);
         MontaGrupoVseq_kv(barCC, grupo, nd);
         }
      }catch (Exception &e)
         {
         return(NULL);
         }
   return(grupo_pai);
   }

//-----------------------------------------------------------------------------
VTGrupo* __fastcall TResCC::IniciaGrupoBarraDefeito(VTBarCC *barCC)
   {
   //variáveis locais
   VTGrupo *grupo_pai, *grupo;

   try{//cria Grupo p/ o ponto de defeito
      grupo_pai = NewObjGrupo(NULL, "Tensões e Correntes ");
      //cria Grupos p/ tipo de defeito
      for (int nd = 0; nd < NUM_MAX_DEF; nd++)
         {//cria Grupo e insere no Grupo pai
         grupo_pai->Add(grupo = NewObjGrupo(NULL, NomeDefeito(nd)));
         //cria Grupos p/ tensão de fase e de seq
         MontaGrupoVfas_pu(barCC, grupo, nd);
         MontaGrupoVfas_kv(barCC, grupo, nd);
         MontaGrupoVseq_pu(barCC, grupo, nd);
         MontaGrupoVseq_kv(barCC, grupo, nd);
         //cria Grupos p/ correntes de defeito de fase e de seq
         MontaGrupoIfas_amp(barCC, grupo, nd);
         MontaGrupoIseq_amp(barCC, grupo, nd);
         }
      }catch (Exception &e)
         {
         return(NULL);
         }
   return(grupo_pai);
   }

//-----------------------------------------------------------------------------
VTGrupo* __fastcall TResCC::IniciaGrupoBarraDefeitoCorrenteAssimetrica(VTBarCC *barCC)
   {
   //variáveis locais
   VTGrupo *grupo_pai, *grupo;

   try{//cria Grupo pai somente c/ título
      grupo_pai = NewObjGrupo(NULL, "Correntes assimétricas");
      //cria Grupos p/ correntes assimétricas p/ tipo de defeito
      for (int nd = 0; nd < NUM_MAX_DEF; nd++)
         {//somente p/ defeito trifásico e fase terra
         if ((nd == defeito3F)||(nd == defeitoFT))
            {//cria Grupo e insere no Grupo pai
            grupo_pai->Add(grupo = NewObjGrupo(NULL, NomeDefeito(nd)));
            //cria Grupos p/ correntes assimétricas de fase
            //MontaGrupoAssim_Ifas_amp(barCC, grupo, nd);
            //inicia lisDADO do Grupo c/ Dado do defeito
            MontaLisDadoIass_amp(barCC, nd, grupo->LisDado);
            }
         }
      }catch (Exception &e)
         {
         return(NULL);
         }
   return(grupo_pai);
   }

//-----------------------------------------------------------------------------
VTGrupo* __fastcall TResCC::IniciaGrupoContribuicao(VTBarCC *barCC)
   {
   //variáveis locais
   VTGrupo *grupo;

   try{//cria Grupo p/ BarCC
      grupo = NewObjGrupo(barCC->Barra, barCC->Barra->Codigo);
      }catch (Exception &e)
         {
         }
   //cria Grupo
   return(grupo);
   }

//-----------------------------------------------------------------------------
VTGrupo* __fastcall TResCC::IniciaGrupoLigacao(VTLigCC *ligCC)
   {
   //variáveis locais
   AnsiString txt;
   VTBarra   *barra;
   VTGrupo   *grupo_pai, *grupo_bar, *grupo_def;

   try{//cria Grupo pai  p/ a Ligacao
      grupo_pai = NewObjGrupo(ligCC->Ligacao, ligCC->Ligacao->Codigo);
      //cria um Grupo p/ cada Barra
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {//verifica se a Ligação possui a Barra
         if ((barra = ligCC->Ligacao->Barra(ind_bar)) == NULL) continue;
         //cria um novo Grupo para Barra e insere no Grupo pai
         if (barra->Codigo.IsEmpty()) txt.sprintf("bar.%d",     ind_bar+1);
         else                         txt.sprintf("bar.%d: %s", ind_bar+1, barra->Codigo.c_str());
         grupo_pai->Add(grupo_bar = NewObjGrupo(barra, txt));
         //cria Grupos p/ cada tipo de defeito
         for (int ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
            {//cria Grupo de tipo de defeito e insere no Grupo da barra
            grupo_bar->Add(grupo_def = NewObjGrupo(NULL, NomeDefeito(ind_def)));
            //cria Grupos p/ tensão de fase e de seq
            MontaLisDadoIfas_amp(ligCC, ind_def, ind_bar, grupo_def->LisDado);
            MontaLisDadoIseq_amp(ligCC, ind_def, ind_bar, grupo_def->LisDado);
            }
         }
      }catch (Exception &e)
         {
         return(NULL);
         }
   return(grupo_pai);
   }

//-----------------------------------------------------------------------------
VTGrupo* __fastcall TResCC::IniciaGrupoPotenciaCurtoCircuito(void)
   {
   //variáveis locais
   VTDado  *dado;
   VTGrupo *grupo;

   try{//cria Grupo pai  p/ Potências de curto-circuito
      grupo = NewObjGrupo(NULL, "Pot. de curto-circuito");
      //insere Dado p/ S3f
      grupo->Add(dado = NewObjDado("Trifásico"));
      //define unidade e valor
      aue->PotComplexa_MVA(redeCC->S3f_mva, dado->valor, dado->unidade);
      //insere Dado p/ Sft
      grupo->Add(dado = NewObjDado("Fase-terra"));
      //define unidade e valor
      aue->PotComplexa_MVA(redeCC->Sft_mva, dado->valor, dado->unidade);

      }catch (Exception &e)
         {
         return(NULL);
         }
   return(grupo);
   }

//-----------------------------------------------------------------------------
VTGrupo* __fastcall TResCC::IniciaGrupoZentrada(void)
   {
   //variáveis locais
   VTDado  *dado;
   VTGrupo *grupo;

   try{//cria Grupo pai  p/ Zentrada
      grupo = NewObjGrupo(NULL, "Zentrada");
      //insere Dado p/ Z0entrada
      grupo->Add(dado = NewObjDado("Z0"));
      //define unidade e valor
      aue->Impedancia_OHM(redeCC->Z0entrada_ohm, dado->valor, dado->unidade);
      //insere Dado p/ Z1entrada
      grupo->Add(dado = NewObjDado("Z1"));
      //define unidade e valor
      aue->Impedancia_OHM(redeCC->Z1entrada_ohm, dado->valor, dado->unidade);
      }catch (Exception &e)
         {
         return(NULL);
         }
   return(grupo);
   }

//-----------------------------------------------------------------------------
VTGrupo* __fastcall TResCC::IniciaGrupoZentradaSoTrechos(void)
   {
   //variáveis locais
   VTDado  *dado;
   VTGrupo *grupo;

   try{//cria Grupo pai  p/ Zentrada
      grupo = NewObjGrupo(NULL, "Zentrada - só trechos");
      //insere Dado p/ Z0entrada
      grupo->Add(dado = NewObjDado("Z0"));
      //define unidade e valor
      aue->Impedancia_OHM(redeCC->Z0entradaSoTrechos_ohm, dado->valor, dado->unidade);
      //insere Dado p/ Z1entrada
      grupo->Add(dado = NewObjDado("Z1"));
      //define unidade e valor
      aue->Impedancia_OHM(redeCC->Z1entradaSoTrechos_ohm, dado->valor, dado->unidade);

      }catch (Exception &e)
         {
         return(NULL);
         }
   return(grupo);
   }

//-----------------------------------------------------------------------------
void __fastcall TResCC::LisDado(VTBarCC *barCC, int ind_def, TList *lisEXT)
   {
   //verifica se deve incluir dados de todos defeitos
   if (ind_def < 0)
      {//chamada recursiva
      for (ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
         {
         LisDado(barCC, ind_def, lisEXT);
         }
      }
   else
      {//inicia lisEXT c/ Dado do defeito indicado
      lisEXT->Add(NewObjDado(NomeDefeito(ind_def)));
      /*if (atributo) */MontaLisDadoVfas_pu(barCC, ind_def, lisEXT);
      /*if (atributo) */MontaLisDadoVseq_pu(barCC, ind_def, lisEXT);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TResCC::LisDado(VTLigCC *ligCC, int ind_def, int ind_bar, TList *lisEXT)
   {
   //verifica se deve incluir dados de todos defeitos
   if (ind_def < 0)
      {//chamada recursiva
      for (ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
         {
         LisDado(ligCC, ind_def, ind_bar, lisEXT);
         }
      }
   else
      {//inicia lisEXT c/ Dado do defeito indicado
      lisEXT->Add(NewObjDado(NomeDefeito(ind_def)));
      MontaLisDadoIfas_amp(ligCC, ind_def, ind_bar, lisEXT);
      MontaLisDadoIseq_amp(ligCC, ind_def, ind_bar, lisEXT);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TResCC::MontaGrupoIfas_amp(VTBarCC *barCC, VTGrupo *pai, int ind_def)
   {
   //variáveis locais
   VTGrupo *grupo;

   //cria um novo Grupo e insere no Grupo pai
   pai->Add(grupo = NewObjGrupo(NULL, "Ifase"));
   //inicia lisDADO do Grupo c/ Dado do defeito indicado
   MontaLisDadoIfas_amp(barCC, ind_def, grupo->LisDado);
   }

//-----------------------------------------------------------------------------
void __fastcall TResCC::MontaGrupoIseq_amp(VTBarCC *barCC, VTGrupo *pai, int ind_def)
   {
   //variáveis locais
   AnsiString txt;
   VTGrupo *grupo;

   //cria um novo Grupo e insere no Grupo pai
   pai->Add(grupo = NewObjGrupo(NULL, "Iseq"));
   //inicia lisDADO do Grupo c/ Dado do defeito indicado
   MontaLisDadoIseq_amp(barCC, ind_def, grupo->LisDado);
   }

//-----------------------------------------------------------------------------
void __fastcall TResCC::MontaGrupoVfas_kv(VTBarCC *barCC, VTGrupo *pai, int ind_def)
   {
   //variáveis locais
   VTGrupo *grupo;

   //cria um novo Grupo e insere no Grupo pai
   pai->Add(grupo = NewObjGrupo(NULL, "Vfase"));
   //inicia lisDADO do Grupo c/ Dado do defeito indicado
   MontaLisDadoVfas_kv(barCC, ind_def, grupo->LisDado);
   }

//-----------------------------------------------------------------------------
void __fastcall TResCC::MontaGrupoVfas_pu(VTBarCC *barCC, VTGrupo *pai, int ind_def)
   {
   //variáveis locais
   VTGrupo *grupo;

   //cria um novo Grupo e insere no Grupo pai
   pai->Add(grupo = NewObjGrupo(NULL, "Vfase(pu)"));
   //inicia lisDADO do Grupo c/ Dado do defeito indicado
   MontaLisDadoVfas_pu(barCC, ind_def, grupo->LisDado);
   }

//-----------------------------------------------------------------------------
void __fastcall TResCC::MontaGrupoVseq_kv(VTBarCC *barCC, VTGrupo *pai, int ind_def)
   {
   //variáveis locais
   VTGrupo *grupo;

   //cria um novo Grupo e insere no Grupo pai
   pai->Add(grupo = NewObjGrupo(NULL, "Vseq"));
   //inicia lisDADO do Grupo c/ Dado do defeito indicado
   MontaLisDadoVseq_kv(barCC, ind_def, grupo->LisDado);
   }

//-----------------------------------------------------------------------------
void __fastcall TResCC::MontaGrupoVseq_pu(VTBarCC *barCC, VTGrupo *pai, int ind_def)
   {
   //variáveis locais
   VTGrupo *grupo;

   //cria um novo Grupo e insere no Grupo pai
   pai->Add(grupo = NewObjGrupo(NULL, "Vseq(pu)"));
   //inicia lisDADO do Grupo c/ Dado do defeito indicado
   MontaLisDadoVseq_pu(barCC, ind_def, grupo->LisDado);
   }

//-----------------------------------------------------------------------------
void __fastcall TResCC::MontaLisDadoIass_amp(VTBarCC *barCC, int ind_def, TList *lisDADO)
   {
   //variáveis locais
   int     ind_fase = 0;
   VTDado  *dado;

   //cria um novo Dado p/ corrente da fase A apenas e insere no Grupo
   lisDADO->Add(dado = NewObjDado("Iass."));
   //define unidade e valor
   aue->Corrente_A(Abs(barCC->Assim_Ifas_amp[ind_def][ind_fase]), dado->valor, dado->unidade);
   }

//-----------------------------------------------------------------------------
void __fastcall TResCC::MontaLisDadoIfas_amp(VTBarCC *barCC, int ind_def, TList *lisDADO)
   {
   //variáveis locais
   VTDado  *dado;

   for (int ind_fase = 0; ind_fase < NUM_MAX_FAS; ind_fase++)
      {//cria um novo Dado p/ corrente de fase e insere no Grupo
      lisDADO->Add(dado = NewObjDado("I" + NomeFase(ind_fase)));
      //define unidade e valor
      aue->Corrente_A(barCC->Ifas_amp[ind_def][ind_fase], dado->valor, dado->unidade);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TResCC::MontaLisDadoIfas_amp(VTLigCC *ligCC, int ind_def, int ind_bar, TList *lisDADO)
   {
   //variáveis locais
   VTDado  *dado;

   for (int ind_fase = 0; ind_fase < NUM_MAX_FAS; ind_fase++)
      {//cria um novo Dado p/ corrente de fase e insere no Grupo
      lisDADO->Add(dado = NewObjDado("I" + NomeFase(ind_fase)));
      //define unidade e valor
      aue->Corrente_A(ligCC->Ifas_amp[ind_def][ind_bar][ind_fase], dado->valor, dado->unidade);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TResCC::MontaLisDadoIseq_amp(VTBarCC *barCC, int ind_def, TList *lisDADO)
   {
   //variáveis locais
   VTDado  *dado;

   for (int ind_seq = 0; ind_seq < NUM_MAX_SEQ; ind_seq++)
      {//cria um novo Dado p/ corrente de sequencia e insere no Grupo
      lisDADO->Add(dado = NewObjDado("I" + NomeSeq(ind_seq)));
      //define unidade e valor
      aue->Corrente_A(barCC->Iseq_amp[ind_def][ind_seq], dado->valor, dado->unidade);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TResCC::MontaLisDadoIseq_amp(VTLigCC *ligCC, int ind_def, int ind_bar, TList *lisDADO)
   {
   //variáveis locais
   VTDado  *dado;

   for (int ind_seq = 0; ind_seq < NUM_MAX_SEQ; ind_seq++)
      {//cria um novo Dado p/ corrente de sequencia e insere no Grupo
      lisDADO->Add(dado = NewObjDado("I" + NomeSeq(ind_seq)));
      //define unidade e valor
      aue->Corrente_A(ligCC->Iseq_amp[ind_def][ind_bar][ind_seq], dado->valor, dado->unidade);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TResCC::MontaLisDadoVfas_kv(VTBarCC *barCC, int ind_def, TList *lisDADO)
   {
   //variáveis locais
   VTDado  *dado;

   for (int ind_fase = 0; ind_fase < NUM_MAX_FAS; ind_fase++)
      {//cria um novo Dado p/ tensão de fase e insere no Grupo
      lisDADO->Add(dado = NewObjDado("V" + NomeFase(ind_fase)));
      //define unidade e valor
      aue->Tensao_KV(barCC->Vfas_kv[ind_def][ind_fase], dado->valor, dado->unidade);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TResCC::MontaLisDadoVfas_pu(VTBarCC *barCC, int ind_def, TList *lisDADO)
   {
   //variáveis locais
   VTDado  *dado;

   for (int ind_fase = 0; ind_fase < NUM_MAX_FAS; ind_fase++)
      {//cria um novo Dado p/ tensão de fase e insere no Grupo
      lisDADO->Add(dado = NewObjDado("V" + NomeFase(ind_fase)));
      //define unidade e valor
      aue->Tensao_PU(barCC->Vfas_pu[ind_def][ind_fase], dado->valor, dado->unidade);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TResCC::MontaLisDadoVseq_kv(VTBarCC *barCC, int ind_def, TList *lisDADO)
   {
   //variáveis locais
   VTDado  *dado;

   for (int ind_seq = 0; ind_seq < NUM_MAX_SEQ; ind_seq++)
      {//cria um novo Dado p/ tensão de sequencia e insere no Grupo
      lisDADO->Add(dado = NewObjDado("V" + NomeSeq(ind_seq)));
      //define unidade e valor
      aue->Tensao_KV(barCC->Vseq_kv[ind_def][ind_seq], dado->valor, dado->unidade);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TResCC::MontaLisDadoVseq_pu(VTBarCC *barCC, int ind_def, TList *lisDADO)
   {
   //variáveis locais
   VTDado  *dado;

   for (int ind_seq = 0; ind_seq < NUM_MAX_SEQ; ind_seq++)
      {//cria um novo Dado p/ tensão de sequencia e insere na lista
      lisDADO->Add(dado = NewObjDado("V" + NomeSeq(ind_seq)));
      //define unidade e valor
      aue->Tensao_PU(barCC->Vseq_pu[ind_def][ind_seq], dado->valor, dado->unidade);
      }
   }

//-----------------------------------------------------------------------------
AnsiString __fastcall TResCC::NomeDefeito(int ind_def)
   {
   //variáveis locais
   char *tipo_def[] = {"3F(abc)", "2F(bc)", "1FT(at)", "1FTz(at)", "2FT(bct)"};

   return(tipo_def[ind_def]);
   }

//-----------------------------------------------------------------------------
AnsiString __fastcall TResCC::NomeFase(int ind_fase)
   {
   //variáveis locais
   int fase_id[] = {faseA, faseB, faseC};

   return(fases->AsStrLower[fase_id[ind_fase]]);
   }

//-----------------------------------------------------------------------------
AnsiString __fastcall TResCC::NomeSeq(int ind_seq)
   {
   return(IntToStr(ind_seq));
   }

//---------------------------------------------------------------------------
//eof