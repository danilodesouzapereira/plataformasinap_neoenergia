//-----------------------------------------------------------------------------
#ifndef VTExtratorH
#define VTExtratorH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
struct strALTERNATIVA         {
                              AnsiString   descricao;
							  bool         rede_original;
							  AnsiString   justificativa;
                              };
struct strDEMANDA_POTENCIA    {
                              int    ano;
                              double s_mva;
                              };
struct strEQUIPAMENTO         {
							  int padrao_construtivo_id;
							  /*
							  struct{
									AnsiString codigo;
									AnsiString tipo_sistema;
									bool antes_manobra;
									}rede;
							  */
                              AnsiString tipo_acao;
                              AnsiString descricao;
                              double     quantidade;
                              };
struct strESTUDO              {
                              int          dominio_id;
                              int          grupo_id;
                              AnsiString   usuario;
                              AnsiString   descricao;
							  int          num_alternativa;
							  AnsiString   justificativa;
                              };
struct strINFORMACAO_TECNICA  {
                              AnsiString tipo;
                              };
struct strOBRA                {
                              AnsiString    codigo;
                              AnsiString    tipo_obra;
							  AnsiString    descricao;
							  double        obrigacao_especial;
							  double        participacao_otr;
							  TDateTime     data_inicio;
							  TDateTime     data_ativacao;
							  TByteDynArray croqui;
							  AnsiString    programa;
							  int		    grupo;
                              int           conjunto_obra;
                              };
struct strSOLICITACAO         {
                              int         id;
                              AnsiString  usuario;
                              AnsiString  descricao;
                              AnsiString  subestacao;
                              AnsiString  alimentador;
                              TDateTime   data;
                              AnsiString  responsavel;
							  AnsiString  cod_ref;
                              AnsiString  cod_doc;
                              };
struct strPOTENCIA_INSTALADA  {
										int    ano;
										double s_mva;
										};
struct strELEMENTO_POTENCIA   { //DVK 2015.04.28
										int    		id;
										AnsiString  codigo;
										};
struct strREDE_ALTERNATIVA    {
							  AnsiString codigoGIS;
							  AnsiString descricao;
							  AnsiString tipoSistema;
							  double     taxa_crescimento_anual;
							  };
struct strVALOR_INFORMACAO_TECNICA  {
                                    int    ano;
                                    double valor;
									};
struct strREDE                {
							  AnsiString codigo;
							  AnsiString tipo_sistema;
							  AnsiString codigo_rede_pai;
							  bool antes_manobra;
							  };
struct strMUNICIPIO           {
							  int idMunicipio;
							  bool principal;
							  };
//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTExtrator : public TObject
   {
   public:
                   __fastcall  VTExtrator(void) {};
      virtual      __fastcall ~VTExtrator(void) {};
      virtual bool __fastcall  InsereAlternativa(TStrings *campos) = 0;
	  virtual bool __fastcall  InsereDemandaPotencia(TStrings *campos) = 0;
	  virtual bool __fastcall  InsereElementoPotencia(TStrings *campos) = 0;  //DVK 2015.04.28
      virtual bool __fastcall  InsereEquipamento(TStrings *campos) = 0;
      virtual bool __fastcall  InsereEstudo(TStrings *campos) = 0;
	  virtual bool __fastcall  InsereInformacaoTecnica(TStrings *campos) = 0;
	  virtual bool __fastcall  InsereMunicipio(TStrings *campos) = 0;
      virtual bool __fastcall  InsereObra(TStrings *campos) = 0;
	  virtual bool __fastcall  InserePotenciaInstalada(TStrings *campos) = 0;
	  virtual bool __fastcall  InsereRede(TStrings *campos) = 0;
      virtual bool __fastcall  InsereRedeAlternativa(TStrings *campos) = 0;
      virtual bool __fastcall  InsereSolicitacao(TStrings *campos) = 0;
      virtual bool __fastcall  InsereValorInformacaoTecnica(TStrings *campos) = 0;
      virtual bool __fastcall  TrataInicio(void) = 0;
      virtual bool __fastcall  TrataTermino(void) = 0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTMonta
//-----------------------------------------------------------------------------
VTExtrator* NewObjExtrator(VTApl *apl_owner);

#endif
//-----------------------------------------------------------------------------
// eof
