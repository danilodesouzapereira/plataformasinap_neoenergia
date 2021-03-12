SELECT
  PerdaTecnica.dttData                   AS Data,   
  TipoRede.strCodigo                     AS TipoRede,
  TipoRede.intTipoRedeId                 AS TipoRede_Id,
  COUNT(PerdaTecnica.intRedeId)          AS QtdeRedes,
  SUM(PerdaTecnica.fltEnergiaSuprimento) AS SomaEnergiaSuprimento,
  SUM(PerdaTecnica.fltEnergiaGerador)    AS SomaEnergiaGerador,
  SUM(PerdaTecnica.fltEnergiaRecebida)   AS SomaEnergiaRecebida,
  SUM(PerdaTecnica.fltEnergiaEntregue)   AS SomaEnergiaEntregue,
  SUM(PerdaTecnica.fltEnergiaCarga)      AS SomaEnergiaCarga, 
  SUM(PerdaTecnica.fltEnergiaPerdaFluxo) AS SomaEnergiaPerdaFluxo,
  SUM(PerdaTecnica.fltEnergiaPerdaTotal) AS SomaEnergiaPerdaTotal,
  SUM(PerdaTecnica.fltPerdaCabo)         AS SomaPerdaCabo,
  SUM(PerdaTecnica.fltPerdaTrafoCu)      AS SomaPerdaTrafoCobre,
  SUM(PerdaTecnica.fltPerdaTrafoFe)      AS SomaPerdaTrafoFerro,
  SUM(PerdaTecnica.fltPerdaCapacitor)    AS SomaPerdaCapacitor,
  SUM(PerdaTecnica.fltPerdaReator)       AS SomaPerdaReator, 
  SUM(PerdaTecnica.fltPerdaRegulador)    AS SomaPerdaRegulador,
  SUM(PerdaTecnica.fltPerdaMedidor)      AS SomaPerdaMedidor, 
  SUM(PerdaTecnica.fltPerdaRamal)        AS SomaPerdaRamal,
  SUM(PerdaTecnica.fltPerdaOutros)       AS SomaPerdaOutros,
  SUM(PerdaTecnica.intQtdeTrafo)         AS SomaQtdeTrafo,
  SUM(PerdaTecnica.intQtdeCabo)          AS SomaQtdeCabo,
  SUM(PerdaTecnica.intQtdeCapacitor)     AS SomaQtdeCapacitor,
  SUM(PerdaTecnica.intQtdeReator)        AS SomaQtdeReator,
  SUM(PerdaTecnica.intQtdeRegulador)     AS SomaQtdeRegulador,
  SUM(PerdaTecnica.intQtdeConsA4)        AS SomaQtdeConsA4,
  SUM(PerdaTecnica.intQtdeConsRes)       AS SomaQtdeConsRes,
  SUM(PerdaTecnica.intQtdeConsCom)       AS SomaQtdeConsCom, 
  SUM(PerdaTecnica.intQtdeConsInd)       AS SomaQtdeConsInd,
  SUM(PerdaTecnica.intQtdeConsRur)       AS SomaQtdeConsRur,
  SUM(PerdaTecnica.intQtdeConsOut)       AS SomaQtdeConsOut    
FROM
  (
  (
  PerdaTecnica INNER JOIN Rede ON
    PerdaTecnica.intRedeId = Rede.intRedeId
  )
  INNER JOIN TipoRede ON
    Rede.intTipoRedeId = TipoRede.intTipoRedeId
  )
WHERE
   PerdaTecnica.dttData BETWEEN [p_DataIni] AND [p_DataFim] 
GROUP BY
  PerdaTecnica.dttData,
  TipoRede.strCodigo,
  TipoRede.intTipoRedeId
  

