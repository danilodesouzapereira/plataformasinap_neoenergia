SELECT
  Perda.Data                     AS Data,   
  TipoRede.Codigo                AS TipoRede,
  TipoRede.TipoRede_Id           AS TipoRede_Id,
  COUNT(Perda.Rede_Id)           AS QtdeRedes,
  SUM(Perda.EnergiaSuprimento)   AS SomaEnergiaSuprimento,
  SUM(Perda.EnergiaGerador)      AS SomaEnergiaGerador,
  SUM(Perda.EnergiaRecebida)     AS SomaEnergiaRecebida,
  SUM(Perda.EnergiaEntregue)     AS SomaEnergiaEntregue,
  SUM(Perda.EnergiaCarga)        AS SomaEnergiaCarga, 
  SUM(Perda.EnergiaPerdaFluxo)   AS SomaEnergiaPerdaFluxo,
  SUM(Perda.EnergiaPerdaTotal)   AS SomaEnergiaPerdaTotal,
  SUM(Perda.PerdaCabo)           AS SomaPerdaCabo,
  SUM(Perda.PerdaTrafoCobre)     AS SomaPerdaTrafoCobre,
  SUM(Perda.PerdaTrafoFerro)     AS SomaPerdaTrafoFerro,
  SUM(Perda.PerdaCapacitor)      AS SomaPerdaCapacitor,
  SUM(Perda.PerdaReator)         AS SomaPerdaReator, 
  SUM(Perda.PerdaRegulador)      AS SomaPerdaRegulador,
  SUM(Perda.PerdaMedidor)        AS SomaPerdaMedidor, 
  SUM(Perda.PerdaRamal)          AS SomaPerdaRamal,
  SUM(Perda.PerdaOutros)         AS SomaPerdaOutros,
  SUM(Perda.QtdeTrafo)           AS SomaQtdeTrafo,
  SUM(Perda.QtdeCabo)            AS SomaQtdeCabo,
  SUM(Perda.QtdeCapacitor)       AS SomaQtdeCapacitor,
  SUM(Perda.QtdeReator)          AS SomaQtdeReator,
  SUM(Perda.QtdeRegulador)       AS SomaQtdeRegulador,
  SUM(Perda.QtdeConsA4)          AS SomaQtdeConsA4,
  SUM(Perda.QtdeConsRes)         AS SomaQtdeConsRes,
  SUM(Perda.QtdeConsCom)         AS SomaQtdeConsCom, 
  SUM(Perda.QtdeConsInd)         AS SomaQtdeConsInd,
  SUM(Perda.QtdeConsRur)         AS SomaQtdeConsRur,
  SUM(Perda.QtdeConsOut)         AS SomaQtdeConsOut    
FROM
  Perda INNER JOIN TipoRede ON
    Perda.TipoRede_id = TipoRede.TipoRede_Id
WHERE
   Perda.Data BETWEEN [p_DataIni] AND [p_DataFim] 
GROUP BY
  Perda.Data,
  TipoRede.Codigo,
  TipoRede.TipoRede_Id
  

