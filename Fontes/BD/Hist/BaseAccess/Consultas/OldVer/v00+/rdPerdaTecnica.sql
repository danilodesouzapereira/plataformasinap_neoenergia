SELECT
    PerdaTecnica.intRedeId                                                  AS RedeId,
    PerdaTecnica.dttData                                                    AS Data,
    PerdaTecnica.fltEnergiaSuprimento                                       AS EnergiaSuprimento,
    PerdaTecnica.fltEnergiaGerador                                          AS EnergiaGerador,
    PerdaTecnica.fltEnergiaRecebida                                         AS EnergiaRecebida,
    PerdaTecnica.fltEnergiaEntregue                                         AS EnergiaEntregue,
    PerdaTecnica.fltEnergiaCarga                                            AS EnergiaCarga,
    PerdaTecnica.fltEnergiaPerdaFluxo                                       AS EnergiaPerdaFluxo,
    PerdaTecnica.fltEnergiaPerdaTotal                                       AS EnergiaPerdaTotal,
    PerdaTecnica.fltPerdaCabo                                               AS PerdaCabo,
    PerdaTecnica.fltPerdaTrafoCu                                            AS PerdaTrafoCu,
    PerdaTecnica.fltPerdaTrafoFe                                            AS PerdaTrafoFe,
    PerdaTecnica.fltPerdaCapacitor                                          AS PerdaCapacitor,
    PerdaTecnica.fltPerdaReator                                             AS PerdaReator,
    PerdaTecnica.fltPerdaRegulador                                          AS PerdaRegulador,
    PerdaTecnica.fltPerdaMedidor                                            AS PerdaMedidor,
    PerdaTecnica.fltPerdaRamal                                              AS PerdaRamal,
    PerdaTecnica.fltPerdaOutros                                             AS PerdaOutros,
    PerdaTecnica.intQtdeTrafo                                               AS QtdeTrafo,
    PerdaTecnica.intQtdeCabo                                                AS QtdeCabo,
    PerdaTecnica.intQtdeCapacitor                                           AS QtdeCapacitor,
    PerdaTecnica.intQtdeReator                                              AS QtdeReator,
    PerdaTecnica.intQtdeRegulador                                           AS QtdeRegulador,
    PerdaTecnica.intQtdeConsA4                                              AS QtdeConsA4,
    PerdaTecnica.intQtdeConsRes                                             AS QtdeConsRes,
    PerdaTecnica.intQtdeConsCom                                             AS QtdeConsCom,
    PerdaTecnica.intQtdeConsInd                                             AS QtdeConsInd,
    PerdaTecnica.intQtdeConsRur                                             AS QtdeConsRur,
    PerdaTecnica.intQtdeConsOut                                             AS QtdeConsOut,
    TipoRede.intTipoRedeId                                                  AS TipoRedeId,
    TipoRede.strCodigo                                                      AS TipoRedeCodigo,
    IIF(ISNULL(GrupoPerdaRede.intGrupoId), 3, GrupoPerdaRede.intGrupoId)    AS GrupoPerdaRedeId,
    Rede.intRedeSupridoraId                                                 AS SupridoraId,
    Rede.strCodigo                                                          AS RedeCodigo
FROM 
   (
   (
   (
   Rede INNER JOIN TipoRede ON 
     Rede.intTipoRedeId = TipoRede.intTipoRedeId
   )
   INNER JOIN PerdaTecnica ON
     Rede.intRedeId = PerdaTecnica.intRedeId
   )
   LEFT OUTER JOIN GrupoPerdaRede ON 
     Rede.intRedeId = GrupoPerdaRede.intRedeId
   )
WHERE 
   Rede.strCodigo = [p_CodRef];
