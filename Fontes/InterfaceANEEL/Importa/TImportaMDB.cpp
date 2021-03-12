//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TImportaMDB.h"
#include "VTBDaneel.h"
#include "VTPreMonta.h"
#include "..\..\Apl\VTApl.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTImporta* NewObjImportaMDB(VTApl *apl_owner)
   {
   return(new TImportaMDB(apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TImportaMDB::TImportaMDB(VTApl *apl_owner)
   {
   //salva ponteiro para objeto Apl
	this->apl = apl_owner;
	//cria objeto BDaneel
	BDaneel = NewObjBDaneel(apl);
	}

//---------------------------------------------------------------------------
__fastcall TImportaMDB::~TImportaMDB(void)
   {
   //destrói objeto Apl local, caso tenha sido criado
	if (BDaneel != NULL) {delete BDaneel; BDaneel = NULL;}
   }

//-----------------------------------------------------------------------------
bool __fastcall TImportaMDB::ExecutaImportacao(AnsiString filename)
	{
	//variáveis locais
	bool       sucesso;
	VTPreMonta *pre_monta = (VTPreMonta*)apl->GetObject(__classid(VTPreMonta));

	try{//inicia objeto PreMonta
		pre_monta->TrataIniciaDados();
		//inicia conexão com a base de dados
		sucesso = BDaneel->Conecta(filename);
		//curvas típicas
		if (sucesso) sucesso = BDaneel->ReadCrvCrgBT();
		if (sucesso) sucesso = BDaneel->ReadCrvCrgMT();
		//condutors
		if (sucesso) sucesso = BDaneel->ReadCodCondutor();
		//AT
		if (sucesso) sucesso = BDaneel->ReadTrafoATATATMT();
		//MT
		if (sucesso) sucesso = BDaneel->ReadCircMT();
		if (sucesso) sucesso = BDaneel->ReadSegmentoMT();
		if (sucesso) sucesso = BDaneel->ReadRegulador();
		if (sucesso) sucesso = BDaneel->ReadChaveMT();
		if (sucesso) sucesso = BDaneel->ReadCargaMT();
		if (sucesso) sucesso = BDaneel->ReadGeradorMT();
		//BT
		if (sucesso) sucesso = BDaneel->ReadTrafoMTMTMTBT();
		if (sucesso) sucesso = BDaneel->ReadSegmentoBT();
		if (sucesso) sucesso = BDaneel->ReadChaveBT();
		if (sucesso) sucesso = BDaneel->ReadRamal();
		if (sucesso) sucesso = BDaneel->ReadGeradorBT();
		if (sucesso) sucesso = BDaneel->ReadCargaBT();
		//fecha conexão com a base de dados
		BDaneel->Desconecta();
		//finaliza pre_monta
		if (sucesso) pre_monta->TrataFimDados();
		}catch(Exception &e)
			{
			sucesso = false;
			}
	return(sucesso);
	}

//-----------------------------------------------------------------------------
// eof


