// ---------------------------------------------------------------------------
#ifndef TBDadoParamH
#define TBDadoParamH
#include <Classes.hpp>
#include "VTBDadoParam.h"
#include <stdio.h>
#include <Data.DB.hpp>

// ---------------------------------------------------------------------------
class strDadoChaveIF;
class strDadoChaveTelecom;
class strDadoParamBloco;
class strDadoParamCarga;
class strDadoParamEqpto;
class strDadoParamRede;
class strDadoTipoEqProt;
class VTVerifCjto;
class VTVerifSe;
class VTVerifCircuito;

// ---------------------------------------------------------------------------
class TBDadoParam : public VTBDadoParam
{
public:
	__fastcall TBDadoParam(VTApl *apl_owner);
	__fastcall ~TBDadoParam(void);
	bool __fastcall Aberto(void);
	void __fastcall DefineCaminho(AnsiString caminho);
	void __fastcall DefineDirBase(AnsiString dir);
	AnsiString __fastcall DirBase(void);
	bool __fastcall GravaDado(VTParamConf *paramConf); // Grava as informações de parConf em mdb
	bool __fastcall ImportaDadoChaveIF(TStringList *lisGrupoSensores, AnsiString arq); // Função para importação dos dados de IFs existentes na rede, a partir de txt
	bool __fastcall ImportaDadoParamEqpto(TList *lisEXT);
	bool __fastcall ImportaDadoTipoEqProt(TList *lisEXT);
	bool __fastcall LeDado(VTParamConf *paramConf, TStringList *lisCodRede);
	VTParamConf* __fastcall ParamConf(void);
	bool __fastcall GravaDadoIndisponibilidade(TList *lisEqptoIndisp); //hk201808: indisp
   bool __fastcall GravaDadoIndisponibilidadePadrao(double vindisp);//hk201808: indisp

    void                    __fastcall SetParamConf(VTParamConf *paramConf);

private:
	bool __fastcall Abre(void);
	bool __fastcall AtualizaDadoParamGerais(sdi_ParamGerais &sdi);
   //gh201806: cria estrutura de dados associada a chave IF
	strDadoChaveIF* __fastcall CriaDadoChaveIF(sdi_ChaveIF &sdi, TList *lisEXT);
	strDadoChaveTelecom* __fastcall CriaDadoChaveTelecom(sdi_ChaveTelecom &sdi, TList *lisEXT);
	strDadoParamBloco* __fastcall CriaDadoParamBloco(sdi_ParamBloco &sdi, TList *lisEXT);
	strDadoParamCarga* __fastcall CriaDadoParamCarga(sdi_ParamCarga &sdi, TList *lisEXT);
	strDadoParamEqpto* __fastcall CriaDadoParamEqpto(sdi_ParamEqpto &sdi, TList *lisEXT);
	strDadoParamRede* __fastcall CriaDadoParamRede(sdi_ParamRede &sdi, TList *lisEXT);
	strDadoTipoEqProt* __fastcall CriaDadoTipoEqProt(sdi_TipoEqProt &sdi, TList *lisEXT);
	VTVerifCircuito* __fastcall CriaDadoVerifCircuito(sdi_VerifCircuito &sdi, TList *lisEXT);
	VTVerifCjto* __fastcall CriaDadoVerifCjto(sdi_VerifCjto &sdi, TList *lisEXT);
	VTVerifSe* __fastcall CriaDadoVerifSe(sdi_VerifSe &sdi, TList *lisEXT);
	TADOConnection* __fastcall CriaObjetoConexao(AnsiString conStr);
	TADOQuery* __fastcall CriaObjetoQuery(TCursorType tipo);
	bool __fastcall CriaTabelaChavesTelecom(void);
	//gh201806: cria tabela para dados de chaves com IF
	bool __fastcall CriaTabelaChavesIF(void);
	bool __fastcall CriaTabelaParamBloco(void);
	bool __fastcall CriaTabelaParamCarga(void);
	bool __fastcall CriaTabelaParamEqpto(void);
	bool __fastcall CriaTabelaParamGerais(void);
	bool __fastcall CriaTabelaParamRede(void);
	bool __fastcall CriaTabelaTipoEqProt(void);
	bool __fastcall CriaTabelaVerifCircuito(void);
	bool __fastcall CriaTabelaVerifCjto(void);
	bool __fastcall CriaTabelaVerifCjtoSe(void);
	bool __fastcall CriaTabelaVerifSe(void);
	bool __fastcall DeletaTabela(AnsiString tabela);
	// bool                 __fastcall DeletaTabelas(void);
	void __fastcall EncerraQuery(TADOQuery *qry);
	TADOQuery* __fastcall ExecutaQuery(AnsiString comSql, bool escrita = false);
	// VTRede*              __fastcall ExisteRede(AnsiString codigo);
	bool __fastcall ExisteCampoTabela(AnsiString tabela, AnsiString campo);
	// hkv201610 Nova funcao: ExisteCampoTabela(AnsiString tabela, AnsiString campo)
	bool __fastcall ExisteTabela(AnsiString tabela);
	VTVerifSe* __fastcall ExisteVerifSe(AnsiString codigo);
	bool __fastcall Fecha(void);
	bool __fastcall GravaDadoChavesTelecom(void);
	bool __fastcall GravaDadoChavesTelecom(sdi_ChaveTelecom &sdi);
	//gh201806: gravação de dados de chaves com IF
	bool __fastcall GravaDadoChaveIF(void);
	bool __fastcall GravaDadoChaveIF(sdi_ChaveTelecom &sdi);
public:
	bool __fastcall GravaDadoParamBloco(void);
	bool __fastcall GravaDadoParamBloco(sdi_ParamBloco &sdi);
	bool __fastcall GravaDadoParamCarga(void);
	bool __fastcall GravaDadoParamCarga(sdi_ParamCarga &sdi);
	bool __fastcall GravaDadoParamEqpto(TList *lisParamEqpto);
	bool __fastcall GravaDadoParamEqpto(sdi_ParamEqpto &sdi, bool flagCamposIF);
	bool __fastcall GravaDadoParamGerais(void);
	bool __fastcall GravaDadoParamGerais(sdi_ParamGerais &sdi, bool flagTprepIF);
	bool __fastcall GravaDadoParamRede(void);
	bool __fastcall GravaDadoParamRede(sdi_ParamRede &sdi);
private:
	bool __fastcall GravaDadoTipoEqProt(void);
	bool __fastcall GravaDadoTipoEqProt(sdi_TipoEqProt &sdi);
	bool __fastcall GravaDadoVerifCircuito(void);
	bool __fastcall GravaDadoVerifCircuito(sdi_VerifCircuito &sdi);
public:    
	bool __fastcall GravaDadoVerifCjto(void);
	bool __fastcall GravaDadoVerifCjto(sdi_VerifCjto &sdi);
	bool __fastcall GravaDadoVerifCjtoSe(void);
	bool __fastcall GravaDadoVerifCjtoSe(sdi_VerifCjtoSe &sdi);
	bool __fastcall GravaDadoVerifSe(void);
	bool __fastcall GravaDadoVerifSe(sdi_VerifSe &sdi);
private:    
	// bool               	__fastcall ImportaDadoParamBloco(void);
	// bool              	__fastcall ImportaDadoParamCarga(void);
	// bool               	__fastcall ImportaDadoParamEqpto(TList *lisParamEqpto);
	// bool               	__fastcall ImportaDadoParamGerais(void);
	// bool               	__fastcall ImportaDadoParamRede(void);
	// bool               	__fastcall ImportaDadoVerifCjto(void);
   bool __fastcall InsereCampoTabela(AnsiString tabela, AnsiString campo);//hk201802a
   //gh201806: leitura de dados do paramConf.mdb com dados de chaves com IF
	bool __fastcall LeDadoChaveIF(void);
	bool __fastcall LeDadoChaveTelecom(void);
	//gh201806: função para leitura das linhas de um arquivo txt
	AnsiString __fastcall LeDadoTexto(FILE* fp, bool &flagFIMLINHA, bool &flagEOF);
   bool __fastcall LeDadoParamIndisp(void);//hk201808: indisp
	bool __fastcall LeDadoParamBloco(void);
	bool __fastcall LeDadoParamCarga(void);
	bool __fastcall LeDadoParamEqpto(TStringList *lisCodRede);
	bool __fastcall LeDadoParamEqptoLimitado(TStringList *slisConsulta, TList *lisParamEqpto);
	bool __fastcall LeDadoParamGerais(void);
	bool __fastcall LeDadoParamRede(void);
	bool __fastcall LeDadoTipoEqProt(void);
	bool __fastcall LeDadoVerifCircuito(void);
	bool __fastcall LeDadoVerifCjto(void);
	bool __fastcall LeDadoVerifCjtoSe(VTVerifCjto *verifCjto);
	bool __fastcall LeDadoVerifSe(void);
	void __fastcall LimpaRegistrosParamEqpto(TList *lisParamEqpto);
	void __fastcall LimpaRegistrosVerifCjto(TList *lisVerifCjto);
	void __fastcall LimpaRegistrosVerifCjtoSe(TList *lisVerifCjto);
	void __fastcall LimpaRegistrosVerifSe(TList *lisVerifSe);
	bool __fastcall ValidaDirBase(void);
	void __fastcall DeletaTabParamCargaRedes(void);
	// hkv201610 código da rede MT associada a carga //hkv201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede
	void __fastcall DeletaTabParamBlocoRedes(void);
	// hkv201610 código da rede MT associada a carga //hkv201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede

protected: // métodos acessados via property

		private : // dados locais

	VTApl *apl;
	AnsiString dbPath; // caminho da base de dados
	AnsiString dirBase; // diretório da base de dados
	bool aberto;
	// objetos de conexão
	TADOConnection *conn;
	TADOQuery *qryTMP;
	TADOCommand *dcom;

private: // dados externos
	// objeto que armazena informações dos parâmetros
	VTParamConf *paramConf;

};
// ---------------------------------------------------------------------------
#endif
