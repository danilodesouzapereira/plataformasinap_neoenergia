SELECT
  Rede.strCodigo                     AS RedeCodigo,
  Rede.intTipoRedeId                 AS TipoRedeId,
  TipoRede.strCodigo                 AS TipoRedeCodigo,
  ArvoreNo.intNoId                   AS NoId,
  ArvoreNo.strCodigo                 AS NoCodigo,
  ArvoreNo.intNoPaiId                AS NoPaiId,
  ArvoreNo.intNivelId                AS NoNivelId,
  PerdaTecnica.intRedeId             AS RedeId,
  PerdaTecnica.fltEnergiaSuprimento  AS EnergiaSuprimento,
  PerdaTecnica.fltEnergiaGerador     AS EnergiaGerador,
  PerdaTecnica.fltEnergiaRecebida    AS EnergiaRecebida,
  PerdaTecnica.fltEnergiaEntregue    AS EnergiaEntregue,
  PerdaTecnica.fltEnergiaCarga       AS EnergiaCarga,
  PerdaTecnica.fltEnergiaPerdaFluxo  AS EnergiaPerdaFluxo,
  PerdaTecnica.fltEnergiaPerdaTotal  AS EnergiaPerdaTotal,
  PerdaTecnica.fltPerdaCabo          AS PerdaCabo,
  PerdaTecnica.fltPerdaTrafoCu       AS PerdaTrafoCu,
  PerdaTecnica.fltPerdaTrafoFe       AS PerdaTrafoFe,
  PerdaTecnica.fltPerdaCapacitor     AS PerdaCapacitor,
  PerdaTecnica.fltPerdaReator        AS PerdaReator,
  PerdaTecnica.fltPerdaRegulador     AS PerdaRegulador,
  PerdaTecnica.fltPerdaMedidor       AS PerdaMedidor,
  PerdaTecnica.fltPerdaRamal         AS PerdaRamal,
  PerdaTecnica.fltPerdaOutros        AS PerdaOutros,
  PerdaTecnica.intQtdeTrafo          AS QtdeTrafo,
  PerdaTecnica.intQtdeCabo           AS QtdeCabo,
  PerdaTecnica.intQtdeCapacitor      AS QtdeCapacitor,
  PerdaTecnica.intQtdeReator         AS QtdeReator,
  PerdaTecnica.intQtdeRegulador      AS QtdeRegulador,
  PerdaTecnica.intQtdeConsA4         AS QtdeConsA4,
  PerdaTecnica.intQtdeConsRes        AS QtdeConsRes,
  PerdaTecnica.intQtdeConsCom        AS QtdeConsCom,
  PerdaTecnica.intQtdeConsInd        AS QtdeConsInd,
  PerdaTecnica.intQtdeConsRur        AS QtdeConsRur,
  PerdaTecnica.intQtdeConsOut        AS QtdeConsOut
FROM
  (
  (
  (
  ArvoreNo INNER JOIN PerdaTecnica ON
     ArvoreNo.intNoId = PerdaTecnica.intNoId
  )
  INNER JOIN Rede ON
     PerdaTecnica.intRedeId = Rede.intRedeId
  )
  INNER JOIN TipoRede ON
     Rede.intTipoRedeId = TipoRede.intTipoRedeId
  )
WHERE
  ArvoreNo.intArvoreId = [p_ArvoreId] AND
  (([p_NivelId] = -1) OR (ArvoreNo.intNivelId <= [p_NivelId]))
ORDER BY
  ArvoreNo.intNoId
