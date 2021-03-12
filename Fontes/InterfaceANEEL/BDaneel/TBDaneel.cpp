//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TBDaneel.h"
#include "TSqlInsert.h"
#include "VTExtrator.h"
#include "VTPreMonta.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Log\VTLog.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTBDaneel* NewObjBDaneel(VTApl *apl_owner)
   {
	return(new TBDaneel(apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TBDaneel::TBDaneel(VTApl *apl_owner)
	{
	//salva ponteiro para objetos
	this->apl = apl_owner;
	pre_monta     = (VTPreMonta*)apl_owner->GetObject(__classid(VTPreMonta));
	plog          = (VTLog*)apl_owner->GetObject(__classid(VTLog));
	//cria objetos para acesso à base de dados
	connection = new TADOConnection(NULL);
	query      = new TADOQuery(NULL);
	//associa ADOQuery com ADOConnection
	query->Connection = connection;
	}

//---------------------------------------------------------------------------
__fastcall TBDaneel::~TBDaneel(void)
	{
	//desocnecta a base de dados
	Desconecta();
	//destrói objetos criados
	if (query)      {delete query;      query      = NULL;}
	if (connection) {delete connection; connection = NULL;}
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::Conecta(AnsiString filename)
	{
	//variáveis locais
	bool        sucesso;

	try{//configura Log
		if (plog) plog->DefineValor("Arquivo", ExtractFileName(filename));
		//verifica se o arquivo existe
		if (! FileExists(filename)) return(false);
		//proteção: verifica se existem os objetos ADOConnection e ADOQuery
		if ((connection == NULL) || (query == NULL)) return(false);
		//garante que a conexão não está ativa
		if (connection->Connected) connection->Connected = false;
		//configura ADOConnection e inicia conexão
		connection->ConnectionString = "Provider=Microsoft.ACE.OLEDB.12.0; Data Source=" + filename;
		connection->LoginPrompt      = false;
		connection->Connected        = true;
		sucesso = connection->Connected;
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro de conexão com base " + filename);
			sucesso = false;
			}
	return(sucesso);
	}

//---------------------------------------------------------------------------
void __fastcall TBDaneel::QueryClose(void)
	{
	//proteção: fecha Query se ela ainda estiver ativer
	if (query->Active) query->Close();
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::QueryExecSql(AnsiString cmd_sql)
	{
	//variáveis locais
	bool sucesso;

	//proteção: fecha Query se ela ainda estiver ativer
	QueryClose();
	//configura ADOQuery
	query->Connection     = connection;
	query->Filtered       = false;
	query->Filter         = "";
	query->CursorLocation = clUseClient;
	query->CursorType     = ctStatic	;
	query->LockType       = ltReadOnly;
	//monta comando SQL
	query->SQL->Clear();
	query->SQL->Add(cmd_sql);
	sucesso = query->ExecSQL();
	//fecha conexão
	query->Connection = NULL;
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::QueryOpenReadOnly(AnsiString tablename)
	{
	//proteção: fecha Query se ela ainda estiver ativer
	QueryClose();
	//configura ADOQuery
	query->Connection     = connection;
	query->Filtered       = false;
	query->Filter         = "";
	query->CursorLocation = clUseClient;
	query->CursorType     = ctStatic	;
	query->LockType       = ltReadOnly;
	//monta comando SQL
	query->SQL->Clear();
	query->SQL->Add("SELECT * FROM " + tablename);
	query->Open();
	//fecha conexão
	query->Connection = NULL;
	return(query->Active);
	}

//---------------------------------------------------------------------------
void __fastcall TBDaneel::Desconecta(void)
	{
	//fecha ADOQuery
	QueryClose();
	//desabilita conexão
	connection->Connected = false;
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::ReadCargaBT(void)
	{
	//variáveis locais
	sdi_CARGA_BT sdi;
	int          nr;
	AnsiString   fieldname;
	AnsiString   tablename = "CargaBT";
	bool         sucesso   = true;

	try{//configura Log
		if (plog) plog->DefineValor("Tabela", tablename);
		//executa consulta da tabela CargaBT
		if (! QueryOpenReadOnly(tablename)) return(false);
		//loop p/ todos registros
		for (nr = 1, query->First(); (sucesso) && (! query->Eof); nr++, query->Next())
			{//configura Log
			if (plog) plog->DefineValor("Registro", IntToStr(nr));
			//lê campos e inicia estrutura
			sdi.CodBase             = query->FieldByName("CodBase")->AsInteger;
			sdi.CodTrafo            = query->FieldByName("CodTrafo")->AsString;
			sdi.CodRamal            = query->FieldByName("CodRml")->AsString;
			sdi.CodConsumidorBT     = query->FieldByName("CodConsBT")->AsString;
			sdi.CodFases            = query->FieldByName("CodFas")->AsString;
			sdi.CodPontoAcoplamento = query->FieldByName("CodPonAcopl")->AsString;
			sdi.SemRedeAssociada    = query->FieldByName("SemRedAssoc")->AsInteger;
			sdi.TipoMedicao         = query->FieldByName("TipMedi")->AsInteger;
			sdi.TipologiaCurvaCarga = query->FieldByName("TipCrvaCarga")->AsString;
			for (int n = 0; n < 12; n++)
				{
				fieldname.sprintf("EnerMedid%02d_MWh", n+1);
				sdi.Energia_mwhmes[n] = query->FieldByName(fieldname)->AsFloat;
				}
//			sdi.Descricao = query->FieldByName("Desc")->AsString;
			//insere CargaBT em PreMonta
			sucesso = pre_monta->InsereCargaBT(sdi);
			}
		QueryClose();
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro de leitura da tabela: " + tablename);
			sucesso = false;
			}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel:: ReadCargaMT(void)
	{
	//variáveis locais
	sdi_CARGA_MT sdi;
	int          nr;
	AnsiString   fieldname;
	AnsiString   tablename = "CargaMT";
	bool         sucesso   = true;

	try{//configura Log
		if (plog) plog->DefineValor("Tabela", tablename);
		//executa consulta da tabela CargaMT
		if (! QueryOpenReadOnly(tablename)) return(false);
		//loop p/ todos registros
		for (nr = 1, query->First(); (sucesso) && (! query->Eof); nr++, query->Next())
			{//configura Log
			if (plog) plog->DefineValor("Registro", IntToStr(nr));
			//lê campos e inicia estrutura
			sdi.CodBase             = query->FieldByName("CodBase")->AsInteger;
			sdi.CodAlimentador      = query->FieldByName("CodAlim")->AsString;
			sdi.CodConsumidorMT     = query->FieldByName("CodConsMT")->AsString;
			sdi.CodFases            = query->FieldByName("CodFas")->AsString;
			sdi.CodPontoAcoplamento = query->FieldByName("CodPonAcopl")->AsString;
			sdi.SemRedeAssociada    = query->FieldByName("SemRedAssoc")->AsInteger;
			sdi.TipologiaCurvaCarga = query->FieldByName("TipCrvaCarga")->AsString;
			for (int n = 0; n < 12; n++)
				{
				fieldname.sprintf("EnerMedid%02d_MWh", n+1);
				sdi.Energia_mwhmes[n] = query->FieldByName(fieldname)->AsFloat;
				}
//			sdi.Descricao = query->FieldByName("Desc")->AsString;
			//insere CargaMT em PreMonta
			sucesso = pre_monta->InsereCargaMT(sdi);
			}
		QueryClose();
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro de leitura da tabela: " + tablename);
			sucesso = false;
			}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel:: ReadChaveBT(void)
	{
	//variáveis locais
	sdi_CHAVE_BT sdi;
	int          nr;
	AnsiString   fieldname;
	AnsiString   tablename = "ChaveBT";
	bool         sucesso   = true;

	try{//configura Log
		if (plog) plog->DefineValor("Tabela", tablename);
		//executa consulta da tabela ChaveBT
		if (! QueryOpenReadOnly(tablename)) return(false);
		//loop p/ todos registros
		for (nr = 1, query->First(); (sucesso) && (! query->Eof); nr++, query->Next())
			{//configura Log
			if (plog) plog->DefineValor("Registro", IntToStr(nr));
			//lê campos e inicia estrutura
			sdi.CodBase              = query->FieldByName("CodBase")->AsInteger;
			sdi.CodTrafo             = query->FieldByName("CodTrafo")->AsString;
			sdi.CodChave             = query->FieldByName("CodChv")->AsString;
			sdi.CodPontoAcoplamento1 = query->FieldByName("CodPonAcopl1")->AsString;
			sdi.CodPontoAcoplamento2 = query->FieldByName("CodPonAcopl2")->AsString;
			sdi.CodFases             = query->FieldByName("CodFas")->AsString;
			sdi.Estado               = query->FieldByName("EstChv")->AsInteger;
//			sdi.Descricao            = query->FieldByName("Desc")->AsString;
			//insere ChaveBT em PreMonta
			sucesso = pre_monta->InsereChaveBT(sdi);
			}
		QueryClose();
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro de leitura da tabela: " + tablename);
			sucesso = false;
			}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::ReadChaveMT(void)
	{
	//variáveis locais
	sdi_CHAVE_MT sdi;
	int          nr;
	AnsiString   fieldname;
	AnsiString   tablename = "ChaveMT";
	bool         sucesso   = true;

	try{//configura Log
		if (plog) plog->DefineValor("Tabela", tablename);
		//executa consulta da tabela ChaveMT
		if (! QueryOpenReadOnly(tablename)) return(false);
		//loop p/ todos registros
		for (nr = 1, query->First(); (sucesso) && (! query->Eof); nr++, query->Next())
			{//configura Log
			if (plog) plog->DefineValor("Registro", IntToStr(nr));
			//lê campos e inicia estrutura
			sdi.CodBase              = query->FieldByName("CodBase")->AsInteger;
			sdi.CodAlimentador       = query->FieldByName("CodAlim")->AsString;
			sdi.CodChave             = query->FieldByName("CodChv")->AsString;
			sdi.CodPontoAcoplamento1 = query->FieldByName("CodPonAcopl1")->AsString;
			sdi.CodPontoAcoplamento2 = query->FieldByName("CodPonAcopl2")->AsString;
			sdi.CodFases             = query->FieldByName("CodFas")->AsString;
			sdi.Estado               = query->FieldByName("EstChv")->AsInteger;
//			sdi.Descricao            = query->FieldByName("Desc")->AsString;
			//insere ChaveMT em PreMonta
			sucesso = pre_monta->InsereChaveMT(sdi);
			}
		QueryClose();
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro de leitura da tabela: " + tablename);
			sucesso = false;
			}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::ReadCircMT(void)
	{
	//variáveis locais
	sdi_CIRCUITO sdi;
	int          nr;
	AnsiString   fieldname;
	AnsiString   tablename = "CircMT";
	bool         sucesso   = true;

	try{//configura Log
		if (plog) plog->DefineValor("Tabela", tablename);
		//executa consulta da tabela CircMT
		if (! QueryOpenReadOnly(tablename)) return(false);
		//loop p/ todos registros
		for (nr = 1, query->First(); (sucesso) && (! query->Eof); nr++, query->Next())
			{//configura Log
			if (plog) plog->DefineValor("Registro", IntToStr(nr));
			//lê campos e inicia estrutura
			sdi.CodBase             = query->FieldByName("CodBase")->AsInteger;
			sdi.CodSubestacao       = query->FieldByName("CodSub")->AsString;
			sdi.CodAlimentador      = query->FieldByName("CodAlim")->AsString;
			sdi.Vnom_kv             = query->FieldByName("TenNom_kV")->AsFloat;
			sdi.Vope_pu             = query->FieldByName("TenOpe_pu")->AsFloat;
			sdi.CodPontoAcoplamento = query->FieldByName("CodPonAcopl")->AsString;
			sdi.CircAtipico         = query->FieldByName("CircAtip")->AsInteger;
			for (int n = 0; n < 12; n++)
				{
				fieldname.sprintf("EnerCirc%02d_MWh", n+1);
				sdi.Energia_mwhmes[n] = query->FieldByName(fieldname)->AsFloat;
				}
			for (int n = 0; n < 12; n++)
				{
				fieldname.sprintf("PerdCirc%02d_MWh", n+1);
				sdi.Perda_mwhmes[n] = query->FieldByName(fieldname)->AsFloat;
				}
			//insere Circuito em PreMonta
			sucesso = pre_monta->InsereCircuito(sdi);
			}
		QueryClose();
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro de leitura da tabela " + tablename);
			sucesso = false;
			}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::ReadCodCondutor(void)
	{
	//variáveis locais
	sdi_CONDUTOR sdi;
	int          nr;
	AnsiString   fieldname;
	AnsiString   tablename = "CodCondutor";
	bool         sucesso   = true;

	try{//configura Log
		if (plog) plog->DefineValor("Tabela", tablename);
		//executa consulta da tabela CodCondutor
		if (! QueryOpenReadOnly(tablename)) return(false);
		//loop p/ todos registros
		for (nr = 1, query->First(); (sucesso) && (! query->Eof); nr++, query->Next())
			{//configura Log
			if (plog) plog->DefineValor("Registro", IntToStr(nr));
			//lê campos e inicia estrutura
			sdi.CodBase     = query->FieldByName("CodBase")->AsInteger;
			sdi.CodCondutor = query->FieldByName("CodCond")->AsString;
			sdi.Rohm_km     = query->FieldByName("Resis_ohms_km")->AsFloat;
			sdi.Xohm_km     = query->FieldByName("Reat_ohms_km")->AsFloat;
			sdi.Imax_amp    = query->FieldByName("CorrMax_A")->AsFloat;
//			sdi.Descricao   = query->FieldByName("Desc")->AsString;
			//insere Condutor em PreMonta
			sucesso = pre_monta->InsereCondutor(sdi);
			}
		QueryClose();
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro de leitura da tabela :" + tablename);
			sucesso = false;
			}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::ReadCrvCrgBT(void)
	{
	//variáveis locais
	sdi_CURVA  sdi;
	int        nr;
	AnsiString fieldname;
	AnsiString tablename = "CrvCrgBT";
	bool       sucesso   = true;

	try{//configura Log
		if (plog) plog->DefineValor("Tabela", tablename);
		//executa consulta da tabela CrvCrgBT
		if (! QueryOpenReadOnly(tablename)) return(false);
		//loop p/ todos registros
		for (nr = 1, query->First(); (sucesso) && (! query->Eof); nr++, query->Next())
			{//configura Log
			if (plog) plog->DefineValor("Registro", IntToStr(nr));
			//lê campos e inicia estrutura
			sdi.CodBase  = query->FieldByName("CodBase")->AsInteger;
			sdi.CodCurva = query->FieldByName("CodCrvCrg")->AsString;
			sdi.TipoDia  = query->FieldByName("TipoDia")->AsString;
			for (int n = 0; n < 96; n++)
				{
				fieldname.sprintf("PotAtv%02d_kW", n+1);
				sdi.P_kw[n] = query->FieldByName(fieldname)->AsFloat;
				}
			//insere Curva Típica BT em PreMonta
			sucesso = pre_monta->InsereCurvaTipicaBT(sdi);
			}
		QueryClose();
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro de leitura da tabela: " + tablename);
			sucesso = false;
			}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::ReadCrvCrgMT(void)
	{
	//variáveis locais
	sdi_CURVA  sdi;
	int        nr;
	AnsiString fieldname;
	AnsiString tablename = "CrvCrgMT";
	bool       sucesso   = true;

	try{//configura Log
		if (plog) plog->DefineValor("Tabela", tablename);
		//executa consulta da tabela CrvCrgMT
		if (! QueryOpenReadOnly(tablename)) return(false);
		//loop p/ todos registros
		for (nr = 1, query->First(); (sucesso) && (! query->Eof); nr++, query->Next())
			{//configura Log
			if (plog) plog->DefineValor("Registro", IntToStr(nr));
			//lê campos e inicia estrutura
			sdi.CodBase  = query->FieldByName("CodBase")->AsInteger;
			sdi.CodCurva = query->FieldByName("CodCrvCrg")->AsString;
			sdi.TipoDia  = query->FieldByName("TipoDia")->AsString;
			for (int n = 0; n < 96; n++)
				{
				fieldname.sprintf("PotAtv%02d_kW", n+1);
				sdi.P_kw[n] = query->FieldByName(fieldname)->AsFloat;
				}
			//insere Curva Típica MT em PreMonta
			sucesso = pre_monta->InsereCurvaTipicaMT(sdi);
			}
		QueryClose();
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro de leitura da tabela: " + tablename);
			sucesso = false;
			}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::ReadGeradorBT(void)
	{
	//variáveis locais
	sdi_GERADOR_BT sdi;
	int            nr;
	AnsiString     fieldname;
	AnsiString     tablename = "GeradorBT";
	bool           sucesso   = true;

	try{//configura Log
		if (plog) plog->DefineValor("Tabela", tablename);
		//executa consulta da tabela GeradorBT
		if (! QueryOpenReadOnly(tablename)) return(false);
		//loop p/ todos registros
		for (nr = 1, query->First(); (sucesso) && (! query->Eof); nr++, query->Next())
			{//configura Log
			if (plog) plog->DefineValor("Registro", IntToStr(nr));
			//lê campos e inicia estrutura
			sdi.CodBase             = query->FieldByName("CodBase")->AsInteger;
			sdi.CodRamal            = query->FieldByName("CodRml")->AsString;
			sdi.CodGerador          = query->FieldByName("CodGeraBT")->AsString;
			sdi.CodPontoAcoplamento = query->FieldByName("CodPonAcopl")->AsString;
			sdi.CodFases            = query->FieldByName("CodFas")->AsString;
			for (int n = 0; n < 12; n++)
				{
				fieldname.sprintf("EnerMedid%02d_MWh", n+1);
				sdi.Energia_mwhmes[n] = query->FieldByName(fieldname)->AsFloat;
				}
//			sdi.Descricao   = query->FieldByName("Desc")->AsString;
			//insere Gerador BT em PreMonta
			sucesso = pre_monta->InsereGeradorBT(sdi);
			}
		QueryClose();
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro de leitura da tabela: " + tablename);
			sucesso = false;
			}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::ReadGeradorMT(void)
	{
	//variáveis locais
	sdi_GERADOR_MT sdi;
	int            nr;
	AnsiString     fieldname;
	AnsiString     tablename = "GeradorMT";
	bool           sucesso   = true;

	try{//configura Log
		if (plog) plog->DefineValor("Tabela", tablename);
		//executa consulta da tabela GeradorMT
		if (! QueryOpenReadOnly(tablename)) return(false);
		//loop p/ todos registros
		for (nr = 1, query->First(); (sucesso) && (! query->Eof); nr++, query->Next())
			{//configura Log
			if (plog) plog->DefineValor("Registro", IntToStr(nr));
			//lê campos e inicia estrutura
			sdi.CodBase             = query->FieldByName("CodBase")->AsInteger;
			sdi.CodAlimentador      = query->FieldByName("CodAlim")->AsString;
			sdi.CodGerador          = query->FieldByName("CodGeraMT")->AsString;
			sdi.CodPontoAcoplamento = query->FieldByName("CodPonAcopl")->AsString;
			sdi.CodFases = query->FieldByName("CodFas")->AsString;
			for (int n = 0; n < 12; n++)
				{
				fieldname.sprintf("EnerMedid%02d_MWh", n+1);
				sdi.Energia_mwhmes[n] = query->FieldByName(fieldname)->AsFloat;
				}
//			sdi.Descricao   = query->FieldByName("Desc")->AsString;
			//insere Gerador MT em PreMonta
			sucesso = pre_monta->InsereGeradorMT(sdi);
			}
		QueryClose();
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro de leitura da tabela: " + tablename);
			sucesso = false;
			}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::ReadRamal(void)
	{
	//variáveis locais
	sdi_RAMAL  sdi;
	int        nr;
	AnsiString fieldname;
	AnsiString tablename = "Ramal";
	bool       sucesso   = true;

	try{//configura Log
		if (plog) plog->DefineValor("Tabela", tablename);
		//executa consulta da tabela Ramal
		if (! QueryOpenReadOnly(tablename)) return(false);
		//loop p/ todos registros
		for (nr = 1, query->First(); (sucesso) && (! query->Eof); nr++, query->Next())
			{//configura Log
			if (plog) plog->DefineValor("Registro", IntToStr(nr));
			//lê campos e inicia estrutura
			sdi.CodBase              = query->FieldByName("CodBase")->AsInteger;
			sdi.CodTrafo             = query->FieldByName("CodTrafo")->AsString;
			sdi.CodRamal             = query->FieldByName("CodRml")->AsString;
			sdi.CodPontoAcoplamento1 = query->FieldByName("CodPonAcopl1")->AsString;
			sdi.CodPontoAcoplamento2 = query->FieldByName("CodPonAcopl2")->AsString;
			sdi.CodFases             = query->FieldByName("CodFas")->AsString;
			sdi.CodCondutor          = query->FieldByName("CodCond")->AsString;
			sdi.Comprimento_km       = query->FieldByName("Comp_km")->AsFloat;
//			sdi.Descricao            = query->FieldByName("Desc")->AsString;
			//insere Ramal BT em PreMonta
			sucesso = pre_monta->InsereRamalBT(sdi);
			}
		QueryClose();
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro de leitura da tabela: " + tablename);
			sucesso = false;
			}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::ReadRegulador(void)
	{
	//variáveis locais
	sdi_REGULADOR sdi;
	int           nr;
	AnsiString    fieldname;
	AnsiString    tablename = "Regulador";
	bool          sucesso   = true;

	try{//configura Log
		if (plog) plog->DefineValor("Tabela", tablename);
		//executa consulta da tabela Regulador
		if (! QueryOpenReadOnly(tablename)) return(false);
		//loop p/ todos registros
		for (nr = 1, query->First(); (sucesso) && (! query->Eof); nr++, query->Next())
			{//configura Log
			if (plog) plog->DefineValor("Registro", IntToStr(nr));
			//lê campos e inicia estrutura
			sdi.CodBase              = query->FieldByName("CodBase")->AsInteger;
			sdi.CodAlimentador       = query->FieldByName("CodAlim")->AsString;
			sdi.CodRegulador         = query->FieldByName("CodRegul")->AsString;
			sdi.SeqBanco             = query->FieldByName("CodBnc")->AsInteger;
			sdi.CodPontoAcoplamento1 = query->FieldByName("CodPonAcopl1")->AsString;
			sdi.CodPontoAcoplamento2 = query->FieldByName("CodPonAcopl2")->AsString;
			sdi.Snom_kva             = query->FieldByName("PotNom_kVA")->AsFloat;
			sdi.TipoRegulador        = query->FieldByName("TipRegul")->AsInteger;
			sdi.Vreg_pu              = query->FieldByName("TenRgl_pu")->AsFloat;
			sdi.CodFasesPri          = query->FieldByName("CodFasPrim")->AsString;
			sdi.CodFasesSec          = query->FieldByName("CodFasSecu")->AsString;
			sdi.R_perc               = query->FieldByName("Resis_%")->AsFloat;
			sdi.Xhl_perc             = query->FieldByName("ReatHL_%")->AsFloat;
			sdi.PerdaTotal_w         = query->FieldByName("PerdTtl_W")->AsFloat;
			sdi.PerdaVazio_w         = query->FieldByName("PerdVz_W")->AsFloat;
			sdi.Energia_mwhano       = query->FieldByName("EnerMedid_MWh")->AsFloat;
//			sdi.Descricao            = query->FieldByName("Desc")->AsString;
			//insere Regulador em PreMonta
			sucesso = pre_monta->InsereReguladorMT(sdi);
			}
		QueryClose();
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro de leitura da tabela: " + tablename);
			sucesso = false;
			}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::ReadSegmentoBT(void)
	{
	//variáveis locais
	sdi_SEGMENTO_BT sdi;
	int             nr;
	AnsiString      fieldname;
	AnsiString      tablename = "SegmentoBT";
	bool            sucesso   = true;

	try{//configura Log
		if (plog) plog->DefineValor("Tabela", tablename);
		//executa consulta da tabela SegmentoBT
		if (! QueryOpenReadOnly(tablename)) return(false);
		//loop p/ todos registros
		for (nr = 1, query->First(); (sucesso) && (! query->Eof); nr++, query->Next())
			{//configura Log
			if (plog) plog->DefineValor("Registro", IntToStr(nr));
			//lê campos e inicia estrutura
			sdi.CodBase              = query->FieldByName("CodBase")->AsInteger;
			sdi.CodTrafo             = query->FieldByName("CodTrafo")->AsString;
			sdi.CodSegmento          = query->FieldByName("CodSegmBT")->AsString;
			sdi.CodPontoAcoplamento1 = query->FieldByName("CodPonAcopl1")->AsString;
			sdi.CodPontoAcoplamento2 = query->FieldByName("CodPonAcopl2")->AsString;
			sdi.CodFases             = query->FieldByName("CodFas")->AsString;
			sdi.CodCondutor          = query->FieldByName("CodCond")->AsString;
			sdi.Comprimento_km       = query->FieldByName("Comp_km")->AsFloat;
//			sdi.Descricao            = query->FieldByName("Desc")->AsString;
			//insere SegmentoBT em PreMonta
			sucesso = pre_monta->InsereSegmentoBT(sdi);
			}
		QueryClose();
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro de leitura da tabela: " + tablename);
			sucesso = false;
			}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::ReadSegmentoMT(void)
	{
	//variáveis locais
	sdi_SEGMENTO_MT sdi;
	int             nr;
	AnsiString      fieldname;
	AnsiString      tablename = "SegmentoMT";
	bool            sucesso   = true;

	try{//configura Log
		if (plog) plog->DefineValor("Tabela", tablename);
		//executa consulta da tabela SegmentoMT
		if (! QueryOpenReadOnly(tablename)) return(false);
		//loop p/ todos registros
		for (nr = 1, query->First(); (sucesso) && (! query->Eof); nr++, query->Next())
			{//configura Log
			if (plog) plog->DefineValor("Registro", IntToStr(nr));
			//lê campos e inicia estrutura
			sdi.CodBase              = query->FieldByName("CodBase")->AsInteger;
			sdi.CodAlimentador       = query->FieldByName("CodAlim")->AsString;
			sdi.CodSegmento          = query->FieldByName("CodSegmMT")->AsString;
			sdi.CodPontoAcoplamento1 = query->FieldByName("CodPonAcopl1")->AsString;
			sdi.CodPontoAcoplamento2 = query->FieldByName("CodPonAcopl2")->AsString;
			sdi.CodFases             = query->FieldByName("CodFas")->AsString;
			sdi.CodCondutor          = query->FieldByName("CodCond")->AsString;
			sdi.Comprimento_km       = query->FieldByName("Comp_km")->AsFloat;
			sdi.ClasseSegmento       = query->FieldByName("ClssSegm")->AsString;
//			sdi.Descricao            = query->FieldByName("Desc")->AsString;
			//insere SegmentoMT em PreMonta
			sucesso = pre_monta->InsereSegmentoMT(sdi);
			}
		QueryClose();
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro de leitura da tabela: " + tablename);
			sucesso = false;
			}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::ReadTrafoATATATMT(void)
	{
	//variáveis locais
	sdi_TRAFO_AT_MT sdi;
	int             nr;
	AnsiString      fieldname;
	AnsiString      tablename = "TrafoATATATMT";
	bool            sucesso   = true;

	try{//configura Log
		if (plog) plog->DefineValor("Tabela", tablename);
		//executa consulta da tabela TrafoATATATMT
		if (! QueryOpenReadOnly(tablename)) return(false);
		//loop p/ todos registros
		for (nr = 1, query->First(); (sucesso) && (! query->Eof); nr++, query->Next())
			{//configura Log
			if (plog) plog->DefineValor("Registro", IntToStr(nr));
			//lê campos e inicia estrutura
			sdi.CodBase          = query->FieldByName("CodBase")->AsInteger;
			sdi.CodSubestacao    = query->FieldByName("CodSub")->AsString;
			sdi.CodTrafo         = query->FieldByName("CodTrafo")->AsString;
			sdi.SeqBanco         = query->FieldByName("CodBnc")->AsInteger;
			sdi.TipoTrafo        = query->FieldByName("TipTrafo")->AsInteger;
			sdi.Vnom_pri_kv      = query->FieldByName("TenPrim_kV")->AsFloat;
			sdi.Vnom_sec_kv      = query->FieldByName("TenSecu_kV")->AsFloat;
			sdi.Vnom_ter_kv      = query->FieldByName("TenTerc_kV")->AsFloat;
			sdi.Snom_mva         = query->FieldByName("PotNom_MVA")->AsFloat;
			sdi.PerdaVazio_perc  = query->FieldByName("PerdVz_%")->AsFloat;
			sdi.PerdaTotal_perc  = query->FieldByName("PerdTtl_%")->AsFloat;
			for (int n = 0; n < 12; n++)
				{
				fieldname.sprintf("EnerMedid%02d_MWh", n+1);
				sdi.Energia_mwhmes[n] = query->FieldByName(fieldname)->AsFloat;
				}
			sdi.SegAlocacaoPerda = query->FieldByName("AlocPerdTrafo")->AsString;
			sdi.Propriedade      = query->FieldByName("Propr")->AsString;
//			sdi.Descricao        = query->FieldByName("Desc")->AsString;
			//insere SegmentoMT em PreMonta
			sucesso = pre_monta->InsereTrafoAT_MT(sdi);
			}
		QueryClose();
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro de leitura da tabela: " + tablename);
			sucesso = false;
			}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::ReadTrafoMTMTMTBT(void)
	{
	//variáveis locais
	sdi_TRAFO_MT_BT sdi;
	int             nr;
	AnsiString      fieldname;
	AnsiString      tablename = "TrafoMTMTMTBT";
	bool            sucesso   = true;

	try{//configura Log
		if (plog) plog->DefineValor("Tabela", tablename);
		//executa consulta da tabela TrafoMTMTMTBT
		if (! QueryOpenReadOnly(tablename)) return(false);
		//loop p/ todos registros
		for (nr = 1, query->First(); (sucesso) && (! query->Eof); nr++, query->Next())
			{//configura Log
			if (plog) plog->DefineValor("Registro", IntToStr(nr));
			//lê campos e inicia estrutura
			sdi.CodBase              = query->FieldByName("CodBase")->AsInteger;
			sdi.CodAlimentador       = query->FieldByName("CodAlim")->AsString;
			sdi.CodTrafo             = query->FieldByName("CodTrafo")->AsString;
			sdi.SeqBanco             = query->FieldByName("CodBnc")->AsInteger;
			sdi.CodPontoAcoplamento1 = query->FieldByName("CodPonAcopl1")->AsString;
			sdi.CodPontoAcoplamento2 = query->FieldByName("CodPonAcopl2")->AsString;
			sdi.Snom_kva             = query->FieldByName("PotNom_kVA")->AsFloat;
			sdi.MRT                  = query->FieldByName("MRT")->AsInteger;
			sdi.TipoTrafo            = query->FieldByName("TipTrafo")->AsInteger;
			sdi.CodFasesPri          = query->FieldByName("CodFasPrim")->AsString;
			sdi.CodFasesSec          = query->FieldByName("CodFasSecu")->AsString;
			sdi.CodFasesTer          = query->FieldByName("CodFasTerc")->AsString;
			sdi.Vnom_sec_kv          = query->FieldByName("TenSecu_kV")->AsFloat;
			sdi.Vtap_pu              = query->FieldByName("Tap_pu")->AsFloat;
			sdi.R_perc               = query->FieldByName("Resis_%")->AsFloat;
			sdi.Xhl_perc             = query->FieldByName("ReatHL_%")->AsFloat;
			sdi.Xht_perc             = query->FieldByName("ReatHT_%")->AsFloat;
			sdi.Xlt_perc             = query->FieldByName("ReatLT_%")->AsFloat;
			sdi.PerdaTotal_perc      = query->FieldByName("PerdTtl_%")->AsFloat;
			sdi.PerdaVazio_perc      = query->FieldByName("PerdVz_%")->AsFloat;
			sdi.ClasseTrafo          = query->FieldByName("ClssTrafo")->AsString;
			sdi.Propriedade          = query->FieldByName("Propr")->AsString;
//			sdi.Descricao            = query->FieldByName("Desc")->AsString;
			//insere Trafo MT em PreMonta
			sucesso = pre_monta->InsereTrafoMT_BT(sdi);
			}
		QueryClose();
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro de leitura da tabela: " + tablename);
			sucesso = false;
			}
	return(sucesso);
	}
/*
//---------------------------------------------------------------------------
bool __fastcall TBDaneel:: WriteBase(int cod_base, int cod_dist, int ano, TDateTime dat_bas)
	{
	//variáveis locais

	try{

		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}
*/

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::WriteCargaBT(sdi_CARGA_BT &sdi)
	{
	//variáveis locais
	bool       sucesso;
	AnsiString fieldname;
	AnsiString tablename = "CargaBT";
	TSqlInsert SqlInsert;

	try{//monta comando SQL para inserir registro na tabela
		SqlInsert.SetTableName(tablename);
		SqlInsert.AddField("CodBase"      , sdi.CodBase             );
		SqlInsert.AddField("CodTrafo"     , sdi.CodTrafo            );
		SqlInsert.AddField("CodRml"       , sdi.CodRamal            );
		SqlInsert.AddField("CodConsBT"    , sdi.CodConsumidorBT     );
		SqlInsert.AddField("CodFas"       , sdi.CodFases            );
		SqlInsert.AddField("CodPonAcopl"  , sdi.CodPontoAcoplamento );
		SqlInsert.AddField("SemRedAssoc"  , sdi.SemRedeAssociada    );
		SqlInsert.AddField("TipMedi"      , sdi.TipoMedicao         );
		SqlInsert.AddField("TipCrvaCarga" , sdi.TipologiaCurvaCarga );
		//vetor de energia
		for (int n = 0; n < 12; n++)
			{
			fieldname.sprintf("EnerMedid%02d_MWh", n+1);
			SqlInsert.AddField(fieldname, sdi.Energia_mwhmes[n]);
			}
		//executa comando SQL
		sucesso = QueryExecSql(SqlInsert.GetSqlCmd());
		}catch(Exception &e)
			{
			sucesso = false;
			}
	if (! sucesso)
		{
		if (plog) plog->Add("Erro de gravação da tabela: " + tablename);
		}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::WriteCargaMT(sdi_CARGA_MT &sdi)
	{
	//variáveis locais
	bool       sucesso;
	AnsiString fieldname;
	AnsiString tablename = "CargaMT";
	TSqlInsert SqlInsert;

	try{//monta comando SQL para inserir registro na tabela
		SqlInsert.SetTableName(tablename);
		SqlInsert.AddField("CodBase"      , sdi.CodBase             );
		SqlInsert.AddField("CodAlim"      , sdi.CodAlimentador      );
		SqlInsert.AddField("CodConsMT"    , sdi.CodConsumidorMT     );
		SqlInsert.AddField("CodFas"       , sdi.CodFases            );
		SqlInsert.AddField("CodPonAcopl"  , sdi.CodPontoAcoplamento );
		SqlInsert.AddField("SemRedAssoc"  , sdi.SemRedeAssociada    );
		SqlInsert.AddField("TipCrvaCarga" , sdi.TipologiaCurvaCarga );
		//vetor de energia
		for (int n = 0; n < 12; n++)
			{
			fieldname.sprintf("EnerMedid%02d_MWh", n+1);
			SqlInsert.AddField(fieldname,  sdi.Energia_mwhmes[n]);
			}
//		SqlInsert.AddField("Desc", sdi.Descricao);
		//executa comando SQL
		sucesso = QueryExecSql(SqlInsert.GetSqlCmd());
		}catch(Exception &e)
			{
			sucesso = false;
			}
	if (! sucesso)
		{
		if (plog) plog->Add("Erro de gravação da tabela: " + tablename);
		}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::WriteChaveBT(sdi_CHAVE_BT &sdi)
	{
	//variáveis locais
	bool       sucesso;
	AnsiString fieldname;
	AnsiString tablename = "ChaveBT";
	TSqlInsert SqlInsert;

	try{//monta comando SQL para inserir registro na tabela
		SqlInsert.SetTableName(tablename);
		SqlInsert.AddField("CodBase"      , sdi.CodBase              );
		SqlInsert.AddField("CodTrafo"     , sdi.CodTrafo             );
		SqlInsert.AddField("CodChv"       , sdi.CodChave             );
		SqlInsert.AddField("CodPonAcopl1" , sdi.CodPontoAcoplamento1 );
		SqlInsert.AddField("CodPonAcopl2" , sdi.CodPontoAcoplamento2 );
		SqlInsert.AddField("CodFas"       , sdi.CodFases             );
		SqlInsert.AddField("EstChv"       , sdi.Estado               );
//		SqlInsert.AddField("Desc"         , sdi.Descricao            );
		//executa comando SQL
		sucesso = QueryExecSql(SqlInsert.GetSqlCmd());
		}catch(Exception &e)
			{
			sucesso = false;
			}
	if (! sucesso)
		{
		if (plog) plog->Add("Erro de gravação da tabela: " + tablename);
		}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::WriteChaveMT(sdi_CHAVE_MT &sdi)
	{
	//variáveis locais
	bool       sucesso;
	AnsiString fieldname;
	AnsiString tablename = "ChaveMT";
	TSqlInsert SqlInsert;

	try{//monta comando SQL para inserir registro na tabela
		SqlInsert.SetTableName(tablename);
		SqlInsert.AddField("CodBase"      , sdi.CodBase              );
		SqlInsert.AddField("CodAlim"      , sdi.CodAlimentador       );
		SqlInsert.AddField("CodChv"       , sdi.CodChave             );
		SqlInsert.AddField("CodPonAcopl1" , sdi.CodPontoAcoplamento1 );
		SqlInsert.AddField("CodPonAcopl2" , sdi.CodPontoAcoplamento2 );
		SqlInsert.AddField("CodFas"       , sdi.CodFases             );
		SqlInsert.AddField("EstChv"       , sdi.Estado               );
//		SqlInsert.AddField("Desc"         , sdi.Descricao            );
		//executa comando SQL
		sucesso = QueryExecSql(SqlInsert.GetSqlCmd());
		}catch(Exception &e)
			{
			sucesso = false;
			}
	if (! sucesso)
		{
		if (plog) plog->Add("Erro de gravação da tabela: " + tablename);
		}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::WriteCircMT(sdi_CIRCUITO &sdi)
	{
	//variáveis locais
	bool       sucesso;
	AnsiString fieldname;
	AnsiString tablename = "CircMT";
	TSqlInsert SqlInsert;

	try{//monta comando SQL para inserir registro na tabela
		SqlInsert.SetTableName(tablename);
		SqlInsert.AddField("CodBase"     , sdi.CodBase             );
		SqlInsert.AddField("CodSub"      , sdi.CodSubestacao       );
		SqlInsert.AddField("CodAlim"     , sdi.CodAlimentador      );
		SqlInsert.AddField("TenNom_kV"   , sdi.Vnom_kv             );
		SqlInsert.AddField("TenOpe_pu"   , sdi.Vope_pu             );
		SqlInsert.AddField("CodPonAcopl" , sdi.CodPontoAcoplamento );
		SqlInsert.AddField("CircAtip"    , sdi.CircAtipico         );
		for (int n = 0; n < 12; n++)
			{
			fieldname.sprintf("EnerCirc%02d_MWh", n+1);
			SqlInsert.AddField(fieldname  , sdi.Energia_mwhmes[n]        );
			}
		for (int n = 0; n < 12; n++)
			{
			fieldname.sprintf("PerdCirc%02d_MWh", n+1);
			SqlInsert.AddField(fieldname  , sdi.Perda_mwhmes[n]        );
			}
		//executa comando SQL
		sucesso = QueryExecSql(SqlInsert.GetSqlCmd());
		}catch(Exception &e)
			{
			sucesso = false;
			}
	if (! sucesso)
		{
		if (plog) plog->Add("Erro de gravação da tabela: " + tablename);
		}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::WriteCodCondutor(sdi_CONDUTOR &sdi)
	{
	//variáveis locais
	bool       sucesso;
	AnsiString fieldname;
	AnsiString tablename = "CodCondutor";
	TSqlInsert SqlInsert;

	try{//monta comando SQL para inserir registro na tabela
		SqlInsert.SetTableName(tablename);
		SqlInsert.AddField("CodBase"       , sdi.CodBase     );
		SqlInsert.AddField("CodCond"       , sdi.CodCondutor );
		SqlInsert.AddField("Resis_ohms_km" , sdi.Rohm_km     );
		SqlInsert.AddField("Reat_ohms_km"  , sdi.Xohm_km     );
		SqlInsert.AddField("CorrMax_A"     , sdi.Imax_amp    );
//		SqlInsert.AddField("Desc"          , sdi.Descricao   );
		//executa comando SQL
		sucesso = QueryExecSql(SqlInsert.GetSqlCmd());
		}catch(Exception &e)
			{
			sucesso = false;
			}
	if (! sucesso)
		{
		if (plog) plog->Add("Erro de gravação da tabela: " + tablename);
		}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::WriteCrvCrgBT(sdi_CURVA &sdi)
	{
	//variáveis locais
	bool       sucesso;
	AnsiString fieldname;
	AnsiString tablename = "CrvCrgBT";
	TSqlInsert SqlInsert;

	try{//monta comando SQL para inserir registro na tabela
		SqlInsert.SetTableName(tablename);
		SqlInsert.AddField("CodBase"   , sdi.CodBase  );
		SqlInsert.AddField("CodCrvCrg" , sdi.CodCurva );
		SqlInsert.AddField("TipoDia"   , sdi.TipoDia  );
		for (int n = 0; n < 96; n++)
			{
			fieldname.sprintf("PotAtv%02d_kW", n+1);
			SqlInsert.AddField(fieldname , sdi.P_kw[n]  );
			}
		//executa comando SQL
		sucesso = QueryExecSql(SqlInsert.GetSqlCmd());
		}catch(Exception &e)
			{
			sucesso = false;
			}
	if (! sucesso)
		{
		if (plog) plog->Add("Erro de gravação da tabela: " + tablename);
		}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel:: WriteCrvCrgMT(sdi_CURVA &sdi)
	{
	//variáveis locais
	bool       sucesso;
	AnsiString fieldname;
	AnsiString tablename = "CrvCrgMT";
	TSqlInsert SqlInsert;

	try{//monta comando SQL para inserir registro na tabela
		SqlInsert.SetTableName(tablename);
		SqlInsert.AddField("CodBase"   , sdi.CodBase  );
		SqlInsert.AddField("CodCrvCrg" , sdi.CodCurva );
		SqlInsert.AddField("TipoDia"   , sdi.TipoDia  );
		for (int n = 0; n < 96; n++)
			{
			fieldname.sprintf("PotAtv%02d_kW", n+1);
			SqlInsert.AddField(fieldname , sdi.P_kw[n]  );
			}
		//executa comando SQL
		sucesso = QueryExecSql(SqlInsert.GetSqlCmd());
		}catch(Exception &e)
			{
			sucesso = false;
			}
	if (! sucesso)
		{
		if (plog) plog->Add("Erro de gravação da tabela: " + tablename);
		}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::WriteGeradorBT(sdi_GERADOR_BT &sdi)
	{
	//variáveis locais
	bool       sucesso;
	AnsiString fieldname;
	AnsiString tablename = "GeradorBT";
	TSqlInsert SqlInsert;

	try{//monta comando SQL para inserir registro na tabela
		SqlInsert.SetTableName(tablename);
		SqlInsert.AddField("CodBase"     , sdi.CodBase             );
		SqlInsert.AddField("CodRml"      , sdi.CodRamal            );
		SqlInsert.AddField("CodGeraBT"   , sdi.CodGerador          );
		SqlInsert.AddField("CodPonAcopl" , sdi.CodPontoAcoplamento );
		SqlInsert.AddField("CodFas"      , sdi.CodFases            );
		for (int n = 0; n < 12; n++)
			{
			fieldname.sprintf("EnerMedid%02d_MWh", n+1);
			SqlInsert.AddField(fieldname , sdi.Energia_mwhmes[n] );
			}
//		SqlInsert.AddField("Desc"  , sdi.Descricao  );
		//executa comando SQL
		sucesso = QueryExecSql(SqlInsert.GetSqlCmd());
		}catch(Exception &e)
			{
			sucesso = false;
			}
	if (! sucesso)
		{
		if (plog) plog->Add("Erro de gravação da tabela: " + tablename);
		}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::WriteGeradorMT(sdi_GERADOR_MT &sdi)
	{
	//variáveis locais
	bool       sucesso;
	AnsiString fieldname;
	AnsiString tablename = "GeradorMT";
	TSqlInsert SqlInsert;

	try{//monta comando SQL para inserir registro na tabela
		SqlInsert.SetTableName(tablename);
		SqlInsert.AddField("CodBase"     , sdi.CodBase             );
		SqlInsert.AddField("CodAlim"     , sdi.CodAlimentador      );
		SqlInsert.AddField("CodGeraMT"   , sdi.CodGerador          );
		SqlInsert.AddField("CodPonAcopl" , sdi.CodPontoAcoplamento );
		SqlInsert.AddField("CodFas"      , sdi.CodFases            );
		for (int n = 0; n < 12; n++)
			{
			fieldname.sprintf("EnerMedid%02d_MWh", n+1);
			SqlInsert.AddField(fieldname , sdi.Energia_mwhmes[n] );
			}
//		SqlInsert.AddField("Desc"  , sdi.Descricao  );
		//executa comando SQL
		sucesso = QueryExecSql(SqlInsert.GetSqlCmd());
		}catch(Exception &e)
			{
			sucesso = false;
			}
	if (! sucesso)
		{
		if (plog) plog->Add("Erro de gravação da tabela: " + tablename);
		}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::WriteRamal(sdi_RAMAL &sdi)
	{
	//variáveis locais
	bool       sucesso;
	AnsiString fieldname;
	AnsiString tablename = "Ramal";
	TSqlInsert SqlInsert;

	try{//monta comando SQL para inserir registro na tabela
		SqlInsert.SetTableName(tablename);
		SqlInsert.AddField("CodBase"      , sdi.CodBase              );
		SqlInsert.AddField("CodTrafo"     , sdi.CodTrafo             );
		SqlInsert.AddField("CodRml"       , sdi.CodRamal             );
		SqlInsert.AddField("CodPonAcopl1" , sdi.CodPontoAcoplamento1 );
		SqlInsert.AddField("CodPonAcopl2" , sdi.CodPontoAcoplamento2 );
		SqlInsert.AddField("CodFas"       , sdi.CodFases             );
		SqlInsert.AddField("CodCond"      , sdi.CodCondutor          );
		SqlInsert.AddField("Comp_km"      , sdi.Comprimento_km       );
//		SqlInsert.AddField("Desc"         , sdi.Descricao            );
		//executa comando SQL
		sucesso = QueryExecSql(SqlInsert.GetSqlCmd());
		}catch(Exception &e)
			{
			sucesso = false;
			}
	if (! sucesso)
		{
		if (plog) plog->Add("Erro de gravação da tabela: " + tablename);
		}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::WriteRegulador(sdi_REGULADOR &sdi)
	{
	//variáveis locais
	bool       sucesso;
	AnsiString fieldname;
	AnsiString tablename = "Regulador";
	TSqlInsert SqlInsert;

	try{//monta comando SQL para inserir registro na tabela
		SqlInsert.SetTableName(tablename);
		SqlInsert.AddField("CodBase"       , sdi.CodBase              );
		SqlInsert.AddField("CodAlim"       , sdi.CodAlimentador       );
		SqlInsert.AddField("CodRegul"      , sdi.CodRegulador         );
		SqlInsert.AddField("CodBnc"        , sdi.SeqBanco             );
		SqlInsert.AddField("CodPonAcopl1"  , sdi.CodPontoAcoplamento1 );
		SqlInsert.AddField("CodPonAcopl2"  , sdi.CodPontoAcoplamento2 );
		SqlInsert.AddField("PotNom_kVA"    , sdi.Snom_kva             );
		SqlInsert.AddField("TipRegul"      , sdi.TipoRegulador        );
		SqlInsert.AddField("TenRgl_pu"     , sdi.Vreg_pu              );
		SqlInsert.AddField("CodFasPrim"    , sdi.CodFasesPri          );
		SqlInsert.AddField("CodFasSecu"    , sdi.CodFasesSec          );
		SqlInsert.AddField("Resis_%"       , sdi.R_perc               );
		SqlInsert.AddField("ReatHL_%"      , sdi.Xhl_perc             );
		SqlInsert.AddField("PerdTtl_W"     , sdi.PerdaTotal_w         );
		SqlInsert.AddField("PerdVz_W"      , sdi.PerdaVazio_w         );
		SqlInsert.AddField("EnerMedid_MWh" , sdi.Energia_mwhano       );
//		SqlInsert.AddField("Desc"          , sdi.Descricao            );
		//executa comando SQL
		sucesso = QueryExecSql(SqlInsert.GetSqlCmd());
		}catch(Exception &e)
			{
			sucesso = false;
			}
	if (! sucesso)
		{
		if (plog) plog->Add("Erro de gravação da tabela: " + tablename);
		}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::WriteSegmentoBT(sdi_SEGMENTO_BT &sdi)
	{
	//variáveis locais
	bool       sucesso;
	AnsiString fieldname;
	AnsiString tablename = "SegmentoBT";
	TSqlInsert SqlInsert;

	try{//monta comando SQL para inserir registro na tabela
		SqlInsert.SetTableName(tablename);
		SqlInsert.AddField("CodBase"      , sdi.CodBase              );
		SqlInsert.AddField("CodTrafo"     , sdi.CodTrafo             );
		SqlInsert.AddField("CodSegmBT"    , sdi.CodSegmento          );
		SqlInsert.AddField("CodPonAcopl1" , sdi.CodPontoAcoplamento1 );
		SqlInsert.AddField("CodPonAcopl2" , sdi.CodPontoAcoplamento2 );
		SqlInsert.AddField("CodFas"       , sdi.CodFases             );
		SqlInsert.AddField("CodCond"      , sdi.CodCondutor          );
		SqlInsert.AddField("Comp_km"      , sdi.Comprimento_km       );
//		SqlInsert.AddField("Desc"         , sdi.Descricao            );
		//executa comando SQL
		sucesso = QueryExecSql(SqlInsert.GetSqlCmd());
		}catch(Exception &e)
			{
			sucesso = false;
			}
	if (! sucesso)
		{
		if (plog) plog->Add("Erro de gravação da tabela: " + tablename);
		}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::WriteSegmentoMT(sdi_SEGMENTO_MT &sdi)
	{
	//variáveis locais
	bool       sucesso;
	AnsiString fieldname;
	AnsiString tablename = "SegmentoMT";
	TSqlInsert SqlInsert;

	try{//monta comando SQL para inserir registro na tabela
		SqlInsert.SetTableName(tablename);
		SqlInsert.AddField("CodBase"      , sdi.CodBase              );
		SqlInsert.AddField("CodAlim"      , sdi.CodAlimentador       );
		SqlInsert.AddField("CodSegmMT"    , sdi.CodSegmento          );
		SqlInsert.AddField("CodPonAcopl1" , sdi.CodPontoAcoplamento1 );
		SqlInsert.AddField("CodPonAcopl2" , sdi.CodPontoAcoplamento2 );
		SqlInsert.AddField("CodFas"       , sdi.CodFases             );
		SqlInsert.AddField("CodCond"      , sdi.CodCondutor          );
		SqlInsert.AddField("Comp_km"      , sdi.Comprimento_km       );
		SqlInsert.AddField("ClssSegm"     , sdi.ClasseSegmento       );
//		SqlInsert.AddField("Desc"         , sdi.Descricao            );
		//executa comando SQL
		sucesso = QueryExecSql(SqlInsert.GetSqlCmd());
		}catch(Exception &e)
			{
			sucesso = false;
			}
	if (! sucesso)
		{
		if (plog) plog->Add("Erro de gravação da tabela: " + tablename);
		}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::WriteTrafoATATATMT(sdi_TRAFO_AT_MT &sdi)
	{
	//variáveis locais
	bool       sucesso;
	AnsiString fieldname;
	AnsiString tablename = "TrafoATATATMT";
	TSqlInsert SqlInsert;

	try{//monta comando SQL para inserir registro na tabela
		SqlInsert.SetTableName(tablename);
		SqlInsert.AddField("CodBase"    , sdi.CodBase         );
		SqlInsert.AddField("CodSub"     , sdi.CodSubestacao   );
		SqlInsert.AddField("CodTrafo"   , sdi.CodTrafo        );
		SqlInsert.AddField("CodBnc"     , sdi.SeqBanco        );
		SqlInsert.AddField("TipTrafo"   , sdi.TipoTrafo       );
		SqlInsert.AddField("TenPrim_kV" , sdi.Vnom_pri_kv     );
		SqlInsert.AddField("TenSecu_kV" , sdi.Vnom_sec_kv     );
		SqlInsert.AddField("TenTerc_kV" , sdi.Vnom_ter_kv     );
		SqlInsert.AddField("PotNom_MVA" , sdi.Snom_mva        );
		SqlInsert.AddField("PerdVz_%"   , sdi.PerdaVazio_perc );
		SqlInsert.AddField("PerdTtl_%"   ,sdi.PerdaTotal_perc );
			for (int n = 0; n < 12; n++)
				{
				fieldname.sprintf("EnerMedid%02d_MWh", n+1);
				SqlInsert.AddField(fieldname , sdi.Energia_mwhmes[n] );
				}
		SqlInsert.AddField("AlocPerdTrafo" , sdi.SegAlocacaoPerda );
		SqlInsert.AddField("Propr"         , sdi.Propriedade      );
//		SqlInsert.AddField("Desc"          , sdi.Descricao        );
		//executa comando SQL
		sucesso = QueryExecSql(SqlInsert.GetSqlCmd());
		}catch(Exception &e)
			{
			sucesso = false;
			}
	if (! sucesso)
		{
		if (plog) plog->Add("Erro de gravação da tabela: " + tablename);
		}
	return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDaneel::WriteTrafoMTMTMTBT(sdi_TRAFO_MT_BT &sdi)
	{
	//variáveis locais
	bool       sucesso;
	AnsiString fieldname;
	AnsiString tablename = "TrafoMTMTMTBT";
	TSqlInsert SqlInsert;

	try{//monta comando SQL para inserir registro na tabela
		SqlInsert.SetTableName(tablename);
		SqlInsert.AddField("CodBase"      , sdi.CodBase              );
		SqlInsert.AddField("CodAlim"      , sdi.CodAlimentador       );
		SqlInsert.AddField("CodTrafo"     , sdi.CodTrafo             );
		SqlInsert.AddField("CodBnc"       , sdi.SeqBanco             );
		SqlInsert.AddField("CodPonAcopl1" , sdi.CodPontoAcoplamento1 );
		SqlInsert.AddField("CodPonAcopl2" , sdi.CodPontoAcoplamento2 );
		SqlInsert.AddField("PotNom_kVA"   , sdi.Snom_kva             );
		SqlInsert.AddField("MRT"          , sdi.MRT                  );
		SqlInsert.AddField("TipTrafo"     , sdi.TipoTrafo            );
		SqlInsert.AddField("CodFasPrim"   , sdi.CodFasesPri          );
		SqlInsert.AddField("CodFasSecu"   , sdi.CodFasesSec          );
		SqlInsert.AddField("CodFasTerc"   , sdi.CodFasesTer          );
		SqlInsert.AddField("TenSecu_kV"   , sdi.Vnom_sec_kv          );
		SqlInsert.AddField("Tap_pu"       , sdi.Vtap_pu              );
		SqlInsert.AddField("Resis_%"      , sdi.R_perc               );
		SqlInsert.AddField("ReatHL_%"     , sdi.Xhl_perc             );
		SqlInsert.AddField("ReatHT_%"     , sdi.Xht_perc             );
		SqlInsert.AddField("ReatLT_%"     , sdi.Xlt_perc             );
		SqlInsert.AddField("PerdTtl_%"    , sdi.PerdaTotal_perc      );
		SqlInsert.AddField("PerdVz_%"     , sdi.PerdaVazio_perc      );
		SqlInsert.AddField("ClssTrafo"    , sdi.ClasseTrafo          );
		SqlInsert.AddField("Propr"        , sdi.Propriedade          );
//		SqlInsert.AddField("Desc"         , sdi.Descricao            );
		//executa comando SQL
		sucesso = QueryExecSql(SqlInsert.GetSqlCmd());
		}catch(Exception &e)
			{
			sucesso = false;
			}
	if (! sucesso)
		{
		if (plog) plog->Add("Erro de gravação da tabela: " + tablename);
		}
	return(sucesso);
	}

//---------------------------------------------------------------------------
//eof

