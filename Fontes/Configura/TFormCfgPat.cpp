//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCfgPat.h"
#include "..\DLL_Stub\Basico.h"
#include "..\BDado\VTBDado.h"
#include "..\Geral\VTGeral.h"
#include "..\Sisplan\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TformCfgPat::TformCfgPat(TComponent* Owner, AnsiString dirBase, AnsiString dirRede)
   : TForm(Owner)
   {

   //cria objeto BDado
   bdado = DLL_NewObjBDado();
   bdado->DefDiretorio(dirBase, dirRede);
   //cria objeto Geral
   geral = bdado->LeGeral();
   //recebe valor do geral
   PAT=geral->PAT ;

   }
//---------------------------------------------------------------------------
__fastcall TformCfgPat::~TformCfgPat(void)
   {

   //destr�i objetos criados
   if (geral) {delete geral; geral = NULL;}
   if (bdado) {delete bdado; bdado = NULL;}

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgPat::ActionCancelaExecute(TObject *Sender)
   {

   ModalResult = mrCancel;

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgPat::ActionConfirmaExecute(TObject *Sender)
   {

   //l� e valida dados editados
   if (! LeDados()) return;
   //atualiza base de dados
   bdado->GravaGeral(geral);
   //fecha o form
   ModalResult = mrOk;

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgPat::ActionDesfazExecute(TObject *Sender)
   {

   //reinicia c/ valores definidos na Base de Dados
   PAT = geral->PAT;
   //atualiza tela
   ExibeDados();

   }
//---------------------------------------------------------------------------

void __fastcall TformCfgPat::ActionEditaExecute(TObject *Sender)
   {

   //vari�veis locais
   int resp;
   //Aviso sobre gravidade de editar este dado
   resp = Confirma("A edi��o destes dados afeta todos os m�dulos do Interplan", "Deseja continuar?");
   if(resp == ID_YES)panelPat->Enabled = true;
   else panelPat->Enabled = false;

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgPat::cboxNumPatClick(TObject *Sender)
   {

   //vari�veis locais
   int npat;

   //prote��o
   try{
      npat = StrToInteger(cboxNumPat->Text);
      }catch(Exception &e)
         {
         Erro("N�mero de patamares inv�lido");
         return;
         }

   //valida n�mero de patamares
   if ((npat < 1)||(npat > 4))
      {
      Erro("N�mero de patamares inv�lido");
      return;
      }

   //redefine n�mero de patamares
   PAT.NUM_PAT = npat;
   //atualiza tela
   ExibeDados();

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgPat::ExibeDados(void)
   {

   //vari�veis locais
   AnsiString titCol[] = {"Per�odo", "Hora Inicial", "Hora Final", "Hora C�lculo"};


   if ( PAT.NUM_PAT>0 && PAT.NUM_PAT<5) cboxNumPat->ItemIndex=PAT.NUM_PAT-1;

   //define atributos do Grid
   gridPat->Options << goEditing << goTabs;
   gridPat->EditorMode = true;

   //define linhas
   gridPat->RowCount = PAT.NUM_PAT+1;
   gridPat->Height   = gridPat->RowCount * gridPat->DefaultRowHeight + 10;

   //preenche t�tulo das colunas
   for (int n = 0; n < gridPat->ColCount; n++)
      {
      gridPat->Cells[n][0] = titCol[n];
      }

   //preenche Grid c/ dados dos Patamares
   for (int np = 0; np < PAT.NUM_PAT; np++)
      {
      gridPat->Cells[0][np+1] = PAT.NOME[np];                 //identifica��o
      gridPat->Cells[1][np+1] = IntToStr(PAT.HORA_INI[np]);  //hora de in�cio
      gridPat->Cells[2][np+1] = IntToStr(PAT.HORA_FIM[np]);  //hora de fim
      gridPat->Cells[3][np+1] = IntToStr(PAT.HORA_DEM[np]);  //hora p/ c�lculo
      gridPat->Cells[4][np+1] = IntToStr(PAT.HORA_DEM[np]);  //hora p/ c�lculo
      }

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgPat::FormClose(TObject *Sender, TCloseAction &Action)
   {

   Action = caFree;

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgPat::FormShow(TObject *Sender)
   {

  //apresenta dados na tela
   ExibeDados();

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgPat::gridPatKeyPress(TObject *Sender, char &Key)
   {

   if (gridPat->Col == 0) return;

   switch (Key)
        {
        case 'E':
        case 'e':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '.':
        case ',':
        case VK_DECIMAL:
        case VK_RETURN:
        case VK_BACK:
        case VK_TAB:
                break;

        default:
                Key=0;
        }

   }
//---------------------------------------------------------------------------
bool __fastcall TformCfgPat::LeDados(void)
   {

   //vari�eis locais
   int soma;

   try
   {

   //l� e valida os dados do Grid de Patamar
   for (int np = 0; np < PAT.NUM_PAT; np++)
      {
      if (! ValNome(PAT.NOME[np]     = gridPat->Cells[0][np+1])) return(false);
      if (! ValHora(PAT.HORA_INI[np] = (gridPat->Cells[1][np+1]).ToInt())) return(false);
      if (! ValHora(PAT.HORA_FIM[np] = (gridPat->Cells[2][np+1]).ToInt())) return(false);
      if (! ValHora(PAT.HORA_DEM[np] = (gridPat->Cells[3][np+1]).ToInt())) return(false);
      //calcula a dura��o do patamar
      PAT.DURA_PAT[np] = PAT.HORA_FIM[np] - PAT.HORA_INI[np];
      if (PAT.DURA_PAT[np] < 0) PAT.DURA_PAT[np] += 24;
      }

   //valida��o: somat�ria de dura��o = 24
   soma = 0;

   for (int np = 0; np < PAT.NUM_PAT; np++) soma += PAT.DURA_PAT[np];

   if (soma != 24)
      {
      Erro("Patamares: Valores inconsistentes");
      return(false);
      }

   //valida��o: hora_ini <= hora_dem <= hora_fim
   for (int np = 0; np < PAT.NUM_PAT; np++)
      {//verifica se o patamar inclui virada do dia
      if (PAT.HORA_INI[np] <= PAT.HORA_FIM[np])
         {
         if ((PAT.HORA_DEM[np] < PAT.HORA_INI[np])||(PAT.HORA_DEM[np] > PAT.HORA_FIM[np]))
            {
            Erro(PAT.NOME[np] + ": Hora de c�lculo inv�lida");
            return(false);
            }
         }
      else
         {
         if ((PAT.HORA_DEM[np] > PAT.HORA_FIM[np])&&(PAT.HORA_DEM[np] < PAT.HORA_INI[np]))
            {
            Erro(PAT.NOME[np] + ": Hora de c�lculo inv�lida");
            return(false);
            }
         }
      }
   }//try
   //tratamento de erro
   catch (EConvertError &exception)
      {
      Erro("Patamares: Hora com valor n�o num�rico");
      return(false);
      }
   //leu os dados corretamente: atualiza objeto geral
   geral->PAT = PAT;
   return(true);

   }
//---------------------------------------------------------------------------
bool __fastcall TformCfgPat::ValHora(int hora)
   {

   if ((hora < 0) || (hora > 24))
      {
      Erro("Hora fora do intervalo [0 a 24]");
      return(false);
      }

   return(true);

   }
//---------------------------------------------------------------------------
bool __fastcall TformCfgPat::ValNome(AnsiString nome)
   {

   nome = nome.Trim();
   if (nome.IsEmpty())
      {
      Erro("Patamar: nome indefinido");
      return(false);
      }

   return(true);

   }
//---------------------------------------------------------------------------

