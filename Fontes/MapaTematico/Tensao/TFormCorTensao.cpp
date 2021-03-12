//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCorTensao.h"
#include "VTCfgCor.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTPadraoRede.h"
#include "..\Geral\VTInfoset.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormCorTensao::TFormCorTensao(TComponent *Owner, VTApl *apl_owner, TAction *Action)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   apl     = apl_owner;
   cfg_cor = (VTCfgCor*)apl->GetObject(__classid(VTCfgCor));
   ActionAtualiza = Action;
   //inicia dados locais
   arq_infoset = "MapaTematicoTensao";
   }

//---------------------------------------------------------------------------
__fastcall TFormCorTensao::~TFormCorTensao(void)
   {
   //salva dados do form
   VTGeral   *geral   = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva configura��o de cores
   geral->Infoset->Clear();
   geral->Infoset->AddInfo("cor_v1" , int(cfg_cor->TENSAO.corV1 ));
   geral->Infoset->AddInfo("cor_v2" , int(cfg_cor->TENSAO.corV2 ));
   geral->Infoset->AddInfo("cor_v3" , int(cfg_cor->TENSAO.corV3 ));
   geral->Infoset->AddInfo("cor_v4" , int(cfg_cor->TENSAO.corV4 ));
   geral->Infoset->AddInfo("cor_v5" , int(cfg_cor->TENSAO.corV5 ));
   geral->Infoset->SaveToFile(arq_infoset);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCorTensao::ExibeDados(int tipoSegmento)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
   VTPadraoRede *padraoRede;

   //atribui cores aos Panels
   PanCorV1->Color  = cfg_cor->TENSAO.corV1;
   PanCorV2->Color  = cfg_cor->TENSAO.corV2;
   PanCorV3->Color  = cfg_cor->TENSAO.corV3;
   PanCorV4->Color  = cfg_cor->TENSAO.corV4;
   PanCorV5->Color  = cfg_cor->TENSAO.corV5;
   //ver o
   padraoRede = geral->ExistePadraoRede(tipoSegmento);
   if(padraoRede != NULL)
	{
	   //labels de tens�o
	   lblV1->Caption = "abaixo de " + DoubleToStr("%5.3f", padraoRede->V1);
	   lblV2->Caption = "entre "     + DoubleToStr("%5.3f", padraoRede->V1)
							+ " e " + DoubleToStr("%5.3f", padraoRede->V2);
	   lblV3->Caption = "entre "     + DoubleToStr("%5.3f", padraoRede->V2)
							+ " e " + DoubleToStr("%5.3f", padraoRede->V3);
	   lblV4->Caption = "entre "     + DoubleToStr("%5.3f", padraoRede->V3)
							 + " e " + DoubleToStr("%5.3f", padraoRede->V4);
	   lblV5->Caption = "acima de "  + DoubleToStr("%5.3f", padraoRede->V4);
   	}
   }

//---------------------------------------------------------------------------
void __fastcall TFormCorTensao::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destr�i o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCorTensao::FormShow(TObject *Sender)
   {
   //inicia cores
   IniciaCor();
   //apresenta dados na tela
   //ExibeDados();
   tabControlNivelVChange(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCorTensao::IniciaCor(void)
   {
   //vari�veis locais
   int      cor_v1, cor_v2, cor_v3, cor_v4, cor_v5;
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //assume padr�o de cores default
   cor_v1  = int(geral->COR.Faixa[diagRUIM]);
   cor_v2  = int(geral->COR.Faixa[diagREGULAR]);
   cor_v3  = int(geral->COR.Faixa[diagBOM]);
   cor_v4  = int(geral->COR.Faixa[diagREGULAR]);
   cor_v5  = int(geral->COR.Faixa[diagRUIM]);
   //verifica se h� configura��o de cores salva em infoset
   if (geral->Infoset->LoadFromFile(arq_infoset))
      {//l� configura��o de cores do infoset
      geral->Infoset->GetInfo("cor_v1" , cor_v1 );
      geral->Infoset->GetInfo("cor_v2" , cor_v2 );
      geral->Infoset->GetInfo("cor_v3" , cor_v3 );
      geral->Infoset->GetInfo("cor_v4" , cor_v4 );
      geral->Infoset->GetInfo("cor_v5" , cor_v5 );
      }
   //inicia estrutura
   cfg_cor->TENSAO.corV1  = TColor(cor_v1 );
   cfg_cor->TENSAO.corV2  = TColor(cor_v2 );
   cfg_cor->TENSAO.corV3  = TColor(cor_v3 );
   cfg_cor->TENSAO.corV4  = TColor(cor_v4 );
   cfg_cor->TENSAO.corV5  = TColor(cor_v5 );
   }

//---------------------------------------------------------------------------
void __fastcall TFormCorTensao::PanCorV1Click(TObject *Sender)
   {
   if (ColorDialog->Execute())
      {
      PanCorV1->Color = ColorDialog->Color;
      cfg_cor->TENSAO.corV1 = ColorDialog->Color;
      ActionAtualiza->Execute();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCorTensao::PanCorV2Click(TObject *Sender)
   {
   if (ColorDialog->Execute())
      {
      PanCorV2->Color = ColorDialog->Color;
      cfg_cor->TENSAO.corV2 = ColorDialog->Color;
      ActionAtualiza->Execute();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCorTensao::PanCorV3Click(TObject *Sender)
   {
   if (ColorDialog->Execute())
      {
      PanCorV3->Color = ColorDialog->Color;
      cfg_cor->TENSAO.corV3 = ColorDialog->Color;
      ActionAtualiza->Execute();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCorTensao::PanCorV4Click(TObject *Sender)
   {
   if (ColorDialog->Execute())
      {
      PanCorV4->Color = ColorDialog->Color;
      cfg_cor->TENSAO.corV4 = ColorDialog->Color;
      ActionAtualiza->Execute();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCorTensao::PanCorV5Click(TObject *Sender)
   {
   if (ColorDialog->Execute())
      {
      PanCorV5->Color = ColorDialog->Color;
      cfg_cor->TENSAO.corV5 = ColorDialog->Color;
      ActionAtualiza->Execute();
      }
   }

//---------------------------------------------------------------------------

void __fastcall TFormCorTensao::tabControlNivelVChange(TObject *Sender)
{
	switch (tabControlNivelV->TabIndex) {

	case 0: //BT
		ExibeDados(redeSEC);
		break;
	case 1: //MT
		ExibeDados(redePRI);
		break;
	case 2: //AT
		ExibeDados(redeSUB);
		break;
    default:
		;
	}
}
//---------------------------------------------------------------------------

