// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPath.h"
#include "DirArq.h"
#include "..\Constante\Const.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTPath* NewObjPath(void)
{
	return (new TPath());
}

// ---------------------------------------------------------------------------
__fastcall TPath::TPath(void)
{
	// inicia diretórios, bases e arquivos com valores padrão
	arq.historico = "";
}

// ---------------------------------------------------------------------------
__fastcall TPath::~TPath(void)
{
	// nada a fazer
}

/*
 //---------------------------------------------------------------------------
 AnsiString __fastcall TPath::ArqArranjo(void)
 {
 //return(DirGeral() + "\\" + DBNAME_ARRANJO);
 return(DirGeral() + "\\ArqCfgEmpresa.txt");
 }
 */

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::ArqBitmap(int bmp_id)
{
	// variáveis locais
	AnsiString arq_bmp = DirBmp() + "\\";

	switch (bmp_id)
	{
	case bmpCAPACITOR:
		arq_bmp += ARQ_BMP_CAPACITOR;
		break;
	case bmpCAPSERIE:
		arq_bmp += ARQ_BMP_CAPSERIE;
		break;
	case bmpCARGA:
		arq_bmp += ARQ_BMP_CARGA;
		break;
	case bmpFILTRO:
		arq_bmp += ARQ_BMP_FILTRO;
		break;
	case bmpGERADOR:
		arq_bmp += ARQ_BMP_GERADOR;
		break;
	case bmpMEDIDOR:
		arq_bmp += ARQ_BMP_MEDIDOR;
		break;
	case bmpMUTUA:
		arq_bmp += ARQ_BMP_MUTUA;
		break;
	case bmpREATOR:
		arq_bmp += ARQ_BMP_REATOR;
		break;
	case bmpREGULADOR:
		arq_bmp += ARQ_BMP_REGULADOR;
		break;
	case bmpSUPRIMENTO:
		arq_bmp += ARQ_BMP_SUPRIMENTO;
		break;
	case bmpTRAFO_H:
		arq_bmp += ARQ_BMP_TRAFO_H;
		break;
	case bmpTRAFO_V:
		arq_bmp += ARQ_BMP_TRAFO_V;
		break;
	case bmpTRAFO3E_H:
		arq_bmp += ARQ_BMP_TRAFO3E_H;
		break;
	case bmpTRAFO3E_V:
		arq_bmp += ARQ_BMP_TRAFO3E_V;
		break;
	case bmpTRAFOZZ:
		arq_bmp += ARQ_BMP_TRAFOZZ;
		break;
	case bmpZREF:
		arq_bmp += ARQ_BMP_ZREF;
		break;
	default:
		arq_bmp += ARQ_BMP_DUMMY;
		break;
	}
	return (arq_bmp);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::ArqRedeEmpresa(void)
{
	return (DirDat() + "\\RedesPadrao\\RedeEmpresa.mdb");
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::ArqRedeSinap(void)
{
	return (DirDat() + "\\RedesPadrao\\RedeSinap.mdb");
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::ArqRelCurto(void)
{
	return (DirTmp() + "\\" + ARQ_REL_CURTO);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::ArqRelFlow(void)
{
	return (DirTmp() + "\\" + ARQ_REL_FLOW);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::ArqRelFlowDebug(void)
{
	return (DirTmp() + "\\" + ARQ_LOG_FLOW);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::ArqTermoUso(void)
{
	return (DirGeral() + "\\" + ARQ_TERMO_USO);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::ArqTermoUsoDemo(void)
{
	return (DirGeral() + "\\" + ARQ_TERMO_USO_DEMO);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::ArqTermoUsoDidatico(void)
{
	return (DirGeral() + "\\" + ARQ_TERMO_USO_DIDATICO);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::DirBmp(void)
{
	return (DirDat() + "\\" + DIR_BMP);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::DirDat(void)
{
	return (DirBase + "\\" + DIR_DAT);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::DirExporta(void)
{
	return (DirBase + "\\" + DIR_EXPORTA);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::DirGeral(void)
{
	return (DirDat() + "\\" + DIR_GERAL);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::DirHelp(void)
{
	return (DirBase + "\\" + DIR_HELP);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::DirHistorico(void)
{
	return (DirDat() + "\\" + DIR_HISTORICO);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::DirIcone(void)
{
	return (DirDat() + "\\" + DIR_ICONE);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::DirImporta(void)
{
	return (DirBase + "\\" + DIR_IMPORTA);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::DirModuloConstrutivo(void)
{
	return (DirDat() + "\\Modulo");
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::DirRedes(void)
{
	return (DirBase + "\\" + DIR_REDES);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::DirTmp(void)
{
	// Variáveis locais
	AnsiString tempWindowsDir;
	TGUID g;
	AnsiString tmpBatch;


	if(dir.tmpBatch.IsEmpty())
//		return (DirBase + "\\" + DIR_TMP);
		{
		// cria o diretorio temporario dessa instancia
		//
		// Isto permite executar em paralelo, pois cada processo(instancia do sinap) utiliza
		// um diretorio tmp proprio

		// pega o diretorio temporario do windows(diretorio temporario do usuario)
	//	tempWindowsDir = System::Ioutils::TPath::GetTempPathW();

		// cria um UID para cada processo batch:
		CreateGUID(g);

		// define o diretorio temporario para estah instancia do sinap <tmp do windows>\<UID>
		dir.tmpBatch = (DirBase + "\\" + DIR_TMP + "\\" + Sysutils::GUIDToString(g));

		// cria o diretorio temporario
		CreateDirectoryA(this->DirTmpBatch.c_str(), NULL);
//		ForceDirectories(dir.tmpBatch);
		return (dir.tmpBatch);
		}
		if(dir.tmpBatch.IsEmpty())
			return (DirBase + "\\" + DIR_TMP);
		else
			return (dir.tmpBatch);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::DirUsuario(void)
{
	return (DirDat() + "\\" + DIR_USUARIO);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::PM_GetArqHistorico(void)
{
	// verifica se dir.bin está definido
	if (arq.historico.IsEmpty())
	{
		arq.historico = DirHistorico() + "\\" + DBNAME_HISTORICO;
	}
	return (arq.historico);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::PM_GetArqRede(void)
{
	// verifica se dir.bin está definido
	if (arq.rede.IsEmpty())
	{
		arq.rede = dir.base + "\\Redes\\" + DBNAME_REDE;
	}
	return (arq.rede);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::PM_GetDirBase(void)
{
	return (dir.base);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::PM_GetDirBin(void)
{
	// verifica se dir.bin está definido
	if (dir.bin.IsEmpty())
	{
		dir.bin = dir.base + "\\" + DIR_BIN;
	}
	return (dir.bin);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TPath::PM_GetDirTmpBatch()
{

	return (dir.tmpBatch);

}


// ---------------------------------------------------------------------------
void __fastcall TPath::PM_SetArqHistorico(AnsiString arq_historico)
{
	arq.historico = arq_historico;
}

// ---------------------------------------------------------------------------
void __fastcall TPath::PM_SetArqRede(AnsiString arq_rede)
{
	arq.rede = arq_rede;
}

// ---------------------------------------------------------------------------
void __fastcall TPath::PM_SetDirBase(AnsiString dir_base)
{
	AnsiString lastChar;

	lastChar = dir_base.AnsiLastChar();
	if (lastChar.AnsiCompare("\\")==0)
	{
        dir_base.Delete(dir_base.Length(),1);
		dir.base = dir_base;
	}
	else
	{
		dir.base = dir_base;
    }

}

// ---------------------------------------------------------------------------
void __fastcall TPath::PM_SetDirBin(AnsiString dir_bin)
{
	dir.bin = dir_bin;
}


// ---------------------------------------------------------------------------
void __fastcall TPath::PM_SetDirTmpBatch(AnsiString tmpDir )
{
	// para criar abaixo do tmp do sinap:
	//dir.tmpBatch = (DirBase + "\\" + DIR_TMP  "\\" + tmpDir);
	dir.tmpBatch = tmpDir;

}


// ---------------------------------------------------------------------------
// eof
