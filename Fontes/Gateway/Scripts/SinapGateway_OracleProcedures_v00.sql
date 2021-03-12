/******************************************************************************
   NAME:       PCK_SNPGATEWAY
   PURPOSE:    Procedimentos, funções e tipos de dados utilizados no sistema
               Gateway.

   Data   | Autor | Versão | Descrição
 ==========================================================================
 20150506 |  MML  |  0.00  | Versão inicial.
******************************************************************************/

-- ALTER TABLE CurvaSegmento MODIFY LOB (blbPontos) (CACHE);

/******************************************************************************
  Observação:
    Para testes de retorno (sql developer)

    VARIABLE EstudoId NUMBER;
    VARIABLE RedeId NUMBER;
    exec :EstudoId := 0;
    exec :RedeId := 2;
    VARIABLE resultSet REFCURSOR
    EXEC PCK_SNPGATEWAY.SPSNP_rdCarga(:EstudoId, :RedeId, :resultSet);
    PRINT :resultSet

    VARIABLE idaux NUMBER;
    exec :idaux := -1;
    EXEC PCK_SNPGATEWAY.SPSNP_wrRede ( 0, :idaux, 1, 1, 1, 'xxx', 'sss', -1, 1, 1, 1, CURRENT_TIMESTAMP, 2, 'Eu')
    PRINT :idaux;

    EXEMPLO DEBUG:

    CORPO da procedure:
    ---------------------
    ...
    -- Variáveis locais
    ...
    conta          NUMBER;
    ...
    SELECT COUNT(*) INTO conta FROM BarraRede WHERE intRedeId = p_RedeId;
    DBMS_OUTPUT.PUT_LINE('Deletando rede '||p_RedeId||', Registros BarraRede = '||conta );

    CHAMADA da procedure:
    ---------------------
    SET SERVEROUTPUT ON
    EXEC PCK_SNPGATEWAY.SPSNP_dlDominio(0, 2)

******************************************************************************/

/******************************************************************************
  INTERFACE DO PACKAGE
******************************************************************************/
CREATE OR REPLACE PACKAGE PCK_SNPGATEWAY IS

  ----------------------------------------------------------------------------
  -- CONSTANTS                                                              --
  ----------------------------------------------------------------------------
  VALINDEF     CONSTANT NUMBER := -1;
  LIMCURVASIST CONSTANT NUMBER := 200;
  BOLTRUE      CONSTANT NUMBER := 1;
  BOLFALSE     CONSTANT NUMBER := 0;

  ----------------------------------------------------------------------------
  -- USER TYPES                                                             --
  ----------------------------------------------------------------------------

  TYPE udtEstudoDom    IS RECORD (intEstudoId NUMBER, intDominioId NUMBER);
  TYPE udtEquip        IS RECORD (intEquipId NUMBER, intTipoEquipId NUMBER);
  TYPE udtRefCursor    IS REF CURSOR;
  TYPE udtNumTable     IS TABLE OF NUMBER INDEX BY BINARY_INTEGER;
  TYPE udtEquipTable   IS TABLE OF udtEquip INDEX BY BINARY_INTEGER;
  TYPE udtEstudoTable  IS TABLE OF udtEstudoDom INDEX BY BINARY_INTEGER;

  ----------------------------------------------------------------------------
  -- STORED FUNCTIONS                                                       --
  ----------------------------------------------------------------------------

  FUNCTION FNSNP_rdErr_FKOrigem( p_MSGERR VARCHAR) RETURN VARCHAR;

  FUNCTION FNSNP_rdErr_FKDestino( p_MSGERR VARCHAR) RETURN VARCHAR;

  FUNCTION FNSNP_rdEquipIdProx RETURN NUMBER;

  FUNCTION FNSNP_rdCaboIdProx RETURN NUMBER;

  FUNCTION FNSNP_rdSuporteIdProx RETURN NUMBER;

  FUNCTION FNSNP_rdArranjoIdProx RETURN NUMBER;

  FUNCTION FNSNP_rdSuporteFixacaoItemProx (p_SuporteId NUMBER) RETURN NUMBER;

  FUNCTION FNSNP_rdCurvaIdProx RETURN NUMBER;

  FUNCTION FNSNP_rdCurva_CurvaIdProx (p_CurvaId NUMBER) RETURN NUMBER;

  FUNCTION FNSNP_rdTipoChaveIdProx RETURN NUMBER;

  FUNCTION FNSNP_rdTipoConsumidorIdProx RETURN NUMBER;

  FUNCTION FNSNP_rdTipoRedeIdProx RETURN NUMBER;

  ----------------------------------------------------------------------------
  -- STORED PROCEDURES                                                      --
  ----------------------------------------------------------------------------

  -- -------
  -- Leitura
  -- -------
  PROCEDURE SPSNP_rdTipoRede ( spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdTipoConsumidor ( spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdTipoChave ( spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdCurvaTipica ( spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdRede ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdRedeNoPai ( p_NoPai NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdBarra ( p_RedeId NUMBER, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdCabo ( p_CaboId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdSuporte ( p_SuporteId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdSuporteUtilizado ( p_SuporteId NUMBER DEFAULT VALINDEF, p_NumVezes OUT NUMBER );

  PROCEDURE SPSNP_rdSuporteUtilizado ( p_SuporteId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdArranjo_00_01 ( p_ArranjoId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdArranjo_02_04 ( p_ArranjoId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdArranjo_03 ( p_ArranjoId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdCapacitorSerie ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdTrecho ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdCarga ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdCargaNL ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdCargaIP ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdCapacitor ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdTrafoZZ ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdTrafo ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdTrafo3E ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdGerador ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdSuprimento ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdFiltro ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );  

  PROCEDURE SPSNP_rdReator ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdMutua ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdChave ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  PROCEDURE SPSNP_rdRegulador ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor );

  -- -------
  -- Remoção
  -- -------
  PROCEDURE SPSNP_dlSuporteFixacao ( p_SuporteId NUMBER );

  PROCEDURE SPSNP_dlTudo;

  PROCEDURE SPSNP_dlTipoRede;

  PROCEDURE SPSNP_dlTipoChave;

  -- -------
  -- Escrita
  -- -------
  PROCEDURE SPSNP_wrEquipamento ( p_EquipId IN OUT NUMBER, p_TipoEquipId NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_CurvaId NUMBER DEFAULT NULL,
                                  p_Barra1Id NUMBER, p_Barra2Id NUMBER DEFAULT NULL, p_Barra3Id NUMBER DEFAULT NULL
                                );

  PROCEDURE SPSNP_wrEquipamentoRede ( p_EquipId IN OUT NUMBER, p_RedeId IN NUMBER );

  PROCEDURE SPSNP_wrRede ( p_RedeId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR,
                           p_TipoRedeId NUMBER, p_NoId IN NUMBER, p_NoPaiId NUMBER, p_Radial NUMBER, p_BarraIniId NUMBER
                         );

  PROCEDURE SPSNP_wrRedeBarraIni ( p_RedeId IN NUMBER, p_BarraIniId NUMBER );

  PROCEDURE SPSNP_wrBarra ( p_BarraId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR,
                            p_VNom FLOAT,p_UtmX NUMBER, p_UtmY NUMBER
                          );

  PROCEDURE SPSNP_wrCurva ( p_CurvaId IN OUT NUMBER, p_Descricao VARCHAR, p_Tipica NUMBER, p_TipoCurvaId NUMBER, p_Pontos BLOB DEFAULT NULL );


  PROCEDURE SPSNP_wrCarga ( p_CargaId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_CurvaId NUMBER, p_Barra1Id NUMBER,
                            p_TipoFaseId NUMBER, p_TipoConsId NUMBER, p_Energia NUMBER, p_ICtePu NUMBER, p_ZCtePu NUMBER, p_SCtePu NUMBER,
                            p_NumConsRes NUMBER, p_NumConsCom NUMBER, p_NumConsInd NUMBER, p_NumConsRur NUMBER, p_NumConsOut NUMBER, p_NumConsA4 NUMBER
                          );

  PROCEDURE SPSNP_wrCargaNL ( p_CargaNLId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_Barra1Id NUMBER,
                              p_OrdemId NUMBER, p_FatorPot NUMBER, p_Indutivo NUMBER, p_kVA NUMBER,
                              p_SNomA NUMBER, p_SNomB NUMBER, p_SNomC NUMBER, p_AngA NUMBER, p_AngB NUMBER, p_AngC NUMBER
                            );

  PROCEDURE SPSNP_wrCargaIP ( p_CargaIPId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_CurvaId NUMBER, p_Barra1Id NUMBER,
                              p_Pnom NUMBER, p_FatorPot NUMBER, p_NumLamp NUMBER, p_TipoFaseId NUMBER,
                              p_ICte NUMBER, p_ZCte NUMBER, p_SCte NUMBER
                            );

  PROCEDURE SPSNP_wrFiltro ( p_FiltroId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_Barra1Id NUMBER,
                             p_Vnom NUMBER, p_Snom NUMBER, p_Z0R NUMBER, p_Z0X NUMBER
                           );

  PROCEDURE SPSNP_wrTrafoZZ ( p_TrafoZZId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_Barra1Id NUMBER,
                             p_Vnom NUMBER, p_Snom NUMBER, p_Z0R NUMBER, p_Z0X NUMBER
                            );

  PROCEDURE SPSNP_wrSuprimento ( p_SuprimentoId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_CurvaId NUMBER, p_Barra1Id NUMBER,
                                 p_TipoSuprId NUMBER, p_Vnom NUMBER, p_Smax NUMBER, p_Pcc3F NUMBER, p_Qcc3F NUMBER, p_PccFT NUMBER, p_QccFT NUMBER,
                                 p_Z0R NUMBER, p_Z0X NUMBER, p_Z1R NUMBER, p_Z1X NUMBER
                               );

  PROCEDURE SPSNP_wrGerador ( p_GeradorId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_CurvaId NUMBER, p_Barra1Id NUMBER,
                              p_TipoLigacaoId NUMBER, p_TipoSuprId NUMBER, p_TipoGerId NUMBER, p_Vnom NUMBER, p_Snom NUMBER, p_Smax NUMBER,
                              p_FatorPotMin NUMBER, p_Z0R NUMBER, p_Z0X NUMBER, p_Z1R NUMBER, p_Z1X NUMBER
                            );

  PROCEDURE SPSNP_wrReator ( p_ReatorId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_CurvaId NUMBER, p_Barra1Id NUMBER,
                             p_TipoLigacaoId NUMBER, p_Vnom NUMBER, p_Qnom NUMBER
                           );

  PROCEDURE SPSNP_wrCapacitor ( p_CapacitorId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_CurvaId NUMBER, p_Barra1Id NUMBER,
                                p_TipoLigacaoId NUMBER, p_Qnom NUMBER, p_Vnom NUMBER
                              );

  PROCEDURE SPSNP_wrCapacitorSerie ( p_CapacitorSerieId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_Barra1Id NUMBER, p_Barra2Id NUMBER,
                                     p_Vnom NUMBER, p_Snom NUMBER, p_ZR NUMBER, p_ZX NUMBER
                                   );

  PROCEDURE SPSNP_wrChave ( p_ChaveId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_Barra1Id NUMBER, p_Barra2Id NUMBER,
                            p_TipoChaveId NUMBER, p_Estado NUMBER, p_Inom NUMBER,
                            p_Telecomando NUMBER, p_Protecao NUMBER, p_TecnoCom NUMBER
                          );

  PROCEDURE SPSNP_wrTrecho ( p_TrechoId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_Barra1Id NUMBER, p_Barra2Id NUMBER,
                             p_Comprimento FLOAT, p_ArranjoId NUMBER
                           );

  PROCEDURE SPSNP_wrRegulador ( p_ReguladorId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_Barra1Id NUMBER, p_Barra2Id NUMBER,
                                p_TipoLigacaoId NUMBER, p_Snom NUMBER, p_Bypass NUMBER, p_FaixaVar NUMBER, p_NumPasso NUMBER, p_SentidoReversivel NUMBER, p_SentidoBarRefId NUMBER,
                                p_AjusteAuto NUMBER, p_CfgFixPassoSel NUMBER, p_CfgAutBarRefId NUMBER, p_CfgAutVpu NUMBER,
                                p_Z0R NUMBER, p_Z0X NUMBER, p_Z1R NUMBER, p_Z1X NUMBER,
                                p_ModeloId NUMBER, p_TipoFaseId NUMBER
                              );

  PROCEDURE SPSNP_wrTrafo ( p_TrafoId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_Barra1Id NUMBER, p_Barra2Id NUMBER,
                            p_AutoTrafo NUMBER, p_PerdaFerro NUMBER, p_LtcTipoUtl NUMBER, p_LtcBarRefId NUMBER, p_LtcVpu NUMBER, p_Z0R NUMBER, p_Z0X NUMBER, p_Z1R NUMBER, p_Z1X NUMBER,
                            p_Enrol1_TipoLigacaoId NUMBER, p_Enrol1_TipoFaseId NUMBER, p_Enrol1_Vnom NUMBER, p_Enrol1_Snom NUMBER, p_Enrol1_RotFase NUMBER, p_Enrol1_Vtap NUMBER, p_Enrol1_ZterraR NUMBER, p_Enrol1_ZterraX NUMBER,
                            p_Enrol2_TipoLigacaoId NUMBER, p_Enrol2_TipoFaseId NUMBER, p_Enrol2_Vnom NUMBER, p_Enrol2_Snom NUMBER, p_Enrol2_RotFase NUMBER, p_Enrol2_Vtap NUMBER, p_Enrol2_ZterraR NUMBER, p_Enrol2_ZterraX NUMBER
                          );

  PROCEDURE SPSNP_wrTrafo3E ( p_Trafo3EId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_Barra1Id NUMBER, p_Barra2Id NUMBER, p_Barra3Id NUMBER,
                              p_PerdaFerro NUMBER, p_Sbase NUMBER,
                              p_ZPS0R NUMBER, p_ZPS0X NUMBER, p_ZPT0R NUMBER, p_ZPT0X NUMBER, p_ZST0R NUMBER, p_ZST0X NUMBER,
                              p_ZPS1R NUMBER, p_ZPS1X NUMBER, p_ZPT1R NUMBER, p_ZPT1X NUMBER, p_ZST1R NUMBER, p_ZST1X NUMBER,
                              p_Enrol1_TipoLigacaoId NUMBER, p_Enrol1_TipoFaseId NUMBER, p_Enrol1_Vnom NUMBER, p_Enrol1_Snom NUMBER, p_Enrol1_RotFase NUMBER, p_Enrol1_Vtap NUMBER, p_Enrol1_ZterraR NUMBER, p_Enrol1_ZterraX NUMBER,
                              p_Enrol2_TipoLigacaoId NUMBER, p_Enrol2_TipoFaseId NUMBER, p_Enrol2_Vnom NUMBER, p_Enrol2_Snom NUMBER, p_Enrol2_RotFase NUMBER, p_Enrol2_Vtap NUMBER, p_Enrol2_ZterraR NUMBER, p_Enrol2_ZterraX NUMBER,
                              p_Enrol3_TipoLigacaoId NUMBER, p_Enrol3_TipoFaseId NUMBER, p_Enrol3_Vnom NUMBER, p_Enrol3_Snom NUMBER, p_Enrol3_RotFase NUMBER, p_Enrol3_Vtap NUMBER, p_Enrol3_ZterraR NUMBER, p_Enrol3_ZterraX NUMBER
                            );

  PROCEDURE SPSNP_wrMutua ( p_MutuaId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_Barra1Id NUMBER,
                            p_Trecho1Id NUMBER, p_Trecho1BarraSntId NUMBER, p_Trecho1Pos NUMBER, p_Trecho2Id NUMBER, p_Trecho2BarraSntId NUMBER, p_Trecho2Pos NUMBER,
                            p_Z0R NUMBER, p_Z0X NUMBER, p_Z1R NUMBER, p_Z1X NUMBER
                          );

  PROCEDURE SPSNP_wrArranjo ( p_ArranjoId IN OUT NUMBER, p_TipoArranjoId NUMBER, p_Descricao VARCHAR, p_ExtId VARCHAR, p_Tipico NUMBER,
                              p_ResTerra NUMBER, p_Vmin NUMBER, p_Vmax NUMBER, p_ColorId NUMBER, p_Status NUMBER, p_NumCabos NUMBER, p_TipoFase NUMBER, p_DataVersao DATE
                            );

  PROCEDURE SPSNP_wrArranjo00 ( p_ArranjoId IN OUT NUMBER, p_TipoArranjoId NUMBER, p_Descricao VARCHAR, p_ExtId VARCHAR, p_Tipico NUMBER, p_SuporteId NUMBER DEFAULT VALINDEF,
                                p_ResTerra NUMBER, p_Vmin NUMBER, p_Vmax NUMBER, p_ColorId NUMBER, p_Status NUMBER, p_NumCabos NUMBER, p_TipoFase NUMBER,
                                p_Cabo1Id NUMBER, p_TipoFase1Id NUMBER DEFAULT VALINDEF, p_FixacaoItem1 NUMBER DEFAULT VALINDEF,
                                p_Cabo2Id NUMBER DEFAULT VALINDEF, p_TipoFase2Id NUMBER DEFAULT VALINDEF, p_FixacaoItem2 NUMBER DEFAULT VALINDEF,
                                p_Cabo3Id NUMBER DEFAULT VALINDEF, p_TipoFase3Id NUMBER DEFAULT VALINDEF, p_FixacaoItem3 NUMBER DEFAULT VALINDEF,
                                p_Cabo4Id NUMBER DEFAULT VALINDEF, p_TipoFase4Id NUMBER DEFAULT VALINDEF, p_FixacaoItem4 NUMBER DEFAULT VALINDEF,
                                p_DataVersao DATE
                              );

  PROCEDURE SPSNP_wrArranjo01 ( p_ArranjoId IN OUT NUMBER, p_TipoArranjoId NUMBER, p_Descricao VARCHAR, p_ExtId VARCHAR, p_Tipico NUMBER,
                                p_ResTerra NUMBER, p_Vmin NUMBER, p_Vmax NUMBER, p_ColorId NUMBER, p_Status NUMBER, p_NumCabos NUMBER, p_TipoFase NUMBER,
                                p_CaboId NUMBER, p_SuporteId NUMBER DEFAULT VALINDEF, p_FixacaoItem NUMBER DEFAULT VALINDEF,
                                p_DataVersao DATE
                              );

  PROCEDURE SPSNP_wrArranjo02 ( p_ArranjoId IN OUT NUMBER, p_TipoArranjoId NUMBER, p_Descricao VARCHAR, p_ExtId VARCHAR, p_Tipico NUMBER, p_SuporteId NUMBER DEFAULT VALINDEF,
                                p_ResTerra NUMBER, p_Iadm NUMBER, p_Vmin NUMBER, p_Vmax NUMBER, p_ColorId NUMBER, p_Status NUMBER, p_NumCabos NUMBER, p_TipoFase NUMBER,
                                p_Z0R NUMBER, p_Z0X NUMBER, p_Z1R NUMBER, p_Z1X NUMBER,
                                p_C0 NUMBER, p_C1 NUMBER,
                                p_DataVersao DATE
                              );

  PROCEDURE SPSNP_wrArranjo04 ( p_ArranjoId IN OUT NUMBER, p_TipoArranjoId NUMBER, p_Descricao VARCHAR, p_ExtId VARCHAR, p_Tipico NUMBER, p_SuporteId NUMBER DEFAULT VALINDEF,
                                p_ResTerra NUMBER, p_Iadm NUMBER, p_Vmin NUMBER, p_Vmax NUMBER, p_ColorId NUMBER, p_Status NUMBER, p_NumCabos NUMBER, p_TipoFase NUMBER,
                                p_Z0R NUMBER, p_Z0X NUMBER, p_Z1R NUMBER, p_Z1X NUMBER,
                                p_C0 NUMBER, p_C1 NUMBER,
                                p_DataVersao DATE
                              );

  PROCEDURE SPSNP_wrArranjo03 ( p_ArranjoId IN OUT NUMBER, p_TipoArranjoId NUMBER, p_Descricao VARCHAR, p_ExtId VARCHAR, p_Tipico NUMBER,
                                p_ResTerra NUMBER, p_Iadm NUMBER, p_Vmin NUMBER, p_Vmax NUMBER, p_ColorId NUMBER, p_Status NUMBER, p_NumCabos NUMBER, p_TipoFase NUMBER,
                                p_MatZ BLOB, p_MatC BLOB, p_DataVersao DATE
                              );

  -- Arranjos
  PROCEDURE SPSNP_wrCabo ( p_CaboId IN OUT NUMBER, p_TipoCaboId NUMBER, p_Descricao VARCHAR, p_ExtId VARCHAR, p_Inom NUMBER, p_Vmin NUMBER, p_Vmax NUMBER, p_Status NUMBER );

  PROCEDURE SPSNP_wrCaboRGMR ( p_CaboId IN OUT NUMBER, p_Descricao VARCHAR, p_ExtId VARCHAR, p_Inom NUMBER, p_Vmin NUMBER, p_Vmax NUMBER, p_R NUMBER, p_GMR NUMBER, p_Status NUMBER );

  PROCEDURE SPSNP_wrCaboZ0Z1 ( p_CaboId IN OUT NUMBER, p_Descricao VARCHAR, p_ExtId VARCHAR, p_Inom NUMBER, p_Vmin NUMBER, p_Vmax NUMBER,
                               p_Z0R NUMBER, p_Z0X NUMBER, p_Z1R NUMBER, p_Z1X NUMBER, p_C0 NUMBER, p_C1 NUMBER, p_Status NUMBER
                             );

  PROCEDURE SPSNP_wrSuporte ( p_SuporteId IN OUT NUMBER, p_TipoSuporteId NUMBER, p_Descricao VARCHAR, p_ExtId VARCHAR, p_Vmin NUMBER, p_Vmax NUMBER, P_NumPontosFix NUMBER, p_Status NUMBER );

  PROCEDURE SPSNP_wrSuporteFixacao ( p_SuporteId NUMBER, p_FixacaoItem IN OUT NUMBER, p_X NUMBER, p_Y NUMBER, p_Codigo VARCHAR );

  -- Tipos
  PROCEDURE SPSNP_wrTipoChave ( p_TipoChaveId IN OUT NUMBER, p_Descricao VARCHAR, p_Status NUMBER, p_CorId NUMBER );

  PROCEDURE SPSNP_wrTipoConsumidor ( p_TipoConsId IN OUT NUMBER, p_Tag NUMBER, p_Descricao VARCHAR, p_Tipico NUMBER, p_CorId NUMBER);

  PROCEDURE SPSNP_wrTipoRede ( p_TipoRedeId IN OUT NUMBER, p_Codigo VARCHAR, p_Descricao VARCHAR, p_Vmin NUMBER, p_Vmax NUMBER, p_ColorId NUMBER, p_Status NUMBER );
  
  
END PCK_SNPGATEWAY;
/

/******************************************************************************
  IMPLEMENTAção DO PACKAGE
******************************************************************************/
-- Inãcio do Package

--------------------------------------------------------------------------------
-- STORED FUNCTIONS                                                           --
--------------------------------------------------------------------------------

CREATE OR REPLACE PACKAGE BODY PCK_SNPGATEWAY
AS

FUNCTION FNSNP_rdErr_FKOrigem(p_MSGERR VARCHAR) RETURN VARCHAR
IS
/******************************************************************************
  NAME:       FNSNP_rdErr_FKOrigem
  PURPOSE:    Retorna a tabela de origem (chave estrangeira) a partir da
              mensagem de erro

    Data   | Autor | Versão | Descrição
  =========================================================================
  20150202 |  0.00  | Versão inicial.
*****************************************************************************/

-- Variáveis locais
retorno VARCHAR(255);
posicao NUMBER;
straux  VARCHAR(1024);
strFK   VARCHAR(1024);

BEGIN
  SELECT INSTR(p_MSGERR,'FK', 1, 1) INTO posicao FROM DUAL;
  SELECT SUBSTR(p_MSGERR, posicao) INTO straux  FROM DUAL;
  SELECT INSTR(straux,')', 1, 1) INTO posicao FROM DUAL;
  SELECT SUBSTR(straux, 1, posicao - 1) INTO strFK  FROM DUAL;
  
  SELECT 
    TABLE_NAME
  INTO
    RETORNO
  FROM 
    USER_CONSTRAINTS
  WHERE 
    CONSTRAINT_NAME = (SELECT R_CONSTRAINT_NAME FROM USER_CONSTRAINTS WHERE constraint_name = strFK);

  RETURN retorno;
END FNSNP_rdErr_FKOrigem;
/*---------------------------------------------------------------------------*/

FUNCTION FNSNP_rdErr_FKDestino(p_MSGERR VARCHAR) RETURN VARCHAR
IS
/******************************************************************************
  NAME:       FNSNP_rdErr_FKDestino
  PURPOSE:    Retorna a tabela de destino (chave estrangeira) a partir da
              mensagem de erro

    Data   | Autor | Versão | Descrição
  =========================================================================
  20150202 |  0.00  | Versão inicial.
*****************************************************************************/

-- Variáveis locais
retorno VARCHAR(255);
posicao NUMBER;
straux  VARCHAR(1024);
strFK   VARCHAR(1024);

BEGIN
  SELECT INSTR(p_MSGERR,'FK', 1, 1) INTO posicao FROM DUAL;
  SELECT SUBSTR(p_MSGERR, posicao) INTO straux  FROM DUAL;
  SELECT INSTR(straux,')', 1, 1) INTO posicao FROM DUAL;
  SELECT SUBSTR(straux, 1, posicao - 1) INTO strFK  FROM DUAL;
  
  SELECT 
    TABLE_NAME
  INTO
    RETORNO
  FROM 
    USER_CONSTRAINTS
  WHERE 
    CONSTRAINT_NAME = strFK;

  RETURN retorno;
END FNSNP_rdErr_FKDestino;
/*---------------------------------------------------------------------------*/

FUNCTION FNSNP_rdEquipIdProx RETURN NUMBER
IS
/******************************************************************************
  NAME:       FNSNP_rdEquipIdProx
  PURPOSE:    Retorna o prãximo identificador de um equipamento

    Data   | Autor | Versão | Descrição
  =========================================================================
  20100908 |  MML  |  0.00  | Versão inicial.
*****************************************************************************/

-- Variáveis locais
retorno NUMBER;

BEGIN
  SELECT
    (NVL(MAX(intEquipId), 0) + 1)
  INTO
    retorno
  FROM
    Equipamento;

  RETURN retorno;
END FNSNP_rdEquipIdProx;
/*---------------------------------------------------------------------------*/

FUNCTION FNSNP_rdCaboIdProx RETURN NUMBER
IS
/******************************************************************************
  NAME:       FNSNP_rdCaboIdProx
  PURPOSE:    Retorna o prãximo identificador de um cabo

    Data   | Autor | Versão | Descrição
  =========================================================================
  20100913 |  MML  |  0.00  | Versão inicial.
*****************************************************************************/

-- Variáveis locais
retorno NUMBER;

BEGIN
  SELECT (NVL(MAX(intCaboId), 0) + 1) INTO retorno FROM Cabo;

  RETURN retorno;
END FNSNP_rdCaboIdProx;
/*---------------------------------------------------------------------------*/

FUNCTION FNSNP_rdSuporteIdProx RETURN NUMBER
IS
/******************************************************************************
  NAME:       FNSNP_rdSuporteIdProx
  PURPOSE:    Retorna o prãximo identificador de um suporte

    Data   | Autor | Versão | Descrição
  =========================================================================
  20100927 |  MML  |  0.00  | Versão inicial.
*****************************************************************************/

-- Variáveis locais
retorno NUMBER;

BEGIN
  SELECT (NVL(MAX(intSuporteId), 0) + 1) INTO retorno FROM Suporte;

  RETURN retorno;
END FNSNP_rdSuporteIdProx;
/*---------------------------------------------------------------------------*/

FUNCTION FNSNP_rdArranjoIdProx RETURN NUMBER
IS
/******************************************************************************
  NAME:       FNSNP_rdArranjoIdProx
  PURPOSE:    Retorna o prãximo identificador de um arranjo

    Data   | Autor | Versão | Descrição
  =========================================================================
  20100927 |  MML  |  0.00  | Versão inicial.
*****************************************************************************/

-- Variáveis locais
retorno NUMBER;

BEGIN
  SELECT (NVL(MAX(intArranjoId), 0) + 1) INTO retorno FROM Arranjo;

  RETURN retorno;
END FNSNP_rdArranjoIdProx;
/*---------------------------------------------------------------------------*/

FUNCTION FNSNP_rdSuporteFixacaoItemProx (p_SuporteId NUMBER) RETURN NUMBER
IS
/******************************************************************************
  NAME:       FNSNP_rdSuporteFixacaoItemProx
  PURPOSE:    Retorna o prãximo item de fixação em um determinado suporte

    Data   | Autor | Versão | Descrição
  =========================================================================
  20100915 |  MML  |  0.00  | Versão inicial.
*****************************************************************************/

-- Variáveis locais
retorno NUMBER;

BEGIN
  SELECT (NVL(MAX(intFixacaoItem), 0) + 1) INTO retorno FROM SuporteFixacao WHERE intSuporteId = p_SuporteId;

  RETURN retorno;
END FNSNP_rdSuporteFixacaoItemProx;
/*---------------------------------------------------------------------------*/

FUNCTION FNSNP_rdCurvaIdProx RETURN NUMBER
IS
/******************************************************************************
  NAME:       FNSNP_rdCurvaIdProx
  PURPOSE:    Retorna o prãximo identificador de um conjunto de curva
              NOTA: os identificadores fornecidos por essa função estão acima
                    da reserva de curvas de sistema (LIMCURVASIST)

    Data   | Autor | Versão | Descrição
  =========================================================================
  20150511 |  MML  |  0.00  | Versão inicial.
*****************************************************************************/

-- Variáveis locais
retorno NUMBER;

BEGIN
  SELECT
    (NVL(MAX(intCurvaId), 0) + 1)
  INTO
    retorno
  FROM
    Curva
  WHERE
    intCurvaId > LIMCURVASIST;

  IF (retorno < LIMCURVASIST) THEN
    retorno := retorno + LIMCURVASIST;
  END IF;

  RETURN retorno;
END FNSNP_rdCurvaIdProx;
/*---------------------------------------------------------------------------*/

FUNCTION FNSNP_rdCurva_CurvaIdProx (p_CurvaId NUMBER) RETURN NUMBER
IS
/******************************************************************************
  NAME:       FNSNP_rdCurva_CurvaIdProx
  PURPOSE:    Retorna o prãximo identificador de uma curva, dentro de um con-
              junto de curvas.

    Data   | Autor | Versão | Descrição
  =========================================================================
  20150511 |  MML  |  0.00  | Versão inicial.
*****************************************************************************/

-- Variáveis locais
retorno NUMBER;

BEGIN
  SELECT
    (NVL(MAX(intCurvaId), 0) + 1)
  INTO
    retorno
  FROM
    Curva
  WHERE
    intCurvaId = p_CurvaId;

  RETURN retorno;
END FNSNP_rdCurva_CurvaIdProx;
/*---------------------------------------------------------------------------*/

FUNCTION FNSNP_rdTipoChaveIdProx RETURN NUMBER
IS
/******************************************************************************
  NAME:       FNSNP_rdTipoChaveIdProx
  PURPOSE:    Retorna o prãximo identificador de um tipo de chave

    Data   | Autor | Versão | Descrição
  =========================================================================
  20150512 |  MML  |  0.00  | Versão inicial.
*****************************************************************************/

-- Variáveis locais
retorno NUMBER;

BEGIN
  SELECT (NVL(MAX(intTipoChaveId), 0) + 1) INTO retorno FROM TipoChave;
  RETURN retorno;

END FNSNP_rdTipoChaveIdProx;
/*---------------------------------------------------------------------------*/

FUNCTION FNSNP_rdTipoConsumidorIdProx RETURN NUMBER
IS
/******************************************************************************
  NAME:       FNSNP_rdTipoConsumidorIdProx
  PURPOSE:    Retorna o prãximo identificador de um tipo de consumidor

    Data   | Autor | Versão | Descrição
  =========================================================================
  20130701 |  MML  |  0.00  | Versão inicial.
*****************************************************************************/

-- Variáveis locais
retorno NUMBER;

BEGIN
  SELECT (NVL(MAX(intTipoConsId), 0) + 1) INTO retorno FROM TipoConsumidor;
  RETURN retorno;

END FNSNP_rdTipoConsumidorIdProx;
/*---------------------------------------------------------------------------*/

FUNCTION FNSNP_rdTipoRedeIdProx RETURN NUMBER
IS
/******************************************************************************
  NAME:       FNSNP_rdTipoRedeIdProx
  PURPOSE:    Retorna o prãximo identificador de um tipo de rede

    Data   | Autor | Versão | Descrição
  =========================================================================
  20150512 |  MML  |  0.00  | Versão inicial.
*****************************************************************************/

-- Variáveis locais
retorno NUMBER;

BEGIN
  SELECT (NVL(MAX(intTipoRedeId), 0) + 1) INTO retorno FROM TipoRede;
  RETURN retorno;

END FNSNP_rdTipoRedeIdProx;
/*---------------------------------------------------------------------------*/

--------------------------------------------------------------------------------
-- STORED PROCEDURES                                                          --
--------------------------------------------------------------------------------

PROCEDURE SPSNP_rdTipoRede ( spcursor OUT udtRefCursor )
-- ****************************************************************************
-- ** Procedure SPSNP_rdTipoRede                                             **
-- **                                                                        **
-- ** Lã todas os tipos de rede existentes no sistema. Caso seja passado um  **
-- ** identificador de um estudo, retorna somente os tipos de rede existen-  **
-- ** tes naquele estudo.                                                    **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20100901 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************
IS
BEGIN
  OPEN spcursor FOR
  SELECT
    TipoRede.intTipoRedeId,
    TipoRede.strCodigo,
    TipoRede.strDescricao,
    TipoRede.fltVMin,
    TipoRede.fltVMax,
    TipoRede.intColorId,
    TipoRede.intStatus,
    (
    SELECT
      COUNT(Rede.intTipoRedeId)
    FROM
      Rede
    WHERE
      Rede.intTipoRedeId = TipoRede.intTipoRedeId
    ) AS intQuantidade
  FROM
    TipoRede;
END SPSNP_rdTipoRede;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdTipoConsumidor ( spcursor OUT udtRefCursor )
-- ****************************************************************************
-- ** Procedure SPSNP_rdTipoConsumidor                                       **
-- **                                                                        **
-- ** Lã todas os tipos de consumidor existentes no sistema.                 **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150511 |  mml  | Versão inicial.                                     **
-- ** 20130701 |  mml  | Inclusão de CorId.                                  **
-- **                                                                        **
-- ****************************************************************************
IS
BEGIN
  OPEN spcursor FOR
  SELECT
    TipoConsumidor.intTipoConsId AS TipoConsId,
    TipoConsumidor.intTag        AS Tag,
    TipoConsumidor.strDescricao  AS Descricao,
    TipoConsumidor.bolTipico     AS Tipico,
    TipoConsumidor.intCorId      AS CorId
  FROM
    TipoConsumidor;

END SPSNP_rdTipoConsumidor;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdTipoChave ( spcursor OUT udtRefCursor )
-- ****************************************************************************
-- ** Procedure SPSNP_rdTipoChave                                            **
-- **                                                                        **
-- ** Lã todas os tipos de chave existentes no sistema.                      **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20100921 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************
IS
BEGIN

  OPEN spcursor FOR
  SELECT
    TipoChave.*
  FROM
    TipoChave;

END SPSNP_rdTipoChave;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdCurvaTipica ( spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdCurvaTipica                                      **
-- **                                                                        **
-- ** Retorna os conjuntos de curvas tãpicas de patamares de potãncia.       **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150511 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************
BEGIN

  OPEN spcursor FOR
  SELECT
    Curva.intCurvaId             AS CurvaId,
    Curva.strDescricao           AS Descricao,
    Curva.intTipoCurvaId         AS TipoCurvaId,
    Curva.blbPontos              AS Pontos
  FROM
    Curva
  WHERE
    Curva.intTipoCurvaId BETWEEN 1 AND 7 AND   -- PQVT, PQ, PQ_MDP, PFP, PFP_MDP, LD, PQVIT
    Curva.bolTipica = 1;

END SPSNP_rdCurvaTipica;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrEquipamento ( p_EquipId IN OUT NUMBER, p_TipoEquipId NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_CurvaId NUMBER DEFAULT NULL,
                                p_Barra1Id NUMBER, p_Barra2Id NUMBER DEFAULT NULL, p_Barra3Id NUMBER DEFAULT NULL
                              )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrEquipamento                                          **
-- **                                                                        **
-- ** Cria um novo equipamento.                                              **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150512 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais
bar1aux NUMBER;
bar2aux NUMBER;
bar3aux NUMBER;
curvaux NUMBER;

BEGIN

  p_EquipId := FNSNP_rdEquipIdProx();

  -- Ao gravar uma barra, ainda não tem o id da Barra (poderia ser colocado o id gerado acima
  IF ((p_Barra1Id <= 0) OR (p_Barra1Id IS NULL)) THEN bar1aux := NULL; ELSE bar1aux := p_Barra1Id; END IF;
  IF ((p_Barra2Id <= 0) OR (p_Barra2Id IS NULL)) THEN bar2aux := NULL; ELSE bar2aux := p_Barra2Id; END IF;
  IF ((p_Barra3Id <= 0) OR (p_Barra3Id IS NULL)) THEN bar3aux := NULL; ELSE bar3aux := p_Barra3Id; END IF;
  IF ((p_CurvaId  <= 0) OR (p_CurvaId  IS NULL)) THEN curvaux := NULL; ELSE curvaux := p_CurvaId;  END IF;

  INSERT INTO
    Equipamento (intEquipId, intTipoEquipId, strCodigo, strExtId, intCurvaId, intBarra1Id, intBarra2Id, intBarra3Id )
  VALUES
     (p_EquipId, p_TipoEquipId,p_Codigo, p_ExtId, curvaux, bar1aux, bar2aux, bar3aux);

  EXCEPTION
    WHEN OTHERS THEN
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM)||';'||p_EquipId||';'||bar1aux);
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrEquipamento;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrEquipamentoRede ( p_EquipId IN OUT NUMBER, p_RedeId IN NUMBER )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrEquipamentoRede                                      **
-- **                                                                        **
-- ** Associa o equipamento à rede passada como parâmetro. Nota: não são fei-**
-- ** tos testes para verificação de existência de registro, por questões de **
-- ** desempenho.                                                            **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150512 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

BEGIN

  INSERT INTO
    EquipamentoRede (intEquipId, intRedeId)
  VALUES
    (p_EquipId, p_RedeId);

  EXCEPTION
    WHEN OTHERS THEN
      p_EquipId := VALINDEF;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM)||';'||p_RedeId||';'||p_EquipId);
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrEquipamentoRede;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrRede ( p_RedeId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR,
                         p_TipoRedeId NUMBER, p_NoId IN NUMBER, p_NoPaiId NUMBER, p_Radial NUMBER, p_BarraIniId NUMBER
                       )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrRede                                                 **
-- **                                                                        **
-- ** Se a identificação da rede for nula, então cria uma nova rede. Caso    **
-- ** contrãrio, atualiza os valores passados como parãmetro.                **
-- ** Ao final, retorna o identificador da rede criada ou atualizada.        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150512 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
    -- Cria nova rede (ini)
    SPSNP_wrEquipamento( p_RedeId, 19 /*p_TipoEquipId*/, p_Codigo, p_ExtId, VALINDEF, p_BarraIniId );

    INSERT INTO
      Rede ( intRedeId, intTipoRedeId, intNoId, intNoPaiId, bolRadial, intBarraIniId )
    VALUES
      ( p_RedeId, p_TipoRedeId, p_NoId, CASE WHEN p_NoPaiId < 0 THEN NULL ELSE p_NoPaiId END, p_Radial, p_BarraIniId );


  EXCEPTION
    WHEN OTHERS THEN
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM)||';'||p_RedeId||';'||p_NoId||';'||p_NoPaiId);
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrRede;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrRedeBarraIni ( p_RedeId IN NUMBER, p_BarraIniId NUMBER )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrRedeBarraIni                                         **
-- **                                                                        **
-- ** Cria uma barra na base de dados. Caso o identificador passado jã exis- **
-- ** ta, atualiza os dados das barras.                                      **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150511 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais
BEGIN
  UPDATE
    Rede
  SET
    intBarraIniId = p_BarraIniId
  WHERE
    intRedeId = p_RedeId;

  EXCEPTION
    WHEN OTHERS THEN
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrRedeBarraIni;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdRede ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdRede                                                 **
-- **                                                                        **
-- ** Efetua a leitura das redes contidas na base de dados. Se a rede não for**
-- ** especificada, retorna os dados de todas as redes.                      **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150512 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
  OPEN spcursor FOR
  SELECT
    Rede.intRedeId          AS RedeId,
    Equipamento.strCodigo   AS Codigo,
    Equipamento.strExtId    AS ExtId,
    Rede.intTipoRedeId      AS TipoRedeId,
    TipoRede.strCodigo      AS TipoRedeCodigo,
    Rede.intNoId            AS NoId,
    Rede.intNoPaiId         AS NoPaiId,
    Rede.bolRadial          AS Radial,
    Rede.intBarraIniId      AS BarraIniId
  FROM
    Rede
      INNER JOIN Equipamento ON
        Rede.intRedeId = Equipamento.intEquipId

      INNER JOIN TipoRede ON
        Rede.intTipoRedeId = TipoRede.intTipoRedeId

  WHERE
    ((p_RedeId = VALINDEF) OR (Rede.intRedeId = p_RedeId));
END SPSNP_rdRede;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdRedeNoPai ( p_NoPai NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdRedeNoPai                                            **
-- **                                                                        **
-- ** Retorna todas as redes de um determinado nó pai. Se o nó pai for -1,   **
-- ** retorna as redes cujos NoPai é nulo                                    **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150512 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
  OPEN spcursor FOR
  SELECT
    Rede.intRedeId        AS RedeId,
    Equipamento.strCodigo AS Codigo,
    Rede.intNoId          AS NoId,
    Rede.intNoPaiId       AS NoPaiId
  FROM
    Rede
      INNER JOIN Equipamento ON
        Rede.intRedeId = Equipamento.intEquipId
  WHERE
    ( 
    ((p_NoPai = VALINDEF) AND (Rede.intNoPaiId IS NULL)) OR
    ((p_NoPai <> VALINDEF) AND (Rede.intNoPaiId = p_NoPai))
    );
END SPSNP_rdRedeNoPai;

/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrBarra ( p_BarraId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR,
                          p_VNom FLOAT,p_UtmX NUMBER, p_UtmY NUMBER
                        )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrBarra                                                **
-- **                                                                        **
-- ** Cria uma barra na base de dados. Caso o identificador passado jã exis- **
-- ** ta, atualiza os dados das barras.                                      **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150511 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais
EXP_GERACAOBARRA EXCEPTION;

BEGIN
  SAVEPOINT A;

  SPSNP_wrEquipamento( p_BarraId, 2 /*p_TipoEquipId*/, p_Codigo, p_ExtId, VALINDEF, VALINDEF );

  IF (p_BarraId = VALINDEF) THEN
    RAISE EXP_GERACAOBARRA;
  END IF;

  INSERT INTO
    Barra (intBarraId, fltVNom, intUtmX, intUtmY)
  VALUES
    (p_BarraId, p_VNom, p_UtmX, p_UtmY);

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrBarra;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrCurva ( p_CurvaId IN OUT NUMBER, p_Descricao VARCHAR, p_Tipica NUMBER, p_TipoCurvaId NUMBER, p_Pontos BLOB DEFAULT NULL )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrCurva                                                **
-- **                                                                        **
-- ** Cria uma nova curva ou atualiza uma existente.                         **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150511 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
  IF (p_CurvaId = VALINDEF) THEN
    -- Cria novo conjunto de curvas (ini)
    p_CurvaId := FNSNP_rdCurvaIdProx();
    INSERT INTO
      Curva (intCurvaId, strDescricao, bolTipica, intTipoCurvaId, blbPontos)
    VALUES
      (p_CurvaId, p_Descricao, p_Tipica, p_TipoCurvaId, p_Pontos);
    -- Cria novo conjunto de curvas (fim)
  ELSE
    -- Atualiza conjunto de curvas existente (ini)
    UPDATE
      Curva
    SET
      strDescricao = p_Descricao,
      bolTipica = p_Tipica,
      intTipoCurvaId = p_TipoCurvaId,
      blbPontos = p_Pontos
    WHERE
      intCurvaId = p_CurvaId;
    -- Atualiza conjunto de curvas existente (fim)
  END IF;

  EXCEPTION
    WHEN OTHERS THEN
      p_CurvaId := VALINDEF;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrCurva;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdBarra ( p_RedeId NUMBER, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdBarra                                                **
-- **                                                                        **
-- ** Efetua a leitura das barras de uma rede. Se a rede não for especifica- **
-- ** da, retorna todas as barras de todas as redes.                         **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150512 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
    OPEN spcursor FOR
    SELECT
      Equipamento.intEquipId                     AS BarraId,
      Equipamento.strCodigo                      AS Codigo,
      Equipamento.strExtId                       AS ExtId,
      EqAux.fltVNom                              AS VNom,
      EqAux.intUtmX                              AS UtmX,
      EqAux.intUtmY                              AS UtmY
    FROM
      Barra EqAux 
        INNER JOIN Equipamento ON
          EqAux.intBarraId = Equipamento.intEquipId
        
        INNER JOIN EquipamentoRede ON
          Equipamento.intEquipId = EquipamentoRede.intEquipId
  WHERE
    ((p_RedeId = VALINDEF) OR (EquipamentoRede.intRedeId = p_RedeId));
        
END SPSNP_rdBarra;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrArranjo ( p_ArranjoId IN OUT NUMBER, p_TipoArranjoId NUMBER, p_Descricao VARCHAR, p_ExtId VARCHAR, p_Tipico NUMBER,
                            p_ResTerra NUMBER, p_Vmin NUMBER, p_Vmax NUMBER, p_ColorId NUMBER, p_Status NUMBER, p_NumCabos NUMBER, p_TipoFase NUMBER, p_DataVersao DATE
                          )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrArranjo                                              **
-- **                                                                        **
-- ** Insere um novo arranjo, porãm essa procedure não deve ser utilizada    **
-- ** isoladamente, isto ã, devem ser usadas as procedures que especificam   **
-- ** os tipos de arranjos  a serem inseridos.                               **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20100809 |  mml  | Versão inicial.                                     **
-- ** 20130730 |  mml  | Introdução do ExtId.                                **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
  IF (p_ArranjoId = VALINDEF) THEN
    -- Cria novo Arranjo (ini)
    p_ArranjoId := FNSNP_rdArranjoIdProx();
    INSERT INTO
      Arranjo ( intArranjoId, intTipoArranjoId, strDescricao, strExtId, bolTipico, fltResTerra, fltVmin, fltVmax, intColorId, intStatus, intNumCabos, intTipoFaseId, dttVersao )
    VALUES
      ( p_ArranjoId, p_TipoArranjoId, p_Descricao, p_ExtId, p_Tipico, p_ResTerra, p_Vmin, p_Vmax, p_ColorId, p_Status, p_NumCabos, p_TipoFase, NVL(p_DataVersao, SYSDATE) );
    -- Cria novo Arranjo (fim)
  ELSE
    -- Atualiza Arranjo existente (ini)
    UPDATE
      Arranjo
    SET
      strDescricao = p_Descricao,
      strExtId = p_ExtId,
      bolTipico = p_Tipico,
      fltResTerra = p_ResTerra,
      fltVmin = p_Vmin,
      fltVmax = p_Vmax,
      intColorId = p_ColorId,
      intStatus = p_Status,
      intNumCabos = p_NumCabos,
      intTipoFaseId = p_TipoFase,
      dttVersao = NVL(p_DataVersao, SYSDATE)
    WHERE
      intArranjoId = p_ArranjoId;
    -- Atualiza Arranjo existente (fim)
  END IF;

  EXCEPTION
    WHEN OTHERS THEN
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrArranjo;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrArranjo00 ( p_ArranjoId IN OUT NUMBER, p_TipoArranjoId NUMBER, p_Descricao VARCHAR, p_ExtId VARCHAR, p_Tipico NUMBER, p_SuporteId NUMBER DEFAULT VALINDEF,
                              p_ResTerra NUMBER, p_Vmin NUMBER, p_Vmax NUMBER, p_ColorId NUMBER, p_Status NUMBER, p_NumCabos NUMBER, p_TipoFase NUMBER,
                              p_Cabo1Id NUMBER, p_TipoFase1Id NUMBER DEFAULT VALINDEF, p_FixacaoItem1 NUMBER DEFAULT VALINDEF,
                              p_Cabo2Id NUMBER DEFAULT VALINDEF, p_TipoFase2Id NUMBER DEFAULT VALINDEF, p_FixacaoItem2 NUMBER DEFAULT VALINDEF,
                              p_Cabo3Id NUMBER DEFAULT VALINDEF, p_TipoFase3Id NUMBER DEFAULT VALINDEF, p_FixacaoItem3 NUMBER DEFAULT VALINDEF,
                              p_Cabo4Id NUMBER DEFAULT VALINDEF, p_TipoFase4Id NUMBER DEFAULT VALINDEF, p_FixacaoItem4 NUMBER DEFAULT VALINDEF,
                              p_DataVersao DATE
                            )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrArranjo00                                            **
-- **                                                                        **
-- ** Insere ou atualiza um arranjo do tipo 01. No arranjo tipo 01 somente é **
-- ** permitida a utilização de cabos monopolares (de 1 a 4 cabos).          **
-- ** Na atualização é considerada a chave primãria da tabela para determina-**
-- ** ção do registro.                                                       **
-- **                                                                        **
-- ** 1..4 cabos monopolares (RGMR), suporte obrigatãrio (arranjoCABOxFASE). **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20100809 |  mml  | Versão inicial.                                     **
-- ** 20130730 |  mml  | Introdução do ExtId.                                **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais
EXP_ARRANJO01 EXCEPTION;
strmsg VARCHAR2(255);

BEGIN

  strmsg := 'Erro na inserção ou atualização do arranjo do tipo 00. ';

  SAVEPOINT A;

  -- Suporte é obrigatãrio
  IF (p_SuporteId = VALINDEF) THEN
      strmsg := 'Nesse tipo de arranjo o suporte é obrigatãrio.';
      RAISE EXP_ARRANJO01;
  END IF;

  -- Testa se os conjuntos de dados estão completos
  IF (p_TipoArranjoId = 0) THEN
    -- Arranjo Tipo 0 (ini)
    IF (p_Cabo1Id <> VALINDEF) THEN
      IF ((p_SuporteId = VALINDEF) OR (p_FixacaoItem1 = VALINDEF) OR (p_TipoFase1Id = VALINDEF)) THEN
        strmsg := 'Falta o item de fixação ou tipo de fase para o cabo 1. ';
        RAISE EXP_ARRANJO01;
      END IF;
    END IF;

    -- Testa se os conjuntos de dados estão completos
    IF (p_Cabo2Id <> VALINDEF) THEN
      IF ((p_SuporteId = VALINDEF) OR (p_TipoFase2Id = VALINDEF) OR (p_FixacaoItem2 = VALINDEF)) THEN
        strmsg := 'Dados incompletos para o cabo 2. ';
        RAISE EXP_ARRANJO01;
      END IF;
    END IF;

    IF (p_Cabo3Id <> VALINDEF) THEN
      IF ((p_SuporteId = VALINDEF) OR (p_TipoFase3Id = VALINDEF) OR (p_FixacaoItem3 = VALINDEF)) THEN
        strmsg := 'Dados incompletos para o cabo 3. ';
        RAISE EXP_ARRANJO01;
      END IF;
    END IF;

    IF (p_Cabo4Id <> VALINDEF) THEN
      IF ((p_SuporteId = VALINDEF) OR (p_TipoFase4Id = VALINDEF) OR (p_FixacaoItem4 = VALINDEF)) THEN
        strmsg := 'Dados incompletos para o cabo 4. ';
        RAISE EXP_ARRANJO01;
      END IF;
    END IF;
    -- Arranjo Tipo 0 (fim)
  END IF;

  /*
  IF (p_TipoArranjoId = 1) THEN
    IF ((p_FixacaoItem1 != VALINDEF) OR (p_FixacaoItem2 != VALINDEF) OR (p_FixacaoItem3 != VALINDEF) OR (p_FixacaoItem4 != VALINDEF)) THEN
      strmsg := 'Fixação não considerada no tipo de arranjo 1. ';
      RAISE EXP_ARRANJO01;
    END IF;
  END IF;
  */

  -- Ajusta os valores de acordo com o tipo de arranjo.
  -- O cabo1 pode ser utilizado para os dois tipos de arranjo, porãm, no caso do arranjo 1
  -- alguns valores podem ser nulos

  IF (p_ArranjoId = VALINDEF) THEN
    -- Cria novo Arranjo (ini)
    SPSNP_wrArranjo(p_ArranjoId, p_TipoArranjoId, p_Descricao, p_ExtId, p_Tipico, p_ResTerra, p_Vmin, p_Vmax, p_ColorId, p_Status, p_NumCabos, p_TipoFase, p_DataVersao);

    IF (p_Cabo1Id <> VALINDEF) THEN
      INSERT INTO
        Arranjo_00_01 (intArranjoId, intCaboId, intCaboItem, intTipoFaseId, intSuporteId, intFixacaoItem)
      VALUES
        (p_ArranjoId, p_Cabo1Id, 1, NULLIF(p_TipoFase1Id, VALINDEF), NULLIF(p_SuporteId, VALINDEF), NULLIF(p_FixacaoItem1, VALINDEF));
    END IF;

    IF (p_Cabo2Id <> VALINDEF) THEN
      INSERT INTO
        Arranjo_00_01 (intArranjoId, intCaboId, intCaboItem, intTipoFaseId, intSuporteId, intFixacaoItem)
      VALUES
        (p_ArranjoId, p_Cabo2Id, 2, p_TipoFase2Id, p_SuporteId, p_FixacaoItem2);
    END IF;

    IF (p_Cabo3Id <> VALINDEF) THEN
      INSERT INTO
        Arranjo_00_01 (intArranjoId, intCaboId, intCaboItem, intTipoFaseId, intSuporteId, intFixacaoItem)
      VALUES
        (p_ArranjoId, p_Cabo3Id, 3, p_TipoFase3Id, p_SuporteId, p_FixacaoItem3);
    END IF;

    IF (p_Cabo4Id <> VALINDEF) THEN
      INSERT INTO
        Arranjo_00_01 (intArranjoId, intCaboId, intCaboItem, intTipoFaseId, intSuporteId, intFixacaoItem)
      VALUES
        (p_ArranjoId, p_Cabo4Id, 4, p_TipoFase4Id, p_SuporteId, p_FixacaoItem4);
    END IF;

    -- Cria novo Arranjo (fim)
  ELSE
    -- Atualiza Arranjo existente (ini)
    SPSNP_wrArranjo(p_ArranjoId, p_TipoArranjoId, p_Descricao, p_ExtId, p_Tipico, p_ResTerra, p_Vmin, p_Vmax, p_ColorId, p_Status, p_NumCabos, p_TipoFase, p_DataVersao);

    UPDATE
      Arranjo_00_01
    SET
      intSuporteId = NULLIF(p_SuporteId, VALINDEF),
      intFixacaoItem = NULLIF(p_FixacaoItem1, VALINDEF)
    WHERE
      intArranjoId = p_ArranjoId AND
      intCaboId = p_Cabo1Id AND
      intCaboItem = 1 AND
      ((p_TipoArranjoId = 1) OR (intTipoFaseId = p_TipoFase1Id));
    IF (p_Cabo2Id <> VALINDEF) THEN
      UPDATE
        Arranjo_00_01
      SET
        intSuporteId = p_SuporteId,
        intFixacaoItem = p_FixacaoItem2
      WHERE
        intArranjoId = p_ArranjoId AND
        intCaboId = p_Cabo2Id AND
        intCaboItem = 2 AND
        intTipoFaseId = p_TipoFase2Id;
    END IF;

    IF (p_Cabo3Id <> VALINDEF) THEN
      UPDATE
        Arranjo_00_01
      SET
        intSuporteId = p_SuporteId,
        intFixacaoItem = p_FixacaoItem3
      WHERE
        intArranjoId = p_ArranjoId AND
        intCaboId = p_Cabo3Id AND
        intCaboItem = 3 AND
        intTipoFaseId = p_TipoFase3Id;
    END IF;

    IF (p_Cabo4Id <> VALINDEF) THEN
      UPDATE
        Arranjo_00_01
      SET
        intSuporteId = p_SuporteId,
        intFixacaoItem = p_FixacaoItem4
      WHERE
        intArranjoId = p_ArranjoId AND
        intCaboId = p_Cabo4Id AND
        intCaboItem = 4 AND
        intTipoFaseId = p_TipoFase4Id;
    END IF;

  -- Atualiza Arranjo existente (fim)
  END IF;
   EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrArranjo00;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrArranjo01 ( p_ArranjoId IN OUT NUMBER, p_TipoArranjoId NUMBER, p_Descricao VARCHAR, p_ExtId VARCHAR, p_Tipico NUMBER,
                              p_ResTerra NUMBER, p_Vmin NUMBER, p_Vmax NUMBER, p_ColorId NUMBER, p_Status NUMBER, p_NumCabos NUMBER, p_TipoFase NUMBER,
                              p_CaboId NUMBER, p_SuporteId NUMBER DEFAULT VALINDEF, p_FixacaoItem NUMBER DEFAULT VALINDEF,
                              p_DataVersao DATE
                            )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrArranjo01                                            **
-- **                                                                        **
-- ** Insere ou atualiza um arranjo do tipo 01. No arranjo tipo 01 somente é **
-- ** permitida a utilização de cabos RGMGR.                                 **
-- ** Na atualização é considerada a chave primãria da tabela para determina-**
-- ** ção do registro.                                                       **
-- **                                                                        **
-- ** 1..1 cabo multiplexado (Z0Z1), suporte opcional (arranjoCABO_Z0Z1).    **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20100809 |  mml  | Versão inicial.                                     **
-- ** 20130730 |  mml  | Introdução do ExtId.                                **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais
EXP_ARRANJO01 EXCEPTION;
strmsg VARCHAR2(255);
intTipoCaboIdCheck NUMBER;

BEGIN

  SAVEPOINT A;

  strmsg := 'Erro na inserção ou atualização do arranjo do tipo 01. ';
  /*
  SELECT intTipoCaboId INTO intTipoCaboIdCheck FROM Cabo WHERE intCaboId = p_CaboId;
  IF (intTipoCaboIdCheck <> 2) THEN
    strmsg := 'Tipo de cabo invãlido para o arranjo. ';
    RAISE EXP_ARRANJO02;
  END IF;
  */

  -- Suporte é opcional
  IF (p_ArranjoId = VALINDEF) THEN
    -- Cria novo Arranjo (ini)
    SPSNP_wrArranjo(p_ArranjoId, p_TipoArranjoId, p_Descricao, p_ExtId, p_Tipico, p_ResTerra, p_Vmin, p_Vmax, p_ColorId, p_Status, p_NumCabos, p_TipoFase, p_DataVersao);
    INSERT INTO
      Arranjo_00_01 (intArranjoId, intCaboId, intCaboItem, intTipoFaseId, intSuporteId, intFixacaoItem)
    VALUES
      (p_ArranjoId, p_CaboId, 1, p_TipoFase, NULLIF(p_SuporteId, VALINDEF), NULLIF(p_FixacaoItem, VALINDEF));

  -- Cria novo Arranjo (fim)
  ELSE
  -- Atualiza Arranjo existente (ini)
    SPSNP_wrArranjo(p_ArranjoId, p_TipoArranjoId, p_Descricao, p_ExtId, p_Tipico, p_ResTerra, p_Vmin, p_Vmax, p_ColorId, p_Status, p_NumCabos, p_TipoFase, p_DataVersao);
    UPDATE
      Arranjo_00_01
    SET
      intSuporteId = NULLIF(p_SuporteId, VALINDEF),
      intFixacaoItem = p_FixacaoItem
    WHERE
      intArranjoId = p_ArranjoId AND
      intCaboId = p_CaboId;

  -- Atualiza Arranjo existente (fim)
  END IF;

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrArranjo01;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrArranjo02 ( p_ArranjoId IN OUT NUMBER, p_TipoArranjoId NUMBER, p_Descricao VARCHAR, p_ExtId VARCHAR, p_Tipico NUMBER, p_SuporteId NUMBER DEFAULT VALINDEF,
                              p_ResTerra NUMBER, p_Iadm NUMBER, p_Vmin NUMBER, p_Vmax NUMBER, p_ColorId NUMBER, p_Status NUMBER, p_NumCabos NUMBER, p_TipoFase NUMBER,
                              p_Z0R NUMBER, p_Z0X NUMBER, p_Z1R NUMBER, p_Z1X NUMBER,
                              p_C0 NUMBER, p_C1 NUMBER,
                              p_DataVersao DATE
                            )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrArranjo02                                            **
-- **                                                                        **
-- ** Unidade das grandezas: ohm / km, F / km e A.                           **
-- **                                                                        **
-- ** arranjo Z0Z1 / C0C1 [ ohm/km ] (arranjoZ0Z1)                           **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20100809 |  mml  | Versão inicial.                                     **
-- ** 20130730 |  mml  | Introdução do ExtId.                                **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais
EXP_ARRANJO03 EXCEPTION;
strmsg VARCHAR2(255);
intTipoArranjoIdAux NUMBER;

BEGIN

  strmsg := 'Erro na inserção ou atualização do arranjo do tipo 02/04. ';

  SAVEPOINT A;

  -- Suporte é opcional
  IF (p_ArranjoId = VALINDEF) THEN
    -- Cria novo Arranjo (ini)

    -- Cria ou atualiza o registro existente na tabela Equipamento
    SPSNP_wrArranjo(p_ArranjoId, p_TipoArranjoId, p_Descricao, p_ExtId, p_Tipico, p_ResTerra, p_Vmin, p_Vmax, p_ColorId, p_Status, p_NumCabos, p_TipoFase, p_DataVersao);

    INSERT INTO
      Arranjo_02_04 ( intArranjoId, fltZ0R, fltZ0X, fltZ1R, fltZ1X, fltC0, fltC1, fltIadm, intSuporteId )
    VALUES
      ( p_ArranjoId, p_Z0R, p_Z0X, p_Z1R, p_Z1X, p_C0, p_C1, p_Iadm, NULLIF(p_SuporteId, VALINDEF) );

  -- Cria novo Arranjo (fim)
  ELSE
  -- Atualiza Arranjo existente (ini)
    SELECT intTipoArranjoId INTO intTipoArranjoIdAux FROM Arranjo WHERE intArranjoId = p_ArranjoId;
    IF (p_TipoArranjoId NOT IN (2, 4)) THEN
      strmsg := 'Arranjo não pode ser atualizado, pois não é do tipo 02/04. ';
    END IF;

    SPSNP_wrArranjo(p_ArranjoId, p_TipoArranjoId, p_Descricao, p_ExtId, p_Tipico, p_ResTerra, p_Vmin, p_Vmax, p_ColorId, p_Status, p_NumCabos, p_TipoFase, p_DataVersao);
    UPDATE
      Arranjo_02_04
    SET
      fltZ0R       = p_Z0R,
      fltZ0X       = p_Z0X,
      fltZ1R       = p_Z1R,
      fltZ1X       = p_Z1X,
      fltC0        = p_C0,
      fltC1        = p_C1,
      fltIadm      = p_Iadm,
      intSuporteId = NULLIF(p_SuporteId, VALINDEF)
    WHERE
      intArranjoId = p_ArranjoId;

  -- Atualiza Arranjo existente (fim)
  END IF;

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrArranjo02;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrArranjo04 ( p_ArranjoId IN OUT NUMBER, p_TipoArranjoId NUMBER, p_Descricao VARCHAR, p_ExtId VARCHAR, p_Tipico NUMBER, p_SuporteId NUMBER DEFAULT VALINDEF,
                              p_ResTerra NUMBER, p_Iadm NUMBER, p_Vmin NUMBER, p_Vmax NUMBER, p_ColorId NUMBER, p_Status NUMBER, p_NumCabos NUMBER, p_TipoFase NUMBER,
                              p_Z0R NUMBER, p_Z0X NUMBER, p_Z1R NUMBER, p_Z1X NUMBER,
                              p_C0 NUMBER, p_C1 NUMBER,
                              p_DataVersao DATE
                            )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrArranjo04                                            **
-- **                                                                        **
-- ** Unidade das grandezas: PU.                                             **
-- **                                                                        **
-- ** arranjo Z0Z1 / C0C1 [ PU Total ] (arranjoPU_SB100)                     **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20100809 |  mml  | Versão inicial.                                     **
-- ** 20130730 |  mml  | Introdução do ExtId.                                **
-- **                                                                        **
-- ****************************************************************************

BEGIN

  SPSNP_wrArranjo02 ( p_ArranjoId, p_TipoArranjoId, p_Descricao, p_ExtId, p_Tipico, p_SuporteId,
                      p_ResTerra, p_Iadm, p_Vmin, p_Vmax, p_ColorId, p_Status, p_NumCabos, p_TipoFase,
                      p_Z0R, p_Z0X, p_Z1R, p_Z1X, p_C0, p_C1, p_DataVersao
                    );

END SPSNP_wrArranjo04;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrArranjo03 ( p_ArranjoId IN OUT NUMBER, p_TipoArranjoId NUMBER, p_Descricao VARCHAR, p_ExtId VARCHAR, p_Tipico NUMBER,
                              p_ResTerra NUMBER, p_Iadm NUMBER, p_Vmin NUMBER, p_Vmax NUMBER, p_ColorId NUMBER, p_Status NUMBER, p_NumCabos NUMBER, p_TipoFase NUMBER,
                              p_MatZ BLOB, p_MatC BLOB, p_DataVersao DATE
                            )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrArranjo03                                            **
-- **                                                                        **
-- ** matriz de impedãncias (4x4: A,B,C,N x A,B,C,N) (arranjoMAT_Z)          **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20100809 |  mml  | Versão inicial.                                     **
-- ** 20130730 |  mml  | Introdução do ExtId.                                **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais
EXP_ARRANJO05 EXCEPTION;
strmsg VARCHAR2(255);
intTipoArranjoIdAux NUMBER;

BEGIN

  strmsg := 'Erro na inserção ou atualização do arranjo do tipo 05. ';

  SAVEPOINT A;

  IF (p_ArranjoId = VALINDEF) THEN
    -- Cria novo Arranjo (ini)

    SPSNP_wrArranjo(p_ArranjoId, p_TipoArranjoId, p_Descricao, p_ExtId, p_Tipico, p_ResTerra, p_Vmin, p_Vmax, p_ColorId, p_Status, p_NumCabos, p_TipoFase, p_DataVersao);
    INSERT INTO
      Arranjo_03 ( intArranjoId, blbMatZ, blbMatC, fltIadm )
    VALUES
      ( p_ArranjoId, p_MatZ, p_MatC, p_Iadm );

  -- Cria novo Arranjo (fim)
  ELSE
  -- Atualiza Arranjo existente (ini)
    SELECT intTipoArranjoId INTO intTipoArranjoIdAux FROM Arranjo WHERE intArranjoId = p_ArranjoId;
    IF (intTipoArranjoIdAux <> 3) THEN
      strmsg := 'Arranjo não pode ser atualizado, pois não é do tipo 03. ';
    END IF;

    -- Cria ou atualiza o registro existente na tabela Equipamento
    SPSNP_wrArranjo(p_ArranjoId, p_TipoArranjoId, p_Descricao, p_ExtId, p_Tipico, p_ResTerra, p_Vmin, p_Vmax, p_ColorId, p_Status, p_NumCabos, p_TipoFase, p_DataVersao);
    UPDATE
      Arranjo_03
    SET
      blbMatZ = p_MatZ,
      blbMatC = p_MatC,
      fltIadm = p_Iadm
    WHERE
      intArranjoId = p_ArranjoId;

  -- Atualiza Arranjo existente (fim)
  END IF;

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;


END SPSNP_wrArranjo03;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdCabo ( p_CaboId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure p_CaboId                                                     **
-- **                                                                        **
-- ** Efetua a leitura dos cabos contidos na base de dados.                  **
-- ** Se o arranjo não for especificado, retorna dados de todos os arranjos. **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20100928 |  mml  | Versão inicial.                                     **
-- ** 20130730 |  mml  | Introdução do ExtId.                                **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
  OPEN spcursor FOR
  SELECT
    Cabo.intCaboId          AS CaboId,
    Cabo.intTipoCaboId      AS TipoCaboId,
    Cabo.strDescricao       AS Descricao,
    NVL(Cabo.strExtId, '')  AS ExtId,
    Cabo.fltInom            AS Inom,
    Cabo.fltVmin            AS Vmin,
    Cabo.fltVmax            AS Vmax,
    Cabo.intStatus          AS Status,
    NVL(CaboRGMR.fltR, 0)   AS RGMR_R,
    NVL(CaboRGMR.fltGMR, 0) AS RGMR_GMR,
    0                       AS Z0Z1_Z0R,
    0                       AS Z0Z1_Z0X,
    0                       AS Z0Z1_Z1R,
    0                       AS Z0Z1_Z1X,
    0                       AS Z0Z1_C0,
    0                       AS Z0Z1_C1
  FROM
    Cabo INNER JOIN CaboRGMR ON
      Cabo.intCaboId = CaboRGMR.intCaboId
  WHERE
    ((p_CaboId = VALINDEF) OR (Cabo.intCaboId = p_CaboId))

  UNION

  SELECT
    Cabo.intCaboId          AS CaboId,
    Cabo.intTipoCaboId      AS TipoCaboId,
    Cabo.strDescricao       AS Descricao,
    NVL(Cabo.strExtId, '')  AS ExtId,
    Cabo.fltInom            AS Inom,
    Cabo.fltVmin            AS Vmin,
    Cabo.fltVmax            AS Vmax,
    Cabo.intStatus          AS Status,
    0                       AS RGMR_R,
    0                       AS RGMR_GMR,
    NVL(CaboZ0Z1.fltZ0R, 0) AS Z0Z1_Z0R,
    NVL(CaboZ0Z1.fltZ0X, 0) AS Z0Z1_Z0X,
    NVL(CaboZ0Z1.fltZ1R, 0) AS Z0Z1_Z1R,
    NVL(CaboZ0Z1.fltZ1X, 0) AS Z0Z1_Z1X,
    NVL(CaboZ0Z1.fltC0, 0)  AS Z0Z1_C0,
    NVL(CaboZ0Z1.fltC1, 0)  AS Z0Z1_C1
  FROM
    Cabo INNER JOIN CaboZ0Z1 ON
      Cabo.intCaboId = CaboZ0Z1.intCaboId
  WHERE
    ((p_CaboId = VALINDEF) OR (Cabo.intCaboId = p_CaboId));

END SPSNP_rdCabo;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdSuporte ( p_SuporteId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdSuporte                                              **
-- **                                                                        **
-- ** Efetua a leitura dos suportes contidos na base de dados.               **
-- ** Se o arranjo não for especificado, retorna dados de todos os arranjos. **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20100928 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
  OPEN spcursor FOR
  SELECT
    Suporte.intSuporteId          AS SuporteId,
    Suporte.intTipoSuporteId      AS TipoSuporteId,
    Suporte.strDescricao          AS Descricao,
    NVL(Suporte.strExtId, '')     AS ExtId,
    Suporte.fltVmin               AS Vmin,
    Suporte.fltVmax               AS Vmax,
    Suporte.intNumPontosFix       AS NumPontosFix,
    Suporte.intStatus             AS Status,
    SuporteFixacao.intFixacaoItem AS FixacaoItem,
    SuporteFixacao.fltX           AS FixacaoX,
    SuporteFixacao.fltY           AS FixacaoY,
    SuporteFixacao.strCodigo      AS FixacaoCodigo
  FROM
    Suporte INNER JOIN SuporteFixacao ON
      Suporte.intSuporteId = SuporteFixacao.intSuporteId
  WHERE
    ((p_SuporteId = VALINDEF) OR (Suporte.intSuporteId = p_SuporteId))
  ORDER BY
    Suporte.intSuporteId;

END SPSNP_rdSuporte;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdSuporteUtilizado ( p_SuporteId NUMBER DEFAULT VALINDEF, p_NumVezes OUT NUMBER )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdSuporteUtilizado                                     **
-- **                                                                        **
-- ** Retona o número de vezes que o suporte é utilizado. Foram criados dois **
-- ** índices para melhoria de desempenho, nas tabelas de arranjo que utili- **
-- ** zam suporte.                                                           **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20110719 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais
conta NUMBER;

BEGIN
   SELECT NVL(COUNT(*), 0) INTO conta FROM ARRANJO_00_01 WHERE intSuporteId = p_SuporteId;
   p_NumVezes := conta;

   SELECT NVL(COUNT(*), 0)INTO conta FROM ARRANJO_02_04 WHERE intSuporteId = p_SuporteId;
   p_NumVezes := p_NumVezes + conta;

END SPSNP_rdSuporteUtilizado;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdSuporteUtilizado ( p_SuporteId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdArranjoUtilizado                                     **
-- **                                                                        **
-- ** Retona o número de vezes que o suporte é utilizado. Foram criados dois **
-- ** índices para melhoria de desempenho, nas tabelas de arranjo que utili- **
-- ** zam suporte.                                                           **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20110719 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais
conta NUMBER;

BEGIN

   OPEN spcursor FOR
   SELECT
     Arranjo.intArranjoId AS ArranjoId,
     Arranjo.strDescricao AS ArranjoDescricao
   FROM
     ARRANJO_00_01
       INNER JOIN Arranjo ON
         ARRANJO_00_01.intArranjoId = Arranjo.intArranjoId

       INNER JOIN Suporte ON
         ARRANJO_00_01.intSuporteId = Suporte.intSuporteId
   WHERE
     Suporte.intSuporteId = p_SuporteId

   UNION

   SELECT
     Arranjo.intArranjoId AS ArranjoId,
     Arranjo.strDescricao AS ArranjoDescricao
   FROM
     ARRANJO_02_04
       INNER JOIN Arranjo ON
         ARRANJO_02_04.intArranjoId = Arranjo.intArranjoId

       INNER JOIN Suporte ON
         ARRANJO_02_04.intSuporteId = Suporte.intSuporteId
   WHERE
     Suporte.intSuporteId = p_SuporteId;

END SPSNP_rdSuporteUtilizado;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdArranjo_00_01 ( p_ArranjoId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdArranjo_00_01                                        **
-- **                                                                        **
-- ** Efetua a leitura dos arranjos tipo 0 e 1  contidos na base de dados.   **
-- ** Se o arranjo não for especificado, retorna dados de todos os arranjos. **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20100928 |  mml  | Versão inicial.                                     **
-- ** 20130730 |  mml  | Introdução do ExtId.                                **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
  OPEN spcursor FOR
  SELECT
    ArjTipo.intArranjoId                  AS ArranjoId,
    Arranjo.intTipoArranjoId              AS TipoArranjoId,
    Arranjo.strDescricao                  AS Descricao,
    NVL(Arranjo.strExtId, '')             AS ExtId,
    Arranjo.bolTipico                     AS Tipico,
    Arranjo.fltResTerra                   AS ResTerra,
    Arranjo.fltVmin                       AS Vmin,
    Arranjo.fltVmax                       AS Vmax,
    Arranjo.intColorId                    AS ColorId,
    Arranjo.intStatus                     AS Status,
    Arranjo.intNumCabos                   AS NumCabos,
    Arranjo.intTipoFaseId                 AS ArranjoTipoFaseId,
    Arranjo.dttVersao                     AS DataVersao,
    ArjTipo.intCaboId                     AS CaboId,
    ArjTipo.intCaboItem                   AS CaboItem,
    NVL(ArjTipo.intTipoFaseId, VALINDEF)  AS TipoFaseId,
    NVL(ArjTipo.intSuporteId, VALINDEF)   AS SuporteId,
    NVL(ArjTipo.intFixacaoItem, VALINDEF) AS FixacaoItem
  FROM
    Arranjo_00_01 ArjTipo
      INNER JOIN Arranjo ON
        ArjTipo.intArranjoId = Arranjo.intArranjoId
  WHERE
    (
      ((p_ArranjoId = VALINDEF) AND (Arranjo.bolTipico = 1)) OR
      (ArjTipo.intArranjoId = p_ArranjoId)
    )
  ORDER BY
    ArjTipo.intArranjoId,
    ArjTipo.intTipoFaseId;

END SPSNP_rdArranjo_00_01;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdArranjo_02_04 ( p_ArranjoId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdArranjo_02_04                                        **
-- **                                                                        **
-- ** Efetua a leitura dos arranjos tipo 2 e 4  contidos na base de dados.   **
-- ** Se o arranjo não for especificado, retorna dados de todos os arranjos. **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20100928 |  mml  | Versão inicial.                                     **
-- ** 20130730 |  mml  | Introdução do ExtId.                                **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
  OPEN spcursor FOR
  SELECT
    ArjTipo.intArranjoId                AS ArranjoId,
    Arranjo.intTipoArranjoId            AS TipoArranjoId,
    Arranjo.strDescricao                AS Descricao,
    NVL(Arranjo.strExtId, '')           AS ExtId,
    Arranjo.bolTipico                   AS Tipico,
    Arranjo.fltResTerra                 AS ResTerra,
    Arranjo.fltVmin                     AS Vmin,
    Arranjo.fltVmax                     AS Vmax,
    Arranjo.intColorId                  AS ColorId,
    Arranjo.intStatus                   AS Status,
    Arranjo.intTipoFaseId               AS ArranjoTipoFaseId,
    Arranjo.dttVersao                   AS DataVersao,
    ArjTipo.fltZ0R                      AS Z0R,
    ArjTipo.fltZ0X                      AS Z0X,
    ArjTipo.fltZ1R                      AS Z1R,
    ArjTipo.fltZ1X                      AS Z1X,
    ArjTipo.fltC0                       AS C0,
    ArjTipo.fltC1                       AS C1,
    ArjTipo.fltIadm                     AS Iadm,
    NVL(ArjTipo.intSuporteId, VALINDEF) AS SuporteId
  FROM
    Arranjo_02_04 ArjTipo
      INNER JOIN Arranjo ON
        ArjTipo.intArranjoId = Arranjo.intArranjoId
  WHERE
    (
      ((p_ArranjoId = VALINDEF) AND (Arranjo.bolTipico = 1)) OR
      (ArjTipo.intArranjoId = p_ArranjoId)
    );

END SPSNP_rdArranjo_02_04;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdArranjo_03 ( p_ArranjoId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdArranjo_03                                           **
-- **                                                                        **
-- ** Efetua a leitura dos arranjos tipo 3  contidos na base de dados.       **
-- ** Se o arranjo não for especificado, retorna dados de todos os arranjos. **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20100928 |  mml  | Versão inicial.                                     **
-- ** 20130730 |  mml  | Introdução do ExtId.                                **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
  OPEN spcursor FOR
  SELECT
    ArjTipo.intArranjoId          AS ArranjoId,
    Arranjo.intTipoArranjoId      AS TipoArranjoId,
    Arranjo.strDescricao          AS Descricao,
    NVL(Arranjo.strExtId, '')     AS ExtId,
    Arranjo.bolTipico             AS Tipico,
    Arranjo.fltResTerra           AS ResTerra,
    Arranjo.fltVmin               AS Vmin,
    Arranjo.fltVmax               AS Vmax,
    Arranjo.intColorId            AS ColorId,
    Arranjo.intStatus             AS Status,
    Arranjo.intTipoFaseId         AS ArranjoTipoFaseId,
    Arranjo.dttVersao             AS DataVersao,
    ArjTipo.blbMatZ               AS MatZ,
    ArjTipo.blbMatC               AS MatC,
    ArjTipo.fltIadm               AS Iadm
  FROM
    Arranjo_03 ArjTipo
      INNER JOIN Arranjo ON
        ArjTipo.intArranjoId = Arranjo.intArranjoId
  WHERE
    (
      ((p_ArranjoId = VALINDEF) AND (Arranjo.bolTipico = 1)) OR
      (ArjTipo.intArranjoId = p_ArranjoId)
    );

END SPSNP_rdArranjo_03;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrCabo ( p_CaboId IN OUT NUMBER, p_TipoCaboId NUMBER, p_Descricao VARCHAR, p_ExtId VARCHAR, p_Inom NUMBER, p_Vmin NUMBER, p_Vmax NUMBER, p_Status NUMBER )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrCabo                                                 **
-- **                                                                        **
-- ** Insere ou atualiza um novo cabo. Esta procedure não deve ser utilizada **
-- ** isoladamente, isto ã, devem ser usadas as procedures que especificam   **
-- ** os tipos de cabos a serem inseridos.                                   **
-- ** Na atualização, somente a corrente nominal é atualizada.               **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150511 |  mml  | Versão inicial.                                     **
-- ** 20130730 |  mml  | Introdução do ExtId.                                **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN

  IF (p_CaboId = VALINDEF) THEN
  -- Cria novo Cabo (ini)
    p_CaboId := FNSNP_rdCaboIdProx();
    INSERT INTO
      Cabo ( intCaboId, intTipoCaboId, strDescricao, strExtId, fltInom, fltVmin, fltVmax, intStatus )
    VALUES
      ( p_CaboId, p_TipoCaboId, p_Descricao, p_ExtId, p_Inom, p_Vmin, p_Vmax, p_Status  );
  -- Cria novo Cabo (fim)
  ELSE
  -- Atualiza Cabo existente (ini)
    UPDATE
      Cabo
    SET
      strDescricao = p_Descricao,
      strExtId = p_ExtId,
      fltInom = p_Inom,
      fltVmin = p_Vmin,
      fltVmax = p_Vmax,
      intStatus = p_Status
    WHERE
      intCaboId = p_CaboId AND
      intTipoCaboId = p_TipoCaboId;
  -- Atualiza Cabo existente (fim)
  END IF;

  EXCEPTION
    WHEN OTHERS THEN
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrCabo;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrCaboRGMR ( p_CaboId IN OUT NUMBER, p_Descricao VARCHAR, p_ExtId VARCHAR, p_Inom NUMBER, p_Vmin NUMBER, p_Vmax NUMBER, p_R NUMBER, p_GMR NUMBER, p_Status NUMBER )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrCaboRGMR                                             **
-- **                                                                        **
-- ** Insere ou atualiza um cabo RGMR (monopolar).                           **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150511 |  mml  | Versão inicial.                                     **
-- ** 20130730 |  mml  | Introdução do ExtId.                                **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN

  IF (p_CaboId = VALINDEF) THEN
    -- Cria novo Cabo (ini)
    SPSNP_wrCabo ( p_CaboId, 0 /* RGMR */, p_Descricao, p_ExtId, p_Inom, p_Vmin, p_Vmax, p_Status );
    INSERT INTO
      CaboRGMR ( intCaboId, fltR, fltGMR )
    VALUES
      ( p_CaboId, p_R, p_GMR  );

  -- Cria novo Cabo (fim)
  ELSE
  -- Atualiza Cabo existente (ini)
    SPSNP_wrCabo ( p_CaboId, 0 /* RGMR */, p_Descricao, p_ExtId, p_Inom, p_Vmin, p_Vmax, p_Status );
    UPDATE
      CaboRGMR
    SET
      fltR = p_R,
      fltGMR = p_GMR
    WHERE
      intCaboId = p_CaboId;
  -- Atualiza Cabo existente (fim)
  END IF;

  EXCEPTION
    WHEN OTHERS THEN
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrCaboRGMR;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrCaboZ0Z1 ( p_CaboId IN OUT NUMBER, p_Descricao VARCHAR, p_ExtId VARCHAR, p_Inom NUMBER, p_Vmin NUMBER, p_Vmax NUMBER,
                             p_Z0R NUMBER, p_Z0X NUMBER, p_Z1R NUMBER, p_Z1X NUMBER, p_C0 NUMBER, p_C1 NUMBER, p_Status NUMBER
                           )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrCaboZ0Z1                                             **
-- **                                                                        **
-- ** Insere ou atualiza um cabo Z0Z1.                                       **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150511 |  mml  | Versão inicial.                                     **
-- ** 20130730 |  mml  | Introdução do ExtId.                                **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
  IF (p_CaboId = VALINDEF) THEN
    -- Cria novo Cabo (ini)
    SPSNP_wrCabo ( p_CaboId, 1 /* Z0Z1 */, p_Descricao, p_ExtId, p_Inom, p_Vmin, p_Vmax, p_Status );
    INSERT INTO
      CaboZ0Z1 ( intCaboId, fltZ0R, fltZ0X, fltZ1R, fltZ1X, fltC0, fltC1 )
    VALUES
      ( p_CaboId, p_Z0R, p_Z0X, p_Z1R, p_Z1X, p_C0, p_C1 );
  -- Cria novo Cabo (fim)
  ELSE
  -- Atualiza Cabo existente (ini)
    SPSNP_wrCabo ( p_CaboId, 1 /* Z0Z1 */, p_Descricao, p_ExtId, p_Inom, p_Vmin, p_Vmax, p_Status );
    UPDATE
      CaboZ0Z1
    SET
      fltZ0R = p_Z0R,
      fltZ0X = p_Z0X,
      fltZ1R = p_Z1R,
      fltZ1X = p_Z1X,
      fltC0  = p_C0,
      fltC1  = p_C1
    WHERE
      intCaboId = p_CaboId;
  -- Atualiza Cabo existente (fim)
  END IF;

  EXCEPTION
    WHEN OTHERS THEN
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrCaboZ0Z1;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrSuporte ( p_SuporteId IN OUT NUMBER, p_TipoSuporteId NUMBER, p_Descricao VARCHAR, p_ExtId VARCHAR, p_Vmin NUMBER, p_Vmax NUMBER, P_NumPontosFix NUMBER, p_Status NUMBER )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrSuporte                                              **
-- **                                                                        **
-- ** Insere ou atualiza um ponto de fixacao em um suporte.                  **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150511 |  mml  | Versão inicial.                                     **
-- ** 20130730 |  mml  | Introdução do ExtId.                                **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
  IF (p_SuporteId = VALINDEF) THEN
    -- Cria novo Suporte (ini)
    p_SuporteId := FNSNP_rdSuporteIdProx();

    INSERT INTO
      Suporte ( intSuporteId, intTipoSuporteId, strDescricao, strExtId, fltVmin, fltVmax, intNumPontosFix, intStatus )
    VALUES
      ( p_SuporteId, p_TipoSuporteId, p_Descricao, p_ExtId, p_Vmin, p_Vmax, P_NumPontosFix, p_Status );
    -- Cria novo Suporte (fim)
  ELSE
    -- Atualiza Suporte existente (ini)
    UPDATE
      Suporte
    SET
      strDescricao = p_Descricao,
      strExtId = p_ExtId,
      fltVmin = p_Vmin,
      fltVmax = p_Vmax,
      intNumPontosFix = p_NumPontosFix,
      intStatus = p_Status
    WHERE
      intSuporteId = p_SuporteId;
    -- Atualiza Suporte existente (fim)
  END IF;

  EXCEPTION
    WHEN OTHERS THEN
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrSuporte;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrSuporteFixacao ( p_SuporteId NUMBER, p_FixacaoItem IN OUT NUMBER, p_X NUMBER, p_Y NUMBER, p_Codigo VARCHAR )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrSuporteFixacao                                       **
-- **                                                                        **
-- ** Insere ou atualiza um ponto de fixacao em um suporte.                  **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150511 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais
BEGIN
  IF (p_FixacaoItem = VALINDEF) THEN
    -- Cria novo ponto de fixacao (ini)
    p_FixacaoItem := FNSNP_rdSuporteFixacaoItemProx(p_SuporteId);

    INSERT INTO
      SuporteFixacao ( intSuporteId, intFixacaoItem, fltX, fltY, strCodigo )
    VALUES
      ( p_SuporteId, p_FixacaoItem, p_X, p_Y, p_Codigo );
    -- Cria novo ponto de fixacao (fim)
  ELSE
    -- Atualiza ponto de fixacao existente (ini)
    UPDATE
      SuporteFixacao
    SET
      fltX = p_X,
      fltY = p_Y,
      strCodigo = p_Codigo
    WHERE
      intSuporteId = p_SuporteId AND
      intFixacaoItem = p_FixacaoItem;
    -- Atualiza ponto de fixacao existente (fim)
  END IF;

  EXCEPTION
    WHEN OTHERS THEN
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrSuporteFixacao;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_dlSuporteFixacao ( p_SuporteId NUMBER )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_dlSuporteFixacao                                       **
-- **                                                                        **
-- ** Elimina todos os pontos de fixação de um suporte.                      **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20100927 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais
BEGIN
  DELETE FROM
    SuporteFixacao
  WHERE
    intSuporteId = p_SuporteId;

  EXCEPTION
    WHEN OTHERS THEN
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_dlSuporteFixacao;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_dlTudo
IS
-- ****************************************************************************
-- ** Procedure SPSNP_dlTudo                                                 **
-- **                                                                        **
-- ** Elimina os equipamentos de um estudo.                                  **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20101228 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Varióveis locais
   tabids_curva   udtNumTable;
   tabids_arranjo udtNumTable;
   maxconta       NUMBER;
   conta          NUMBER;

BEGIN
  SAVEPOINT A;

  DELETE FROM CapacitorSerie;
  DELETE FROM Carga;
  DELETE FROM CargaIP;
  DELETE FROM Capacitor;
  DELETE FROM Gerador;
  DELETE FROM Suprimento;
  DELETE FROM Filtro;
  DELETE FROM Reator;
  DELETE FROM TrafoZZ;
  DELETE FROM Mutua;
  DELETE FROM Chave;
  DELETE FROM Regulador;
  DELETE FROM Trafo;
  DELETE FROM Trafo3E;
  DELETE FROM EquipamentoRede;
  DELETE FROM Rede;
  DELETE FROM Trecho;
  DELETE FROM Arranjo_00_01;
  DELETE FROM Arranjo_02_04;
  DELETE FROM Arranjo_03;
  DELETE FROM Arranjo;
  DELETE FROM SuporteFixacao;
  DELETE FROM Suporte;
  DELETE FROM CaboRGMR;
  DELETE FROM CaboZ0Z1;
  DELETE FROM Cabo;
  -- Elimina possibilidade de erro de integridade referencial
  UPDATE Equipamento SET intBarra1Id = NULL, intBarra2Id = NULL, intBarra3Id = NULL;
  DELETE FROM Barra;
  DELETE FROM Equipamento;
  DELETE FROM Curva;
  DELETE FROM TipoChave;
  DELETE FROM TipoRede;
  
  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN (-2291, -2292)) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';REMOÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';REMOÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_dlTudo;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_dlTipoRede
IS
-- ****************************************************************************
-- ** Procedure SPSNP_dlTipoRede                                             **
-- **                                                                        **
-- ** Elimina os domínios associados a uma determinada rede.                 **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20111019 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
  DELETE FROM TipoRede;

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';REMOÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';REMOÇÃO;'||'MSG: '||SQLERRM);
      END IF;
END SPSNP_dlTipoRede;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_dlTipoChave
IS
-- ****************************************************************************
-- ** Procedure SPSNP_dlTipoChave                                            **
-- **                                                                        **
-- ** Elimina os domínios associados a uma determinada rede.                 **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20111019 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
  -- OOPS
  -- Quando a proteção for migrada para o Oracle, alguns tipos de proteção dependem do tipo de chava
  -- Talvez seja necessario não eliminar os tipos de chave utilizados na tabela FuncProtTipoChave
  DELETE FROM TipoChave;

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';REMOÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';REMOÇÃO;'||'MSG: '||SQLERRM);
      END IF;
END SPSNP_dlTipoChave;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdCapacitorSerie ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdCapacitorSerie                                       **
-- **                                                                        **
-- ** Retorna os capacitores de uma rede específica ou de todas as redes vin-**
-- ** culadas ao estudo.                                                     **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20100805 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

BEGIN
  OPEN spcursor FOR
  SELECT
    Equipamento.strCodigo                      AS Codigo,
    Equipamento.strExtId                       AS ExtId,
    Equipamento.intBarra1Id                    AS Barra1,
    Equipamento.intBarra2Id                    AS Barra2,
    EquipamentoRede.intRedeId                  AS RedeId,
    EqAux.intCapacitorSerieId                  AS CapacitorSerieId,
    EqAux.fltVnom                              AS Vnom,
    EqAux.fltSnom                              AS Snom,
    EqAux.fltZR                                AS ZR,
    EqAux.fltZX                                AS ZX
  FROM
    CapacitorSerie EqAux
      INNER JOIN Equipamento ON
        EqAux.intCapacitorSerieId = Equipamento.intEquipId

      INNER JOIN EquipamentoRede ON
        Equipamento.intEquipId = EquipamentoRede.intEquipId
  WHERE
    ((p_RedeId = VALINDEF) OR (EquipamentoRede.intRedeId = p_RedeId));

END SPSNP_rdCapacitorSerie;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrCapacitorSerie ( p_CapacitorSerieId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_Barra1Id NUMBER, p_Barra2Id NUMBER,
                                   p_Vnom NUMBER, p_Snom NUMBER, p_ZR NUMBER, p_ZX NUMBER
                                 )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrCapacitorSerie                                       **
-- **                                                                        **
-- ** Se a identificação do capacitor for nula, cria um novo capacitor. Caso **
-- ** contrãrio, atualiza os valores passados como parãmetro.                **
-- ** Ao final, retorna o identificador do capacitor criado ou atualizado.   **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20100805 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais
BEGIN
  SAVEPOINT A;

  -- Cria o equipamento
  SPSNP_wrEquipamento( p_CapacitorSerieId, 6 /*p_TipoEquipId*/, p_Codigo, p_ExtId, VALINDEF, p_Barra1Id, p_Barra2Id );

  -- Cria novo capacitor série
  INSERT INTO
    CapacitorSerie (intCapacitorSerieId, fltVnom, fltSnom, fltZR, fltZX)
  VALUES
    (p_CapacitorSerieId, p_Vnom, p_Snom, p_ZR, p_ZX);

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrCapacitorSerie;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdTrecho ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdTrecho                                               **
-- **                                                                        **
-- ** Retorna todos os trechos vinculados a uma rede específica.             **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20100915 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

BEGIN
  OPEN spcursor FOR

  SELECT
    Equipamento.strCodigo                      AS Codigo,
    Equipamento.strExtId                       AS ExtId,
    Equipamento.intBarra1Id                    AS Barra1,
    Equipamento.intBarra2Id                    AS Barra2,
    EquipamentoRede.intRedeId                  AS RedeId,
    EqAux.intTrechoId                          AS TrechoId,
    EqAux.fltComprimento                       AS Comprimento,
    EqAux.intArranjoId                         AS ArranjoId,
    Arranjo.intTipoArranjoId                   AS TipoArranjoId,
    Arranjo.bolTipico                          AS Tipico
  FROM
    Trecho EqAux
      INNER JOIN Equipamento ON
        EqAux.intTrechoId = Equipamento.intEquipId

      INNER JOIN EquipamentoRede ON
        Equipamento.intEquipId = EquipamentoRede.intEquipId

      INNER JOIN Arranjo ON
        EqAux.intArranjoId = Arranjo.intArranjoId
  WHERE
    ((p_RedeId = VALINDEF) OR (EquipamentoRede.intRedeId = p_RedeId));

END SPSNP_rdTrecho;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrTrecho ( p_TrechoId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_Barra1Id NUMBER, p_Barra2Id NUMBER,
                           p_Comprimento FLOAT, p_ArranjoId NUMBER
                         )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrTrecho                                               **
-- **                                                                        **
-- ** Insere um trecho. Note que é necessário especificar o suporte que foi  **
-- ** utilizado.                                                             **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150511 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
  SAVEPOINT A;

  -- Cria o equipamento
  SPSNP_wrEquipamento( p_TrechoId, 28 /*p_TipoEquipId*/, p_Codigo, p_ExtId, VALINDEF, p_Barra1Id, p_Barra2Id );

  -- Cria novo trecho
  INSERT INTO
    Trecho (intTrechoId, fltComprimento, intArranjoId)
  VALUES
    (p_TrechoId, p_Comprimento, p_ArranjoId);

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrTrecho;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdCarga ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdCarga                                                **
-- **                                                                        **
-- ** Retorna a carga vinculada a uma rede específica ou todas as cargas,    **
-- ** caso o parãmetro passado tenha o valor -1.                             **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150511 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

BEGIN
  OPEN spcursor FOR
  SELECT
    Equipamento.strCodigo                      AS Codigo,
    Equipamento.strExtId                       AS ExtId,
    Equipamento.intBarra1Id                    AS Barra1,
    EquipamentoRede.intRedeId                  AS RedeId,
    EqAux.intCargaId                           AS CargaId,
    EqAux.intTipoFaseId                        AS TipoFaseId,
    TipoConsumidor.intTag                      AS TipoConsTag, -- No monta espera TAG e não o ID
    EqAux.fltEnergia                           AS Energia,
    Curva.bolTipica                            AS Tipica,
    Curva.intCurvaId                           AS CurvaId,
    Curva.blbPontos                            AS Pontos,
    EqAux.fltICtepu                            AS ICtePu,
    EqAux.fltZCtepu                            AS ZCtePu,
    EqAux.fltSCtepu                            AS SCtePu,
    EqAux.intNumConsRes                        AS NumConsRes,
    EqAux.intNumConsCom                        AS NumConsCom,
    EqAux.intNumConsInd                        AS NumConsInd,
    EqAux.intNumConsRur                        AS NumConsRur,
    EqAux.intNumConsOut                        AS NumConsOut,
    EqAux.intNumConsA4                         AS NumConsA4
  FROM
    Carga EqAux
      INNER JOIN Equipamento ON
        EqAux.intCargaId = Equipamento.intEquipId

      INNER JOIN EquipamentoRede ON
        Equipamento.intEquipId = EquipamentoRede.intEquipId

      INNER JOIN TipoConsumidor ON
        EqAux.intTipoConsId = TipoConsumidor.intTipoConsId

      INNER JOIN Curva ON
        Equipamento.intCurvaId = Curva.intCurvaId
  WHERE
    ((p_RedeId = VALINDEF) OR (EquipamentoRede.intRedeId = p_RedeId));

END SPSNP_rdCarga;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrCarga ( p_CargaId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_CurvaId NUMBER, p_Barra1Id NUMBER,
                          p_TipoFaseId NUMBER, p_TipoConsId NUMBER, p_Energia NUMBER, p_ICtePu NUMBER, p_ZCtePu NUMBER, p_SCtePu NUMBER,
                          p_NumConsRes NUMBER, p_NumConsCom NUMBER, p_NumConsInd NUMBER, p_NumConsRur NUMBER, p_NumConsOut NUMBER, p_NumConsA4 NUMBER
                        )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrCarga                                                **
-- **                                                                        **
-- ** Insere uma carga.                                                      **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150511 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
  SAVEPOINT A;

  -- Cria o equipamento
  SPSNP_wrEquipamento( p_CargaId, 7 /*p_TipoEquipId*/, p_Codigo, p_ExtId, p_CurvaId, p_Barra1Id );

  -- Cria nova carga
  INSERT INTO
    Carga (intCargaId, intTipoFaseId, intTipoConsId, fltEnergia, fltICtepu, fltZCtepu, fltSCtepu, intNumConsRes, intNumConsCom, intNumConsInd, intNumConsRur, intNumConsOut, intNumConsA4 )
  VALUES
    ( p_CargaId, p_TipoFaseId, p_TipoConsId, p_Energia, p_ICtePu, p_ZCtePu, p_SCtePu, p_NumConsRes, p_NumConsCom, p_NumConsInd, p_NumConsRur, p_NumConsOut, p_NumConsA4);

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrCarga;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdCargaNL ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdCargaNL                                              **
-- **                                                                        **
-- ** Retorna a carga harmônica (Não Linear) vinculada a uma rede específica **
-- ** ou todas as cargas, caso o parâmetro passado tenha o valor -1.         **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20110206 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

BEGIN
  OPEN spcursor FOR
  SELECT
    Equipamento.strCodigo                      AS Codigo,
    Equipamento.strExtId                       AS ExtId,
    Equipamento.intBarra1Id                    AS Barra1,
    EquipamentoRede.intRedeId                  AS RedeId,
    EqAux.intCargaNLId                         AS CargaNLId,
    EqAux.intOrdemId                           AS OrdemId,
    EqAux.fltFatorPot                          AS FatorPot,
    EqAux.bolIndutivo                          AS Indutivo,
    EqAux.bolkVA                               AS kVA,
    EqAux.fltSNomA                             AS SNomA,
    EqAux.fltSNomB                             AS SNomB,
    EqAux.fltSNomC                             AS SNomC,
    EqAux.fltAngA                              AS AngA,
    EqAux.fltAngB                              AS AngB,
    EqAux.fltAngC                              AS AngC
  FROM
    CargaNL EqAux
      INNER JOIN Equipamento ON
        EqAux.intCargaNLId = Equipamento.intEquipId

      INNER JOIN EquipamentoRede ON
        Equipamento.intEquipId = EquipamentoRede.intEquipId
  WHERE
    ((p_RedeId = VALINDEF) OR (EquipamentoRede.intRedeId = p_RedeId))
  ORDER BY
    Equipamento.intEquipId,
    EqAux.intOrdemId;

END SPSNP_rdCargaNL;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrCargaNL ( p_CargaNLId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_Barra1Id NUMBER,
                            p_OrdemId NUMBER, p_FatorPot NUMBER, p_Indutivo NUMBER, p_kVA NUMBER,
                            p_SNomA NUMBER, p_SNomB NUMBER, p_SNomC NUMBER, p_AngA NUMBER, p_AngB NUMBER, p_AngC NUMBER
                          )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrCargaNL                                              **
-- **                                                                        **
-- ** Insere uma carga harmônica (Não Linear).                               **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20120206 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais
BEGIN
  SAVEPOINT A;

  -- Cria o equipamento
  SPSNP_wrEquipamento( p_CargaNLId, 7 /*p_TipoEquipId*/, p_Codigo, p_ExtId, VALINDEF, p_Barra1Id );

  -- Cria nova carga
  INSERT INTO
    CargaNL (intCargaNLId, intOrdemId, fltFatorPot, bolIndutivo, bolkVA, fltSNomA, fltSNomB, fltSNomC, fltAngA, fltAngB, fltAngC)
  VALUES
    ( p_CargaNLId, p_OrdemId, p_FatorPot, p_Indutivo, p_kVA, p_SNomA, p_SNomB, p_SNomC, p_AngA, p_AngB, p_AngC );

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrCargaNL;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdCargaIP ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdCargaIP                                              **
-- **                                                                        **
-- ** Retorna todas as cargas IP vinculadas à rede passada como parâmetro.   **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150511 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

BEGIN

  OPEN spcursor FOR
  SELECT
    Equipamento.strCodigo                      AS Codigo,
    Equipamento.strExtId                       AS ExtId,
    Equipamento.intBarra1Id                    AS Barra1,
    EquipamentoRede.intRedeId                  AS RedeId,
    EqAux.intCargaIPId                         AS CargaIPId,
    EqAux.intTipoFaseId                        AS TipoFaseId,
    EqAux.fltPnom                              AS Pnom,
    EqAux.fltFatorPot                          AS FatPot,
    EqAux.intNumLamp                           AS NumLamp,
    EqAux.fltICtePU                            AS ICtePU,
    EqAux.fltZCtePU                            AS ZCtePU,
    EqAux.fltSCtePU                            AS SCtePU,
    Curva.bolTipica                            AS Tipica,
    Curva.intCurvaId                           AS CurvaId,
    Curva.blbPontos                            AS Pontos
  FROM
    CargaIP EqAux
      INNER JOIN Equipamento ON
        EqAux.intCargaIPId = Equipamento.intEquipId

      INNER JOIN EquipamentoRede ON
        Equipamento.intEquipId = EquipamentoRede.intEquipId

      INNER JOIN Curva ON
        Equipamento.intCurvaId = Curva.intCurvaId

  WHERE
    ((p_RedeId = VALINDEF) OR (EquipamentoRede.intRedeId = p_RedeId));

END SPSNP_rdCargaIP;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrCargaIP ( p_CargaIPId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_CurvaId NUMBER, p_Barra1Id NUMBER,
                            p_Pnom NUMBER, p_FatorPot NUMBER, p_NumLamp NUMBER, p_TipoFaseId NUMBER,
                            p_ICte NUMBER, p_ZCte NUMBER, p_SCte NUMBER
                          )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrCargaIP                                              **
-- **                                                                        **
-- ** Insere uma carga de iluminação pública.                                **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150511 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
  SAVEPOINT A;

  -- Cria o equipamento
  SPSNP_wrEquipamento( p_CargaIPId, 7 /*p_TipoEquipId*/, p_Codigo, p_ExtId, p_CurvaId, p_Barra1Id );

  -- Cria nova carga
  INSERT INTO
    CargaIP (intCargaIPId, intTipoFaseId, fltPnom, fltFatorPot, intNumLamp, fltICtePU, fltZCtePU, fltSCtePU )
  VALUES
    ( p_CargaIPId, p_TipoFaseId, p_Pnom, p_FatorPot, p_NumLamp, p_ICte, p_ZCte, p_SCte );

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrCargaIP;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdCapacitor ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdCapacitor                                            **
-- **                                                                        **
-- ** Retorna os capacitores associados à rede passada como parâmetro.       **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150511 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

BEGIN

  OPEN spcursor FOR
  SELECT
    Equipamento.strCodigo                      AS Codigo,
    Equipamento.strExtId                       AS ExtId,
    Equipamento.intBarra1Id                    AS Barra1,
    EquipamentoRede.intRedeId                  AS RedeId,
    EqAux.intCapacitorId                       AS CapacitorId,
    EqAux.intTipoLigacaoId                     AS TipoLigacaoId,
    EqAux.fltQnom                              AS Qnom,
    EqAux.fltVnom                              AS Vnom,
    Curva.bolTipica                            AS Tipica,
    Curva.intCurvaId                           AS CurvaId,
    Curva.blbPontos                            AS Pontos
  FROM
    Capacitor EqAux
      INNER JOIN Equipamento ON
        EqAux.intCapacitorId = Equipamento.intEquipId

      INNER JOIN EquipamentoRede ON
        Equipamento.intEquipId = EquipamentoRede.intEquipId

      INNER JOIN Curva ON
        Equipamento.intCurvaId = Curva.intCurvaId

  WHERE
    ((p_RedeId = VALINDEF) OR (EquipamentoRede.intRedeId = p_RedeId));

END SPSNP_rdCapacitor;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrCapacitor ( p_CapacitorId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_CurvaId NUMBER, p_Barra1Id NUMBER,
                              p_TipoLigacaoId NUMBER, p_Qnom NUMBER, p_Vnom NUMBER
                            )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrCapacitor                                            **
-- **                                                                        **
-- ** Insere um capacitor.                                                   **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150511 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
  SAVEPOINT A;

  -- Cria o equipamento
  SPSNP_wrEquipamento( p_CapacitorId, 5 /*p_TipoEquipId*/, p_Codigo, p_ExtId, p_CurvaId, p_Barra1Id );

  -- Cria novo capacitor
  INSERT INTO
    Capacitor (intCapacitorId, intTipoLigacaoId, fltQnom, fltVnom)
  VALUES
    ( p_CapacitorId, p_TipoLigacaoId, p_Qnom, p_Vnom );

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrCapacitor;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdGerador ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdGerador                                              **
-- **                                                                        **
-- ** Retorna os geradores de uma rede específica ou de todas as redes vin-  **
-- ** culadas ao estudo.                                                     **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150511 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

BEGIN

  OPEN spcursor FOR
  SELECT
    Equipamento.strCodigo                      AS Codigo,
    Equipamento.strExtId                       AS ExtId,
    Equipamento.intBarra1Id                    AS Barra1,
    EquipamentoRede.intRedeId                  AS RedeId,
    EqAux.intGeradorId                         AS GeradorId,
    EqAux.intTipoLigacaoId                     AS TipoLigacaoId,
    EqAux.intTipoSuprId                        AS TipoSuprId,
    EqAux.intTipoGerId                         AS TipoGerId,
    EqAux.fltVnom                              AS Vnom,
    EqAux.fltSnom                              AS Snom,
    EqAux.fltSmax                              AS Smax,
    EqAux.fltFatorPotMin                       AS FatorPotMin,
    EqAux.fltZ0R                               AS Z0R,
    EqAux.fltZ0X                               AS Z0X,
    EqAux.fltZ1R                               AS Z1R,
    EqAux.fltZ1X                               AS Z1X,
    Curva.bolTipica                            AS Tipica,
    Curva.intCurvaId                           AS CurvaId,
    Curva.blbPontos                            AS Pontos
  FROM
    Gerador EqAux
      INNER JOIN Equipamento ON
        EqAux.intGeradorId = Equipamento.intEquipId

      INNER JOIN EquipamentoRede ON
        Equipamento.intEquipId = EquipamentoRede.intEquipId

      INNER JOIN Curva ON
        Equipamento.intCurvaId = Curva.intCurvaId
  WHERE
    ((p_RedeId = VALINDEF) OR (EquipamentoRede.intRedeId = p_RedeId));


END SPSNP_rdGerador;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrGerador ( p_GeradorId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_CurvaId NUMBER, p_Barra1Id NUMBER,
                            p_TipoLigacaoId NUMBER, p_TipoSuprId NUMBER, p_TipoGerId NUMBER, p_Vnom NUMBER, p_Snom NUMBER, p_Smax NUMBER,
                            p_FatorPotMin NUMBER, p_Z0R NUMBER, p_Z0X NUMBER, p_Z1R NUMBER, p_Z1X NUMBER
                          )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrGerador                                              **
-- **                                                                        **
-- ** Insere ou atualiza um gerador.                                         **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150511 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN

  SAVEPOINT A;

  -- Cria o equipamento
  SPSNP_wrEquipamento( p_GeradorId, 10 /*p_TipoEquipId*/, p_Codigo, p_ExtId, p_CurvaId, p_Barra1Id );

  -- Insere na tabela do equipamento específico
  INSERT INTO
    Gerador (intGeradorId, intTipoLigacaoId, intTipoSuprId, intTipoGerId, fltVnom, fltSnom, fltSmax, fltFatorPotMin, fltZ0R, fltZ0X, fltZ1R, fltZ1X)
  VALUES
    ( p_GeradorId, p_TipoLigacaoId, p_TipoSuprId, p_TipoGerId, p_Vnom, p_Snom, p_sMax, p_FatorPotMin, p_Z0R, p_Z0X, p_Z1R, p_Z1X);

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrGerador;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdSuprimento ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdSuprimento                                           **
-- **                                                                        **
-- ** Retorna os suprimentos de uma rede específica ou de todas as redes vin **
-- ** culadas ao estudo.                                                     **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20101022 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

BEGIN

  OPEN spcursor FOR
  SELECT
    Equipamento.strCodigo                      AS Codigo,
    Equipamento.strExtId                       AS ExtId,
    Equipamento.intBarra1Id                    AS Barra1,
    EquipamentoRede.intRedeId                  AS RedeId,
    EqAux.intSuprimentoId                      AS SuprimentoId,
    EqAux.intTipoSuprId                        AS TipoSuprId,
    EqAux.fltVnom                              AS Vnom,
    EqAux.fltSmax                              AS Smax,
    EqAux.fltPcc3F                             AS Pcc3F,
    EqAux.fltQcc3F                             AS Qcc3F,
    EqAux.fltPccFT                             AS PccFT,
    EqAux.fltQccFT                             AS QccFT,
    EqAux.fltZ0R                               AS Z0R,
    EqAux.fltZ0X                               AS Z0X,
    EqAux.fltZ1R                               AS Z1R,
    EqAux.fltZ1X                               AS Z1X,
    Curva.bolTipica                            AS Tipica,
    Curva.intCurvaId                           AS CurvaId,
    Curva.blbPontos                            AS Pontos
  FROM
    Suprimento EqAux
      INNER JOIN Equipamento ON
        EqAux.intSuprimentoId = Equipamento.intEquipId

      INNER JOIN EquipamentoRede ON
        Equipamento.intEquipId = EquipamentoRede.intEquipId

      INNER JOIN Curva ON
        Equipamento.intCurvaId = Curva.intCurvaId
  WHERE
    ((p_RedeId = VALINDEF) OR (EquipamentoRede.intRedeId = p_RedeId));

END SPSNP_rdSuprimento;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrSuprimento ( p_SuprimentoId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_CurvaId NUMBER, p_Barra1Id NUMBER,
                               p_TipoSuprId NUMBER, p_Vnom NUMBER, p_Smax NUMBER, p_Pcc3F NUMBER, p_Qcc3F NUMBER, p_PccFT NUMBER, p_QccFT NUMBER,
                               p_Z0R NUMBER, p_Z0X NUMBER, p_Z1R NUMBER, p_Z1X NUMBER
                             )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrSuprimento                                           **
-- **                                                                        **
-- ** Insere ou atualiza um gerador.                                         **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150525 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
  SAVEPOINT A;

  -- Cria o equipamento
  SPSNP_wrEquipamento( p_SuprimentoId, 23 /*p_TipoEquipId*/, p_Codigo, p_ExtId, p_CurvaId, p_Barra1Id );

  -- Insere na tabela do equipamento específico
  INSERT INTO
    Suprimento (intSuprimentoId, intTipoSuprId, fltVnom, fltSmax, fltPcc3F, fltQcc3F, fltPccFT, fltQccFT, fltZ0R, fltZ0X, fltZ1R, fltZ1X)
  VALUES
    ( p_SuprimentoId, p_TipoSuprId, p_Vnom, p_Smax, p_Pcc3F, p_Qcc3F, p_PccFT, p_QccFT, p_Z0R, p_Z0X, p_Z1R, p_Z1X );

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrSuprimento;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdFiltro ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdFiltro                                               **
-- **                                                                        **
-- ** Retorna os filtros de uma rede específica ou de todas as redes vincu-  **
-- ** ladas ao estudo.                                                       **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150525 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

BEGIN

  OPEN spcursor FOR
  SELECT
    Equipamento.strCodigo                      AS Codigo,
    Equipamento.strExtId                       AS ExtId,
    Equipamento.intBarra1Id                    AS Barra1,
    EquipamentoRede.intRedeId                  AS RedeId,
    EqAux.intFiltroId                          AS FiltroId,
    EqAux.fltVnom                              AS Vnom,
    EqAux.fltSnom                              AS Snom,
    EqAux.fltZ0R                               AS Z0R,
    EqAux.fltZ0X                               AS Z0X
  FROM
    Filtro EqAux
      INNER JOIN Equipamento ON
        EqAux.intFiltroId = Equipamento.intEquipId

      INNER JOIN EquipamentoRede ON
        Equipamento.intEquipId = EquipamentoRede.intEquipId
  WHERE
    ((p_RedeId = VALINDEF) OR (EquipamentoRede.intRedeId = p_RedeId));

END SPSNP_rdFiltro;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrFiltro ( p_FiltroId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_Barra1Id NUMBER,
                           p_Vnom NUMBER, p_Snom NUMBER, p_Z0R NUMBER, p_Z0X NUMBER
                         )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrFiltro                                               **
-- **                                                                        **
-- ** Insere ou atualiza um filtro.                                          **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150511 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN

  SAVEPOINT A;

  -- Cria o equipamento
  SPSNP_wrEquipamento( p_FiltroId, 11 /*p_TipoEquipId*/, p_Codigo, p_ExtId, VALINDEF, p_Barra1Id );

  -- Insere na tabela do equipamento específico
  INSERT INTO
    Filtro (intFiltroId, fltVnom, fltSnom, fltZ0R, fltZ0X)
  VALUES
    ( p_FiltroId, p_Vnom, p_Snom, p_Z0R, p_Z0X );

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrFiltro;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdReator ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdReator                                               **
-- **                                                                        **
-- ** Retorna os Reatoress de uma rede específica ou de todas as redes vincu-**
-- ** ladas ao estudo.                                                       **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150525 |  mml  | Versão inicial.                                     **
-- ** 20130816 |  mml  | Inclusão da curva.                                  **
-- **                                                                        **
-- ****************************************************************************

BEGIN

  OPEN spcursor FOR
  SELECT
    Equipamento.strCodigo                      AS Codigo,
    Equipamento.strExtId                       AS ExtId,
    Equipamento.intBarra1Id                    AS Barra1,
    EquipamentoRede.intRedeId                  AS RedeId,
    EqAux.intReatorId                          AS ReatorId,
    EqAux.intTipoLigacaoId                     AS TipoLigacaoId,
    EqAux.fltVnom                              AS Vnom,
    EqAux.fltQnom                              AS Qnom,
    Curva.bolTipica                            AS Tipica,
    Curva.intCurvaId                           AS CurvaId,
    Curva.blbPontos                            AS Pontos
  FROM
    Reator EqAux
      INNER JOIN Equipamento ON
        EqAux.intReatorId = Equipamento.intEquipId

      INNER JOIN EquipamentoRede ON
        Equipamento.intEquipId = EquipamentoRede.intEquipId

      INNER JOIN Curva ON
        Equipamento.intCurvaId = Curva.intCurvaId
  WHERE
    ((p_RedeId = VALINDEF) OR (EquipamentoRede.intRedeId = p_RedeId));

END SPSNP_rdReator;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrReator ( p_ReatorId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_CurvaId NUMBER, p_Barra1Id NUMBER,
                           p_TipoLigacaoId NUMBER, p_Vnom NUMBER, p_Qnom NUMBER
                         )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrReator                                               **
-- **                                                                        **
-- ** Insere ou atualiza um Reator.                                          **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150525 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN

  SAVEPOINT A;

  -- Cria o equipamento
  SPSNP_wrEquipamento( p_ReatorId, 18 /*p_TipoEquipId*/, p_Codigo, p_ExtId, p_CurvaId, p_Barra1Id );

  -- Insere na tabela do equipamento específico
  INSERT INTO
    Reator (intReatorId, intTipoLigacaoId, fltVnom, fltQnom)
  VALUES
    ( p_ReatorId, p_TipoLigacaoId, p_Vnom, p_Qnom );

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrReator;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdTrafoZZ ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdTrafoZZ                                              **
-- **                                                                        **
-- ** Retorna os trafos ZZ de uma rede específica ou de todas as redes vincu-**
-- ** ladas ao estudo.                                                       **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150525 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

BEGIN

  OPEN spcursor FOR
  SELECT
    Equipamento.strCodigo                      AS Codigo,
    Equipamento.strExtId                       AS ExtId,
    Equipamento.intBarra1Id                    AS Barra1,
    EquipamentoRede.intRedeId                  AS RedeId,
    EqAux.intTrafoZZId                         AS TrafoZZId,
    EqAux.fltVnom                              AS Vnom,
    EqAux.fltSnom                              AS Snom,
    EqAux.fltZ0R                               AS Z0R,
    EqAux.fltZ0X                               AS Z0X
  FROM
    TrafoZZ EqAux
      INNER JOIN Equipamento ON
        EqAux.intTrafoZZId = Equipamento.intEquipId

      INNER JOIN EquipamentoRede ON
        Equipamento.intEquipId = EquipamentoRede.intEquipId
  WHERE
    ((p_RedeId = VALINDEF) OR (EquipamentoRede.intRedeId = p_RedeId));

END SPSNP_rdTrafoZZ;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrTrafoZZ ( p_TrafoZZId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_Barra1Id NUMBER,
                            p_Vnom NUMBER, p_Snom NUMBER, p_Z0R NUMBER, p_Z0X NUMBER
                          )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrTrafoZZ                                              **
-- **                                                                        **
-- ** Insere ou atualiza um trafoZZ.                                         **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150525 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN

  SAVEPOINT A;

  -- Cria o equipamento
  SPSNP_wrEquipamento( p_TrafoZZId, 26 /*p_TipoEquipId*/, p_Codigo, p_ExtId, VALINDEF, p_Barra1Id );

  -- Insere na tabela do equipamento específico
  INSERT INTO
    TrafoZZ (intTrafoZZId, fltVnom, fltSnom, fltZ0R, fltZ0X)
  VALUES
    ( p_TrafoZZId, p_Vnom, p_Snom, p_Z0R, p_Z0X );

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrTrafoZZ;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdMutua ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdMutua                                                **
-- **                                                                        **
-- ** Retorna as mãtuas de uma rede específica ou de todas as redes vincula- **
-- ** das ao estudo.                                                         **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150525 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

BEGIN

  OPEN spcursor FOR
  SELECT
    Equipamento.strCodigo                      AS Codigo,
    Equipamento.strExtId                       AS ExtId,
    Equipamento.intBarra1Id                    AS Barra1,
    EquipamentoRede.intRedeId                  AS RedeId,
    EqAux.intMutuaId                           AS MutuaId,
    EqAux.intTrecho1Id                         AS Trecho1Id,
    EqAux.intTrecho1BarraSntId                 AS Trecho1BarraSntId,
    EqAux.fltTrecho1Pos                        AS Trecho1Pos,
    EqAux.intTrecho2Id                         AS Trecho2Id,
    EqAux.intTrecho2BarraSntId                 AS Trecho2BarraSntId,
    EqAux.fltTrecho2Pos                        AS Trecho2Pos,
    EqAux.fltZ0R                               AS Z0R,
    EqAux.fltZ0X                               AS Z0X,
    EqAux.fltZ1R                               AS Z1R,
    EqAux.fltZ1X                               AS Z1X
  FROM
    Mutua EqAux
      INNER JOIN Equipamento ON
        EqAux.intMutuaId = Equipamento.intEquipId

      INNER JOIN EquipamentoRede ON
        Equipamento.intEquipId = EquipamentoRede.intEquipId
  WHERE
    ((p_RedeId = VALINDEF) OR (EquipamentoRede.intRedeId = p_RedeId));

END SPSNP_rdMutua;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrMutua ( p_MutuaId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_Barra1Id NUMBER,
                          p_Trecho1Id NUMBER, p_Trecho1BarraSntId NUMBER, p_Trecho1Pos NUMBER, p_Trecho2Id NUMBER, p_Trecho2BarraSntId NUMBER, p_Trecho2Pos NUMBER,
                          p_Z0R NUMBER, p_Z0X NUMBER, p_Z1R NUMBER, p_Z1X NUMBER
                        )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrMutua                                                **
-- **                                                                        **
-- ** Se a identificação da mãtua for nula, cria um novo capacitor. Caso con-**
-- ** trãrio, atualiza os valores passados como parãmetro.                   **
-- ** Ao final, retorna o identificador da mãtua criada ou atualizada.       **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150525 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais
BEGIN

  SAVEPOINT A;

  -- Cria o equipamento
  SPSNP_wrEquipamento( p_MutuaId, 15 /*p_TipoEquipId*/, p_Codigo, p_ExtId, VALINDEF, p_Barra1Id );

  -- Insere na tabela do equipamento específico
  INSERT INTO
    Mutua (intMutuaId, intTrecho1Id, intTrecho1BarraSntId, fltTrecho1Pos, intTrecho2Id, intTrecho2BarraSntId, fltTrecho2Pos, fltZ0R, fltZ0X, fltZ1R, fltZ1X)
  VALUES
    (p_MutuaId, p_Trecho1Id, p_Trecho1BarraSntId, p_Trecho1Pos, p_Trecho2Id, p_Trecho2BarraSntId, p_Trecho2Pos, p_Z0R, p_Z0X, p_Z1R, p_Z1X);

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrMutua;

/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdChave ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdChave                                                **
-- **                                                                        **
-- ** Retorna as chaves de uma rede específica ou de todas as redes vincula- **
-- ** das ao estudo.                                                         **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150525 |  mml  | Versão inicial.                                     **
-- ** 20130701 |  mml  | Inclusão de Telecomando, Protecao e TecnoCom.       **
-- **                                                                        **
-- ****************************************************************************

BEGIN

  OPEN spcursor FOR
  SELECT
    Equipamento.strCodigo                      AS Codigo,
    Equipamento.strExtId                       AS ExtId,
    Equipamento.intBarra1Id                    AS Barra1,
    Equipamento.intBarra2Id                    AS Barra2,
    EquipamentoRede.intRedeId                  AS RedeId,
    EqAux.intChaveId                           AS ChaveId,
    EqAux.intTipoChaveId                       AS TipoChaveId,
    EqAux.intEstado                            AS Estado,
    EqAux.fltInom                              AS Inom,
    EqAux.bolTelecomando                       AS Telecomando,
    EqAux.bolProtecao                          AS Protecao,
    EqAux.intTecnoCom                          AS TecnoCom
  FROM
    Chave EqAux
      INNER JOIN Equipamento ON
        EqAux.intChaveId = Equipamento.intEquipId

      INNER JOIN EquipamentoRede ON
        Equipamento.intEquipId = EquipamentoRede.intEquipId
  WHERE
    ((p_RedeId = VALINDEF) OR (EquipamentoRede.intRedeId = p_RedeId));

END SPSNP_rdChave;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrChave ( p_ChaveId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_Barra1Id NUMBER, p_Barra2Id NUMBER,
                          p_TipoChaveId NUMBER, p_Estado NUMBER, p_Inom NUMBER,
                          p_Telecomando NUMBER, p_Protecao NUMBER, p_TecnoCom NUMBER
                        )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrChave                                                **
-- **                                                                        **
-- ** Se a identificação da chave for nula, cria uma nova chave. Caso contrã-**
-- ** rio, atualiza os valores passados como parãmetro.                      **
-- ** Ao final, retorna o identificador da chave criada ou atualizada.       **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150525 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais
BEGIN

  SAVEPOINT A;

  -- Cria o equipamento
  SPSNP_wrEquipamento( p_ChaveId, 9 /*p_TipoEquipId*/, p_Codigo, p_ExtId, VALINDEF, p_Barra1Id, p_Barra2Id );

  -- Insere na tabela do equipamento específico
  INSERT INTO
    Chave (intChaveId, intTipoChaveId, intEstado, fltInom, bolTelecomando, bolProtecao, intTecnoCom)
  VALUES
    (p_ChaveId, p_TipoChaveId, p_Estado, p_Inom, p_Telecomando, p_Protecao, p_TecnoCom);

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrChave;

/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdRegulador ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdRegulador                                            **
-- **                                                                        **
-- ** Retorna as chaves de uma rede específica ou de todas as redes vincula- **
-- ** das ao estudo.                                                         **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150525 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

BEGIN

  OPEN spcursor FOR
  SELECT
    Equipamento.strCodigo                      AS Codigo,
    Equipamento.strExtId                       AS ExtId,
    Equipamento.intBarra1Id                    AS Barra1,
    Equipamento.intBarra2Id                    AS Barra2,
    EquipamentoRede.intRedeId                  AS RedeId,
    EqAux.intReguladorId                       AS ReguladorId,
    EqAux.intTipoLigacaoId                     AS TipoLigacaoId,
    EqAux.fltSnom                              AS Snom,
    EqAux.bolBypass                            AS Bypass,
    EqAux.fltFaixaVar                          AS FaixaVar,
    EqAux.intNumPasso                          AS NumPasso,
    EqAux.bolSentidoReversivel                 AS SentidoReversivel,
    NVL(EqAux.intSentidoBarRefId, VALINDEF)    AS SentidoBarRefId,
    EqAux.bolAjusteAuto                        AS AjusteAuto,
    EqAux.intCfgFixPassoSel                    AS CfgFixPassoSel,
    NVL(EqAux.intCfgAutBarRefId, VALINDEF)     AS CfgAutBarRefId,
    EqAux.fltCfgAutVpu                         AS CfgAutVpu,
    EqAux.fltZ0R                               AS Z0R,
    EqAux.fltZ0X                               AS Z0X,
    EqAux.fltZ1R                               AS Z1R,
    EqAux.fltZ1X                               AS Z1X
  FROM
    Regulador EqAux
      INNER JOIN Equipamento ON
        EqAux.intReguladorId = Equipamento.intEquipId

      INNER JOIN EquipamentoRede ON
        Equipamento.intEquipId = EquipamentoRede.intEquipId
  WHERE
    ((p_RedeId = VALINDEF) OR (EquipamentoRede.intRedeId = p_RedeId));

END SPSNP_rdRegulador;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrRegulador ( p_ReguladorId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_Barra1Id NUMBER, p_Barra2Id NUMBER,
                              p_TipoLigacaoId NUMBER, p_Snom NUMBER, p_Bypass NUMBER, p_FaixaVar NUMBER, p_NumPasso NUMBER, p_SentidoReversivel NUMBER, p_SentidoBarRefId NUMBER,
                              p_AjusteAuto NUMBER, p_CfgFixPassoSel NUMBER, p_CfgAutBarRefId NUMBER, p_CfgAutVpu NUMBER,
                              p_Z0R NUMBER, p_Z0X NUMBER, p_Z1R NUMBER, p_Z1X NUMBER,
                              p_ModeloId NUMBER, p_TipoFaseId NUMBER
                            )

IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrRegulador                                            **
-- **                                                                        **
-- ** Se a identificação do regulador for nula, cria um novo regulador. Caso **
-- ** contrãrio, atualiza os valores passados como parãmetro.                **
-- ** Ao final, retorna o identificador do regulador criado ou atualizado.   **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150525 |  mml  | Versão inicial.                                     **
-- ** 20140306 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais
BEGIN

  SAVEPOINT A;

  -- Cria o equipamento
  SPSNP_wrEquipamento( p_ReguladorId, 21 /*p_TipoEquipId*/, p_Codigo, p_ExtId, VALINDEF, p_Barra1Id, p_Barra2Id );

  -- Insere na tabela correspondente
  INSERT INTO
    Regulador (intReguladorId, intTipoLigacaoId, fltSnom, bolBypass, fltFaixaVar, intNumPasso, bolSentidoReversivel, intSentidoBarRefId, bolAjusteAuto, intCfgFixPassoSel, intCfgAutBarRefId, fltCfgAutVpu, fltZ0R, fltZ0X, fltZ1R, fltZ1X, intModeloId, intTipoFaseId )
  VALUES
    (p_ReguladorId, p_TipoLigacaoId, p_Snom, p_Bypass, p_FaixaVar, p_NumPasso, p_SentidoReversivel, NULLIF(p_SentidoBarRefId, VALINDEF), p_AjusteAuto, p_CfgFixPassoSel, NULLIF(p_CfgAutBarRefId, VALINDEF), p_CfgAutVpu, p_Z0R, p_Z0X, p_Z1R, p_Z1X, p_ModeloId, p_TipoFaseId );

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrRegulador;

/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdTrafo ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdTrafo                                                **
-- **                                                                        **
-- ** Retorna os transformadores de uma rede específica ou de todas as redes **
-- ** vinculadas ao estudo.                                                  **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150525 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

BEGIN

  OPEN spcursor FOR
  SELECT
    Equipamento.strCodigo                      AS Codigo,
    Equipamento.strExtId                       AS ExtId,
    Equipamento.intBarra1Id                    AS Barra1,
    Equipamento.intBarra2Id                    AS Barra2,
    EquipamentoRede.intRedeId                  AS RedeId,
    EqAux.intTrafoId                           AS TrafoId,
    EqAux.bolAutoTrafo                         AS AutoTrafo,
    EqAux.fltPerdaFerro                        AS PerdaFerro,
    EqAux.intLtcTipoUtl                        AS LtcTipoUtl,
    EqAux.intLtcBarRefId                       AS LtcBarRefId,
    EqAux.fltLtcVpu                            AS LtcVpu,
    EqAux.fltZ0R                               AS Z0R,
    EqAux.fltZ0X                               AS Z0X,
    EqAux.fltZ1R                               AS Z1R,
    EqAux.fltZ1X                               AS Z1X,
    EqAux.intTipoLigacao1Id                    AS Enrol1_TipoLigacao,
    EqAux.intTipoFase1Id                       AS Enrol1_TipoFase,
    EqAux.fltVnom1                             AS Enrol1_Vnom,
    EqAux.fltSnom1                             AS Enrol1_Snom,
    EqAux.fltRotFase1                          AS Enrol1_RotFase,
    EqAux.fltVtap1                             AS Enrol1_Vtap,
    EqAux.fltZterraR1                          AS Enrol1_ZterraR,
    EqAux.fltZterraX1                          AS Enrol1_ZterraX,
    EqAux.intTipoLigacao2Id                    AS Enrol2_TipoLigacao,
    EqAux.intTipoFase2Id                       AS Enrol2_TipoFase,
    EqAux.fltVnom2                             AS Enrol2_Vnom,
    EqAux.fltSnom2                             AS Enrol2_Snom,
    EqAux.fltRotFase2                          AS Enrol2_RotFase,
    EqAux.fltVtap2                             AS Enrol2_Vtap,
    EqAux.fltZterraR2                          AS Enrol2_ZterraR,
    EqAux.fltZterraX2                          AS Enrol2_ZterraX
  FROM
    Trafo EqAux
      INNER JOIN Equipamento ON
        EqAux.intTrafoId = Equipamento.intEquipId

      INNER JOIN EquipamentoRede ON
        Equipamento.intEquipId = EquipamentoRede.intEquipId
  WHERE
    ((p_RedeId = VALINDEF) OR (EquipamentoRede.intRedeId = p_RedeId));

END SPSNP_rdTrafo;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrTrafo ( p_TrafoId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_Barra1Id NUMBER, p_Barra2Id NUMBER,
                          p_AutoTrafo NUMBER, p_PerdaFerro NUMBER, p_LtcTipoUtl NUMBER, p_LtcBarRefId NUMBER, p_LtcVpu NUMBER, p_Z0R NUMBER, p_Z0X NUMBER, p_Z1R NUMBER, p_Z1X NUMBER,
                          p_Enrol1_TipoLigacaoId NUMBER, p_Enrol1_TipoFaseId NUMBER, p_Enrol1_Vnom NUMBER, p_Enrol1_Snom NUMBER, p_Enrol1_RotFase NUMBER, p_Enrol1_Vtap NUMBER, p_Enrol1_ZterraR NUMBER, p_Enrol1_ZterraX NUMBER,
                          p_Enrol2_TipoLigacaoId NUMBER, p_Enrol2_TipoFaseId NUMBER, p_Enrol2_Vnom NUMBER, p_Enrol2_Snom NUMBER, p_Enrol2_RotFase NUMBER, p_Enrol2_Vtap NUMBER, p_Enrol2_ZterraR NUMBER, p_Enrol2_ZterraX NUMBER
                        )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrTrafo                                                **
-- **                                                                        **
-- ** Se a identificação do trafo for nula, cria um novo trafo. Caso contrã- **
-- ** rio, atualiza os valores passados como parãmetro.                      **
-- ** Ao final, retorna o identificador do trafo criado ou atualizado.       **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150525 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais
BEGIN

  SAVEPOINT A;

  -- Cria o equipamento
  SPSNP_wrEquipamento( p_TrafoId, 24 /*p_TipoEquipId*/, p_Codigo, p_ExtId, VALINDEF, p_Barra1Id, p_Barra2Id );

  -- Insere na tabela correspondente
  INSERT INTO
    Trafo (intTrafoId, bolAutoTrafo, fltPerdaFerro, intLtcTipoUtl, intLtcBarRefId, fltLtcVpu, fltZ0R, fltZ0X, fltZ1R, fltZ1X,
           intTipoLigacao1Id, intTipoFase1Id, fltVnom1, fltSnom1, fltRotFase1, fltVtap1, fltZterraR1, fltZterraX1,
           intTipoLigacao2Id, intTipoFase2Id, fltVnom2, fltSnom2, fltRotFase2, fltVtap2, fltZterraR2, fltZterraX2
          )
  VALUES
    (p_TrafoId, p_AutoTrafo, p_PerdaFerro, p_LtcTipoUtl, NULLIF(p_LtcBarRefId, VALINDEF), p_LtcVpu, p_Z0R, p_Z0X, p_Z1R, p_Z1X,
     p_Enrol1_TipoLigacaoId, p_Enrol1_TipoFaseId, p_Enrol1_Vnom, p_Enrol1_Snom, p_Enrol1_RotFase, p_Enrol1_Vtap, p_Enrol1_ZterraR, p_Enrol1_ZterraX,
     p_Enrol2_TipoLigacaoId, p_Enrol2_TipoFaseId, p_Enrol2_Vnom, p_Enrol2_Snom, p_Enrol2_RotFase, p_Enrol2_Vtap, p_Enrol2_ZterraR, p_Enrol2_ZterraX
    );

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrTrafo;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_rdTrafo3E ( p_RedeId NUMBER DEFAULT VALINDEF, spcursor OUT udtRefCursor )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_rdTrafo3E                                              **
-- **                                                                        **
-- ** Retorna os transformadores de uma rede específica ou de todas as redes **
-- ** vinculadas ao estudo.                                                  **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150525 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

BEGIN
  OPEN spcursor FOR
  SELECT
    Equipamento.strCodigo                      AS Codigo,
    Equipamento.strExtId                       AS ExtId,
    Equipamento.intBarra1Id                    AS Barra1,
    Equipamento.intBarra2Id                    AS Barra2,
    Equipamento.intBarra3Id                    AS Barra3,
    EquipamentoRede.intRedeId                  AS RedeId,
    EqAux.intTrafo3EId                         AS Trafo3EId,
    EqAux.fltPerdaFerro                        AS PerdaFerro,
    EqAux.fltSbase                             AS Sbase,
    EqAux.fltZPS0R                             AS ZPS0R,
    EqAux.fltZPS0X                             AS ZPS0X,
    EqAux.fltZPT0R                             AS ZPT0R,
    EqAux.fltZPT0X                             AS ZPT0X,
    EqAux.fltZST0R                             AS ZST0R,
    EqAux.fltZST0X                             AS ZST0X,
    EqAux.fltZPS1R                             AS ZPS1R,
    EqAux.fltZPS1X                             AS ZPS1X,
    EqAux.fltZPT1R                             AS ZPT1R,
    EqAux.fltZPT1X                             AS ZPT1X,
    EqAux.fltZST1R                             AS ZST1R,
    EqAux.fltZST1X                             AS ZST1X,
    EqAux.intTipoLigacao1Id                    AS Enrol1_TipoLigacao,
    EqAux.intTipoFase1Id                       AS Enrol1_TipoFase,
    EqAux.fltVnom1                             AS Enrol1_Vnom,
    EqAux.fltSnom1                             AS Enrol1_Snom,
    EqAux.fltRotFase1                          AS Enrol1_RotFase,
    EqAux.fltVtap1                             AS Enrol1_Vtap,
    EqAux.fltZterraR1                          AS Enrol1_ZterraR,
    EqAux.fltZterraX1                          AS Enrol1_ZterraX,
    EqAux.intTipoLigacao2Id                    AS Enrol2_TipoLigacao,
    EqAux.intTipoFase2Id                       AS Enrol2_TipoFase,
    EqAux.fltVnom2                             AS Enrol2_Vnom,
    EqAux.fltSnom2                             AS Enrol2_Snom,
    EqAux.fltRotFase2                          AS Enrol2_RotFase,
    EqAux.fltVtap2                             AS Enrol2_Vtap,
    EqAux.fltZterraR2                          AS Enrol2_ZterraR,
    EqAux.fltZterraX2                          AS Enrol2_ZterraX,
    EqAux.intTipoLigacao3Id                    AS Enrol3_TipoLigacao,
    EqAux.intTipoFase3Id                       AS Enrol3_TipoFase,
    EqAux.fltVnom3                             AS Enrol3_Vnom,
    EqAux.fltSnom3                             AS Enrol3_Snom,
    EqAux.fltRotFase3                          AS Enrol3_RotFase,
    EqAux.fltVtap3                             AS Enrol3_Vtap,
    EqAux.fltZterraR3                          AS Enrol3_ZterraR,
    EqAux.fltZterraX3                          AS Enrol3_ZterraX
  FROM
    Trafo3E EqAux
      INNER JOIN Equipamento ON
        EqAux.intTrafo3EId = Equipamento.intEquipId

      INNER JOIN EquipamentoRede ON
        Equipamento.intEquipId = EquipamentoRede.intEquipId
  WHERE
    ((p_RedeId = VALINDEF) OR (EquipamentoRede.intRedeId = p_RedeId));

END SPSNP_rdTrafo3E;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrTrafo3E ( p_Trafo3EId IN OUT NUMBER, p_Codigo VARCHAR, p_ExtId VARCHAR, p_Barra1Id NUMBER, p_Barra2Id NUMBER, p_Barra3Id NUMBER,
                            p_PerdaFerro NUMBER, p_Sbase NUMBER,
                            p_ZPS0R NUMBER, p_ZPS0X NUMBER, p_ZPT0R NUMBER, p_ZPT0X NUMBER, p_ZST0R NUMBER, p_ZST0X NUMBER,
                            p_ZPS1R NUMBER, p_ZPS1X NUMBER, p_ZPT1R NUMBER, p_ZPT1X NUMBER, p_ZST1R NUMBER, p_ZST1X NUMBER,
                            p_Enrol1_TipoLigacaoId NUMBER, p_Enrol1_TipoFaseId NUMBER, p_Enrol1_Vnom NUMBER, p_Enrol1_Snom NUMBER, p_Enrol1_RotFase NUMBER, p_Enrol1_Vtap NUMBER, p_Enrol1_ZterraR NUMBER, p_Enrol1_ZterraX NUMBER,
                            p_Enrol2_TipoLigacaoId NUMBER, p_Enrol2_TipoFaseId NUMBER, p_Enrol2_Vnom NUMBER, p_Enrol2_Snom NUMBER, p_Enrol2_RotFase NUMBER, p_Enrol2_Vtap NUMBER, p_Enrol2_ZterraR NUMBER, p_Enrol2_ZterraX NUMBER,
                            p_Enrol3_TipoLigacaoId NUMBER, p_Enrol3_TipoFaseId NUMBER, p_Enrol3_Vnom NUMBER, p_Enrol3_Snom NUMBER, p_Enrol3_RotFase NUMBER, p_Enrol3_Vtap NUMBER, p_Enrol3_ZterraR NUMBER, p_Enrol3_ZterraX NUMBER
                          )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrTrafo3E                                              **
-- **                                                                        **
-- ** Se a identificação do trafo for nula, cria um novo trafo. Caso contrã- **
-- ** rio, atualiza os valores passados como parãmetro.                      **
-- ** Ao final, retorna o identificador do trafo criado ou atualizado.       **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150525 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais
BEGIN

  SAVEPOINT A;

  -- Cria o equipamento
  SPSNP_wrEquipamento( p_Trafo3EId, 25 /*p_TipoEquipId*/, p_Codigo, p_ExtId, VALINDEF, p_Barra1Id, p_Barra2Id, p_Barra3Id );

  -- Insere na tabela correspondente
  INSERT INTO
    Trafo3E (intTrafo3EId, fltPerdaFerro, fltSbase, fltZPS0R, fltZPS0X, fltZPT0R, fltZPT0X, fltZST0R, fltZST0X, fltZPS1R, fltZPS1X, fltZPT1R, fltZPT1X, fltZST1R, fltZST1X,
             intTipoLigacao1Id, intTipoFase1Id, fltVnom1, fltSnom1, fltRotFase1, fltVtap1, fltZterraR1, fltZterraX1,
             intTipoLigacao2Id, intTipoFase2Id, fltVnom2, fltSnom2, fltRotFase2, fltVtap2, fltZterraR2, fltZterraX2,
             intTipoLigacao3Id, intTipoFase3Id, fltVnom3, fltSnom3, fltRotFase3, fltVtap3, fltZterraR3, fltZterraX3
            )
  VALUES
    (p_Trafo3EId, p_PerdaFerro, p_Sbase, p_ZPS0R, p_ZPS0X, p_ZPT0R, p_ZPT0X, p_ZST0R, p_ZST0X, p_ZPS1R, p_ZPS1X, p_ZPT1R, p_ZPT1X, p_ZST1R, p_ZST1X,
     p_Enrol1_TipoLigacaoId, p_Enrol1_TipoFaseId, p_Enrol1_Vnom, p_Enrol1_Snom, p_Enrol1_RotFase, p_Enrol1_Vtap, p_Enrol1_ZterraR, p_Enrol1_ZterraX,
     p_Enrol2_TipoLigacaoId, p_Enrol2_TipoFaseId, p_Enrol2_Vnom, p_Enrol2_Snom, p_Enrol2_RotFase, p_Enrol2_Vtap, p_Enrol2_ZterraR, p_Enrol2_ZterraX,
     p_Enrol3_TipoLigacaoId, p_Enrol3_TipoFaseId, p_Enrol3_Vnom, p_Enrol3_Snom, p_Enrol3_RotFase, p_Enrol3_Vtap, p_Enrol3_ZterraR, p_Enrol3_ZterraX
    );

  EXCEPTION
    WHEN OTHERS THEN
      ROLLBACK TO SAVEPOINT A;
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;

END SPSNP_wrTrafo3E;

/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrTipoChave ( p_TipoChaveId IN OUT NUMBER, p_Descricao VARCHAR, p_Status NUMBER, p_CorId NUMBER )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrTipoChave                                            **
-- **                                                                        **
-- ** Atualiza ou cadastra um novo tipo de chave.                            **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150512 |  mml  | Versão inicial.                                     **
-- ** 20130701 |  mml  | Inclusão de CorId                                   **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
  INSERT INTO TipoChave ( intTipoChaveId, strDescricao, intStatus, intCorId )
  VALUES ( p_TipoChaveId, p_Descricao, p_Status, p_CorId );

  EXCEPTION
    WHEN OTHERS THEN
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;
END SPSNP_wrTipoChave;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrTipoConsumidor ( p_TipoConsId IN OUT NUMBER, p_Tag NUMBER, p_Descricao VARCHAR, p_Tipico NUMBER, p_CorId NUMBER)
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrTipoConsumidor                                       **
-- **                                                                        **
-- ** Atualiza ou cadastra um novo tipo de consumidor.                       **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20130701 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
  INSERT INTO TipoConsumidor ( intTipoConsId, intTag, strDescricao, bolTipico, intCorId )
  VALUES ( p_TipoConsId, p_Tag, p_Descricao, p_Tipico, p_CorId );

  EXCEPTION
    WHEN OTHERS THEN
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;
END SPSNP_wrTipoConsumidor;
/*---------------------------------------------------------------------------*/

PROCEDURE SPSNP_wrTipoRede ( p_TipoRedeId IN OUT NUMBER, p_Codigo VARCHAR, p_Descricao VARCHAR, p_Vmin NUMBER, p_Vmax NUMBER, p_ColorId NUMBER, p_Status NUMBER )
IS
-- ****************************************************************************
-- ** Procedure SPSNP_wrTipoRede                                             **
-- **                                                                        **
-- ** Atualiza ou cadastra um novo tipo de rede.                             **
-- **                                                                        **
-- **   Data   | Autor | Modificação                                         **
-- ** 20150512 |  mml  | Versão inicial.                                     **
-- **                                                                        **
-- ****************************************************************************

-- Variáveis locais

BEGIN
  INSERT INTO TipoRede ( intTipoRedeId, strCodigo, strDescricao, fltVMin, fltVMax, intColorId, intStatus )
  VALUES ( p_TipoRedeId, p_Codigo, p_Descricao, p_Vmin, p_Vmax, p_ColorId, p_Status );

  EXCEPTION
    WHEN OTHERS THEN
      IF (SQLCODE IN ( -2291, -2292 )) THEN -- Erro chave estrangeira
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||FNSNP_rdErr_FKDestino(SQLERRM)||';'||FNSNP_rdErr_FKOrigem(SQLERRM));
      ELSE            
        raise_application_error(-20000,';'||SQLCODE||';INSERÇÃO;'||'MSG: '||SQLERRM);
      END IF;
END SPSNP_wrTipoRede;
/*---------------------------------------------------------------------------*/

-- Fim do Package
END PCK_SNPGATEWAY;
/
